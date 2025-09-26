#include "DataHatchSenderApi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "Utils/TokenStorage.h"

void DataHatchSenderApi::sendAPI(int escotilhaId, float distancia, float lux) {
    String url_in = "https://yoshimi-vazadas.tecnomaub.site/api/sensores";

    if (WiFi.status() == WL_CONNECTED) {
        Preferences prefs;
        prefs.begin("escotilha", true);
        String apiToken = prefs.getString("api_token", "");
        prefs.end();

        if (apiToken.length() == 0) {
            Serial.println("Token não encontrado. Rode o provisionamento.");
            return;
        }

        HTTPClient http;
        http.begin(url_in);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("Accept", "application/json");
        http.addHeader("Authorization", "Bearer " + apiToken);

        DynamicJsonDocument jsonDoc(256);
        jsonDoc["serial_number"] = "ESP32-PORTA01";
        jsonDoc["distancia"] = distancia;
        jsonDoc["luz_ambiente"] = lux;

        String jsonPayload;
        serializeJson(jsonDoc, jsonPayload);

        Serial.print("Enviando payload: ");
        Serial.println(jsonPayload);

        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode == 200 || httpResponseCode == 201) {
            String payload = http.getString();
            Serial.println("Resposta da API:");
            Serial.println(payload);
        } else {
            Serial.print("Erro ao tentar enviar os dados! Código: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi não conectado.");
    }
}