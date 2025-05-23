#ifndef SCORES_H
#define SCORES_H

#define MAX_SCORES 5

typedef struct score {
    int pontos;
    struct score *proximo;
} Score;

typedef struct listaScores {
    int tamanho;
    Score *inicio;
} ListaScores;

void inserir_score(ListaScores *lista, int pontos);
void salvar_scores(ListaScores *lista);
void carregar_scores(ListaScores *lista);
void liberar_scores(ListaScores *lista);

#endif
