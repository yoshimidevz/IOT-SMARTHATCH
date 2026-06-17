#include "RelayController.h"
#include <Arduino.h>

#define RELAY_1 14
#define RELAY_2 27
#define RELAY_3 26
#define RELAY_4 25

#define RELAY_ON  LOW
#define RELAY_OFF HIGH

// Energizado = retraído (normal) | Sem energia = avançado (mola, fecha)
static bool estadoFechado = false;

void initRelay() {
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  abrirSolenoides(); // começa energizado/retraído
}

void fecharSolenoides() {
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_2, RELAY_OFF);
  digitalWrite(RELAY_3, RELAY_OFF);
  digitalWrite(RELAY_4, RELAY_OFF);
  estadoFechado = true;
  Serial.println("[RELAY] Solenoides FECHADOS (sem energia, mola avança)");
}

void abrirSolenoides() {
  digitalWrite(RELAY_1, RELAY_ON);
  digitalWrite(RELAY_2, RELAY_ON);
  digitalWrite(RELAY_3, RELAY_ON);
  digitalWrite(RELAY_4, RELAY_ON);
  estadoFechado = false;
  Serial.println("[RELAY] Solenoides ABERTOS (energizado, retraído)");
}

bool isFechado() {
  return estadoFechado;
}