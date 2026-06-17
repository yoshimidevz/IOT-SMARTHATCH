#include "MqttSender.h"
#include "SensorReader.h"
#include "RelayController.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

#define MQTT_SERVER   "72.62.140.117"
#define MQTT_PORT     1883
#define MQTT_USER     "incubadora"
#define MQTT_PASSWORD "@Vps123/Incuba2026"
#define SERIAL_NUMBER "ESP32-PORTA01"
#define TOPIC_SENSOR  "escotilha/" SERIAL_NUMBER "/sensor"
#define TOPIC_COMANDO "escotilha/" SERIAL_NUMBER "/comando"
#define OVERRIDE_DURACAO_MS 300000 // 5 minutos

static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

static bool overrideAtivo = false;
static unsigned long overrideExpiraEm = 0;

static void onComando(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) message += (char)payload[i];

  Serial.println("[MQTT] Comando recebido: " + message);

  DynamicJsonDocument doc(128);
  deserializeJson(doc, message);
  String action = doc["action"] | "";

  if (action == "abrir") {
    abrirSolenoides();
    overrideAtivo = true;
    overrideExpiraEm = millis() + OVERRIDE_DURACAO_MS;
    Serial.println("[OVERRIDE] Manual ativado por 5 minutos");
  } else if (action == "fechar") {
    fecharSolenoides();
    overrideAtivo = false;
    Serial.println("[OVERRIDE] Cancelado pelo motorista");
  }
}

static void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("[MQTT] Conectando...");
    if (mqttClient.connect("ESP32-SmartHatch", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println(" conectado!");
      mqttClient.subscribe(TOPIC_COMANDO);
      Serial.println("[MQTT] Subscrito em: " TOPIC_COMANDO);
    } else {
      Serial.printf(" falhou (rc=%d), tentando em 5s\n", mqttClient.state());
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
  }
}

static void setupOTA() {
  ArduinoOTA.setHostname("esp32-smarthatch");
  ArduinoOTA.setPassword("SmartHatch2026");
  ArduinoOTA.onStart([]() { Serial.println("[OTA] Iniciando..."); });
  ArduinoOTA.onEnd([]() { Serial.println("[OTA] Concluído!"); });
  ArduinoOTA.onError([](ota_error_t e) { Serial.printf("[OTA] Erro: %u\n", e); });
  ArduinoOTA.begin();
  Serial.println("[OTA] Pronto");
}

static void mqttSenderTask(void* pvParameters) {
  vTaskDelay(pdMS_TO_TICKS(3000));

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(onComando);
  setupOTA();

  SensorData data;
  for (;;) {
    ArduinoOTA.handle();

    if (!mqttClient.connected()) reconnectMQTT();
    mqttClient.loop();

    // Expira o override manual automaticamente
    if (overrideAtivo && millis() > overrideExpiraEm) {
      overrideAtivo = false;
      Serial.println("[OVERRIDE] Expirado, retomando automacao");
    }

    if (xQueueReceive(sensorQueue, &data, pdMS_TO_TICKS(100)) == pdPASS) {
      if (!overrideAtivo) {
        if (data.vazamento && !isFechado()) {
          fecharSolenoides();
        } else if (!data.vazamento && isFechado()) {
          abrirSolenoides();
        }
      }

      StaticJsonDocument<256> doc;
      doc["serial_number"] = SERIAL_NUMBER;
      doc["lux"]           = data.lux;
      doc["distancia_cm"]  = data.distanciaCm;
      doc["vazamento"]     = data.vazamento;
      doc["solenoide"]     = isFechado() ? "fechado" : "aberto";
      doc["override"]      = overrideAtivo;
      doc["timestamp"]     = millis();

      char buffer[256];
      serializeJson(doc, buffer);

      if (mqttClient.publish(TOPIC_SENSOR, buffer))
        Serial.printf("[MQTT] Publicado: %s\n", buffer);
      else
        Serial.println("[MQTT] Falha ao publicar");
    }
  }
}

void startMqttSenderTask() {
  xTaskCreatePinnedToCore(mqttSenderTask, "MqttSender", 10000, NULL, 1, NULL, 1);
}