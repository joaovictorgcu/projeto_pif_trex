#ifndef CACTO_H
#define CACTO_H

#include "raylib.h"
#include "dinossauro.h" // Para usar o tipo Dinossauro

// Estrutura do cacto
typedef struct cacto {
    float x, y;
    float largura, altura;
    int passou;
    struct cacto *proximo;
} Cacto;

// Lista encadeada de cactos
typedef struct listaCactos {
    int tamanho;
    Cacto *inicio;
} ListaCactos;

// Funções dos cactos
void inserir_cacto_final(ListaCactos *lista, float x, float y, float largura, float altura);
void atualizar_cactos(ListaCactos *lista, float velocidade);
void desenhar_cactos(ListaCactos *lista, Texture2D sprite);
int verificar_colisao(Dinossauro *dino, ListaCactos *lista);
void liberar_cactos(ListaCactos *lista);

#endif
