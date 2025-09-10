#include "routs.h"
#include <Arduino.h>

void configurarRotas() {
    server.on("/", handleRoot);
    server.on("/ligar", ligarLED);
    server.on("/desligar", desligarLED);
    server.begin();
    Serial.println("Servidor web iniciado!");
}