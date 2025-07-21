#pragma once

#include <HTTPClient.h>
#include <WiFi.h>

class DataHortaSenderApi {
public:
    void sendAPI(float ultrassonic, float fotoresistor, String status_luz);
};