#pragma once
#include <WiFiManager.h>
#include <Arduino.h>

inline void connectWiFi() {
  WiFiManager wm;
  wm.setConnectRetries(3);
  wm.setConnectTimeout(10);
  wm.setConfigPortalTimeout(180);

  if (!wm.autoConnect("SmartHatch_Setup")) {
    Serial.println("[WiFi] Falha. Reiniciando...");
    delay(3000);
    ESP.restart();
  }

  Serial.printf("[WiFi] Conectado: %s\n", WiFi.localIP().toString().c_str());
}