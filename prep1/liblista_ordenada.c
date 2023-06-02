#include <stdio.h>
#include <stdlib.h>
#include "liblista_ordenada.h"

typedef struct elemento {
    int chave;  /* chave da struct */
    /* pode ter mais campos, mas a chave sera usada sempre nas operacoes */
} elemento_t;

/*
 * nodo_t eh uma estrutura interna que representa cada nodo da Lista.
 * Externamente o usuario da biblioteca nao vai poder usa-la, pois
 * nao pode ter acesso direto ao ponteiro prox.
*/
typedef struct nodo {
    elemento_t *elemento;
    struct nodo *prox;
} nodo_t;

/*
 * Representacao da Lista 
*/
typedef struct lista {
    nodo_t *ini;
} lista_t;

/* 
 * Cria uma Lista vazia 
*/
lista_t *lista_cria()
{
    lista_t *lista = (lista_t *)malloc(sizeof(lista_t));
    if (lista == NULL)
        return NULL;
    
    lista->ini = NULL;
    return lista;
}

/* 
 * Destroi a Lista e a aponta para NULL
*/ 
void lista_destroi (lista_t **l)
{
    if (*l == NULL)
        return;
    
    nodo_t *atual = (*l)->ini;
    nodo_t *proximo;
    
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual->elemento);
        free(atual);
        atual = proximo;
    }
    
    free(*l);
    *l = NULL;
}

/* 
 * Adiciona um elemento em ordem de acordo com o valor elemento->chave na Lista. 
 * Retorna 1 em caso de sucesso e 0 caso contrario.
*/
int lista_insere_ordenado (lista_t *l, elemento_t *elemento)
{

}

/* 
 * Retira o elemento da Lista e a mantem em ordem.
 * Retorna 1 em caso de sucesso e 0 caso elemento nao esteja na Lista.
*/
int lista_remove_ordenado (lista_t *l, elemento_t *elemento)
{
    
}