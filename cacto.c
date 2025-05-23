#include "cacto.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA_CACTO 25.0f      
#define ALTURA_MIN 40.0f    
#define ALTURA_MAX 60.0f         


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

void inserir_cactos_aleatorios(ListaCactos *lista, float x, float y_base) {
    float altura1 = ALTURA_MIN + ((float)rand() / RAND_MAX) * (ALTURA_MAX - ALTURA_MIN);
    inserir_cacto_final(lista, x, y_base - altura1, LARGURA_CACTO, altura1);

    if (rand() % 5 == 0) {  // 0 em 0,1,2,3,4 (20%)
        float altura2 = ALTURA_MIN + ((float)rand() / RAND_MAX) * (ALTURA_MAX - ALTURA_MIN);
        inserir_cacto_final(lista, x + LARGURA_CACTO + 4, y_base - altura2, LARGURA_CACTO, altura2); // +4 para dar espaço entre eles
    }
}

// mexe cacto esquerda p direita
void atualizar_cactos(ListaCactos *lista, float velocidade) {
    for (Cacto *atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        atual->x -= velocidade;
    }
}

void desenhar_cactos(ListaCactos *lista, Texture2D sprite) {
    for (Cacto *atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        DrawTexturePro(
            sprite,
            (Rectangle){0, 0, sprite.width, sprite.height},
            (Rectangle){atual->x, atual->y, atual->largura, atual->altura},
            (Vector2){0, 0}, 0, WHITE
        );
    }
}

// retornar 1 se tiver colisão entre o dinossauro e qualquer cacto
int verificar_colisao(Dinossauro *dino, ListaCactos *lista) {
    Rectangle retDino = { dino->x, dino->y, dino->largura, dino->altura };
    for (Cacto *atual = lista->inicio; atual != NULL; atual = atual->proximo) {
        Rectangle retCacto = { atual->x, atual->y, atual->largura, atual->altura };
        if (CheckCollisionRecs(retDino, retCacto)) return 1;
    }
    return 0;
}

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
