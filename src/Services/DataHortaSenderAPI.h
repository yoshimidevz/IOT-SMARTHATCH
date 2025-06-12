#pragma once

#include <HTTPClient.h>
#include <WiFi.h>

class DataHortaSenderApi {
public:
    void sendAPI(float umidade_solo, float umidade_ar, float temp_ar, String status_luz);
};