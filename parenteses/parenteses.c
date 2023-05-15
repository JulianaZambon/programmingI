#include "libpilha.h"
#include <stdio.h>
#include <stdlib.h>

int main () 
{
    char e; /*entrada*/
    int parenteses = 0, colchetes = 0, chaves = 0; /*var controle*/
    int erro = 0;
    pilha_t *pilha;

    pilha = pilha_cria();
    scanf("%c", &e);

    while( e != '\n'){
        /*para verificar somente os parenteses, colchetes e chaves*/
        if( e == '(' || e == ')' || e == '{' || e == '}' || e == '[' || e == ']'){
            push(pilha, e);
        }

        scanf("%c", &e);
    }

    /*começar a desempilhar até ficar vazia*/
    pop(pilha, &e);

    /*para verificar toda a fila*/
    while(!pilha_vazia(pilha)){

        /*fechar*/
        if(e == ')' || parenteses > 0)
            parenteses++;
        else if (e == ']' || colchetes > 0)
            colchetes++;
        else if (e == '}' || chaves > 0)
            chaves++;

        /*abrir*/
        else if(e == '(')
            parenteses = 0;
        else if (e == '[')
            colchetes = 0;
        else if (e == '{')
            chaves = 0;



        if(e == '(' && parenteses == 0){
		    erro = 1;
		    break;
	    }
	    if(e == '[' && colchetes == 0){
		    erro = 1;
		    break;
	    }
	    if(e == '{' && chaves == 0){
		    erro = 1;
		    break;
	    }
	    if(parenteses > colchetes || parenteses > chaves){
		    erro = 1;
		    break;
	    }
	    if(colchetes > chaves){
		    erro = 1;
		    break;
	    }

        pop(pilha, &e);
    }

    if (erro == 1)
		printf("INCORRETA\n");
	else
		printf("CORRETA\n");

    pilha_destroi(pilha);
    return 0;
}