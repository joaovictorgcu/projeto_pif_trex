# Nome do executável
TARGET = trex_runner

# Lista dos arquivos-fonte
SRCS = main.c dinossauro.c cacto.c cenario.c nuvem.c passaro.c scores.c menu.c

# Flags comuns
CFLAGS = -Wall -std=c99 -O2

# Detecta o sistema operacional
ifeq ($(OS),Windows_NT)
    # Windows (MinGW/MSYS2)
    CC = gcc
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -lm
    EXE = .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        # macOS
        CC = gcc
        LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo -lm
        EXE =
    else
        # Linux
        CC = gcc
        LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
        EXE =
    endif
endif

# Compila tudo
all: $(TARGET)$(EXE)

$(TARGET)$(EXE): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

# Limpa os arquivos gerados
clean:
	rm -f $(TARGET)$(EXE)

.PHONY: all clean
