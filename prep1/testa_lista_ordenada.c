#include <stdio.h>
#include <stdlib.h>
#include "liblista_ordenada.h"

void teste_lista_cria()
{
    lista_t *lista = lista_cria();
    if (lista != NULL) {
        printf("Teste lista_cria: Sucesso\n");
    } else {
        printf("Teste lista_cria: Falha\n");
    }
}

void teste_lista_insere_ordenado()
{
    lista_t *lista = lista_cria();

    elemento_t *elemento1 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento1->chave = 5;
    lista_insere_ordenado(lista, elemento1);

    elemento_t *elemento2 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento2->chave = 2;
    lista_insere_ordenado(lista, elemento2);

    elemento_t *elemento3 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento3->chave = 7;
    lista_insere_ordenado(lista, elemento3);

    elemento_t *elemento4 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento4->chave = 4;
    lista_insere_ordenado(lista, elemento4);

    /*verificando se os elementos foram inseridos corretamente*/
    nodo_t *atual = lista->ini;
    int valores_corretos = 1;
    if (atual->elemento != elemento2 || atual->prox->elemento != elemento4 ||
        atual->prox->prox->elemento != elemento1 || atual->prox->prox->prox->elemento != elemento3) {
        valores_corretos = 0;
    }

    if (valores_corretos) {
        printf("Teste lista_insere_ordenado: Sucesso\n");
    } else {
        printf("Teste lista_insere_ordenado: Falha\n");
    }

    lista_destroi(&lista);
    free(elemento1);
    free(elemento2);
    free(elemento3);
    free(elemento4);
}

void teste_lista_remove_ordenado()
{
    lista_t *lista = lista_cria();

    elemento_t *elemento1 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento1->chave = 5;
    lista_insere_ordenado(lista, elemento1);

    elemento_t *elemento2 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento2->chave = 2;
    lista_insere_ordenado(lista, elemento2);

    elemento_t *elemento3 = (elemento_t *)malloc(sizeof(elemento_t));
    elemento3->chave = 7;
    lista_insere_ordenado(lista, elemento3);

    /*remover um elemento*/
    int removido = lista_remove_ordenado(lista, elemento2);

    /*verificando se o elemento foi removido corretamente*/
    nodo_t *atual = lista->ini;
    int valor_correto = atual->elemento != elemento2;

    if (removido && valor_correto) {
        printf("Teste lista_remove_ordenado: Sucesso\n");
    } else {
        printf("Teste lista_remove_ordenado: Falha\n");
    }

    lista_destroi(&lista);
    free(elemento1);
    free(elemento3);
}

int main()
{
    teste_lista_cria();
    teste_lista_insere_ordenado();
    teste_lista_remove_ordenado();

    return 0;
}


