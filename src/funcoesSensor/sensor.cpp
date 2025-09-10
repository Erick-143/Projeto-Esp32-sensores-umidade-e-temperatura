#include <SPIFFS.h>
#include "routs/routs.h"
#include <Arduino.h>

void handleRoot() {
  //chamada quando alguem acessa a página principal
  File file = SPIFFS.open("/index.html", "r");//r de leitura
  if (!file) { // Se não conseguir abrir o arquivo
    server.send(500, "text/plain", "Erro ao abrir index.html"); // Retorna erro 500
    return;
  }
  server.streamFile(file, "text/html"); //pega o conteudo do arquivo e envia como resposta
  file.close(); // Fecha o arquivo após enviar, o usuário vê o conteúdo do index.html
}
// Função para lidar com a rota "/ligar" (liga o LED)
void ligarLED() {
  digitalWrite(LED_BUILTIN, HIGH); // Liga o LED conectado ao GPIO2
  server.send(200, "text/plain", "LED ligado"); // Responde com texto simples
}

// Função para lidar com a rota "/desligar" (desliga o LED)
void desligarLED() {
  digitalWrite(LED_BUILTIN, LOW); // Desliga o LED conectado ao GPIO2
  server.send(200, "text/plain", "LED desligado"); // Responde com texto simples
}