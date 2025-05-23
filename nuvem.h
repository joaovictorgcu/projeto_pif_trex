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


void inicializar_nuvens(Nuvem nuvens[], int largura, int altura);

void atualizar_nuvens(Nuvem nuvens[], int largura);

void desenhar_nuvem(Vector2 posicao, float escala, Color cor);

#endif
