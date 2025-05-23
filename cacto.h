#ifndef CACTO_H
#define CACTO_H

#include "raylib.h"
#include "dinossauro.h"

typedef struct cacto {
    float x, y;
    float largura, altura;
    int passou;
    struct cacto *proximo;
} Cacto;

typedef struct listaCactos {
    int tamanho;
    Cacto *inicio;
} ListaCactos;

void inserir_cacto_final(ListaCactos *lista, float x, float y, float largura, float altura);
void atualizar_cactos(ListaCactos *lista, float velocidade);
void desenhar_cactos(ListaCactos *lista, Texture2D sprite);
int verificar_colisao(Dinossauro *dino, ListaCactos *lista);
void liberar_cactos(ListaCactos *lista);

#endif
