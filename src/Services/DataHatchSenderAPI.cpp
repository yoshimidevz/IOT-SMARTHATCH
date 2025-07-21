#include "DataHatchSenderApi.h"

void DataHortaSenderApi::sendAPI(float ultrassonic, float fotoresistor, String status_luz) {
    String url_in = "http://85.31.63.241:8082/dadosEscotilha";

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(url_in);

        http.addHeader("Content-Type", "application/json");

        String jsonPayload = "{\"ultrassonic\":" + String(ultrassonic) + ","
                             "\"fotoresistor\":" + String(fotoresistor) + ","
                             "\"luz_ambiente\":\"" + status_luz + "\"}";

        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode == 201) {
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