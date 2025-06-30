#pragma once

#include <string>
#include <vector>

class IWiFiConnector {
public:
    using Endpoint = std::pair<std::string, int8_t>; // SSID and RSSI
    using EndpointsList = std::vector<Endpoint>;

    virtual ~IWiFiConnector() = default;

    virtual bool test_connection(const std::string &ssid, const std::string &password) = 0;
    virtual EndpointsList get_endpoints() = 0;
};
