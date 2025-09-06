#include <Arduino.h>
#include <DHT.h>

// Defina o pino onde o DHT11 está conectado
#define DHTPIN 4       // exemplo: GPIO4
#define DHTTYPE DHT11  // Tipo do sensor: DHT11 ou DHT22

DHT dht(DHTPIN, DHTTYPE);
//teste h

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Iniciando leitura do DHT11...");
}

void loop() {
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
    Serial.println("Falha ao ler do sensor DHT!");
  } else {
    Serial.printf("Temperatura: %.1f °C | Umidade: %.1f %%\n", temp, hum);
  }

  delay(2000); // espera 2s antes da próxima leitura
}
