#pragma once

#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

#include "esp_http_server.h"

#include "../wifi/iwifi_connector.h"
#include "spi_flash.h"
#include "../nv_storage.h"

using Credentials = std::unordered_map<std::string, std::string>;
using CredentialsCallback = std::function<void(const Credentials &)>;

class HttpServer {
    enum class FormFieldType {
        TEXT,
        WIFI,
        PASSWORD
    };

    struct FormField {
        FormFieldType type;
        std::string name;
        std::string label;
        std::string icon;
    };

public:
    HttpServer(std::shared_ptr<SPIFlash> spi_flash,
               std::shared_ptr<IWiFiConnector> connector,
               IWiFiConnector::EndpointsList &&endpoints,
               CredentialsCallback &&callback,
               std::shared_ptr<NVStorage> storage = nullptr);
    ~HttpServer();

private:
    void start();
    void stop();

    static esp_err_t index_handler(httpd_req_t *req);
    static esp_err_t index_head(httpd_req_t *req);
    static esp_err_t favicon_handler(httpd_req_t *req);
    static esp_err_t endpoints_handler(httpd_req_t *req);
    static esp_err_t credentials_handler(httpd_req_t *req);

    esp_err_t parse_form_fields(const std::string &data, Credentials &credentials);

    std::shared_ptr<SPIFlash> m_spi_flash;
    std::shared_ptr<IWiFiConnector> m_wifi_connector;
    IWiFiConnector::EndpointsList m_endpoints;
    CredentialsCallback m_callback;
    std::shared_ptr<NVStorage> m_storage;
    httpd_handle_t m_server = nullptr;
    httpd_config_t m_server_config = HTTPD_DEFAULT_CONFIG();
};