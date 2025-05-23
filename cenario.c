#include "cenario.h"
#include <math.h>

// Matrizes globais
Vector2 montanhas[2][3];
Vector2 raiosSol[12][2];
Color linhasCeu[720];

// Montanhas no fundo
void desenhar_montanhas(int largura, int altura) {
    // Definir pontos das montanhas via matriz
    montanhas[0][0] = (Vector2){0, altura - 20};
    montanhas[0][1] = (Vector2){largura * 0.3f, altura * 0.4f};
    montanhas[0][2] = (Vector2){largura * 0.6f, altura - 20};

    montanhas[1][0] = (Vector2){largura * 0.4f, altura - 20};
    montanhas[1][1] = (Vector2){largura * 0.7f, altura * 0.5f};
    montanhas[1][2] = (Vector2){largura, altura - 20};

    // Desenhar as montanhas
    DrawTriangle(montanhas[0][0], montanhas[0][1], montanhas[0][2], (Color){50, 120, 80, 255});
    DrawTriangle(montanhas[1][0], montanhas[1][1], montanhas[1][2], (Color){60, 150, 100, 255});

    // Detalhes (pedrinhas aleatórias)
    for (int i = 0; i < 20; i++) {
        float x = GetRandomValue(0, largura);
        float y = GetRandomValue(altura * 0.6f, altura - 30);
        DrawCircle(x, y, 1, (Color){100, 150, 120, 255});
    }
}

// Sol animado
void desenhar_sol(Vector2 posicao, float raio, float tempo) {
    Color corSol = (Color){255, 255, 100, 255};
    Color corHalo = (Color){255, 200, 50, 100};

    // Desenha o sol central
    DrawCircleV(posicao, raio, corSol);

    // Desenha halos
    for (int i = 1; i <= 5; i++) {
        float haloRaio = raio + i * 10 + sinf(tempo) * 5;
        DrawCircleV(posicao, haloRaio, Fade(corHalo, 0.1f / i));
    }

    // Calcula raios do sol e armazena na matriz
    int raios = 12;
    for (int i = 0; i < raios; i++) {
        float angle = 2 * PI * i / raios + tempo * 0.5f;
        raiosSol[i][0] = (Vector2){
            posicao.x + cosf(angle) * raio,
            posicao.y + sinf(angle) * raio
        };
        raiosSol[i][1] = (Vector2){
            posicao.x + cosf(angle) * (raio + 30 + sinf(tempo * 2 + i) * 10),
            posicao.y + sinf(angle) * (raio + 30 + sinf(tempo * 2 + i) * 10)
        };
        DrawLineEx(raiosSol[i][0], raiosSol[i][1], 3, Fade(corSol, 0.7f));
    }
}

// Céu diurno com gradiente e detalhes
void desenhar_ceu_diurno(int largura, int altura, float tempo) {
    Color topColor = (Color){50, 150, 255, 255};
    Color bottomColor = (Color){135, 206, 235, 255};

    // Calcula linhas do céu e armazena na matriz
    for (int y = 0; y < altura; y++) {
        float ratio = (float)y / altura;
        linhasCeu[y] = (Color){
            (unsigned char)(topColor.r + (bottomColor.r - topColor.r) * ratio),
            (unsigned char)(topColor.g + (bottomColor.g - topColor.g) * ratio),
            (unsigned char)(topColor.b + (bottomColor.b - topColor.b) * ratio),
            255
        };
        DrawLine(0, y, largura, y, linhasCeu[y]);
    }

    // Detalhes como pontos de luz
    for (int i = 0; i < 5; i++) {
        float x = GetRandomValue(0, largura);
        float y = GetRandomValue(0, altura/3);
        float size = GetRandomValue(1, 3);
        DrawCircle(x, y, size, (Color){255, 255, 255, 100});
    }
}

// Chão marrom com linhas
void desenhar_chao(int largura, int altura) {
    Color corChaoBase = (Color){139, 69, 19, 255};
    Color corChaoTopo = (Color){160, 82, 45, 255};

    // Chão com gradiente
    DrawRectangleGradientV(0, altura - 20, largura, 20, corChaoTopo, corChaoBase);

    // Linhas no chão (fendas)
    for (int i = 0; i < largura; i += 40) {
        Color corLinha = (Color){101, 67, 33, 255};
        DrawRectangle(i, altura - 25, 20, 2, corLinha);
    }
}
