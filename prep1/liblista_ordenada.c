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
int lista_insere_ordenado(lista_t *l, elemento_t *elemento)
{
    if (l == NULL || elemento == NULL)
        return 0;
    
    nodo_t *novo_nodo = (nodo_t *)malloc(sizeof(nodo_t));
    if (novo_nodo == NULL)
        return 0;
    
    novo_nodo->elemento = elemento;
    novo_nodo->prox = NULL;
    
    if (l->ini == NULL) {
        /*se a lista estiver vazia, o novo nodo se torna o primeiro elemento*/
        l->ini = novo_nodo;
        return 1;
    }
    
    nodo_t *atual = l->ini;
    nodo_t *anterior = NULL;
    
    /*percorre a lista ate encontrar a posicao correta para inserir o novo elemento*/
    while (atual != NULL && atual->elemento->chave < elemento->chave) {
        anterior = atual;
        atual = atual->prox;
    }
    
    if (anterior == NULL) {
        /*se o novo elemento deve ser inserido no início da lista*/
        novo_nodo->prox = l->ini;
        l->ini = novo_nodo;
    } else {
        /*se o novo elemento deve ser inserido no meio ou final da lista*/
        anterior->prox = novo_nodo;
        novo_nodo->prox = atual;
    }   
    return 1;
}

/* 
 * Retira o elemento da Lista e a mantem em ordem.
 * Retorna 1 em caso de sucesso e 0 caso elemento nao esteja na Lista.
*/
int lista_remove_ordenado(lista_t *l, elemento_t *elemento)
{
    if (l == NULL || elemento == NULL || l->ini == NULL) 
        return 0;
    
    nodo_t *atual = l->ini;
    nodo_t *anterior = NULL;
    
    /* compara o elemento atual da lista com o elemento recebido */
    while (atual != NULL && atual->elemento != elemento) {
        anterior = atual;
        atual = atual->prox;
    }
    
    if (atual == NULL)
        /*se o elemento nao foi encontrado na lista*/
        return 0;

    /*se o elemento foi encontrado*/
    if (anterior == NULL) {
        /*o elemento a ser removido eh o primeiro da lista*/
        l->ini = atual->prox;
    } else {
        /*o campo prox do elemento anterior é atualizado para pular o 
        elemento a ser removido, mantendo a ordem da lista.*/
        anterior->prox = atual->prox;
    }
    free(atual->elemento);
    free(atual);
    return 1;
}