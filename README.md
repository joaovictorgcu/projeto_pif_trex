
# 🦖 T-Rex Runner

Projeto desenvolvido para a cadeira de **Programação Imperativa e Funcional**, que consiste na implementação do clássico jogo **T-Rex Runner**, famoso por aparecer no navegador Google Chrome quando não há conexão com a internet.

## 📚 Descrição

O objetivo deste projeto é aplicar conceitos fundamentais da programação estruturada utilizando a linguagem **C**, além de práticas com desenvolvimento gráfico por meio da biblioteca **Raylib**.

## 👨‍💻 Autores

- Luis Eduardo Bérard de Paiva Moura Rodrigues  
- Pablo José Pellegrino Cintra  
- João Victor Guimarães Cacalcanti Uchoa  

---

## 🚀 Como Rodar o Jogo

### ✅ Pré-requisitos

- **Compilador C** (GCC recomendado)  
- **Biblioteca Raylib** ([https://www.raylib.com/](https://www.raylib.com/))  
- **Git** para clonar o repositório  

---

## 🔥 Passo inicial (para todos os sistemas)

Clone o repositório:

```bash
git clone https://github.com/joaovictorgcu/projeto_pif_trex.git
cd projeto_pif_trex
```

---

## 🐧 Instruções para Linux

1. **Instalar dependências:**

```bash
sudo apt update
sudo apt install build-essential gcc make
sudo apt install libraylib-dev
```

> ⚠️ Se `libraylib-dev` não estiver disponível, siga as instruções de instalação manual no site oficial da [Raylib](https://www.raylib.com/).

2. **Compilar o jogo:**

```bash
make
```

3. **Rodar o jogo:**

```bash
./trex_runner
```

4. **(Opcional) Limpar arquivos gerados:**

```bash
make clean
```

---

## 🍎 Instruções para macOS

1. **Instalar dependências:**

```bash
brew install raylib
brew install make gcc
```

2. **Compilar o jogo:**

```bash
make
```

3. **Rodar o jogo:**

```bash
./trex_runner
```

4. **(Opcional) Limpar arquivos gerados:**

```bash
make clean
```

---

## 🪟 Instruções para Windows (via MSYS2)

1. **Instalar MSYS2:**  
Baixe em: [https://www.msys2.org/](https://www.msys2.org/)

2. **Abrir o terminal `MSYS2 MinGW 64-bit`.**

3. **Atualizar o sistema e instalar dependências:**

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-raylib git
```

4. **Navegar até a pasta do projeto (exemplo abaixo, ajuste conforme seu caminho):**

```bash
cd /c/Caminho/Para/Seu/projeto_pif_trex
```

5. **Compilar o jogo:**

```bash
make
```

6. **Rodar o jogo:**

```bash
./trex_runner.exe
```

7. **(Opcional) Limpar arquivos gerados:**

```bash
make clean
```

---

## ⚠️ Erros Comuns e Soluções

- **`raylib not found`**  
Verifique se a biblioteca Raylib está corretamente instalada e se o compilador está configurado para encontrá-la.

- **`Permission denied` (Linux/macOS):**  
Se não conseguir executar o jogo, dê permissão:

```bash
chmod +x trex_runner
./trex_runner
```

- **No Windows, erro de 'command not found' ao usar `make`:**  
Certifique-se de estar no terminal correto: `MSYS2 MinGW 64-bit`, não use o `MSYS` padrão.

---

## 🎮 Divirta-se jogando!  
Se gostar, ⭐ deixe uma estrela no repositório!
