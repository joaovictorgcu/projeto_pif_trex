#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_ESTRELAS 200
#define MAX_NUVENS 10
#define MAX_PASSAROS 15
#define MAX_SCORES 5

typedef enum {
    MENU,
    JOGANDO,
    GAME_OVER,
    SCORES
} EstadoJogo;

typedef enum {
    FACIL,
    MEDIO,
    DIFICIL
} Dificuldade;


struct score {
    int pontos;
    struct score *proximo;
};
typedef struct score Score;

struct listaScores {
    int tamanho;
    Score *inicio;
};
typedef struct listaScores ListaScores;

struct dinossauro {
    float x, y;
    float largura, altura;
    float velocidadeY;
    int pulando;
    Texture2D sprite;
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

struct estrela {
    Vector2 posicao;
    float raio;
    float brilho;
    float velocidade;
};
typedef struct estrela Estrela;

struct nuvem {
    Vector2 posicao;
    float escala;
    float velocidade;
    Color cor;
};
typedef struct nuvem Nuvem;

struct passaro {
    Vector2 posicao;
    float velocidade;
    float tamanho;
    float tempoAnimacao;
};
typedef struct passaro Passaro;

struct opcaoMenu {
    char texto[20];
    Rectangle area;
    Color corNormal;
    Color corSelecionada;
    int selecionada;
};
typedef struct opcaoMenu OpcaoMenu;

void inserir_score(ListaScores *lista, int pontos) {
    Score *novo = (Score*)malloc(sizeof(Score));
    novo->pontos = pontos;
    novo->proximo = NULL;

    if (lista->inicio == NULL || pontos > lista->inicio->pontos) {
        novo->proximo = lista->inicio;
        lista->inicio = novo;
    } else {
        Score *atual = lista->inicio;
        Score *anterior = NULL;
        
        while (atual != NULL && pontos <= atual->pontos) {
            anterior = atual;
            atual = atual->proximo;
        }
        
        anterior->proximo = novo;
        novo->proximo = atual;
    }
    
    lista->tamanho++;
    
    if (lista->tamanho > MAX_SCORES) {
        Score *atual = lista->inicio;
        int contador = 1;
        
        while (contador < MAX_SCORES) {
            atual = atual->proximo;
            contador++;
        }
        
        Score *temp = atual->proximo;
        atual->proximo = NULL;
        
        while (temp != NULL) {
            Score *proximo = temp->proximo;
            free(temp);
            temp = proximo;
            lista->tamanho--;
        }
    }
}

void salvar_scores(ListaScores *lista) {
    FILE *arquivo = fopen("Scores/scores.txt", "w");
    if (arquivo == NULL) {
        return;
    }
    
    Score *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d\n", atual->pontos);
        atual = atual->proximo;
    }
    
    fclose(arquivo);
}

void carregar_scores(ListaScores *lista) {
    FILE *arquivo = fopen("Scores/scores.txt", "r");
    if (arquivo == NULL) {
        return;
    }
    
    int pontos;
    while (fscanf(arquivo, "%d", &pontos) == 1) {
        inserir_score(lista, pontos);
    }
    
    fclose(arquivo);
}

void liberar_scores(ListaScores *lista) {
    Score *atual = lista->inicio;
    while (atual != NULL) {
        Score *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    lista->inicio = NULL;
    lista->tamanho = 0;
}

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

void atualizar_cactos(ListaCactos *lista, float velocidade) {
    Cacto *atual = lista->inicio;
    while (atual != NULL) {
        atual->x -= velocidade;
        atual = atual->proximo;
    }
}

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

void desenhar_dinossauro(Dinossauro *dino) {
    if (dino->sprite.id == 0) {
        Color corBase = (Color){50, 50, 50, 255};
        Color corDetalhe = (Color){70, 70, 70, 255};
        
        DrawRectangleGradientV(dino->x, dino->y, dino->largura, dino->altura, (Color){80, 80, 80, 255}, corBase);
        
        DrawRectangle(dino->x + dino->largura - 10, dino->y - 15, 20, 20, corDetalhe);
        DrawRectangle(dino->x + dino->largura + 5, dino->y - 10, 3, 3, BLACK);
        
        DrawRectangle(dino->x + 5, dino->y + 10, 5, 3, corDetalhe);
        DrawRectangle(dino->x + 2, dino->y + dino->altura, 8, 5, corDetalhe);
        DrawRectangle(dino->x + dino->largura - 10, dino->y + dino->altura, 8, 5, corDetalhe);
        
        DrawRectangleLinesEx((Rectangle){dino->x, dino->y, dino->largura, dino->altura}, 1, (Color){30, 30, 30, 255});
    } else {
        DrawTexturePro(dino->sprite, 
                      (Rectangle){0, 0, dino->sprite.width, dino->sprite.height}, 
                      (Rectangle){dino->x, dino->y, dino->largura, dino->altura}, 
                      (Vector2){0, 0}, 0, WHITE);
    }
}

void inicializar_estrelas(Estrela estrelas[], int largura, int altura) {
    for (int i = 0; i < MAX_ESTRELAS; i++) {
        estrelas[i].posicao = (Vector2){GetRandomValue(0, largura), GetRandomValue(0, altura/2)};
        estrelas[i].raio = GetRandomValue(1, 3) / 2.0f;
        estrelas[i].brilho = GetRandomValue(50, 100) / 100.0f;
        estrelas[i].velocidade = GetRandomValue(1, 3) / 10.0f;
    }
}

void atualizar_estrelas(Estrela estrelas[], int largura, int altura) {
    for (int i = 0; i < MAX_ESTRELAS; i++) {
        estrelas[i].brilho = 0.5f + 0.5f * sinf(GetTime() * estrelas[i].velocidade);
        if (estrelas[i].brilho < 0.3f) estrelas[i].brilho = 0.3f;
    }
}

void desenhar_estrelas(Estrela estrelas[]) {
    for (int i = 0; i < MAX_ESTRELAS; i++) {
        Color corEstrela = (Color){255, 255, 200, (unsigned char)(255 * estrelas[i].brilho)};
        DrawCircleV(estrelas[i].posicao, estrelas[i].raio, corEstrela);
        if (estrelas[i].raio > 1.0f) {
            DrawCircleV(estrelas[i].posicao, estrelas[i].raio * 1.5f, Fade(corEstrela, 0.3f * estrelas[i].brilho));
        }
    }
}

void inicializar_nuvens(Nuvem nuvens[], int largura, int altura) {
    for (int i = 0; i < MAX_NUVENS; i++) {
        nuvens[i].posicao = (Vector2){GetRandomValue(-100, largura), GetRandomValue(30, altura/3)};
        nuvens[i].escala = GetRandomValue(50, 120) / 100.0f;
        nuvens[i].velocidade = GetRandomValue(5, 15) / 10.0f;
        nuvens[i].cor = (Color){255, 255, 255, GetRandomValue(200, 240)};
    }
}

void atualizar_nuvens(Nuvem nuvens[], int largura) {
    for (int i = 0; i < MAX_NUVENS; i++) {
        nuvens[i].posicao.x += nuvens[i].velocidade;
        if (nuvens[i].posicao.x > largura + 100) {
            nuvens[i].posicao.x = -100;
            nuvens[i].posicao.y = GetRandomValue(30, 150);
        }
    }
}

void desenhar_nuvem(Vector2 posicao, float escala, Color cor) {
    float raio = 20 * escala;
    
    DrawCircleV((Vector2){posicao.x + raio, posicao.y + raio}, raio, cor);
    DrawCircleV((Vector2){posicao.x + raio * 2.5f, posicao.y + raio}, raio, cor);
    DrawCircleV((Vector2){posicao.x + raio * 1.5f, posicao.y + raio * 0.7f}, raio * 0.8f, cor);
    DrawCircleV((Vector2){posicao.x + raio * 1.5f, posicao.y + raio * 1.3f}, raio * 0.8f, cor);
}

void inicializar_passaros(Passaro passaros[], int largura, int altura) {
    for (int i = 0; i < MAX_PASSAROS; i++) {
        passaros[i].posicao = (Vector2){GetRandomValue(0, largura), GetRandomValue(50, altura/3)};
        passaros[i].velocidade = GetRandomValue(100, 200) / 100.0f;
        passaros[i].tamanho = GetRandomValue(5, 15) / 10.0f;
        passaros[i].tempoAnimacao = GetRandomValue(0, 100) / 100.0f;
    }
}

void atualizar_passaros(Passaro passaros[], int largura) {
    for (int i = 0; i < MAX_PASSAROS; i++) {
        passaros[i].posicao.x += passaros[i].velocidade;
        passaros[i].tempoAnimacao += 0.1f;
        
        if (passaros[i].posicao.x > largura + 20) {
            passaros[i].posicao.x = -20;
            passaros[i].posicao.y = GetRandomValue(50, 150);
        }
    }
}

void desenhar_passaro(Vector2 posicao, float tamanho, float tempo) {
    float asaOffset = sinf(tempo * 10) * 3 * tamanho;
    Color corPassaro = (Color){139, 69, 19, 255};
    
    DrawTriangle(
        (Vector2){posicao.x, posicao.y},
        (Vector2){posicao.x + 10 * tamanho, posicao.y + asaOffset},
        (Vector2){posicao.x + 10 * tamanho, posicao.y - asaOffset},
        corPassaro
    );
}

void desenhar_lua(Vector2 posicao, float raio) {
    DrawCircleV(posicao, raio, (Color){220, 220, 220, 255});
    DrawCircle(posicao.x - raio*0.3f, posicao.y - raio*0.2f, raio*0.15f, (Color){180, 180, 180, 255});
    DrawCircle(posicao.x + raio*0.4f, posicao.y + raio*0.1f, raio*0.2f, (Color){180, 180, 180, 255});
    DrawCircle(posicao.x - raio*0.1f, posicao.y + raio*0.3f, raio*0.1f, (Color){180, 180, 180, 255});
    DrawCircleV(posicao, raio * 1.2f, Fade((Color){220, 220, 255, 255}, 0.1f));
}

void desenhar_montanhas(int largura, int altura) {
    Color corMontanhas1 = (Color){50, 120, 80, 255};
    Color corMontanhas2 = (Color){60, 150, 100, 255};
    
    DrawTriangle(
        (Vector2){0, altura - 20},
        (Vector2){largura * 0.3f, altura * 0.4f},
        (Vector2){largura * 0.6f, altura - 20},
        corMontanhas1
    );
    
    DrawTriangle(
        (Vector2){largura * 0.4f, altura - 20},
        (Vector2){largura * 0.7f, altura * 0.5f},
        (Vector2){largura, altura - 20},
        corMontanhas2
    );
    
    for (int i = 0; i < 20; i++) {
        float x = GetRandomValue(0, largura);
        float y = GetRandomValue(altura * 0.6f, altura - 30);
        DrawCircle(x, y, 1, (Color){100, 150, 120, 255});
    }
}

void desenhar_sol(Vector2 posicao, float raio, float tempo) {
    Color corSol = (Color){255, 255, 100, 255};
    Color corHalo = (Color){255, 200, 50, 100};
    
    DrawCircleV(posicao, raio, corSol);
    
    for (int i = 1; i <= 5; i++) {
        float haloRaio = raio + i * 10 + sinf(tempo) * 5;
        DrawCircleV(posicao, haloRaio, Fade(corHalo, 0.1f / i));
    }
    
    int raios = 12;
    for (int i = 0; i < raios; i++) {
        float angle = 2 * PI * i / raios + tempo * 0.5f;
        Vector2 start = {posicao.x + cosf(angle) * raio, posicao.y + sinf(angle) * raio};
        Vector2 end = {posicao.x + cosf(angle) * (raio + 30 + sinf(tempo * 2 + i) * 10), 
                   posicao.y + sinf(angle) * (raio + 30 + sinf(tempo * 2 + i) * 10)};
        DrawLineEx(start, end, 3, Fade(corSol, 0.7f));
    }
}

void desenhar_ceu_diurno(int largura, int altura, float tempo) {
    Color topColor = (Color){50, 150, 255, 255};
    Color bottomColor = (Color){135, 206, 235, 255};
    
    for (int y = 0; y < altura; y++) {
        float ratio = (float)y / altura;
        Color lineColor = (Color){
            (unsigned char)(topColor.r + (bottomColor.r - topColor.r) * ratio),
            (unsigned char)(topColor.g + (bottomColor.g - topColor.g) * ratio),
            (unsigned char)(topColor.b + (bottomColor.b - topColor.b) * ratio),
            255
        };
        DrawLine(0, y, largura, y, lineColor);
    }
    
    for (int i = 0; i < 5; i++) {
        float x = GetRandomValue(0, largura);
        float y = GetRandomValue(0, altura/3);
        float size = GetRandomValue(1, 3);
        DrawCircle(x, y, size, (Color){255, 255, 255, 100});
    }
}

void desenhar_menu(OpcaoMenu opcoes[], int numOpcoes, int modoNoite) {
    Color corFundo = modoNoite ? (Color){20, 10, 40, 200} : (Color){135, 206, 235, 200};
    Color corBorda = modoNoite ? (Color){100, 100, 150, 255} : (Color){50, 50, 50, 255};
    
    DrawRectangle(opcoes[0].area.x - 20, opcoes[0].area.y - 40, 
                 opcoes[0].area.width + 40, (opcoes[numOpcoes-1].area.y - opcoes[0].area.y) + opcoes[0].area.height + 60, 
                 corFundo);
    
    DrawRectangleLinesEx((Rectangle){opcoes[0].area.x - 20, opcoes[0].area.y - 40, 
                                    opcoes[0].area.width + 40, (opcoes[numOpcoes-1].area.y - opcoes[0].area.y) + opcoes[0].area.height + 60}, 
                         2, corBorda);
    
    Color corTitulo = modoNoite ? (Color){200, 200, 255, 255} : (Color){50, 50, 50, 255};
    DrawText("DINO RUNNER", opcoes[0].area.x, opcoes[0].area.y - 30, 20, corTitulo);
    
    for (int i = 0; i < numOpcoes; i++) {
        Color corTexto = opcoes[i].selecionada ? opcoes[i].corSelecionada : opcoes[i].corNormal;
        DrawRectangleRec(opcoes[i].area, Fade(corTexto, 0.3f));
        DrawRectangleLinesEx(opcoes[i].area, 2, corTexto);
        
        int textWidth = MeasureText(opcoes[i].texto, 20);
        int textX = opcoes[i].area.x + (opcoes[i].area.width - textWidth) / 2;
        int textY = opcoes[i].area.y + (opcoes[i].area.height - 20) / 2;
        
        DrawText(opcoes[i].texto, textX, textY, 20, corTexto);
    }
}

void desenhar_tela_scores(ListaScores *lista, int largura, int altura, int modoNoite) {
    Color corFundo = modoNoite ? (Color){20, 10, 40, 200} : (Color){135, 206, 235, 200};
    Color corBorda = modoNoite ? (Color){100, 100, 150, 255} : (Color){50, 50, 50, 255};
    Color corTexto = modoNoite ? (Color){200, 200, 255, 255} : (Color){50, 50, 50, 255};
    
    int larguraQuadro = 300;
    int alturaQuadro = 300;
    int xQuadro = (largura - larguraQuadro) / 2;
    int yQuadro = (altura - alturaQuadro) / 2;
    
    DrawRectangle(xQuadro, yQuadro, larguraQuadro, alturaQuadro, corFundo);
    DrawRectangleLinesEx((Rectangle){xQuadro, yQuadro, larguraQuadro, alturaQuadro}, 2, corBorda);
    
    DrawText("MELHORES PONTUAÇÕES", xQuadro + 20, yQuadro + 20, 20, corTexto);
    DrawLine(xQuadro + 20, yQuadro + 50, xQuadro + larguraQuadro - 20, yQuadro + 50, corTexto);
    
    Score *atual = lista->inicio;
    int y = yQuadro + 70;
    int posicao = 1;
    
    if (atual == NULL) {
        DrawText("Nenhuma pontuação registrada", xQuadro + 30, y, 18, corTexto);
    } else {
        while (atual != NULL && posicao <= MAX_SCORES) {
            char textoScore[50];
            sprintf(textoScore, "%d. %d pontos", posicao, atual->pontos);
            DrawText(textoScore, xQuadro + 30, y, 18, corTexto);
            
            y += 30;
            atual = atual->proximo;
            posicao++;
        }
    }
    
    DrawText("Pressione ESC para voltar", xQuadro + 30, yQuadro + alturaQuadro - 40, 15, corTexto);
}

int main(void) {
    float tempoJogo = 0.0f;
    int modoNoite = 0;
    int larguraTela = 800;
    int alturaTela = 450;
    int pontuacao = 0;
    int ultimaPontuacaoMudanca = 0;
    float tempoGlobal = 0.0f;
    EstadoJogo estadoAtual = MENU;
    int jogoRodando = 1;


    InitWindow(larguraTela, alturaTela, "Jogo Dino");
    InitAudioDevice();
    Sound somPulo = LoadSound("pulo.wav");
    SetSoundVolume(somPulo, 0.6f);
    Sound somColisao = LoadSound("colisao.wav");
    SetTargetFPS(60);
    srand(time(NULL));
    Dificuldade dificuldadeAtual = FACIL;
    ListaScores scores;
    scores.inicio = NULL;
    scores.tamanho = 0;
    carregar_scores(&scores);

    OpcaoMenu opcoesMenu[4];
    int opcaoSelecionada = 0;


    // Jogar
    strcpy(opcoesMenu[0].texto, "Jogar");
    opcoesMenu[0].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 - 90, 200, 40};
    opcoesMenu[0].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[0].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[0].selecionada = 1;

// Dificuldade
    strcpy(opcoesMenu[1].texto, "Dificuldade: Facil");  // inicia com "Fácil"
    opcoesMenu[1].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 - 30, 200, 40};
    opcoesMenu[1].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[1].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[1].selecionada = 0;

// Scores
    strcpy(opcoesMenu[2].texto, "Scores");
    opcoesMenu[2].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 + 30, 200, 40};
    opcoesMenu[2].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[2].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[2].selecionada = 0;

// Sair
    strcpy(opcoesMenu[3].texto, "Sair");
    opcoesMenu[3].area = (Rectangle){larguraTela/2 - 100, alturaTela/2 + 90, 200, 40};
    opcoesMenu[3].corNormal = (Color){100, 100, 100, 255};
    opcoesMenu[3].corSelecionada = (Color){255, 255, 0, 255};
    opcoesMenu[3].selecionada = 0;
    Estrela estrelas[MAX_ESTRELAS];
    inicializar_estrelas(estrelas, larguraTela, alturaTela);

    Nuvem nuvens[MAX_NUVENS];
    inicializar_nuvens(nuvens, larguraTela, alturaTela);

    Passaro passaros[MAX_PASSAROS];
    inicializar_passaros(passaros, larguraTela, alturaTela);

    Dinossauro *dino = (Dinossauro*)malloc(sizeof(Dinossauro));
    dino->x = 50;
    dino->y = alturaTela - 60;
    dino->largura = 60;  // Aumentado de 40 para 60
    dino->altura = 60;   // Aumentado de 40 para 60
    dino->velocidadeY = 0;
    dino->pulando = 0;
    
    dino->sprite = LoadTexture("Sprites/dino.png");
    
    Texture2D spriteCacto = LoadTexture("Sprites/cacto.png");

    float gravidade = 0.6f;
    float forcaPulo = -12.0f;

    ListaCactos cactos;
    cactos.inicio = NULL;
    cactos.tamanho = 0;

    int jogoAtivo = 1;
    int podeReiniciar = 0;
    float distanciaUltimoCacto = 300.0f;
    float distanciaMinima = 250.0f;

    while (jogoRodando) {
        if (IsKeyPressed(KEY_Q)) jogoRodando = 0;
        
        BeginDrawing();

        tempoGlobal += GetFrameTime();
        tempoJogo += GetFrameTime();
        
        if (pontuacao - ultimaPontuacaoMudanca >= 20) {
            modoNoite = !modoNoite;
            ultimaPontuacaoMudanca = pontuacao;
        }

        if (modoNoite) {
            Color corTopo = (Color){5, 5, 15, 255};
            Color corBase = (Color){20, 10, 40, 255};
            for (int y = 0; y < alturaTela; y++) {
                float proporcao = (float)y / alturaTela;
                Color corLinha = (Color){
                    (unsigned char)(corTopo.r + (corBase.r - corTopo.r) * proporcao),
                    (unsigned char)(corTopo.g + (corBase.g - corTopo.g) * proporcao),
                    (unsigned char)(corTopo.b + (corBase.b - corTopo.b) * proporcao),
                    255
                };
                DrawLine(0, y, larguraTela, y, corLinha);
            }

            atualizar_estrelas(estrelas, larguraTela, alturaTela);
            desenhar_estrelas(estrelas);
            desenhar_montanhas(larguraTela, alturaTela);
            desenhar_lua((Vector2){larguraTela - 100, 100}, 40);
            DrawRectangle(0, alturaTela - 20, larguraTela, 20, (Color){30, 25, 35, 255});
            
            for (int i = 0; i < 10; i++) {
                float x = GetRandomValue(0, larguraTela);
                DrawRectangle(x, alturaTela - 25, 3, 5, (Color){50, 45, 60, 255});
            }
        } else {
            desenhar_ceu_diurno(larguraTela, alturaTela, tempoGlobal);
            desenhar_sol((Vector2){larguraTela - 80, 80}, 40, tempoGlobal);
            
            atualizar_nuvens(nuvens, larguraTela);
            for (int i = 0; i < MAX_NUVENS; i++) {
                desenhar_nuvem(nuvens[i].posicao, nuvens[i].escala, nuvens[i].cor);
            }
            
            atualizar_passaros(passaros, larguraTela);
            for (int i = 0; i < MAX_PASSAROS; i++) {
                desenhar_passaro(passaros[i].posicao, passaros[i].tamanho, passaros[i].tempoAnimacao + tempoGlobal);
            }
            
            desenhar_montanhas(larguraTela, alturaTela);
            
            Color corChaoBase = (Color){139, 69, 19, 255};
            Color corChaoTopo = (Color){160, 82, 45, 255};
            DrawRectangleGradientV(0, alturaTela - 20, larguraTela, 20, corChaoTopo, corChaoBase);
        }

        for (int i = 0; i < larguraTela; i += 40) {
            Color corLinha = modoNoite ? (Color){80, 70, 90, 255} : (Color){101, 67, 33, 255};
            DrawRectangle(i, alturaTela - 25, 20, 2, corLinha);
        }

        switch (estadoAtual) {
            case MENU:
                if (IsKeyPressed(KEY_DOWN)) {
                    opcoesMenu[opcaoSelecionada].selecionada = 0;
                    opcaoSelecionada = (opcaoSelecionada + 1) % 3;
                    opcoesMenu[opcaoSelecionada].selecionada = 1;
                }
                
                if (IsKeyPressed(KEY_UP)) {
                    opcoesMenu[opcaoSelecionada].selecionada = 0;
                    opcaoSelecionada = (opcaoSelecionada - 1 + 3) % 3;
                    opcoesMenu[opcaoSelecionada].selecionada = 1;
                }
                
                if (IsKeyPressed(KEY_ENTER)) {
                    if (opcaoSelecionada == 0) {
                        estadoAtual = JOGANDO;
                        jogoAtivo = 1;
                        pontuacao = 0;
                        ultimaPontuacaoMudanca = 0;
                        modoNoite = 0;
                        
                        dino->x = 50;
                        dino->y = alturaTela - 60;
                        dino->velocidadeY = 0;
                        dino->pulando = 0;
                        
                        Cacto *atual = cactos.inicio;
                        while (atual != NULL) {
                            Cacto *temp = atual;
                            atual = atual->proximo;
                            free(temp);
                        }
                        cactos.inicio = NULL;
                        cactos.tamanho = 0;
                        
                        distanciaUltimoCacto = 300.0f;
                    } 
                    else if(opcaoSelecionada == 1) {
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
                    }
                    else if (opcaoSelecionada == 2) {
                        estadoAtual = SCORES;
                    } else if (opcaoSelecionada == 3) {
                        jogoRodando = 0;
                    }
                }
                
                desenhar_menu(opcoesMenu, 4, modoNoite);
                break;
                
            case JOGANDO:
                char textoPontuacao[20];
                sprintf(textoPontuacao, "Pontos: %d", pontuacao);
                
                Color corSombra = modoNoite ? (Color){100, 100, 150, 255} : (Color){50, 50, 50, 255};
                Color corTexto = modoNoite ? (Color){200, 200, 255, 255} : (Color){240, 240, 100, 255};
                
                DrawText(textoPontuacao, 11, 11, 20, corSombra);
                DrawText(textoPontuacao, 10, 10, 20, corTexto);
                
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
                    float velocidadeObstaculo = 5.0f;
                    switch (dificuldadeAtual) {
                        case FACIL:
                            velocidadeObstaculo = 5.0f;
                            break;
                        case MEDIO:
                            velocidadeObstaculo = 7.0f;
                            break;
                        case DIFICIL:
                            velocidadeObstaculo = 9.0f;
                            break;
                    }
                    atualizar_cactos(&cactos, velocidadeObstaculo);
                    distanciaUltimoCacto += velocidadeObstaculo;

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
                            
                            if (pontuacao % 20 == 0) {
                                modoNoite = !modoNoite;
                                ultimaPontuacaoMudanca = pontuacao;
                            }
                        }
                        atual = atual->proximo;
                    }

                    if (verificar_colisao(dino, &cactos)) {
                        jogoAtivo = 0;
                        podeReiniciar = 0;
                        PlaySound(somColisao);
                        estadoAtual = GAME_OVER;
                        
                        inserir_score(&scores, pontuacao);
                        salvar_scores(&scores);
                    }
                }

                desenhar_dinossauro(dino);
                desenhar_cactos(&cactos, spriteCacto);
                break;
                
            case GAME_OVER:
                sprintf(textoPontuacao, "Pontos: %d", pontuacao);
                DrawText(textoPontuacao, 11, 11, 20, corSombra);
                DrawText(textoPontuacao, 10, 10, 20, corTexto);
                
                desenhar_dinossauro(dino);
                desenhar_cactos(&cactos, spriteCacto);
                
                Color corSombraGO = modoNoite ? (Color){100, 0, 0, 255} : (Color){50, 50, 0, 255};
                Color corGO = modoNoite ? (Color){255, 80, 80, 255} : (Color){255, 255, 0, 255};
                
                DrawText("GAME OVER!", (larguraTela - MeasureText("GAME OVER!", 40)) / 2 + 2, alturaTela / 2 - 40 + 2, 40, corSombraGO);
                DrawText("GAME OVER!", (larguraTela - MeasureText("GAME OVER!", 40)) / 2, alturaTela / 2 - 40, 40, corGO);
                
                DrawText("Pressione R para reiniciar", (larguraTela - MeasureText("Pressione R para reiniciar", 20)) / 2, alturaTela / 2 + 20, 20, corTexto);
                DrawText("Pressione ESC para voltar ao menu", (larguraTela - MeasureText("Pressione ESC para voltar ao menu", 20)) / 2, alturaTela / 2 + 50, 20, corTexto);
                
                if (IsKeyPressed(KEY_R)) {
                    estadoAtual = JOGANDO;
                    jogoAtivo = 1;
                    pontuacao = 0;
                    ultimaPontuacaoMudanca = 0;
                    modoNoite = 0;
                    
                    dino->x = 50;
                    dino->y = alturaTela - 60;
                    dino->velocidadeY = 0;
                    dino->pulando = 0;
                    
                    Cacto *atual = cactos.inicio;
                    while (atual != NULL) {
                        Cacto *temp = atual;
                        atual = atual->proximo;
                        free(temp);
                    }
                    cactos.inicio = NULL;
                    cactos.tamanho = 0;
                    
                    distanciaUltimoCacto = 300.0f;
                }
                
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = MENU;
                }
                break;
                
            case SCORES:
                desenhar_tela_scores(&scores, larguraTela, alturaTela, modoNoite);
                
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = MENU;
                }
                break;
        }

        EndDrawing();
    }

    Cacto *temp;
    while (cactos.inicio != NULL) {
        temp = cactos.inicio;
        cactos.inicio = cactos.inicio->proximo;
        free(temp);
    }

    liberar_scores(&scores);

    if (dino->sprite.id != 0) UnloadTexture(dino->sprite);
    if (spriteCacto.id != 0) UnloadTexture(spriteCacto);

    UnloadSound(somPulo);
    UnloadSound(somColisao);
    CloseAudioDevice();
    free(dino);
    CloseWindow();
    return 0;
}