#include <Arduino.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <DHT.h>
#include "routs.h"

#define DHTPIN   4
#define DHTTYPE  DHT22   

extern WebServer server;   // declarado em routs.h
DHT dht(DHTPIN, DHTTYPE);  // objeto global do sensor

static void enviarRota(const char* url, const char* fsPath, const char* contentType) {
  server.on(url, HTTP_GET, [fsPath, contentType]() {
    File f = SPIFFS.open(fsPath, "r");
    if (!f) {
      server.send(404, "text/plain", "Not found");
      return;
    }
    server.streamFile(f, contentType);
    f.close();
  });
}

void enviarLeituras() {
  float temperatura = dht.readTemperature();  // °C
  float umidade     = dht.readHumidity();     // %

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("[DHT] leitura invalida (NaN)");
    server.sendHeader("Cache-Control", "no-store");
    server.send(500, "application/json", "{\"error\":\"sensor\"}");
    return;
  }

  Serial.printf("[DHT] T=%.1f °C  H=%.1f %%\n", temperatura, umidade);

  char buf[96];
  snprintf(buf, sizeof(buf),
           "{\"temperatura\":%.1f,\"umidade\":%.1f}",
           temperatura, umidade);
  Serial.printf("JSON -> %s\n", buf);

  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", buf);
}


static void rotaNaoEncontrada() {
  server.send(404, "text/plain", "Not found");
}


void configurarRotas() {
  dht.begin();
  delay(1500); // pequena pausa para o sensor iniciar

  
  enviarRota("/",                   "/index.html",          "text/html");
  enviarRota("/style.css",          "/style.css",           "text/css");
  enviarRota("/assets/LogoUema.png","/LogoUema.png", "image/png");
  
 
  server.on("/api/leitura", HTTP_GET, enviarLeituras);

  
  server.onNotFound(rotaNaoEncontrada);

  server.begin();
  Serial.println("Servidor web iniciado!");
  Serial.print("DHT: ");
  Serial.println(DHTTYPE == DHT22 ? "DHT22" : "DHT11");
}
