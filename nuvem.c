#include "nuvem.h"

// Inicializa as nuvens com posições, escalas e velocidades aleatórias
void inicializar_nuvens(Nuvem nuvens[], int largura, int altura) {
    for (int i = 0; i < MAX_NUVENS; i++) {
        nuvens[i].posicao = (Vector2){GetRandomValue(-100, largura), GetRandomValue(30, altura/3)};
        nuvens[i].escala = GetRandomValue(50, 120) / 100.0f;
        nuvens[i].velocidade = GetRandomValue(5, 15) / 10.0f;
        nuvens[i].cor = (Color){255, 255, 255, GetRandomValue(200, 240)};
    }
}

// Atualiza a posição das nuvens, fazendo elas "voltarem" ao sair da tela
void atualizar_nuvens(Nuvem nuvens[], int largura) {
    for (int i = 0; i < MAX_NUVENS; i++) {
        nuvens[i].posicao.x += nuvens[i].velocidade;
        if (nuvens[i].posicao.x > largura + 100) {
            nuvens[i].posicao.x = -100;
            nuvens[i].posicao.y = GetRandomValue(30, 150);
        }
    }
}

// Desenha uma nuvem composta por vários círculos
void desenhar_nuvem(Vector2 posicao, float escala, Color cor) {
    float raio = 20 * escala;
    DrawCircleV((Vector2){posicao.x + raio, posicao.y + raio}, raio, cor);
    DrawCircleV((Vector2){posicao.x + raio * 2.5f, posicao.y + raio}, raio, cor);
    DrawCircleV((Vector2){posicao.x + raio * 1.5f, posicao.y + raio * 0.7f}, raio * 0.8f, cor);
    DrawCircleV((Vector2){posicao.x + raio * 1.5f, posicao.y + raio * 1.3f}, raio * 0.8f, cor);
}
