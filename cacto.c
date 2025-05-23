#include "cacto.h"
#include <stdlib.h>

// Insere um novo cacto no final da lista
void inserir_cacto_final(ListaCactos *lista, float x, float y, float largura, float altura) {
    Cacto *novo = (Cacto*)malloc(sizeof(Cacto));
    novo->x = x;
    novo->y = y;
    novo->largura = largura;
    novo->altura = altura;
    novo->passou = 0;
    novo->proximo = NULL;
    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        Cacto *aux = lista->inicio;
        while (aux->proximo != NULL) {
            aux = aux->proximo;
        }
        aux->proximo = novo;
    }
    lista->tamanho++;
}

// Atualiza a posição dos cactos (movimento para a esquerda)
void atualizar_cactos(ListaCactos *lista, float velocidade) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        atual->x -= velocidade;
        atual = atual->proximo;
    }
}

// Desenha todos os cactos da lista
void desenhar_cactos(ListaCactos *lista, Texture2D sprite) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        if (sprite.id == 0) {
            Color verdeBase = (Color){34, 139, 34, 255};
            Color verdeClaro = (Color){50, 205, 50, 255};
            DrawRectangleGradientV(atual->x, atual->y, atual->largura, atual->altura, verdeClaro, verdeBase);
            DrawRectangleLinesEx((Rectangle){atual->x, atual->y, atual->largura, atual->altura}, 2, (Color){0, 100, 0, 255});
        } else {
            DrawTexturePro(sprite,
                (Rectangle){0, 0, sprite.width, sprite.height},
                (Rectangle){atual->x, atual->y, atual->largura, atual->altura},
                (Vector2){0, 0}, 0, WHITE);
        }
        atual = atual->proximo;
    }
}

// Verifica colisão entre o dinossauro e qualquer cacto da lista
int verificar_colisao(Dinossauro *dino, ListaCactos *lista) {
    Cacto *atual = lista->inicio;
    Rectangle retDino = { dino->x, dino->y, dino->largura, dino->altura };
    while (atual != NULL) {
        Rectangle retCacto = { atual->x, atual->y, atual->largura, atual->altura };
        if (CheckCollisionRecs(retDino, retCacto)) return 1;
        atual = atual->proximo;
    }
    return 0;
}

// Libera toda a memória da lista de cactos
void liberar_cactos(ListaCactos *lista) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        Cacto *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    lista->inicio = NULL;
    lista->tamanho = 0;
}
