#include "http_server.h"

#include <regex>

#include "jsmn.h"
#include "mbedtls/base64.h"

#include "esp_log.h"
#include "common.h"

static constexpr size_t BASE64_LENGTH = 128;

static constexpr auto TAG = "HTTP Server";

HttpServer::HttpServer(std::shared_ptr<SPIFlash> spi_flash,
                       std::shared_ptr<IWiFiConnector> connector,
                       IWiFiConnector::EndpointsList &&endpoints,
                       CredentialsCallback &&callback,
                       std::shared_ptr<NVStorage> storage)
    : m_spi_flash(std::move(spi_flash))
    , m_wifi_connector(std::move(connector))
    , m_endpoints(std::move(endpoints))
    , m_callback(std::move(callback))
    , m_storage(std::move(storage)) {
    m_server_config.stack_size = 8196;
    start();
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::start() {
    const httpd_uri_t index = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = &HttpServer::index_handler,
        .user_ctx  = this
    };

    // A handler to return headers for the index page
    // Used to prefill the form with data from NVS
    const httpd_uri_t index_head = {
        .uri       = "/",
        .method    = HTTP_HEAD,
        .handler   = &HttpServer::index_head,
        .user_ctx  = this
    };

    const httpd_uri_t favicon = {
        .uri       = "/favicon.ico",
        .method    = HTTP_GET,
        .handler   = &HttpServer::favicon_handler,
        .user_ctx  = this
    };

    const httpd_uri_t endpoints = {
        .uri       = "/endpoints",
        .method    = HTTP_GET,
        .handler   = &HttpServer::endpoints_handler,
        .user_ctx  = this
    };

    const httpd_uri_t credentials = {
        .uri       = "/set_credentials",
        .method    = HTTP_POST,
        .handler   = &HttpServer::credentials_handler,
        .user_ctx  = this
    };

     // Start the httpd server
     ESP_LOGI(TAG, "Starting server on port: '%d'", m_server_config.server_port);
     if (httpd_start(&m_server, &m_server_config) == ESP_OK) {
         // Set URI handlers
         ESP_LOGI(TAG, "Registering URI handlers");
         httpd_register_uri_handler(m_server, &index);
         httpd_register_uri_handler(m_server, &index_head);
         httpd_register_uri_handler(m_server, &favicon);
         httpd_register_uri_handler(m_server, &endpoints);
         httpd_register_uri_handler(m_server, &credentials);
     } else {
         ESP_LOGE(TAG, "Failed to start server");
     }
}

void HttpServer::stop() {
    if (m_server) {
        ESP_LOGI(TAG, "Stopping HTTP server");
        httpd_stop(m_server);
        m_server = nullptr;
    }

    ESP_LOGI(TAG, "HTTP server stopped");
}

esp_err_t HttpServer::index_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Handling index request");

    HttpServer *self = static_cast<HttpServer*>(req->user_ctx);

    auto index_file = self->m_spi_flash->open_file("index.html", std::fstream::in);
    if (!index_file.is_open()) {
        ESP_LOGE(TAG, "Failed to open index HTML");

        return ESP_FAIL; // Return error if favicon file is not found
    }

    auto index_html = std::string((std::istreambuf_iterator<char>(index_file)),
                                   std::istreambuf_iterator<char>());
    index_file.close();

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, index_html.c_str(), index_html.size());

    return ESP_OK;
}

esp_err_t HttpServer::index_head(httpd_req_t *req) {
    ESP_LOGI(TAG, "Handling index HEAD request");

    HttpServer *self = static_cast<HttpServer*>(req->user_ctx);

    // We need headers data until sent is called, so we push them to a vector
    using HeaderType = std::pair<std::string, std::string>;
    std::vector<HeaderType> headers;

    auto set_encoded_header = [self, &headers](const std::string &key) {
        if (!self->m_storage->has_key(key)) {
            ESP_LOGW(TAG, "Key '%s' not found in storage", key.c_str());
            return; // Return empty pair if key is not found
        }

        std::string value = self->m_storage->get_string(key);

        unsigned char output[BASE64_LENGTH];
        size_t outlen;

        if (mbedtls_base64_encode(output, BASE64_LENGTH, &outlen, (const unsigned char*)value.c_str(), value.length()) != 0) {
            ESP_LOGE(TAG, "Failed to encode value for key: %s", key.c_str());
            return;
        }

        ESP_LOGI(TAG, "Setting header for key: %s with value: %s, encoded to: %s", key.c_str(), value.c_str(), output);

        headers.emplace_back(HeaderType{"x-" + key, {(const char*)output, outlen}});
    };

    httpd_resp_set_type(req, "text/html");

    set_encoded_header(NVS_WEATHER_API_TOKEN_KEY);
    set_encoded_header(NVS_WEATHER_CITY_KEY);
    set_encoded_header(NVS_OWNER_NAME_KEY);
    set_encoded_header(NVS_TIMEZONE_KEY);

    for (const auto &header : headers) {
        httpd_resp_set_hdr(req, header.first.c_str(), header.second.c_str());
    }

    httpd_resp_send(req, "", 0);

    return ESP_OK;
}

esp_err_t HttpServer::favicon_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Handling favicon request");

    HttpServer *self = static_cast<HttpServer*>(req->user_ctx);

    auto favicon_file = self->m_spi_flash->open_file("favicon.ico", std::fstream::in | std::fstream::binary);
    if (!favicon_file.is_open()) {
        ESP_LOGE(TAG, "Failed to open favicon file");

        return ESP_FAIL; // Return error if favicon file is not found
    }

    auto favicon_data = std::string(std::istreambuf_iterator<char>(favicon_file),
                                    std::istreambuf_iterator<char>());
    favicon_file.close();

    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_send(req, favicon_data.c_str(), favicon_data.size());

    return ESP_OK;
}

esp_err_t HttpServer::endpoints_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Handling endpoint request");

    auto self = static_cast<HttpServer*>(req->user_ctx);

    std::string response = "[";

    for (const auto &endpoint : self->m_endpoints) {
        response += "{\"ssid\": \"" + endpoint.first + "\", \"strength\": " + std::to_string(endpoint.second) + "},";
    }
    if (!self->m_endpoints.empty()) {
        response.pop_back(); // Remove the last comma
    }

    response += "]";

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response.c_str(), response.size());

    return ESP_OK;
}

esp_err_t HttpServer::credentials_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "Handling credentials request");

    auto buffer = std::string((size_t)req->content_len, '\0');

    // Read the POST data
    int ret = httpd_req_recv(req, (char*)buffer.c_str(), req->content_len);
    if (ret <= 0) {
        return ESP_FAIL; // Return error or success
    }

    auto self = static_cast<HttpServer*>(req->user_ctx);
    Credentials credentials;
    if (self->parse_form_fields(buffer, credentials) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to parse form fields");
        return ESP_FAIL; // Return error
    }

    self->m_callback(credentials);

    httpd_resp_set_status(req, "200 OK");
    httpd_resp_send(req, "Credentials received", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

// Parse form-data into Credentials
esp_err_t HttpServer::parse_form_fields(const std::string &data, Credentials &credentials)
{
    // Find the boundary from the first line
    std::istringstream stream(data);
    std::string line;
    if (!std::getline(stream, line)) {
        return ESP_FAIL; // Empty body
    }

    // Extract boundary (remove leading dashes and any trailing whitespace)
    std::string boundary = line;
    if (boundary.substr(0, 2) == "--") {
        boundary = boundary.substr(2);
    }
    // Remove carriage return if present
    if (!boundary.empty() && boundary.back() == '\r') {
        boundary.pop_back();
    }

    std::string currentField;
    std::string currentValue;
    bool inValue = false;

    // Process remaining lines
    while (std::getline(stream, line)) {
        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Check if this is a boundary line
        if (line == "--" + boundary || line == "--" + boundary + "--") {
            // Save previous field if we have one
            if (!currentField.empty() && inValue) {
                // Remove trailing newline from value if present
                if (!currentValue.empty() && currentValue.back() == '\n') {
                    currentValue.pop_back();
                }
                credentials[currentField] = currentValue;
            }

            // Reset for next field
            currentField.clear();
            currentValue.clear();
            inValue = false;

            // Stop processing if this is the final boundary
            if (line == "--" + boundary + "--") {
                break;
            }
        }
        // Check if this is a Content-Disposition header
        else if (line.find("Content-Disposition:") == 0) {
            // Extract field name using regex
            std::regex nameRegex(R"-(name="([^"]+)")-");
            std::smatch match;
            if (std::regex_search(line, match, nameRegex)) {
                currentField = match[1].str();
            }
        }
        // Empty line indicates start of field value
        else if (line.empty() && !currentField.empty()) {
            inValue = true;
        }
        // Collect field value lines
        else if (inValue) {
            if (!currentValue.empty()) {
                currentValue += "\n";
            }
            currentValue += line;
        }
    }

    return ESP_OK; // Implement parsing logic here
}