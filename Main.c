#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct dinossauro {
    float x, y;
    float largura, altura;
    float velocidadeY;
    int pulando;
};

typedef struct dinossauro Dino;

struct cacto {
    float x, y;
    float largura, altura;
    struct cacto *proximo;
};

typedef struct cacto Cacto;

struct listaCacto {
    int tamanho;
    Cacto *inicio;
};

typedef struct listaCacto ListaCacto;

void inserir_cacto_final(ListaCacto *lista, float x, float y, float largura, float altura) {
    Cacto *novo = (Cacto*)malloc(sizeof(Cacto));
    if (novo == NULL) {
        printf("Erro ao alocar memória para novo cacto.\n");
        exit(1);
    }
    novo->x = x;
    novo->y = y;
    novo->largura = largura;
    novo->altura = altura;
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

void atualizar_cactos(ListaCacto *lista, float velocidade) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        atual->x -= velocidade;
        atual = atual->proximo;
    }
}

void desenhar_cactos(ListaCacto *lista) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        DrawRectangle(atual->x, atual->y, atual->largura, atual->altura, DARKGREEN);
        atual = atual->proximo;
    }
}

int verificar_colisao(Dino *dino, ListaCacto *lista) {
    Cacto *atual = lista->inicio;
    Rectangle rDino = { dino->x, dino->y, dino->largura, dino->altura };

    while (atual != NULL) {
        Rectangle rCacto = { atual->x, atual->y, atual->largura, atual->altura };
        if (CheckCollisionRecs(rDino, rCacto)) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

int main(void)
{
    int larguraTela = 800;
    int alturaTela = 450;

    InitWindow(larguraTela, alturaTela, "Jogo Dino");
    SetTargetFPS(60);
    srand(time(NULL)); 

    Dino *dinossauro = (Dino*)malloc(sizeof(Dino));
    dinossauro->x = 50;
    dinossauro->y = alturaTela - 60;
    dinossauro->largura = 40;
    dinossauro->altura = 40;
    dinossauro->velocidadeY = 0;
    dinossauro->pulando = 0;

    float gravidade = 0.6f;
    float forcaPulo = -12.0f;

    ListaCacto cactos;
    cactos.inicio = NULL;
    cactos.tamanho = 0;

    int jogoAtivo = 1;
    float distanciaUltimoCacto = 300.0f; 
    float distanciaMinima = 250.0f;

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (jogoAtivo) {
            // Pulo
            if (IsKeyPressed(KEY_SPACE) && !dinossauro->pulando) {
                dinossauro->velocidadeY = forcaPulo;
                dinossauro->pulando = 1;
            }

            // Gravidade
            dinossauro->velocidadeY += gravidade;
            dinossauro->y += dinossauro->velocidadeY;

            if (dinossauro->y >= alturaTela - dinossauro->altura - 20) {
                dinossauro->y = alturaTela - dinossauro->altura - 20;
                dinossauro->velocidadeY = 0;
                dinossauro->pulando = 0;
            }

            // Atualizar cactos
            atualizar_cactos(&cactos, 5.0f);
            distanciaUltimoCacto += 5.0f;

            // Gerar cacto com chance após distância mínima
            if (distanciaUltimoCacto >= distanciaMinima && GetRandomValue(0, 99) < 4) {
                inserir_cacto_final(&cactos, larguraTela, alturaTela - 60, 20, 40);
                distanciaUltimoCacto = 0.0f;
            }

            // Colisão
            if (verificar_colisao(dinossauro, &cactos)) {
                jogoAtivo = 0;
            }
        }

        // Dino
        DrawRectangle(dinossauro->x, dinossauro->y, dinossauro->largura, dinossauro->altura, DARKGRAY);
        DrawRectangle(dinossauro->x + dinossauro->largura - 15, dinossauro->y - 10, 25, 25, DARKGRAY);
        DrawCircle(dinossauro->x + dinossauro->largura + 5, dinossauro->y - 5, 4, RAYWHITE);
        DrawRectangle(dinossauro->x + 5, dinossauro->y + dinossauro->altura, 10, 5, DARKGRAY);
        DrawRectangle(dinossauro->x + dinossauro->largura - 15, dinossauro->y + dinossauro->altura, 10, 5, DARKGRAY);

        // Cactos
        desenhar_cactos(&cactos);

        if (!jogoAtivo) {
            DrawText("GAME OVER!", larguraTela / 2 - 100, alturaTela / 2 - 20, 40, RED);
        }

        EndDrawing();
    }

    // Limpeza
    Cacto *temp;
    while (cactos.inicio != NULL) {
        temp = cactos.inicio;
        cactos.inicio = cactos.inicio->proximo;
        free(temp);
    }

    free(dinossauro);
    CloseWindow();

    return 0;
}
