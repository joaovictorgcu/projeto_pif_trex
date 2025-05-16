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
typedef struct dinossauro Dinossauro;

struct cacto {
    float x, y;
    float largura, altura;
    int passou;
    struct cacto *proximo;
};
typedef struct cacto Cacto;

struct listaCactos {
    int tamanho;
    Cacto *inicio;
};
typedef struct listaCactos ListaCactos;

void inserir_cacto_final(ListaCactos *lista, float x, float y, float largura, float altura) {
    Cacto *novo = (Cacto*)malloc(sizeof(Cacto));
    if (novo == NULL) {
        printf("Erro ao alocar memória para novo cacto.\n");
        exit(1);
    }
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

void atualizar_cactos(ListaCactos *lista, float velocidade) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        atual->x -= velocidade;
        atual = atual->proximo;
    }
}

void desenhar_cactos(ListaCactos *lista) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        DrawRectangleRounded((Rectangle){atual->x, atual->y, atual->largura, atual->altura}, 0.2, 4, DARKGREEN);
        atual = atual->proximo;
    }
}

int verificar_colisao(Dinossauro *dino, ListaCactos *lista) {
    Cacto *atual = lista->inicio;
    Rectangle retDino = { dino->x, dino->y, dino->largura, dino->altura };

    while (atual != NULL) {
        Rectangle retCacto = { atual->x, atual->y, atual->largura, atual->altura };
        if (CheckCollisionRecs(retDino, retCacto)) {
            return 1;
        }
        atual = atual->proximo;
    }
    return 0;
}

int main(void) {
    int larguraTela = 800;
    int alturaTela = 450;
    int pontuacao = 0;

    InitWindow(larguraTela, alturaTela, "Jogo Dino");
    InitAudioDevice();
    Sound somPulo = LoadSound("pulo.wav");
    SetSoundVolume(somPulo, 0.6f); 
    Sound somColisao = LoadSound("colisao.wav");
    SetTargetFPS(60);
    srand(time(NULL)); 

    Dinossauro *dino = (Dinossauro*)malloc(sizeof(Dinossauro));
    dino->x = 50;
    dino->y = alturaTela - 60;
    dino->largura = 40;
    dino->altura = 40;
    dino->velocidadeY = 0;
    dino->pulando = 0;

    float gravidade = 0.6f;
    float forcaPulo = -12.0f;

    ListaCactos cactos;
    cactos.inicio = NULL;
    cactos.tamanho = 0;

    int jogoAtivo = 1;
    int podeReiniciar = 0;
    float distanciaUltimoCacto = 300.0f; 
    float distanciaMinima = 250.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawRectangleGradientV(0, 0, larguraTela, alturaTela, SKYBLUE, RAYWHITE);
        DrawRectangle(0, alturaTela - 20, larguraTela, 20, DARKBROWN);

        for (int i = 0; i < larguraTela; i += 40) {
            DrawRectangle(i, alturaTela - 25, 20, 2, GRAY);
        }

        char textoPontuacao[20];
        sprintf(textoPontuacao, "Pontos: %d", pontuacao);
        DrawText(textoPontuacao, 11, 11, 20, LIGHTGRAY);
        DrawText(textoPontuacao, 10, 10, 20, DARKGRAY);

        if (jogoAtivo) {
            if (IsKeyPressed(KEY_SPACE) && !dino->pulando) {
                dino->velocidadeY = forcaPulo;
                dino->pulando = 1;
                PlaySound(somPulo);
            }

            dino->velocidadeY += gravidade;
            dino->y += dino->velocidadeY;

            if (dino->y >= alturaTela - dino->altura - 20) {
                dino->y = alturaTela - dino->altura - 20;
                dino->velocidadeY = 0;
                dino->pulando = 0;
            }

            atualizar_cactos(&cactos, 5.0f);
            distanciaUltimoCacto += 5.0f;

            if (distanciaUltimoCacto >= distanciaMinima && GetRandomValue(0, 99) < 4) {
                float alturaCacto = GetRandomValue(30, 60);
                float larguraCacto = GetRandomValue(15, 25);
                inserir_cacto_final(&cactos, larguraTela, alturaTela - 20 - alturaCacto, larguraCacto, alturaCacto);
                distanciaUltimoCacto = 0.0f;
            }

            Cacto *atual = cactos.inicio;
            while (atual != NULL) {
                if (!atual->passou && atual->x + atual->largura < dino->x) {
                    pontuacao++;
                    atual->passou = 1;
                }
                atual = atual->proximo;
            }

            if (verificar_colisao(dino, &cactos)) {
                jogoAtivo = 0;
                podeReiniciar = 0;
                PlaySound(somColisao);
            }
        }

        DrawRectangleRounded((Rectangle){dino->x, dino->y, dino->largura, dino->altura}, 0.3, 4, DARKGRAY);
        DrawRectangleRounded((Rectangle){dino->x + dino->largura - 18, dino->y - 15, 30, 30}, 0.3, 4, DARKGRAY);
        DrawCircle(dino->x + dino->largura + 6, dino->y - 5, 4, WHITE);
        DrawCircle(dino->x + dino->largura + 6, dino->y - 5, 2, BLACK);
        DrawRectangle(dino->x + 5, dino->y + dino->altura, 10, 5, DARKGRAY);
        DrawRectangle(dino->x + dino->largura - 15, dino->y + dino->altura, 10, 5, DARKGRAY);

        desenhar_cactos(&cactos);

        if (!jogoAtivo) {
            DrawText("GAME OVER!", (larguraTela - MeasureText("GAME OVER!", 40)) / 2 + 2, alturaTela / 2 - 40 + 2, 40, GRAY);
            DrawText("GAME OVER!", (larguraTela - MeasureText("GAME OVER!", 40)) / 2, alturaTela / 2 - 40, 40, RED);

            char textoFinal[30];
            sprintf(textoFinal, "Pontuação final: %d", pontuacao);
            int larguraTextoFinal = MeasureText(textoFinal, 20);
            DrawText(textoFinal, (larguraTela - larguraTextoFinal) / 2, alturaTela / 2 + 0, 20, DARKGRAY);

            int larguraTextoReinicio = MeasureText("Pressione ESPACO para reiniciar", 20);
            DrawText("Pressione ESPACO para reiniciar", (larguraTela - larguraTextoReinicio) / 2, alturaTela / 2 + 30, 20, GRAY);

            if (IsKeyReleased(KEY_SPACE)) podeReiniciar = 1;

            if (podeReiniciar && IsKeyPressed(KEY_SPACE)) {
                dino->x = 50;
                dino->y = alturaTela - 60;
                dino->velocidadeY = 0;
                dino->pulando = 0;

                Cacto *temp;
                while (cactos.inicio != NULL) {
                    temp = cactos.inicio;
                    cactos.inicio = cactos.inicio->proximo;
                    free(temp);
                }
                cactos.tamanho = 0;

                jogoAtivo = 1;
                distanciaUltimoCacto = 300.0f;
                podeReiniciar = 0;
                pontuacao = 0;
            }
        }

        EndDrawing();
    }

    Cacto *temp;
    while (cactos.inicio != NULL) {
        temp = cactos.inicio;
        cactos.inicio = cactos.inicio->proximo;
        free(temp);
    }

    UnloadSound(somPulo);
    UnloadSound(somColisao);
    CloseAudioDevice();
    free(dino);
    CloseWindow();
    return 0;
}
