#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dinossauro.h"
#include "cacto.h"
#include "cenario.h"
#include "nuvem.h"
#include "passaro.h"
#include "scores.h"
#include "menu.h"

#define MAX_NUVENS 10
#define MAX_PASSAROS 15

typedef enum { MENU, JOGANDO, GAME_OVER, SCORES } EstadoJogo;
typedef enum { FACIL, MEDIO, DIFICIL } Dificuldade;
void inserir_cactos_aleatorios(ListaCactos *lista, float x, float y_base);

int main(void) {
    int larguraTela = 800, alturaTela = 450;
    float tempoGlobal = 0.0f;
    int pontuacao = 0;
    int ultimoPontoComSom = -10;
    EstadoJogo estadoAtual = MENU;
    int jogoRodando = 1;

    InitWindow(larguraTela, alturaTela, "TREX RUNNER");
    SetExitKey(KEY_NULL); 
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(NULL));

    Sound somGameOver = LoadSound("Sprites/Sons/GameOver.mp3");
    Sound somPontuacao = LoadSound("Sprites/Sons/pontos.wav");
    Sound somPulo = LoadSound("Sprites/Sons/pulo.wav");
    SetSoundVolume(somGameOver, 0.8f);
    SetSoundVolume(somPontuacao, 0.8f);
    SetSoundVolume(somPulo, 0.8f);


    Dificuldade dificuldadeAtual = FACIL;


    ListaScores scores = {0, NULL};
    carregar_scores(&scores);


    OpcaoMenu opcoesMenu[4];
    int opcaoSelecionada = 0;
    strcpy(opcoesMenu[0].texto, "Jogar");
    opcoesMenu[0].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 - 90, 200, 40};
    opcoesMenu[0].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[0].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[0].selecionada = 1;

    strcpy(opcoesMenu[1].texto, "Dificuldade: Facil");
    opcoesMenu[1].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 - 30, 200, 40};
    opcoesMenu[1].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[1].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[1].selecionada = 0;

    strcpy(opcoesMenu[2].texto, "Scores");
    opcoesMenu[2].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 + 30, 200, 40};
    opcoesMenu[2].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[2].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[2].selecionada = 0;

    strcpy(opcoesMenu[3].texto, "Sair");
    opcoesMenu[3].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 + 90, 200, 40};
    opcoesMenu[3].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[3].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[3].selecionada = 0;


    Nuvem nuvens[MAX_NUVENS];
    inicializar_nuvens(nuvens, larguraTela, alturaTela);
    Passaro passaros[MAX_PASSAROS];
    inicializar_passaros(passaros, larguraTela, alturaTela);
    Dinossauro dino;
    inicializar_dinossauro(&dino, 50, alturaTela - 60, 60, 60, "Sprites/Imagens/dino.png");

    ListaCactos cactos = {0, NULL};
    Texture2D spriteCacto = LoadTexture("Sprites/Imagens/cacto.png");

    float gravidade = 0.6f, forcaPulo = -12.0f;
    float distanciaUltimoCacto = 300.0f, distanciaMinima = 250.0f;
    int jogoAtivo = 1;

    while (jogoRodando && !WindowShouldClose()) {
        BeginDrawing();
        tempoGlobal += GetFrameTime();

        // cenario
        desenhar_ceu_diurno(larguraTela, alturaTela, tempoGlobal);
        desenhar_sol((Vector2){larguraTela - 80, 80}, 40, tempoGlobal);
        atualizar_nuvens(nuvens, larguraTela);
        for (int i = 0; i < MAX_NUVENS; i++)
            desenhar_nuvem(nuvens[i].posicao, nuvens[i].escala, nuvens[i].cor);
        atualizar_passaros(passaros, larguraTela);
        for (int i = 0; i < MAX_PASSAROS; i++)
            desenhar_passaro(passaros[i].posicao, passaros[i].tamanho, passaros[i].tempoAnimacao + tempoGlobal);
        desenhar_montanhas(larguraTela, alturaTela);
        desenhar_chao(larguraTela, alturaTela);

        // estados do jogo
        switch (estadoAtual) {
            case MENU:
                if (IsKeyPressed(KEY_DOWN)) {
                    opcoesMenu[opcaoSelecionada].selecionada = 0;
                    opcaoSelecionada = (opcaoSelecionada + 1) % 4;
                    opcoesMenu[opcaoSelecionada].selecionada = 1;
                }
                if (IsKeyPressed(KEY_UP)) {
                    opcoesMenu[opcaoSelecionada].selecionada = 0;
                    opcaoSelecionada = (opcaoSelecionada - 1 + 4) % 4;
                    opcoesMenu[opcaoSelecionada].selecionada = 1;
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    if (opcaoSelecionada == 0) {
                        estadoAtual = JOGANDO;
                        jogoAtivo = 1;
                        pontuacao = 0;
                        dino.x = 50;
                        dino.y = alturaTela - 60;
                        dino.velocidadeY = 0;
                        dino.pulando = 0;
                        liberar_cactos(&cactos);
                        distanciaUltimoCacto = 300.0f;
                    } else if(opcaoSelecionada == 1) {
                        dificuldadeAtual = (dificuldadeAtual + 1) % 3;
                        switch (dificuldadeAtual) {
                            case FACIL:
                                strcpy(opcoesMenu[1].texto, "Dificuldade: Facil");
                                break;
                            case MEDIO:
                                strcpy(opcoesMenu[1].texto, "Dificuldade: Medio");
                                break;
                            case DIFICIL:
                                strcpy(opcoesMenu[1].texto, "Dificuldade: Dificil");
                                break;
                        }
                    } else if (opcaoSelecionada == 2) {
                        estadoAtual = SCORES;
                    } else if (opcaoSelecionada == 3) {
                        jogoRodando = 0;
                    }
                }
                desenhar_menu(opcoesMenu, 4);
                break;

            case JOGANDO: {
                char textoPontuacao[20];
                sprintf(textoPontuacao, "Pontos: %d", pontuacao);
                DrawText(textoPontuacao, 11, 11, 20, (Color){50,50,50,255});
                DrawText(textoPontuacao, 10, 10, 20, (Color){240,240,100,255});

                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = MENU;
                    break;
                }

                if (jogoAtivo) {
                    int pular = IsKeyPressed(KEY_SPACE);
                    atualizar_dinossauro(&dino, gravidade, forcaPulo, alturaTela, pular);
                    if (pular) PlaySound(somPulo);

                    float velocidadeObstaculo = (dificuldadeAtual == FACIL) ? 5.0f : (dificuldadeAtual == MEDIO) ? 7.0f : 9.0f;
                    atualizar_cactos(&cactos, velocidadeObstaculo);
                    distanciaUltimoCacto += velocidadeObstaculo;
                    if (distanciaUltimoCacto >= distanciaMinima && GetRandomValue(0, 99) < 4) {
                        inserir_cactos_aleatorios(&cactos, larguraTela, alturaTela - 20);
                        distanciaUltimoCacto = 0.0f;
                    }

                    Cacto *atual = cactos.inicio;
                    while (atual != NULL) {
                        if (!atual->passou && atual->x + atual->largura < dino.x) {
                            pontuacao++;
                            atual->passou = 1;
                            if (pontuacao % 10 == 0 && pontuacao != ultimoPontoComSom) {
                                PlaySound(somPontuacao);
                                ultimoPontoComSom = pontuacao;
                            }
                        }
                        atual = atual->proximo;
                    }
                    if (verificar_colisao(&dino, &cactos)) {
                        jogoAtivo = 0;
                        PlaySound(somGameOver);
                        estadoAtual = GAME_OVER;
                        inserir_score(&scores, pontuacao);
                        salvar_scores(&scores);
                    }
                }
                desenhar_dinossauro(&dino);
                desenhar_cactos(&cactos, spriteCacto);
                break;
            }

            case GAME_OVER: {
                char textoPontuacao[20];
                sprintf(textoPontuacao, "Pontos: %d", pontuacao);
                Color corSombraGO = (Color){50, 50, 0, 255};
                Color corGO = (Color){255, 255, 0, 255};
                Color corTexto = (Color){240, 240, 100, 255};
                DrawText(textoPontuacao, 11, 11, 20, corSombraGO);
                DrawText(textoPontuacao, 10, 10, 20, corTexto);
                desenhar_dinossauro(&dino);
                desenhar_cactos(&cactos, spriteCacto);
                DrawText("GAME OVER!", (larguraTela - MeasureText("GAME OVER!", 40)) / 2 + 2, alturaTela / 2 - 40 + 2, 40, corSombraGO);
                DrawText("GAME OVER!", (larguraTela - MeasureText("GAME OVER!", 40)) / 2, alturaTela / 2 - 40, 40, corGO);
                DrawText("Pressione R para reiniciar", (larguraTela - MeasureText("Pressione R para reiniciar", 20)) / 2, alturaTela / 2 + 20, 20, corTexto);
                DrawText("Pressione ESC para voltar ao menu", (larguraTela - MeasureText("Pressione ESC para voltar ao menu", 20)) / 2, alturaTela / 2 + 50, 20, corTexto);
                if (IsKeyPressed(KEY_R)) {
                    estadoAtual = JOGANDO;
                    jogoAtivo = 1;
                    pontuacao = 0;
                    dino.x = 50;
                    dino.y = alturaTela - 60;
                    dino.velocidadeY = 0;
                    dino.pulando = 0;
                    liberar_cactos(&cactos);
                    distanciaUltimoCacto = 300.0f;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = MENU;
                }
                break;
            }

            case SCORES:
                desenhar_tela_scores(&scores, larguraTela, alturaTela);
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = MENU;
                }
                break;
        }
        EndDrawing();
    }

    liberar_cactos(&cactos);
    liberar_scores(&scores);
    if (dino.sprite.id != 0) UnloadTexture(dino.sprite);
    if (spriteCacto.id != 0) UnloadTexture(spriteCacto);
    UnloadSound(somPulo);
    UnloadSound(somGameOver);
    UnloadSound(somPontuacao);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
