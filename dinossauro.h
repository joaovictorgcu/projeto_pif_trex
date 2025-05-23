#ifndef DINOSSAURO_H
#define DINOSSAURO_H

#include "raylib.h"

typedef struct dinossauro {
    float x, y;
    float largura, altura;
    float velocidadeY;
    int pulando;
    Texture2D sprite;
} Dinossauro;


void desenhar_dinossauro(Dinossauro *dino);
void inicializar_dinossauro(Dinossauro *dino, int x, int y, int largura, int altura, const char* caminhoSprite);
void atualizar_dinossauro(Dinossauro *dino, float gravidade, float forcaPulo, int alturaTela, int pular);

#endif
