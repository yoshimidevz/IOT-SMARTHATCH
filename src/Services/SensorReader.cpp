#include "SensorReader.h"
#include <Arduino.h>

#define TEMT_PIN       34
#define TRIG_PIN        5
#define ECHO_PIN       18
#define RESISTOR    10000.0
#define AMOSTRAS       10
#define LUX_THRESHOLD  300.0  // lux acima disso = luz inesperada
#define DIST_THRESHOLD 20.0   // cm acima disso = bica aberta
#define INTERVALO    2000

QueueHandle_t sensorQueue;

static QueueHandle_t luxQueue;
static QueueHandle_t distQueue;

static float lerLux() {
  long soma = 0;
  for (int i = 0; i < AMOSTRAS; i++) {
    soma += analogRead(TEMT_PIN);
    delay(10);
  }
  float adcMedio = soma / (float)AMOSTRAS;
  float tensao = adcMedio * (3.3 / 4095.0);
  float corrente_uA = (tensao / RESISTOR) * 1e6;
  float lux = corrente_uA / 0.1;
  return lux < 0 ? 0 : lux;
}

static float lerDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) return 999.0;
  return duracao * 0.034 / 2.0;
}

static void luxTask(void* pvParameters) {
  analogReadResolution(12);
  analogSetPinAttenuation(TEMT_PIN, ADC_11db);

  for (;;) {
    float lux = lerLux();
    xQueueSend(luxQueue, &lux, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(INTERVALO));
  }
}

static void distTask(void* pvParameters) {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  for (;;) {
    float dist = lerDistancia();
    xQueueSend(distQueue, &dist, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(INTERVALO));
  }
}

static void fusionTask(void* pvParameters) {
  float lux  = 0;
  float dist = 0;

  for (;;) {
    if (xQueueReceive(luxQueue,  &lux,  pdMS_TO_TICKS(5000)) == pdPASS &&
        xQueueReceive(distQueue, &dist, pdMS_TO_TICKS(5000)) == pdPASS) {

      bool luzAlta     = lux  > LUX_THRESHOLD;
      bool bicaAberta  = dist > DIST_THRESHOLD;
      bool vazamento   = luzAlta && bicaAberta;

      Serial.printf("[FUSAO] Lux: %.1f | Dist: %.1f cm | Luz alta: %s | Bica aberta: %s | Vazamento: %s\n",
        lux, dist,
        luzAlta    ? "SIM" : "NAO",
        bicaAberta ? "SIM" : "NAO",
        vazamento  ? "SIM" : "NAO"
      );

      SensorData data = { lux, dist, vazamento };
      xQueueSend(sensorQueue, &data, portMAX_DELAY);
    }
  }
}

void startSensorTask() {
  sensorQueue = xQueueCreate(10, sizeof(SensorData));
  luxQueue    = xQueueCreate(5,  sizeof(float));
  distQueue   = xQueueCreate(5,  sizeof(float));

  xTaskCreatePinnedToCore(luxTask,    "LuxTask",    4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(distTask,   "DistTask",   4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(fusionTask, "FusionTask", 4096, NULL, 1, NULL, 0);
}