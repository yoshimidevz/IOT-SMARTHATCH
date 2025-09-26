#pragma once

#include <HTTPClient.h>
#include <WiFi.h>
#include <Arduino.h>

class DataHatchSenderApi {
public:
    void sendAPI(float distancia, float lux);
};