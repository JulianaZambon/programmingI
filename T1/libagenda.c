#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libagenda.h"

/* Cria uma agenda vazia:
   O mes_atual deve ser inicializado com 1; ptr_mes_atual deve ser definido. */
agenda_t* cria_agenda()
{
   agenda_t *nova_agenda = malloc(sizeof(agenda_t));  /* aloca memoria para a agenda */

   if (nova_agenda == NULL)
      return NULL; /* caso tenha falha na alocacao de memoria */

   nova_agenda->ptr_mes_atual->dias = NULL; /* dias deve ser inicializado com NULL */
   nova_agenda->ptr_mes_atual->prox = nova_agenda->ptr_mes_atual; /* prox deve ser inicializado com ele mesmo */
   nova_agenda->ptr_mes_atual->ant = nova_agenda->ptr_mes_atual; /* ant deve ser inicializado com ele mesmo */
   nova_agenda->ptr_mes_atual->mes = 1; /* mes_atual deve ser inicializado com 1 */
   return nova_agenda;  
}

/* Cria um compromisso:
   Retorna um compromisso com as informacoes de data de hc, um identificador
   id e uma string de descricao. A funcao deve alocar um novo espaco de
   armazenamento para receber a string descricao. */
compromisso_t *cria_compromisso(horario_compromisso_t hc, int id, char *descricao)
{
   compromisso_t *novo_compromisso = malloc(sizeof(compromisso_t)); /* aloca memoria para o compromisso */

   if (novo_compromisso == NULL)
      return NULL; /* caso tenha falha na alocacao de memoria */

   novo_compromisso->inicio = hc.ini_h;
   novo_compromisso->fim = hc.fim_h;
   novo_compromisso->id = id;
   novo_compromisso->descricao = malloc(sizeof(char) * (strlen(descricao) + 1));

   if (novo_compromisso->descricao == NULL) {
      free(novo_compromisso);
      return NULL; /* caso tenha falha na alocacao de memoria */
   }

   strcpy(novo_compromisso->descricao, descricao);
   novo_compromisso->prox = NULL;
   return novo_compromisso;
}

/* destroi a descricao de um compromisso */
void destroi_descricao_compromisso(compromisso_t* compr)
{
   if (compr == NULL)
      return;

   free(compr->descricao);
}

/* Marca um compromisso na agenda:
   valores de retorno possiveis:
    -1: compromisso tem interseccao com outro
     0: erro de alocacao de memoria
     1: sucesso

    A lista de compromisso eh ordenada pelo horario de inicio. Eh necessario
    testar a interseccao entre horarios de inicio e de fim no compromisso novo
    considerando o compromisso anterior e o proximo, caso existam. */
int marca_compromisso_agenda(agenda_t *agenda, int dia, compromisso_t *compr)
{
   if (agenda == NULL || compr == NULL)
      return 0;

   /* para percorrer a lista de compromissos */
   compromisso_t *atual = agenda->ptr_mes_atual;
   compromisso_t *anterior = NULL;

   /* verifica se o dia informado como parametro existe */
   if (dia < 1 || dia > 31)
      return 0;

   /* verifica se o compromisso tem interseccao com outro */
   while (atual != NULL) {
      /* verifica se o compromisso atual tem interseccao com o compromisso a ser inserido */
      if (atual->inicio <= compr->inicio && compr->inicio < atual->fim)
         return -1;

      if (atual->inicio < compr->fim && compr->fim <= atual->fim)
         return -1;

      /* verifica se o compromisso a ser inserido tem interseccao com o compromisso atual */
      if (compr->inicio <= atual->inicio && atual->inicio < compr->fim)
         return -1;

      if (compr->inicio < atual->fim && atual->fim <= compr->fim)
         return -1;

      /* proximo compromisso */
      anterior = atual;
      atual = atual->prox;
   }

   /* insere o compromisso na lista */
   if (anterior == NULL) {
      /* compromisso a ser inserido é o primeiro da lista */
      agenda->ptr_mes_atual = compr;
   } else {
      /* compromisso a ser inserido não é o primeiro da lista */
      anterior->prox = compr;
   }

   /* sucesso */
   return 1;
}

/* Desmarca o compromisso compr da agenda:
   O parametro compr eh um ponteiro para um compromisso existente da agenda.
   valores de retorno possiveis:
    1: em caso de sucesso
    0: caso nao tenha encontrado o compr */
int desmarca_compromisso_agenda(agenda_t *agenda, int dia, compromisso_t *compr)
{
   if (agenda == NULL || compr == NULL)
      return 0;

   /* para percorrer a lista de compromissos */
   compromisso_t *atual = agenda->ptr_mes_atual;
   compromisso_t *anterior = NULL;

   while (atual != NULL) {
      /* verifica se o compromisso atual é o compromisso a ser removido */
      if (atual == compr) {
         /* remove o compromisso da lista */
         if (anterior == NULL) {
            /* compromisso a ser removido é o primeiro da lista */
            agenda->ptr_mes_atual = atual->prox;
         } else {
            /* compromisso a ser removido não é o primeiro da lista */
            anterior->prox = atual->prox;
         }

         /* libera a memoria alocada para o compromisso */
         destroi_descricao_compromisso(atual);
         free(atual);
         return 1;
      }

      /* proximo compromisso */
      anterior = atual;
      atual = atual->prox;
   }
   return 0;
}

/* Imprime a agenda do mes atual */
void imprime_agenda_mes(agenda_t *agenda)
{
   if (agenda == NULL)
      return;

   compromisso_t *compr = agenda->ptr_mes_atual;

   while (compr != NULL) {
      printf("%d %d %d %d %s\n", agenda->mes_atual, compr->inicio, compr->fim,
             compr->id, compr->descricao);
      compr = compr->prox;
   }
}

/* Retorna o mes atual da agenda. */
int mes_atual_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return 0;

   return agenda->mes_atual;
}

/* Ajusta o mes_atual para 1; caso o mes esteja alocado, ptr_mes_atual
 * apontara para o mes 1, caso contrario para NULL. */
void prim_mes_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return;

   agenda->mes_atual = 1;

   if (agenda->ptr_mes_atual != NULL)
      agenda->ptr_mes_atual = agenda->ptr_mes_atual->prox;
   else
      agenda->ptr_mes_atual = NULL;
}

/* Avanca a agenda para o proximo mes, incrementando mes_atual.
 * O ponteiro ptr_mes_atual deve ser atualizado para apontar para o novo mes_atual.
 * Se o novo mes_atual nao existir deve ser alocado. A funcao retorna o inteiro
 * mes_atual em caso de sucesso ou 0 caso contrario.  */
int prox_mes_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return 0;

   agenda->mes_atual++;          /* incrementa */

   /* libera a memoria alocada para o mes atual */
   if (agenda->ptr_mes_atual != NULL)
      free(agenda->ptr_mes_atual);

   agenda->ptr_mes_atual = NULL; /* atualiza o ponteiro */

   /* verifica se o mes existe */
   if (agenda->mes_atual > 0 && agenda->mes_atual <= 12) {
      agenda->ptr_mes_atual = malloc(sizeof(compromisso_t));
      if (agenda->ptr_mes_atual == NULL)
         return 0;
   }
   /* sucesso */
   return agenda->mes_atual;
}

/* Analogo ao prox_mes_agenda porem decrementa mes_atual. */
int ant_mes_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return 0;

   agenda->mes_atual--;          /* decrementa */

   /* libera a memoria alocada para o mes atual */
   if (agenda->ptr_mes_atual != NULL)
      free(agenda->ptr_mes_atual);

   agenda->ptr_mes_atual = NULL; /* atualiza o ponteiro */

   /* verifica se o mes existe */
   if (agenda->mes_atual > 0 && agenda->mes_atual <= 12) {
      agenda->ptr_mes_atual = malloc(sizeof(compromisso_t));
      if (agenda->ptr_mes_atual == NULL)
         return 0;
   }
   /* sucesso */
   return agenda->mes_atual;
}

/* Retorna um ponteiro para a lista ligada de compromissos de um dia do mes
   ou NULL se vazia. A lista de compromissos retornada pode ser percorrida
   usando-se a funcao prox_compr. */
compromisso_t *compr_agenda(agenda_t *agenda, int dia)
{
   if (agenda == NULL)
      return NULL;

   compromisso_t *compr = agenda->ptr_mes_atual;

   while (compr != NULL) {
      /* procura compromissos que tenham o mesmo dia informado como parametro */
      if (compr->inicio == dia)
         return compr;

      compr = compr->prox;
   }
   return NULL;
}

/* Retorna o primeiro compromisso da lista de compromissos compr e avanca
 * para o prox. Retorna NULL se a lista esta vazia, ou seja, sem compromissos. */
compromisso_t *prox_compr(compromisso_t *compr)
{
   if (compr == NULL)
      return NULL;

   return compr->prox;
}

/*------------------------------------------------------------------------*/
/* As funcoes abaixo sao usadas para acessar os membros da struct compromisso
   obtidos com a funcao prox_compr. */
horario_compromisso_t hc_compr(compromisso_t *compr)
{
   return (horario_compromisso_t){compr->inicio, compr->fim};
}

int id_compr(compromisso_t *compr)
{
   return compr->id;
}

char *descricao_compr(compromisso_t *compr)
{
   return compr->descricao;
}
