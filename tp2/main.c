#include <stdio.h>
#include "libAgenda.h"

int main ()
{
   char entrada;

   struct agenda ag;
   struct compromisso compr;
   struct agenda *ptrAg = &ag;
   struct compromisso *ptrCompr = &compr;

   scanf("%d", &ag.ano);
   ag = criaAgenda(ag.ano);

   while (entrada != 's')
   {
      if (leCompromisso (ptrAg, ptrCompr))
         printf("Compromisso inserido com sucesso!\n");
      else if (!verificaDisponibilidade(ptrAg, ptrCompr))
         printf("Data/Hora ocupada, compromisso nao inserido\n");
      else
         printf("Data e/ou hora invalidos, compromiso nao inserido\n");

      scanf(" %c", &entrada);
   }

   listaCompromissos(&ag);
}