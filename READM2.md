# ESP32 + DHT22 (ou DHT11) — Web Server em modo AP com Página HTML (SPIFFS)

Projeto que lê **temperatura** e **umidade** com um sensor DHT no **ESP32**, cria uma rede Wi-Fi própria (modo **Access Point**) e exibe os valores em tempo real via página web servida pelo próprio microcontrolador (arquivos no **SPIFFS**). A comunicação front-end ⇄ ESP32 é feita por **JSON** através da rota `/api/readings`.

---

## 🧰 Tecnologias & Ferramentas

- **Placa:** ESP32 
- **Editor/IDE:** Visual Studio Code
- **Build/Upload:** PlatformIO
- **Controle de versão:** Git & GitHub
- **Sistema de arquivos:** SPIFFS
- **Servidor HTTP:** `WebServer` 
- **Front-end:** HTML + CSS + JavaScript 
- **Formato de dados:** JSON
- **Drivers USB:** CP210x - Baseada na placa usada

---

## 🔌 Hardware

- ***ESP32 ***
- **DHT22** (ou **DHT11**)
- **Resistor** de **10 kΩ** (pull-up no pino DATA)
- **Jumpers** e **protoboard**

### Ligações (DHT no GPIO 4)

| DHT | ESP32 |
|-----|-------|
| VCC | 3.3V  |
| GND | GND   |
| DATA| GPIO **4** |

> **Pull-up:** Resistor **entre DATA e 3.3V**.  
> **Tipo de sensor:** se usar **DHT11**, ajuste no código `#define DHTTYPE DHT11`.

---

## 🗂️ Estrutura do Projeto
<img width="682" height="570" alt="image" src="https://github.com/user-attachments/assets/d329539a-7167-447b-b51d-8a866d7b5a8c" />


---

## ⚙️ Configuração (PlatformIO)

`platformio.ini`:

    [env:esp32doit-devkit-v1]
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    monitor_speed = 115200
    board_build.filesystem = spiffs

    lib_deps =
      adafruit/DHT sensor library @ ^1.4.4
      adafruit/Adafruit Unified Sensor @ ^1.1.14

> **Drivers USB:** instale CP210x.
---

## 🧩 Trechos-chave do Código

**`main.cpp` (resumo):**
```
   #include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>//criar serivdor wev
#include <SPIFFS.h>//pegar o html
#include "routs/routs.h"

#define AP_SSID  "ESP32_REDE"
#define AP_SENHA "12345678"

WebServer server(80);  
// cria um abjeto que representa o servidor

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
```

**`routs.cpp` (API/páginas e DHT):**
```
 #include <Arduino.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <DHT.h>
#include "routs.h"

// === Ajuste aqui o seu sensor ===
#define DHTPIN   4
#define DHTTYPE  DHT22    

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

void enviarPaginaPrincipal() {
  enviarArquivo("/index.html", "text/html");
}

void enviarEstilo() {
  enviarArquivo("/style.css", "text/css");
}

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

void rotaNaoEncontrada() {
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
```

**`index.html` (script do front-end):**
```
   <script>
    async function leituraAtual() {
      try {
        const r = await fetch('/api/leitura', { cache: 'no-store' });
        if (!r.ok) throw new Error('HTTP ' + r.status);
        const { temperatura, umidade } = await r.json();

        document.getElementById('temperatura').textContent =
          (typeof temperatura === 'number') ? temperatura.toFixed(1) : '--';
        document.getElementById('umidade').textContent =
          (typeof umidade === 'number') ? umidade.toFixed(1) : '--';
      } catch (e) {
        console.log(e);
      }
    }
    leituraAtual();
    setInterval(leituraAtual, 2000);
  </script>
```
---

## 🖥️ Como Rodar

### 1) Clonar

    git clone https://github.com/<usuario>/<repo>.git](https://github.com/Erick-143/Projeto-Esp32-sensores-umidade-e-temperatura.git)

### 2) Abrir no VS Code (PlatformIO instalado)

- Verifique drivers USB instalados (CP210x/CH340).  
- Selecione a **porta** correta.

### 3) Compilar & Gravar o firmware

- **PlatformIO → Upload**  
  ou

        pio run -t upload

### 4) Gravar os arquivos do site (SPIFFS)

- **PlatformIO → Upload Filesystem Image**  
  ou

        pio run -t uploadfs

> **Importante:** sempre que mudar algo em `data/`, rode `uploadfs` novamente.


## 🔧 Configurações Rápidas

- **SSID/Senha do AP:** altere em `main.cpp`:

        #define AP_SSID  "MeuESP32"
        #define AP_SENHA "minhaSenha123"

- **Pino do DHT:** altere `#define DHTPIN 4`  
- **Tipo de sensor:** `#define DHTTYPE DHT22` (ou `DHT11`)
