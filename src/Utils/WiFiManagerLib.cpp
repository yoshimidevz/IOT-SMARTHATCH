#include "WiFiManagerLib.h"

void WiFiManagerLib::setupWiFi(const char* apName, const char* apPassword) {
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(240);
    
    if (!wifiManager.autoConnect(apName, apPassword)) {
        Serial.println(F("Falha na conexao. Resetar e tentar novamente..."));
        delay(3000);
        ESP.restart();
        delay(5000);
    }

    Serial.println(F("Conectado na rede Wifi."));
    Serial.print(F("Endereco IP: "));
    Serial.println(WiFi.localIP());
}
