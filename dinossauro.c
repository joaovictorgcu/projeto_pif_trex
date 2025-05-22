#include "dinossauro.h"
#include <stdlib.h>
#include <string.h>

// Inicializa o dinossauro
void inicializar_dinossauro(Dinossauro *dino, int x, int y, int largura, int altura, const char* caminhoSprite) {
    dino->x = x;
    dino->y = y;
    dino->largura = largura;
    dino->altura = altura;
    dino->velocidadeY = 0;
    dino->pulando = 0;
    if (caminhoSprite && strlen(caminhoSprite) > 0) {
        dino->sprite = LoadTexture(caminhoSprite);
    } else {
        dino->sprite.id = 0; // Usar desenho padrão
    }
}

// Atualiza a física do dinossauro (pulo e gravidade)
// pular: 1 se o jogador apertou o botão de pulo, 0 caso contrário
void atualizar_dinossauro(Dinossauro *dino, float gravidade, float forcaPulo, int alturaTela, int pular) {
    if (pular && !dino->pulando) {
        dino->velocidadeY = forcaPulo;
        dino->pulando = 1;
    }
    dino->velocidadeY += gravidade;
    dino->y += dino->velocidadeY;
    if (dino->y >= alturaTela - dino->altura - 20) {
        dino->y = alturaTela - dino->altura - 20;
        dino->velocidadeY = 0;
        dino->pulando = 0;
    }
}

// Desenha o dinossauro na tela
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
