# Projeto-Esp32-sensores-umidade-e-temperatura
# 🚀 Guia Completo: Git e GitHub para Iniciantes  

Bem-vindo(a)! Este guia foi feito para te ajudar a começar com **Git** e **GitHub**, de forma simples e prática.  

---

## 📥 1. Instalando o Git  

👉 **Windows / Linux / macOS**  
1. Acesse o site oficial: [https://git-scm.com/downloads](https://git-scm.com/downloads)  
2. Baixe o instalador para o seu sistema operacional.  
3. Durante a instalação, pode deixar as opções padrão.  
4. Após instalar, abra o terminal (ou Git Bash no Windows) e digite:  
   ```bash
   git --version
   Se aparecer a versão, deu tudo certo ✅
🖥️ 2. Criando uma conta no GitHub

Entre em https://github.com

Clique em Sign up e crie sua conta gratuita.

Confirme seu e-mail.

⚙️ 3. Configurando o Git

No terminal do seu pc, configure seu nome e e-mail (eles aparecerão nos commits):
```bash
git config --global user.name "Seu Nome"
git config --global user.email "seuemail@example.com"
```
Para conferir se deu certo:
```bash
git config --list
```
💻 4. Clonando um repositório existente no visual estudio code

Vá no prompt de comando do seu pc e digite 
```bash
git clone https://github.com/Erick-143/Projeto-Esp32-sensores-umidade-e-temperatura.git
```
A ideia é baixar as pastas do repositório remoto para o seu computador.

📝 5. Usando o projeto no VS Code

Abra o Visual Studio Code.

Clique em File > Open Folder e selecione a pasta do repositório clonado.

O VS Code já vai reconhecer que é um projeto Git.

🔄 6. Trabalhando no projeto com VS Code
➕ Fazendo alterações e commit

Edite os arquivos que quiser.

Vá até o menu lateral Source Control (raminho).

Clique no + para adicionar os arquivos alterados (equivale a git add).

Escreva uma mensagem de commit.

Clique no ✔️ para salvar (equivale a git commit).

🌍 Enviando mudanças para o GitHub

No VS Code, clique em Sync Changes ou Push.
Isso envia suas alterações para o repositório remoto.

⬇️ Pegando alterações do repositório original

Para atualizar seu repositório local com mudanças feitas por outros:

No VS Code, clique em Pull.

Ou use no terminal:
```bash
git pull
```
🌿 Trabalhando com branches

No canto inferior esquerdo do VS Code aparece a branch atual.

Clique nela para criar uma nova branch ou mudar para outra.

Isso é o mesmo que usar git checkout -b nome no terminal.


