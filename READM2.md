# ESP32 + DHT22 (ou DHT11) — Web Server em modo AP com Página HTML (SPIFFS)

Projeto que lê **temperatura** e **umidade** com um sensor DHT no **ESP32**, cria uma rede Wi-Fi própria (modo **Access Point**) e exibe os valores em tempo real via página web servida pelo próprio microcontrolador (arquivos no **SPIFFS**). A comunicação front-end ⇄ ESP32 é feita por **JSON** através da rota `/api/readings`.

---

## 🧰 Tecnologias & Ferramentas

- **Placa:** ESP32 
- **Editor/IDE:** Visual Studio Code
- **Build/Upload:** PlatformIO
- **Controle de versão:** Git & GitHub
- **Sistema de arquivos:** SPIFFS
- **Servidor HTTP:** `WebServer` (Arduino)
- **Front-end:** HTML + CSS + JavaScript (fetch)
- **Formato de dados:** JSON
- **Drivers USB:** CP210x ou CH340 (dependendo da sua placa)

---

## 🔌 Hardware

- **ESP32 ***
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

    #include <Arduino.h>
    #include <WiFi.h>
    #include <WebServer.h>
    #include <SPIFFS.h>
    #include "routs/routs.h"

    #define AP_SSID  "ESP32_REDE"
    #define AP_SENHA "12345678"

    WebServer server(80);

    void setup() {
      Serial.begin(115200);
      if (!SPIFFS.begin(true)) { Serial.println("Erro ao montar SPIFFS"); return; }

      if (WiFi.softAP(AP_SSID, AP_SENHA)) {
        Serial.println("Rede criada!");
        Serial.print("IP do ESP32: ");
        Serial.println(WiFi.softAPIP());
      }

      configurarRotas();   // registra rotas e server.begin()
    }

    void loop() {
      server.handleClient();
    }

**`routs.cpp` (API/páginas e DHT):**

    #include <Arduino.h>
    #include <SPIFFS.h>
    #include <WebServer.h>
    #include <DHT.h>
    #include "routs.h"

    #define DHTPIN  4
    #define DHTTYPE DHT22     // troque para DHT11 se necessário
    static DHT dht(DHTPIN, DHTTYPE);

    extern WebServer server;

    static void enviarArquivo(const char* nome, const char* tipo) {
      File f = SPIFFS.open(nome, "r");
      if (!f) { server.send(404, "text/plain", "Arquivo nao encontrado"); return; }
      server.streamFile(f, tipo);
      f.close();
    }

    static void enviarLeituras() {
      float t = dht.readTemperature();
      float h = dht.readHumidity();

      if (isnan(t) || isnan(h)) {
        server.sendHeader("Cache-Control","no-store");
        server.send(500, "application/json", "{\"error\":\"sensor\"}");
        return;
      }

      char buf[96];
      snprintf(buf, sizeof(buf), "{\"temperature\":%.1f,\"humidity\":%.1f}", t, h);
      server.sendHeader("Cache-Control","no-store");
      server.send(200, "application/json", buf);
    }

    void configurarRotas() {
      dht.begin();
      delay(1500);  // estabilização inicial
      server.on("/",          HTTP_GET, []{ enviarArquivo("/index.html","text/html"); });
      server.on("/style.css", HTTP_GET, []{ enviarArquivo("/style.css","text/css"); });
      server.on("/api/readings", HTTP_GET, enviarLeituras);
      server.begin();
    }

**`index.html` (script do front-end):**

    <script>
    async function refreshReadings() {
      try {
        const r = await fetch('/api/readings', { cache: 'no-store' });
        if (!r.ok) throw new Error('HTTP ' + r.status);
        const data = await r.json();
        const temperature = Number(data.temperature);
        const humidity    = Number(data.humidity);

        document.getElementById('temperatura').textContent =
          Number.isFinite(temperature) ? temperature.toFixed(1) : '--';
        document.getElementById('umidade').textContent =
          Number.isFinite(humidity) ? humidity.toFixed(1) : '--';
      } catch (e) { console.log(e); }
    }
    refreshReadings();
    setInterval(refreshReadings, 2000);
    </script>

---

## 🖥️ Como Rodar

### 1) Clonar

    git clone [https://github.com/<usuario>/<repo>.git](https://github.com/Erick-143/Projeto-Esp32-sensores-umidade-e-temperatura.git)

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
