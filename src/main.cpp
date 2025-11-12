#include <Arduino.h>
#include <HCSR04.h>
#include <BH1750.h>
#include <Wire.h>
#include "Utils/TokenStorage.h"
#include "Services/TokenProvisioning.h"
#include "Services/DataHatchSenderAPI.h"
#include "Utils/WiFiManagerLib.h"

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>

const int echoPin = 26;
const int trigPin = 25;

const int pistaoDois = 19; //pistão esquerda superior
const int pistaoUm = 18; //pistão direita inferior

BH1750 lightMeter(0x23);
WifiManager wifiConnect;
DataHatchSenderApi sender;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 60000; // 360000 1 hora e 300 5min

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

float readLux(){
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    return lux;
  }
}

bool securitySystem(float distance, float lux){
  bool lightOk = (lux < 100);
  bool distanceOk = (distance > 0 && distance < 5);

  if(lightOk && distanceOk){
    Serial.println("Acionado.");
    digitalWrite(pistaoDois, HIGH);
    digitalWrite(pistaoUm, HIGH);
    return true;
  } else {
    Serial.println("Desligado.");     
    digitalWrite(pistaoDois, LOW);
    digitalWrite(pistaoUm, LOW);
    return false;
  }
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

  // wifiConnect.connect();

  // Serial.print("IP ESP32: ");
  // Serial.println(WiFi.localIP());

  // String token = TokenProvisioning::provisionToken("ESP32-PORTA01");
  // if (token != "") {
  //   Serial.print("Token provisionado: ");
  //   Serial.println(token);
  //   saveTokenToFlash(token);
  // } else {
  //   Serial.println("Falha ao provisionar token!");
  // }

  // String tokenLido = readTokenFromFlash();
  
}

void loop() {
  float distance = readDistance();
  float lux = readLux();

  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.print(" cm, Luminosidade: ");
  Serial.print(lux);
  Serial.println(" lx");

  // if (WiFi.status() != WL_CONNECTED) {
  //   Serial.println("WiFi desconectado, tentando reconectar...");
  //   wifiConnect.connect();
  //   delay(5000);
  // } else {
  //   unsigned long currentMillis = millis();
  //   if (currentMillis - lastSendTime >= sendInterval) {
  //     sender.sendAPI(distance, lux);
  //     lastSendTime = currentMillis;
  //   }
  // }

  delay(500);
}
