#include "passaro.h"
#include <math.h>

// Inicializa os pássaros com posições, velocidades e tamanhos aleatórios
void inicializar_passaros(Passaro passaros[], int largura, int altura) {
    for (int i = 0; i < MAX_PASSAROS; i++) {
        passaros[i].posicao = (Vector2){GetRandomValue(0, largura), GetRandomValue(50, altura/3)};
        passaros[i].velocidade = GetRandomValue(100, 200) / 100.0f;
        passaros[i].tamanho = GetRandomValue(5, 15) / 10.0f;
        passaros[i].tempoAnimacao = GetRandomValue(0, 100) / 100.0f;
    }
}

// Atualiza a posição e animação dos pássaros
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

// Desenha um pássaro simples animado (asa batendo)
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
