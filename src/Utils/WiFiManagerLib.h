#pragma once

#include <WiFiManager.h>
#include <Arduino.h>

class WifiManager {
public:
    void connect() {
        WiFiManager wm;
        if (!wm.autoConnect("Yoshimi", "123456")) {
            Serial.println("Failed to connect!!");
        } else {
            Serial.println("Successful connection!!");
        }
    }
};