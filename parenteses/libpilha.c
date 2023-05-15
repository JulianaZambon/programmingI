#ifndef _LIBpilha_t_H
#define _LIBpilha_t_H
#include <stdlib.h>

typedef struct nodo {
    char dado;
    struct nodo *prox;
} nodo_t;

typedef struct pilha {
    struct nodo *topo;
    int tamanho;
} pilha_t;

/* 
 * Cria e retorna uma nova pilha.
 * Retorna NULL em caso de erro de alocação.
*/
pilha_t *pilha_cria ()
{
    pilha_t *pilha;
    if (!(pilha = malloc(sizeof(pilha_t))))
        return NULL;

    pilha->tamanho = 0;
    pilha->topo = NULL;
    
    return pilha;
}

/* Desaloca toda memoria da pilha e faz pilha receber NULL. */
void pilha_destroi (pilha_t **pilha)
{
    nodo_t *aux;

    if (*pilha == NULL)
        return NULL;

    while ((*pilha)->topo != NULL){
        aux = (*pilha)->topo;
        (*pilha)->topo = ((*pilha)->topo)->prox;
        free(aux);
    }

    free(*pilha);
    pilha = NULL;
    return;
}

/* 
 * Insere dado na pilha (politica LIFO). Retorna 1
 * em caso de sucesso e 0 em caso de falha.
*/
int push (pilha_t *pilha, char dado)
{
    nodo_t *aux;
    nodo_t *novo;

    if (pilha == NULL) /*caso a pilha não exista*/
        return 0;

    if (!(novo = malloc (sizeof(nodo_t))))
        return 0;

    novo->dado = dado;
    aux = pilha->topo;
    novo->prox = aux;

    (pilha)->topo = novo;
    pilha->tamanho++;
    return 1;
}

/* 
 * Remove o topo (politica LIFO) e retorna o elemento 
 * no parametro dado. A funcao retorna 1 em caso de 
 * sucesso e 0 no caso da pilha estar vazia.
*/
int pop (pilha_t *pilha, char *dado)
{
    nodo_t *aux;

    if (pilha == NULL || pilha->topo == NULL)
        return 0;
    
    aux = pilha->topo;
    pilha->topo = (pilha->topo)->prox;
    *dado = aux->dado;
    free(aux);
    pilha->tamanho--;
    return 1;
}
 
/* Similar ao pop, mas retorna o elemento dado sem remove-lo. */
int pilha_topo (pilha_t *pilha, char *dado)
{
    if (pilha == NULL || pilha->topo == NULL)
        return 0;

    *dado = (pilha->topo)->dado;
    return 1;
}

/* Retorna o numero de elementos da pilha, que pode ser 0. */
int pilha_tamanho (pilha_t *pilha)
{
    if (pilha == NULL)
        return 0;
    
    return pilha->tamanho;
}

/* Retorna 1 se pilha vazia, 0 em caso contrario. */ 
int pilha_vazia (pilha_t *pilha)
{
    if (pilha == NULL || pilha->topo == NULL)
        return 1;

    return 0;
}

#endif
