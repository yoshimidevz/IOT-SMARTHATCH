#include "TokenProvisioning.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "Utils/TokenStorage.h"

String TokenProvisioning::provisionToken(const String &serialNumber) {
    String url = "https://212.85.19.3/api/escotilha/register-login";

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi não conectado.");
        return "";
    }

    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Accept", "application/json");

    DynamicJsonDocument doc(256);
    doc["serial_number"] = serialNumber;

    String payload;
    serializeJson(doc, payload);

    Serial.print("Enviando POST para register-login com payload: ");
    Serial.println(payload);

    int code = http.POST(payload);

    if (code == 200 || code == 201) {
        String resp = http.getString();
        Serial.println("Resposta do servidor:");
        Serial.println(resp);

        DynamicJsonDocument respDoc(512);
        DeserializationError error = deserializeJson(respDoc, resp);

        if (!error && respDoc["data"]["token"].is<String>()) {
            String token = respDoc["data"]["token"].as<String>();
            saveTokenToFlash(token);
            Serial.println("Token provisionado e salvo!");
            http.end();
            return token;
        } else {
            Serial.println("Erro ao interpretar JSON da resposta.");
        }
    } else {
        Serial.print("Erro provisionando token! Código HTTP: ");
        Serial.println(code);
        String resp = http.getString();
        Serial.println("Resposta do servidor:");
        Serial.println(resp);
    }

    http.end();
    return "";
}
