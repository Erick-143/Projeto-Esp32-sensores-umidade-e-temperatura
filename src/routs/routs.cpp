#include <Arduino.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <DHT.h>
#include "routs.h"

#define DHTPIN   4
#define DHTTYPE  DHT22   // troque para DHT11 se for o seu caso

extern WebServer server;   // declarado em routs.h
DHT dht(DHTPIN, DHTTYPE);  // objeto global do sensor

// --------- Helper: registra rota estática (SPIFFS -> HTTP) ----------
static void routeStatic(const char* url, const char* fsPath, const char* contentType) {
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

// ------------------------- API /api/leitura --------------------------
static void enviarLeituras() {
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

// ------------------------ 404 padrão -------------------------------
static void rotaNaoEncontrada() {
  server.send(404, "text/plain", "Not found");
}

// ----------------------- Registrar rotas ---------------------------
void configurarRotas() {
  dht.begin();
  delay(1500); // pequena pausa para o sensor iniciar

  // ===== Estáticos (1 linha por arquivo) =====
  routeStatic("/",                   "/index.html",          "text/html");
  routeStatic("/style.css",          "/style.css",           "text/css");
  routeStatic("/assets/LogoUema.png","/LogoUema.png", "image/png");
  // Adicione aqui outros arquivos quando precisar:
  // routeStatic("/script.js",        "/script.js",           "application/javascript");
  // routeStatic("/assets/icone.svg", "/assets/icone.svg",    "image/svg+xml");

  // ===== API =====
  server.on("/api/leitura", HTTP_GET, enviarLeituras);

  // ===== 404 =====
  server.onNotFound(rotaNaoEncontrada);

  server.begin();
  Serial.println("Servidor web iniciado!");
  Serial.print("DHT: ");
  Serial.println(DHTTYPE == DHT22 ? "DHT22" : "DHT11");
}
