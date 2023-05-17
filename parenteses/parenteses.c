#include "libpilha.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char e; /* entrada */
    int parenteses = 0, colchetes = 0, chaves = 0; /* variáveis de controle */
    int erro = 0;
    pilha_t *pilha;

    pilha = pilha_cria();
    scanf("%c", &e);

    while (e != '\n')
    {
        /* para verificar somente os parenteses, colchetes e chaves, 
        ignorando outros caracteres*/
        if (e == '(' || e == ')' || e == '{' || e == '}' || e == '[' || e == ']')
        {
            if (e == '(')
                parenteses++;
            else if (e == '[')
                colchetes++;
            else if (e == '{')
                chaves++;

            push(pilha, e);
        }

        scanf("%c", &e);
    }

    /* desempilhar até ficar vazia */
    while (!pilha_vazia(pilha))
    {
        if (!pop(pilha, &e))
        {
            erro = 1;
            break;
        }

        /* fechar */
        if (e == ')')
            parenteses--;
        else if (e == ']')
            colchetes--;
        else if (e == '}')
            chaves--;

        if (parenteses < 0 || colchetes < 0 || chaves < 0)
        {
            erro = 1;
            break;
        }
    }

    if (!pilha_vazia(pilha))
        erro = 1;

    if (erro == 1 || parenteses != 0 || colchetes != 0 || chaves != 0)
        printf("INCORRETA\n");
    else
        printf("CORRETA\n");

    pilha_destroi(&pilha);
    return 0;
}