#include "menu.h"
#include <stdio.h>

void desenhar_menu(OpcaoMenu opcoes[], int numOpcoes) {
    Color corFundo = (Color){135, 206, 235, 200};
    Color corBorda = (Color){50, 50, 50, 255};

    DrawRectangle(opcoes[0].area.x - 20, opcoes[0].area.y - 40,
        opcoes[0].area.width + 40, (opcoes[numOpcoes-1].area.y - opcoes[0].area.y) + opcoes[0].area.height + 60,
        corFundo);
    DrawRectangleLinesEx((Rectangle){opcoes[0].area.x - 20, opcoes[0].area.y - 40,
        opcoes[0].area.width + 40, (opcoes[numOpcoes-1].area.y - opcoes[0].area.y) + opcoes[0].area.height + 60},
        2, corBorda);

    Color corTitulo = (Color){50, 50, 50, 255};
    DrawText("TREX RUNNER", opcoes[0].area.x, opcoes[0].area.y - 30, 20, corTitulo);

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

void desenhar_tela_scores(ListaScores *lista, int largura, int altura) {
    Color corFundo = (Color){135, 206, 235, 200};
    Color corBorda = (Color){50, 50, 50, 255};
    Color corTexto = (Color){50, 50, 50, 255};

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
