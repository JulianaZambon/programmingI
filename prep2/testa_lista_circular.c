#include <stdio.h>
#include "liblista_circular.h"

void teste_lista_cria() 
{
    lista_t *l = lista_cria();
    if (l != NULL) {
        printf("Lista criada com sucesso.\n");
    } else {
        printf("Erro ao criar a lista.\n");
    }
}

void teste_lista_destroi() 
{
    lista_t *l = lista_cria();
    /*fazer operacoes*/

    lista_destroi(&l);

    if (l == NULL) {
        printf("Lista destruída com sucesso.\n");
    } else {
        printf("Erro ao destruir a lista.\n");
    }
}

void teste_lista_insere_ordenado() 
{
    lista_t *l = lista_cria();
    elemento_t *elemento1 = malloc(sizeof(elemento_t));
    elemento1->chave = 10;

    int resultado = lista_insere_ordenado(l, elemento1);

    if (resultado == 1) {
        printf("Elemento inserido na lista com sucesso.\n");
    } else {
        printf("Erro ao inserir elemento na lista.\n");
    }
}

void teste_lista_remove_ordenado() 
{
    lista_t *l = lista_cria();
    elemento_t *elemento1 = malloc(sizeof(elemento_t));
    elemento1->chave = 10;

    lista_insere_ordenado(l, elemento1);

    int resultado = lista_remove_ordenado(l, elemento1);

    if (resultado == 1) {
        printf("Elemento removido da lista com sucesso.\n");
    } else {
        printf("Erro ao remover elemento da lista.\n");
    }
}

void teste_lista_vazia() 
{
    lista_t *l = lista_cria();
    
    int vazia = lista_vazia(l);

    if (vazia == 1) {
        printf("A lista está vazia.\n");
    } else {
        printf("A lista não está vazia.\n");
    }
}

int main() 
{
    teste_lista_cria();
    teste_lista_destroi();
    teste_lista_insere_ordenado();
    teste_lista_remove_ordenado();
    teste_lista_vazia();

    return 0;
}