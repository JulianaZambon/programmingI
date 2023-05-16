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
fila_t *fila_cria ()
{
    fila_t *fila;
    if (!(fila = malloc(sizeof(fila_t))))
        return NULL;

    fila->tamanho = 0;
    fila->cabeca = NULL;
    fila->cauda = NULL;

    return fila;
}

/* Desaloca toda memoria da fila e faz fila receber NULL. */
void fila_destroi (fila_t **fila)
{
    nodo_t *aux;

    if (*fila == NULL)
        return;

    while (((*fila)->cabeca != NULL) && ((*fila)->cauda != NULL)){
        aux = (*fila)->cabeca;
        (*fila)->cabeca = ((*fila)->cabeca)->prox;
        free(aux);
    }

    free(*fila);
    fila = NULL;
    return;
}

/* 
 * Insere dado no final da fila (politica FIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int enqueue (fila_t *fila, int dado)
{
    nodo_t *aux;
    nodo_t *novo;

    if (fila == NULL)
        return 0;

    if (!(novo = malloc (sizeof(nodo_t))))
        return 0;

    novo->dado = dado;
    aux = fila->cauda;
    novo->prox = aux;

    (fila)->cauda = novo;
    fila->tamanho++;
    return 1;
}

/* 
 * Remove o elemento do inicio da fila (politica FIFO) e o retorna
 * no parametro dado. Nao confundir com o retorno da funcao.
 * A funcao retorna 1 em caso de sucesso e 0 no caso da fila estar vazia.
*/
int dequeue (fila_t *fila, int *dado)
{
    nodo_t *aux;

    if (fila == NULL || fila->cabeca == NULL)
        return 0;

    aux = fila->cabeca;
    fila->cabeca = (fila->cabeca)->prox;
    *dado = aux->dado;
    free(aux);
    fila->tamanho--;
    return 1;
}
 
/* Retorna o numero de elementos da fila, que pode ser 0. */
int fila_tamanho (fila_t *fila)
{
    if (fila == NULL)
        return 0;

    return fila->tamanho;
}

/* Retorna 1 se fila vazia, 0 em caso contrario. */ 
int fila_vazia (fila_t *fila)
{
    if (fila == NULL || fila->cabeca == NULL || fila->cauda == NULL)
        return 1;

    return 0;
}

#endif