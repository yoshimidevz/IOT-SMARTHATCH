#ifndef WIFI_MANAGER_LIB_H
#define WIFI_MANAGER_LIB_H

#include <WiFi.h>
#include <WiFiManager.h>

class WiFiManagerLib {
public:
    void setupWiFi(const char* apName, const char* apPassword);
};

#endif
