#include <Arduino.h>

// #include <BLEDevice.h>
// #include <BLEServer.h>
// #include <BLEUtils.h>
// #include <BLE2902.h>

#include <SPI.h>
#include <HCSR04.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>

// #include "../lib/WifiManager/MyWifiManager.h"
// #include "Services/DataHortaSenderApi.h"

// #include <ArduinoJson.h>
// #include <WiFi.h>
// #include <time.h>
// #include "config.hpp"
// #include "WiFiManagerLib.h"
// #include "httpFunctions.h"

// BLECharacteristic *characteristicTX;
// bool deviceConnected = false; 

// #define SERVICE_UUID           "ab0828b1-198e-4351-b779-901fa0e0371e"
// #define CHARACTERISTIC_UUID_RX "4ac8a682-9736-4e5d-932b-e9b31405049c"
// #define CHARACTERISTIC_UUID_TX "0972EF8C-7613-4075-AD52-756F33D4DA91"

const int ledPin = 2;
const int echoPin = 26;
const int trigPin = 25;

const int PISTAO_1 = 19;  
const int PISTAO_2 = 21;
const int PISTAO_3 = 22;
const int PISTAO_4 = 23;

long duracao;
float distancia;
// bool deviceConnected = false;

// float readSensorData();

// class ServerCallbacks: public BLEServerCallbacks {
//     void onConnect(BLEServer* pServer) {
//       deviceConnected = true;
//     };

//     void onDisconnect(BLEServer* pServer) {
//       deviceConnected = false;
//     }
// };

// class CharacteristicCallbacks: public BLECharacteristicCallbacks {
//     void onWrite(BLECharacteristic *characteristic) {
//       //retorna ponteiro para o registrador contendo o valor atual da caracteristica
//       std::string rxValue = characteristic->getValue(); 
//       //verifica se existe dados (tamanho maior que zero)
//       if (rxValue.length() > 0) {
//         Serial.println("*********");
//         Serial.print("Received Value: ");

//         for (int i = 0; i < rxValue.length(); i++) {
//           Serial.print(rxValue[i]);
//         }

//         Serial.println();

//         // Do stuff based on the command received
//         if (rxValue.find("L1") != -1) { 
//           Serial.print("Turning LED ON!");
//           digitalWrite(echoPin, HIGH);
//         }
//         else if (rxValue.find("L0") != -1) {
//           Serial.print("Turning LED OFF!");
//           digitalWrite(echoPin, LOW);
//         }
//         // Do stuff based on the command received from the app
//         else if (rxValue.find("B1") != -1) { 
//           Serial.print("Turning Buzzer ON!");
//           digitalWrite(trigPin, HIGH);
//         }
//         else if (rxValue.find("B0") != -1) {
//           Serial.print("Turning Buzzer OFF!");
//           digitalWrite(trigPin, LOW);
//         }

//         Serial.println();
//         Serial.println("*********");
//       }
//     }
// };

// float medirDistancia() {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
  
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
  
//   duracao = pulseIn(echoPin, HIGH);
  
//   return (duracao / 2) * velocidadeSom;
// }

void setup() {
  Serial.begin(115200);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(ledPin, LOW);

  // BLEDevice::init("ESP32-BLE"); // nome do dispositivo bluetooth
  // // Create the BLE Server
  // BLEServer *server = BLEDevice::createServer(); //cria um BLE server 
  // server->setCallbacks(new ServerCallbacks()); //seta o callback do server
  // // Create the BLE Service
  // BLEService *service = server->createService(SERVICE_UUID);
  // // Create a BLE Characteristic para envio de dados
  // characteristicTX = service->createCharacteristic(
  //                     CHARACTERISTIC_UUID_TX,
  //                     BLECharacteristic::PROPERTY_NOTIFY
  //                   );
                      
  // characteristicTX->addDescriptor(new BLE2902());

  // // Create a BLE Characteristic para recebimento de dados
  // BLECharacteristic *characteristic = service->createCharacteristic(
  //                                        CHARACTERISTIC_UUID_RX,
  //                                        BLECharacteristic::PROPERTY_WRITE
  //                                      );

  // characteristic->setCallbacks(new CharacteristicCallbacks());
  // // Start the service
  // service->start();
  // // Start advertising (descoberta do ESP32)
  // server->getAdvertising()->start();
}

void loop() {
  // distancia = medirDistancia();

  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  if (distancia > distanciaLimite) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ACESO - Distância maior que 3cm");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED APAGADO - Distância menor ou igual a 3cm");
  }
  
  delay(1000);

  // if (deviceConnected) {
  //   if (trigPin.read()){
  //       //recupera a leitura da temperatura do ambiente
  //       float tempAmbiente = sensor.ambient();
  //       //recupera a leitura da temperatura do objeto apontado pelo sensor
  //       float tempObjeto   = sensor.object();
    
  //       // Let's convert the value to a char array:
  //       char txString[8]; // make sure this is big enuffz
  //       dtostrf(tempAmbiente, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer

  //       characteristicTX->setValue(txString); //seta o valor que a caracteristica notificará (enviar)       
  //       characteristicTX->notify(); // Envia o valor para o smartphone

  //       Serial.print("*** Sent Value: ");
  //       Serial.print(txString);
  //       Serial.println(" ***");
  //   }
  // }
}