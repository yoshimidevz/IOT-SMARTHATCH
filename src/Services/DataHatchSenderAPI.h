#pragma once

#include <HTTPClient.h>
#include <WiFi.h>

class DataHatchSenderApi {
public:
    void sendAPI(int escotilhaId, float distancia, float lux);
};