#include <stdio.h>
#include "libAgenda.h"

int main()
{
    int ano;
    char entrada;

    printf("--> Entre com o ano:\n");
    scanf("%d", &ano);

    struct agenda ag = criaAgenda(ano);

    while(entrada != 's')
    { 
        struct compromisso compr = leCompromisso();

        if (validaData(compr.data_compr, ag) == 1 && verificaDisponibilidade(compr, ag) == 1)
        {
            ag =  marcaCompromisso(ag, compr);
            printf("Compromisso inserido com sucesso!\n");
        }

        printf("--> Digite um char qualquer ou s para sair\n");
        scanf(" %c", &entrada); 
    }

    listaCompromissos(ag);
}