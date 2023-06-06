#include <stdlib.h>
#include "liblista_circular.h"
/* 
 * Cria uma Lista vazia 
*/
lista_t *lista_cria ()
{
    lista_t *l;

    if (!(l = malloc(sizeof(lista_t))))
        return NULL;

    l->ini = NULL;

    return l;
}

/* 
 * Destroi a Lista e a aponta para NULL
*/ 
void lista_destroi(lista_t **l) 
{
    if (l == NULL || *l == NULL)
        return;

    nodo_t *atual = (*l)->ini; /*atual eh atualizado com o inicio da lista*/
    nodo_t *prox;

    while (atual != NULL) {
        prox = atual->prox; /*armazena o proximo noh em proximo*/
        free(atual->elemento);
        free(atual);
        atual = prox; /*atualiza o ponteiro*/
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

    nodo_t *novo = malloc(sizeof(nodo_t));
    if (novo == NULL)
        return 0;

    novo->elemento = elemento;
    novo->prox = NULL;
    novo->prev = NULL;

    if (lista_vazia(l)) {
        /*se estiver vazia um novo noh eh atribuido ao inicio da lista*/
        l->ini = novo;
        return 1;
    }

    nodo_t *atual = l->ini;
    nodo_t *anterior = NULL;

    /*o noh atual possui uma chave maior ou igual a chave do novo elemento*/
    while (atual != NULL && atual->elemento->chave < elemento->chave) {
        anterior = atual;
        atual = atual->prox;
    }

    /*para inserir na posicao correta*/
    if (anterior == NULL) {
        /*indica que o novo noh sera o novo inicio da lista*/
        novo->prox = l->ini;
        l->ini->prev = novo;
        l->ini = novo;
    } else {
        /*o novo noh eh inserido entre o anterior e o atual,
         atualizando os ponteiros prox e prev */
        novo->prox = atual;
        novo->prev = anterior;
        anterior->prox = novo;
        if (atual != NULL)
            atual->prev = novo;
    }

    return 1;
}

/* 
 * Retira o elemento da Lista e a mantem em ordem.
 * Retorna 1 em caso de sucesso e 0 caso elemento nao esteja na Lista.
*/
int lista_remove_ordenado(lista_t *l, elemento_t *elemento) 
{
    if (l == NULL || elemento == NULL || lista_vazia(l))
        return 0;

    nodo_t *atual = l->ini;
    nodo_t *anterior = NULL;

    /*encontrar o elemento a ser removido*/
    while (atual != NULL && atual->elemento != elemento) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
        return 0;

    if (anterior == NULL) {
        l->ini = atual->prox;
        if (l->ini != NULL)
            l->ini->prev = NULL;
            /*esta no meio da lista*/
    } else {
        anterior->prox = atual->prox;
        if (atual->prox != NULL)
            atual->prox->prev = anterior;
    }

    free(atual->elemento);
    free(atual);

    return 1;
}

/* Retorna 1 se a lista esta vazia e 0 caso contrario */
int lista_vazia (lista_t *l)
{
   if (l == NULL || l->ini == NULL)
        return 1;

    return 0; 
}
