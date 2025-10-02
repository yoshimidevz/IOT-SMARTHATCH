#include <Arduino.h>
#include <HCSR04.h>
#include <BH1750.h>
#include <Wire.h>
#include "Utils/WiFiManagerLib.h"
#include "Services/DataHatchSenderAPI.h"
#include "Utils/TokenStorage.h"
#include "Services/DataHatchSenderAPI.h"
#include "Services/TokenProvisioning.h"

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>

//ultrassônico
const int echoPin = 26;
const int trigPin = 25;

const int pistaoDois = 19; //pistão esquerda superior
const int pistaoUm = 18; //pistão direita inferior

BH1750 lightMeter(0x23);
WifiManager wifiConnect;
DataHatchSenderApi sender;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 300; // 360000 1 hora e 300 5min


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

float readLux(){
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    // Serial.print("Light: ");
    // Serial.print(lux);
    // Serial.println(" lx");
    return lux;
  }
}

bool securitySystem(float distance, float lux){
  bool lightOk = (lux < 100);
  bool distanceOk = (distance > 0 && distance < 10);

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

  WiFi.disconnect(true, true);

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(pistaoDois, OUTPUT);
  pinMode(pistaoUm, OUTPUT);

  wifiConnect.connect();

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }

  String token = readTokenFromFlash();

  if (token == "") {
    Serial.println("Nenhum token salvo, provisionando...");
    token = TokenProvisioning::provisionToken("ESP32-PORTA01");
    if (token != "") {
        Serial.print("Token provisionado: ");
        Serial.println(token);
    } else {
        Serial.println("Falha ao provisionar token!");
    }
  }
}

void loop() {
  float distance = readDistance();
  float lux = readLux();

  securitySystem(distance, lux);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi desconectado, tentando reconectar...");
    wifiConnect.connect();
    delay(5000);
  } else {
    sender.sendAPI(distance, lux);
  }

  delay(5000);
} 