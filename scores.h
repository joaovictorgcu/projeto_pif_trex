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

// Insere um novo score na lista em ordem decrescente
void inserir_score(ListaScores *lista, int pontos);

// Salva os scores no arquivo "Scores/scores.txt"
void salvar_scores(ListaScores *lista);

// Carrega os scores do arquivo "Scores/scores.txt"
void carregar_scores(ListaScores *lista);

// Libera toda a memória da lista de scores
void liberar_scores(ListaScores *lista);

#endif
