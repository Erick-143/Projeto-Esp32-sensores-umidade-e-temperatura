#include <Arduino.h>      // Biblioteca principal do Arduino, inclui funções básicas
#include <WiFi.h>         // Biblioteca para funcionalidades WiFi (modo AP e STA)
#include <WebServer.h>    // Biblioteca para criar um servidor web simples
#include <SPIFFS.h>       // Biblioteca para acessar o sistema de arquivos SPIFFS

// Define o nome e a senha da rede WiFi que o ESP32 vai criar (modo Access Point)
#define AP_SSID "ESP32_REDE"       // Nome da rede WiFi (SSID)
#define AP_SENHA "12345678"        // Senha da rede WiFi

WebServer server(80); // Cria um servidor web na porta 80 (HTTP padrão)

// Função para lidar com requisições à página principal "/"
void handleRoot() {
  // Tenta abrir o arquivo index.html do SPIFFS em modo leitura
  File file = SPIFFS.open("/index.html", "r");
  if (!file) { // Se não conseguir abrir o arquivo
    server.send(500, "text/plain", "Erro ao abrir index.html"); // Retorna erro 500
    return;
  }
  server.streamFile(file, "text/html"); // Envia o conteúdo do arquivo como resposta HTML
  file.close(); // Fecha o arquivo após enviar
}

// Função para lidar com a rota "/ligar" (liga o LED)
void ligarLED() {
  digitalWrite(2, HIGH); // Liga o LED conectado ao GPIO2
  server.send(200, "text/plain", "LED ligado"); // Responde com texto simples
}

// Função para lidar com a rota "/desligar" (desliga o LED)
void desligarLED() {
  digitalWrite(2, LOW); // Desliga o LED conectado ao GPIO2
  server.send(200, "text/plain", "LED desligado"); // Responde com texto simples
}

void setup() {
  Serial.begin(115200); // Inicializa a comunicação serial para debug
  pinMode(2, OUTPUT);   // Define o pino 2 (GPIO2) como saída (para o LED)

  // Inicializa o sistema de arquivos SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao montar SPIFFS"); // Mostra erro se não conseguir montar
    return; // Sai do setup se falhar
  }

  // Cria uma rede WiFi própria (Access Point)
  WiFi.softAP(AP_SSID, AP_SENHA);
  Serial.println("Rede criada!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.softAPIP());  // Mostra o IP do ESP32 (normalmente 192.168.4.1)

  // Define as rotas do servidor web
  server.on("/", handleRoot);         // Página principal
  server.on("/ligar", ligarLED);      // Rota para ligar o LED
  server.on("/desligar", desligarLED);// Rota para desligar o LED
  server.begin();                     // Inicia o servidor web
  Serial.println("Servidor web iniciado!");
}

void loop() {
  server.handleClient(); // Mantém o servidor web rodando e responde às requisições
}