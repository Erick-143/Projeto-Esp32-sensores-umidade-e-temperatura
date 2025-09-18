#include <Arduino.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <DHT.h>
#include "routs.h"

// === Ajuste aqui o seu sensor ===
#define DHTPIN   4
#define DHTTYPE  DHT22     // TROQUE para DHT11 se for o seu caso

DHT dht(DHTPIN, DHTTYPE);  // objeto global do sensor

// Envia arquivo do SPIFFS
static void enviarArquivo(const char* nome_arquivo, const char* tipo_de_arquivo) {
  File f = SPIFFS.open(nome_arquivo, "r");
  if (!f) {
    server.send(404, "text/plain", "Arquivo nao encontrado");
    return;
  }
  server.streamFile(f, tipo_de_arquivo);
  f.close();
}

static void enviarPaginaPrincipal() {
  enviarArquivo("/index.html", "text/html");
}

static void enviarEstilo() {
  enviarArquivo("/style.css", "text/css");
}

// API usada pelo seu HTML (JSON): /api/readings
void enviarLeituras() {
  float temperatura = dht.readTemperature();  // °C
  float umidade     = dht.readHumidity();     // %

  if (isnan(temperatura) || isnan(umidade)) {
    // pode remover o sendHeader se quiser simplificar
    Serial.println("[DHT] leitura invalida (NaN)");
    server.sendHeader("Cache-Control", "no-store");
    server.send(500, "application/json", "{\"error\":\"sensor\"}");
    return;
  }

  
  Serial.printf("[DHT] T=%.1f °C  H=%.1f %%\n", temperatura, umidade);

  char buf[96]; // buffer temporário p/ montar o JSON
  snprintf(buf, sizeof(buf),
           "{\"temperatura\":%.1f,\"umidade\":%.1f}",
           temperatura, umidade);

             Serial.printf("JSON -> %s\n", buf);

  server.sendHeader("Cache-Control", "no-store"); // evita cache
  server.send(200, "application/json", buf);
}

static void rotaNaoEncontrada() {
  server.send(404, "text/plain", "Rota nao encontrada");
}

void configurarRotas() {
  // Inicia o DHT aqui
  dht.begin();
  delay(1500); // pequena pausa para o sensor iniciar

  server.on("/",           HTTP_GET, enviarPaginaPrincipal);
  server.on("/style.css",  HTTP_GET, enviarEstilo);
  server.on("/api/leitura", HTTP_GET, enviarLeituras); 
  server.onNotFound(rotaNaoEncontrada);

  server.begin();
  Serial.println("Servidor web iniciado!");
  Serial.print("DHT: ");
  Serial.println(DHTTYPE == DHT22 ? "DHT22" : "DHT11");
}
