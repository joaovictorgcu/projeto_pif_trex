#include "scores.h"
#include <stdio.h>
#include <stdlib.h>

void inserir_score(ListaScores *lista, int pontos) {
    Score *novo = (Score*)malloc(sizeof(Score));
    novo->pontos = pontos;
    novo->proximo = NULL;

    if (lista->inicio == NULL || pontos > lista->inicio->pontos) {
        novo->proximo = lista->inicio;
        lista->inicio = novo;
    } else {
        Score *atual = lista->inicio;
        Score *anterior = NULL;
        while (atual != NULL && pontos <= atual->pontos) {
            anterior = atual;
            atual = atual->proximo;
        }
        anterior->proximo = novo;
        novo->proximo = atual;
    }
    lista->tamanho++;

    // deixa apenas os MAX_SCORES
    if (lista->tamanho > MAX_SCORES) {
        Score *atual = lista->inicio;
        int contador = 1;
        while (contador < MAX_SCORES) {
            atual = atual->proximo;
            contador++;
        }
        Score *temp = atual->proximo;
        atual->proximo = NULL;
        while (temp != NULL) {
            Score *proximo = temp->proximo;
            free(temp);
            temp = proximo;
            lista->tamanho--;
        }
    }
}

void salvar_scores(ListaScores *lista) {
    FILE *arquivo = fopen("Scores/scores.txt", "w");
    if (arquivo == NULL) return;
    Score *atual = lista->inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d\n", atual->pontos);
        atual = atual->proximo;
    }
    fclose(arquivo);
}

void carregar_scores(ListaScores *lista) {
    FILE *arquivo = fopen("Scores/scores.txt", "r");
    if (arquivo == NULL) return;
    int pontos;
    while (fscanf(arquivo, "%d", &pontos) == 1) {
        inserir_score(lista, pontos);
    }
    fclose(arquivo);
}

void liberar_scores(ListaScores *lista) {
    Score *atual = lista->inicio;
    while (atual != NULL) {
        Score *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    lista->inicio = NULL;
    lista->tamanho = 0;
}
