#include "routs/routs.h"
#include <Arduino.h>      // Biblioteca principal do Arduino, inclui funções básicas
#include <WiFi.h>        
//cria um acces point (cria uma rede a outros dispositivos podem se conectar)
#include <WebServer.h>    // Biblioteca para criar um servidor web simples
//o esp fica responde as requisições http
#include <SPIFFS.h>       // Biblioteca para acessar o sistema de arquivos SPIFFS

// Define o nome e a senha da rede WiFi que o ESP32 vai criar (modo Access Point)
#define AP_SSID "ESP32_REDE"       // Nome da rede WiFi (SSID)
#define AP_SENHA "12345678"        // Senha da rede WiFi

WebServer server(80); 
//roda o servidor na porta 80 (porta padrão para HTTP)

void setup() {
  Serial.begin(115200); // escolhe a velocidade da comunicação serial
  pinMode(LED_BUILTIN, OUTPUT);   // Define o pino 2 (GPIO2) como saída (para o LED)

  // serve pra acessar arquivos do pc que foi refenciado no código
  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao montar SPIFFS"); // Mostra erro se não conseguir montar
    return; // Sai do setup se falhar
  }

  // Cria uma rede WiFi própria (Access Point)
  if (WiFi.softAP(AP_SSID, AP_SENHA)==true){
  Serial.println("Rede criada!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.softAPIP());  // Mostra o IP do ESP32 (normalmente 192.168.4.1)
  } else {
    Serial.println("Erro ao criar a rede");
    return; // Sai do setup se falhar
  }
 // Chama rotas do servidor web
  configurarRotas();
}

void loop() {
  server.handleClient(); // Mantém o servidor web rodando e responde às requisições
}