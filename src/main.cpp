#include <Arduino.h>
#include <HCSR04.h>
#include <BH1750.h>
#include <Wire.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//Sensor ultrassônico
const int echoPin = 26;
const int trigPin = 25;

const int pistaoDois = 19; //pistão esquerda superior
const int pistaoUm = 18; //pistão direita inferior

BH1750 lightMeter(0x23);

//BH1750 PINOUTS
// VCC - PROTOBOARD: 08 PINOUT: 3.3V / LARANJA
// GND - PROTOBOARD: 09 PINOUT: GND / PRETO
// SCL - PROTOBOARD: 10 PINOUT: D22 / AMARELO
// SDA - PROTOBOARD: 11 PINOUT: D21 / VERDE
// ADDR - PROTOBOARD: 12 PINOUT: GND ou desconectadoQ / AZUL

float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // 30 ms timeout (~5m)
  if (duration == 0) {
    return -1; // erro na leitura
  }
  float distance = duration / 58.0;
  return distance;
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22); 

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  pinMode(pistaoDois, OUTPUT);
  pinMode(pistaoUm, OUTPUT);

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }
}

void loop() {
  float distance = readDistance();
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(2000);


  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    if(lux > 100 && distance > 0 && distance < 10) {
      Serial.println("Acionado.");
      digitalWrite(pistaoDois, HIGH);
      digitalWrite(pistaoUm, HIGH);
    } else {
      Serial.println("Desligado.");
      digitalWrite(pistaoDois, LOW);
      digitalWrite(pistaoUm, LOW);
    }
  }
  delay(2000);
}