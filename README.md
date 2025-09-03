# Projeto-Esp32-sensores-umidade-e-temperatura
# 🚀 Guia Completo: Git e GitHub para Iniciantes  

Bem-vindo(a)! Este guia foi feito para te ajudar a começar com **Git** e **GitHub**, de forma simples e prática.  

---

## 📥 1. Instalando o Git  

👉 **Windows / Linux / macOS**  
1. Acesse o site oficial
 ```bash
 [https://git-scm.com/downloads](https://git-scm.com/downloads)
```
4. Baixe o instalador para o seu sistema operacional.  
5. Durante a instalação, pode deixar as opções padrão.  
6. Após instalar, abra o terminal (ou Git Bash no Windows) e digite:  
   ```bash
   git --version
   Se aparecer a versão, deu tudo certo ✅
## 🖥️ 2. Criando uma conta no GitHub

Entre em 
```bash
https://github.com
```

Clique em Sign up e crie sua conta gratuita.

Confirme seu e-mail.

## ⚙️ 3. Configurando o Git

No terminal do seu pc, configure seu nome e e-mail (eles aparecerão nos commits):
```bash
git config --global user.name "Seu Nome"
git config --global user.email "seuemail@example.com"
```
Para conferir se deu certo:
```bash
git config --list
```
## 💻 4. Clonando um repositório existente no visual estudio code

Vá no prompt de comando do seu pc e digite 
```bash
git clone https://github.com/Erick-143/Projeto-Esp32-sensores-umidade-e-temperatura.git
```
A ideia é baixar as pastas do repositório remoto para o seu computador.

## 📝 5. Usando o projeto no VS Code

Abra o Visual Studio Code.

Clique em File > Open Folder e selecione a pasta do repositório clonado.

O VS Code já vai reconhecer que é um projeto Git.

## 🔄 6. Trabalhando no projeto com VS Code
➕ Fazendo alterações e commit

Edite os arquivos que quiser.

Vá até o menu lateral Source Control (raminho).

Clique no + para adicionar os arquivos alterados (equivale a git add).

Escreva uma mensagem de commit.

Clique no ✔️ para salvar (equivale a git commit).

## 🌍 7. Enviando mudanças para o GitHub

No VS Code, clique em Sync Changes ou Push.
Isso envia suas alterações para o repositório remoto.

## ⬇️ 8. Pegando alterações do repositório original

Para atualizar seu repositório local com mudanças feitas por outros:

No VS Code, clique em Pull.

Ou use no terminal:
```bash
git pull
```
## 🌿 9. Trabalhando com branches

No canto inferior esquerdo do VS Code aparece a branch atual.

Clique nela para criar uma nova branch ou mudar para outra.

Isso é o mesmo que usar git checkout -b nome no terminal.

-----------------------------------------------------------------------
## Resumo das pastas do PlatformIO

📄 platformio.ini

Arquivo de configuração do projeto. Diz qual placa você usa, qual framework (Arduino/ESP-IDF), quais bibliotecas baixar e opções de compilação. É o “painel de controle” do projeto.

📁 src/

Onde fica o código principal do seu firmware (os arquivos que o dispositivo vai executar). Pense como a pasta “programa”.

📁 include/

Arquivos de cabeçalho (declarações e definições que outros arquivos usam). É útil para organizar nomes e estruturas que aparecem em vários arquivos.

📁 lib/

Bibliotecas que você mesmo criou e quer manter junto do projeto. Coloque aqui código reaproveitável (pequenos módulos que seu projeto usa).

📁 data/

Arquivos que o dispositivo pode ler em tempo de execução (páginas web, imagens, arquivos JSON, etc.) — usados quando você grava um sistema de arquivos no dispositivo (SPIFFS/LittleFS).

📁 test/

Arquivos de teste automatizado. Serve para escrever verificações que garantem que partes do seu código continuam funcionando.

📁 .pio/ ou .pioenvs/

Pasta que o PlatformIO cria automaticamente com o resultado da compilação (binários, objetos, pastas por ambiente). É gerada pelo sistema — você normalmente não precisa mexer nela.

📁 .pio/libdeps/ (ou .piolibdeps/)

Local onde o PlatformIO baixa as bibliotecas que você listou em platformio.ini. Cada ambiente pode ter suas próprias libs aqui.

📁 .vscode/

Configurações do Visual Studio Code relacionadas ao projeto (atalhos de debug, tarefas). Usada só se você usar o VS Code.

📄 README.md / LICENSE / .gitignore

Arquivos de documentação e suporte:

README.md: explica o que o projeto faz e como usar;

LICENSE: licença do projeto;

.gitignore: lista de arquivos/pastas que o Git deve ignorar.

📁 docs/ (opcional)

Documentação extra do projeto: manuais, imagens, tutoriais e explicações mais longas.

## Baixar o PlatformIO no Visual Studio Code + drivers

## 🧩 1. Instalar a extensão PlatformIO IDE

No VS Code, clique em Extensões (ícone de quadradinhos à esquerda).

Pesquise PlatformIO IDE e clique em Instalar.

Depois da instalação, abra o ícone PlatformIO (casa/roda no menu lateral) para confirmar que abriu.

## 🔌 2. Instalar drivers USB do seu ESP32
vá no link abaixo e baixe o CPX210x Windows Drivers
```bash
https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads
```
Extraia a pasta e baixe o app de acordo com o seu sistema operacional
```bash
CP210x_VCP_Installer_x64
CP210x_VCP_Installer_x86
```
## 📚 3. Adicionar biblioteca DHT11 ao projeto 
Na pasta que vcs vão clonar, a biblioteca já está adicionada.
Adicione essa biblioteva caso você vá criar outra pasta separada do projeto

No platformio.ini do projeto, adicione em lib_deps:
```bash
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
  adafruit/DHT sensor library
  adafruit/Adafruit Unified Sensor
```



