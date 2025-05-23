#ifndef CENARIO_H
#define CENARIO_H

#include "raylib.h"

// Matrizes usadas para desenhar os elementos do cenário
extern Vector2 montanhas[2][3]; 
extern Vector2 raiosSol[12][2];   
extern Color linhasCeu[720];       

// Desenha as montanhas no fundo
void desenhar_montanhas(int largura, int altura);

// Desenha o sol animado
void desenhar_sol(Vector2 posicao, float raio, float tempo);

// Desenha o céu diurno (gradiente e detalhes)
void desenhar_ceu_diurno(int largura, int altura, float tempo);

// Desenha o chão (faixa marrom e linhas)
void desenhar_chao(int largura, int altura);

#endif
