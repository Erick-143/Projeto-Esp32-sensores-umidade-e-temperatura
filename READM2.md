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
<img width="343" height="432" alt="image" src="https://github.com/user-attachments/assets/bbe46d3b-b62b-40ac-919b-5fa7faf6a57f" />



---
## 🌐 Como funciona (visão rápida)
## 1.
O ESP32 sobe uma rede AP (ex.: ESP32_REDE) e inicia um web server.

## 2.
A página index.html + style.css vêm do SPIFFS.

## 3.
O front-end chama GET /api/leitura a cada 1 s.

## 4.
O back-end lê o DHT e responde { "temperatura": x, "umidade": y }.

## 5.
A página atualiza os <span> e plota um gráfico 0..100 (escala comum a T e U):

° Vermelho = Temperatura

° Azul = Umidade

## ⚙️ Configuração (PlatformIO)

`platformio.ini`:
```
[env:esp32c3-supermini]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
upload_speed = 460800
board_build.filesystem = spiffs

build_flags = 
  -DARDUINO_USB_CDC_ON_BOOT=1 
  -DARDUINO_USB_MODE=1

lib_deps =
  adafruit/DHT sensor library @ ^1.4.4
  adafruit/Adafruit Unified Sensor @ ^1.1.14

> **Drivers USB:** instale CP210x.
```
## 🧩 Trechos-chave do Código

**`routs.cpp` (resumo):**
```
static void routeStatic(const char* url, const char* fsPath, const char* contentType) {
  server.on(url, HTTP_GET, [fsPath, contentType]() {
    File f = SPIFFS.open(fsPath, "r");
    if (!f) { server.send(404, "text/plain", "Not found"); return; }
    server.streamFile(f, contentType);
    f.close();
  });
}

```
Uso típico:
```
routeStatic("/",                   "/index.html",   "text/html");
routeStatic("/style.css",          "/style.css",    "text/css");
routeStatic("/assets/LogoUema.png","/LogoUema.png", "image/png"); // arquivo físico é /LogoUema.png
server.on("/api/leitura", HTTP_GET, enviarLeituras);
```

**`API` (/api/leitura)(JSON):**
```
static void enviarLeituras() {
  float temperatura = dht.readTemperature(); // °C
  float umidade     = dht.readHumidity();    // %
  if (isnan(temperatura) || isnan(umidade)) {
    server.sendHeader("Cache-Control", "no-store");
    server.send(500, "application/json", "{\"error\":\"sensor\"}");
    return;
  }
  char buf[96];
  snprintf(buf, sizeof(buf),
           "{\"temperatura\":%.1f,\"umidade\":%.1f}",
           temperatura, umidade);
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", buf);
}

**`API` (/api/leitura)(JSON):**
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
  } catch (e) { console.log(e); }
}
leituraAtual();
setInterval(leituraAtual, 1000); // 1 s
</script>



  
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

## ▶️ Como rodar
1. Conecte o ESP32 ao PC (drivers CP210x/CH340 se precisar).

2. No VS Code (PlatformIO):

° Upload (firmware)

° Upload Filesystem Image (arquivos do data/).

3. Abra o Serial Monitor (115200) e veja:

° “Rede criada!”

° IP do AP (geralmente 192.168.4.1)

No celular/notebook, conecte-se à rede AP do ESP32.

Acesse no navegador: http://192.168.4.1
 ✅
 
## 🔧 Configurações rápidas
° Tipo de sensor: #define DHTTYPE DHT22 → troque para DHT11 se precisar.

° Pino do DHT: #define DHTPIN 4 (ajuste conforme seu hardware).

° SSID/Senha do AP: em main.cpp (AP_SSID, AP_SENHA).

° Janela do gráfico: graf_MAX_PONTOS (ex.: 120 pontos ≈ 2 min a 1 s).

° Cores: vermelho = Temperatura, azul = Umidade (mantidas no HTML e no JS).
