
# T-Rex Runner 

Projeto da cadeira de Programação Imperativa e Funcional

## 📚 Descrição do Projeto

Este projeto consiste na implementação do clássico jogo **T-Rex Runner**, popularmente conhecido por aparecer no navegador Google Chrome quando não há conexão com a internet. Desenvolvido como parte da disciplina **Programação Imperativa e Funcional**, o objetivo principal foi aplicar conceitos fundamentais da programação estruturada utilizando a linguagem **C**.

## 👨‍💻 Autores

- Luis Eduardo Bérard de Paiva Moura Rodrigues  
- Pablo José Pellegrino Cintra  
- João Victor Guimarães Cacalcanti Uchoa  

## 🚀 Como Rodar o Jogo T-Rex Runner

### ✅ Pré-requisitos

Antes de rodar o jogo, é necessário ter instalado no seu sistema:

- **Compilador C** (GCC recomendado)  
- **Biblioteca Raylib** ([https://www.raylib.com/](https://www.raylib.com/))  

---

## 🐧 Instruções para Linux

1. **Instalar dependências:**

```bash
sudo apt update
sudo apt install build-essential gcc make
sudo apt install libraylib-dev
```

> Obs.: Se `libraylib-dev` não estiver disponível, instale manualmente seguindo as instruções no site oficial da Raylib.

2. **Clonar o repositório:**

```bash
git clone https://github.com/seu-usuario/seu-repositorio.git
cd seu-repositorio
```

3. **Compilar o jogo:**

```bash
make
```

4. **Rodar o jogo:**

```bash
./trex_runner
```

5. **(Opcional) Limpar arquivos gerados:**

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

## 🪟 Instruções para Windows (via MSYS2 ou MinGW)

1. **Instalar MSYS2:**  
Baixe e instale em: [https://www.msys2.org/](https://www.msys2.org/)

2. **Abrir o terminal MSYS2 MinGW 64-bit.**

3. **Instalar dependências:**

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-raylib
```

4. **Navegar até a pasta do projeto:**

```bash
cd /c/Caminho/Para/Seu/Projeto
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

- **Erro `raylib not found`:**  
Verifique se a biblioteca Raylib está corretamente instalada no seu sistema.

- **Erro de permissões (`Permission denied`) ao rodar no Linux/macOS:**  
Execute o comando abaixo para dar permissão de execução:

```bash
chmod +x trex_runner
./trex_runner
```

---

## 🎮 Divirta-se jogando!
