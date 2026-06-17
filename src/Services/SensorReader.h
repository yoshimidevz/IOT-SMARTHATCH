#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

struct SensorData {
  float lux;
  float distanciaCm;
  bool  vazamento;
};

extern QueueHandle_t sensorQueue;

void startSensorTask();