#ifndef CENARIO_H
#define CENARIO_H

#include "raylib.h"

// matrizes para desenhar o cenário
extern Vector2 montanhas[2][3]; 
extern Vector2 raiosSol[12][2];   
extern Color linhasCeu[720];       

void desenhar_montanhas(int largura, int altura);


void desenhar_sol(Vector2 posicao, float raio, float tempo);


void desenhar_ceu_diurno(int largura, int altura, float tempo);
void desenhar_chao(int largura, int altura);

#endif
