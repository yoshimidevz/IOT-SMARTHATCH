#include "config.h"
#include <HTTPClient.h>

void postJson(String json)
{
    HTTPClient http;
    int httpResponseCode;

    http.begin("http://" + String(SERVER_IP) + ":5000/");
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.POST(json);

    Serial.println(httpCode == HTTP_CODE_OK ? "POST ok" : "POST Fail");

    http.end();
}
