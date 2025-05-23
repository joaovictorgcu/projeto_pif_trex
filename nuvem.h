#ifndef NUVEM_H
#define NUVEM_H

#include "raylib.h"

#define MAX_NUVENS 10

typedef struct nuvem {
    Vector2 posicao;
    float escala;
    float velocidade;
    Color cor;
} Nuvem;

// Inicializa o array de nuvens
void inicializar_nuvens(Nuvem nuvens[], int largura, int altura);

// Atualiza a posição das nuvens
void atualizar_nuvens(Nuvem nuvens[], int largura);

// Desenha uma nuvem em determinada posição, escala e cor
void desenhar_nuvem(Vector2 posicao, float escala, Color cor);

#endif
