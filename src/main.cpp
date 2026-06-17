#include <Arduino.h>
#include "Utils/WiFiManagerLib.h"
#include "Services/SensorReader.h"
#include "Services/RelayController.h"
#include "Services/MqttSender.h"

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== SmartHatch Iniciando ===");

  connectWiFi();

  initRelay();
  startSensorTask();
  startMqttSenderTask();
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}