#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>//criar serivdor wev
#include <SPIFFS.h>//pegar o html
#include "routs/routs.h"

#define AP_SSID  "ESP32_GRUPO2"
#define AP_SENHA "12345678"

WebServer server(80);  


void setup() {
  Serial.begin(115200);  
  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao montar SPIFFS");
    return;
  }

  if (WiFi.softAP(AP_SSID, AP_SENHA)) {
    Serial.println("Rede criada!");
    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.softAPIP());// o serial.printa o ip do esp32
  } else {
    Serial.println("Erro ao criar a rede");
    return;
  }

  configurarRotas();  // Registra rotas e inicia o servidor
}

void loop() {
  server.handleClient();
}
