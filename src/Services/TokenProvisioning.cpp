#include "TokenProvisioning.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Utils/TokenStorage.h"
#include <WiFi.h>

String provisionToken(const String &serialNumber) {
    if (WiFi.status() != WL_CONNECTED) return "";

    String url = "https://yoshimi-vazadas.tecnomaub.site/api/escotilha/register-login";

    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument doc(256);
    doc["serial_number"] = serialNumber;

    String payload;
    serializeJson(doc, payload);

    Serial.print("Enviando POST para register-login com payload: ");
    Serial.println(payload);

    int code = http.POST(payload);

    if (code == 200 || code == 201) {
        String resp = http.getString();
        DynamicJsonDocument respDoc(512);
        deserializeJson(respDoc, resp);
        String token = respDoc["data"]["token"];
        saveTokenToFlash(token);
        Serial.println("Token provisionado e salvo!");
        return token;
    } else {
        Serial.print("Erro provisionando token: ");
        Serial.println(code);
        return "";
    }

    http.end();
}
