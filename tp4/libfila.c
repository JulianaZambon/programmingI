#ifndef _LIBfila_t_H
#define _LIBfila_t_H

#include <stdlib.h>

typedef struct nodo {
    int dado;
    struct nodo *prox;
} nodo_t;

typedef struct fila {
    nodo_t *cabeca;
    nodo_t *cauda;
    int tamanho;
} fila_t;

/* 
 * Cria e retorna uma nova fila.
 * Retorna NULL em caso de erro de alocação.
 */
fila_t *fila_cria()
{
    fila_t *fila;

    if (!(fila = malloc(sizeof(fila_t))))
        return NULL;

    fila->tamanho = 0;
    fila->cabeca = NULL;
    fila->cauda = NULL;

    return fila;
}

/* Desaloca toda memória da fila e faz fila receber NULL. */
void fila_destroi(fila_t **fila)
{
    nodo_t *aux;

    if (*fila == NULL)
        return;

    while ((*fila)->cabeca != NULL) {
        aux = (*fila)->cabeca;
        (*fila)->cabeca = ((*fila)->cabeca)->prox;
        free(aux);
    }

    free(*fila);
    *fila = NULL;
}

/* 
 * Insere dado no final da fila (política FIFO).
 * Retorna 1 em caso de sucesso e 0 em caso de falha.
 */
int enqueue(fila_t *fila, int dado)
{
    nodo_t *novo;

    if (fila == NULL)
        return 0;

    if (!(novo = malloc(sizeof(nodo_t))))
        return 0;

    novo->dado = dado;
    novo->prox = NULL;

    if (fila->cauda == NULL) {
        fila->cabeca = novo;
        fila->cauda = novo;
    } else {
        fila->cauda->prox = novo;
        fila->cauda = novo;
    }

    fila->tamanho++;
    return 1;
}

/* 
 * Remove o elemento do início da fila (política FIFO) e o retorna
 * no parâmetro dado. A função retorna 1 em caso de sucesso e 0 no caso da fila estar vazia.
 */
int dequeue(fila_t *fila, int *dado)
{
    nodo_t *aux;

    if (fila == NULL || fila->cabeca == NULL)
        return 0;

    aux = fila->cabeca;
    fila->cabeca = fila->cabeca->prox;
    *dado = aux->dado;
    free(aux);
    fila->tamanho--;
    return 1;
}

/* Retorna o número de elementos da fila, que pode ser 0. */
int fila_tamanho(fila_t *fila)
{
    if (fila == NULL)
        return 0;

    return fila->tamanho;
}

/* Retorna 1 se a fila está vazia, 0 caso contrário. */
int fila_vazia(fila_t *fila)
{
    if (fila == NULL || fila->cabeca == NULL || fila->cauda == NULL)
        return 1;

    return 0;
}

#endif