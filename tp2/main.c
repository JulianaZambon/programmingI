#include <stdio.h>
#include "libAgenda.h"

int main ()
{
   int ano;
   char entrada;

   printf("--> Entre com o ano:\n");
   scanf("%d", &ano);

   struct agenda ag = criaAgenda(ano);

   while (entrada != 's')
   {    
      struct compromisso compr;
      leCompromisso(&ag, &compr);

      if (validaData(&ag, &compr) == 1 && 
      verificaDisponibilidade(&ag, &compr) == 1)
      {
         marcaCompromisso(&ag, &compr);
         printf("Compromisso inserido com sucessso!\n");
      }

      printf("--> Digite um char qualquer ou s para sair\n");
      scanf("%c", &entrada);
   } 

   listaCompromissos(&ag);
}