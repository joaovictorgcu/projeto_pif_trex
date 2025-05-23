#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "scores.h"

typedef struct opcaoMenu {
    char texto[20];
    Rectangle area;
    Color corNormal;
    Color corSelecionada;
    int selecionada;
} OpcaoMenu;

void desenhar_menu(OpcaoMenu opcoes[], int numOpcoes);
void desenhar_tela_scores(ListaScores *lista, int largura, int altura);

#endif
