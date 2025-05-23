#ifndef PASSARO_H
#define PASSARO_H

#include "raylib.h"

#define MAX_PASSAROS 15

typedef struct passaro {
    Vector2 posicao;
    float velocidade;
    float tamanho;
    float tempoAnimacao;
} Passaro;

// Inicializa o array de pássaros
void inicializar_passaros(Passaro passaros[], int largura, int altura);

// Atualiza a posição e animação dos pássaros
void atualizar_passaros(Passaro passaros[], int largura);

// Desenha um pássaro na tela
void desenhar_passaro(Vector2 posicao, float tamanho, float tempo);

#endif
