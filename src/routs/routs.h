#pragma once
#include <WebServer.h>

// Declarações externas para o objeto server e as funções das rotas
extern WebServer server;
void handleRoot();
void ligarLED();
void desligarLED();
void configurarRotas();