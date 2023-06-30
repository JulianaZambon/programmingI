#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libagenda.h"

/* Cria uma agenda vazia:
   O mes_atual deve ser inicializado com 1; ptr_mes_atual deve ser definido. */
agenda_t* cria_agenda()
{
   agenda_t *nova_agenda = malloc(sizeof(agenda_t));

   if (nova_agenda == NULL)
      return NULL; /* caso tenha falha na alocacao de memoria */

   nova_agenda->mes_atual = 1;
   nova_agenda->ptr_mes_atual = malloc(sizeof(compromisso_t));

   if (nova_agenda->ptr_mes_atual == NULL) {
      free(nova_agenda);
      return NULL; /* caso tenha falha na alocacao de memoria */
   }

   nova_agenda->ptr_mes_atual->dias = NULL;
   nova_agenda->ptr_mes_atual->prox = nova_agenda->ptr_mes_atual;
   nova_agenda->ptr_mes_atual->ant = nova_agenda->ptr_mes_atual;
   nova_agenda->ptr_mes_atual->mes = 1;
   return nova_agenda;
}

/* Cria um compromisso:
   Retorna um compromisso com as informacoes de data de hc, um identificador
   id e uma string de descricao. A funcao deve alocar um novo espaco de
   armazenamento para receber a string descricao. */
compromisso_t *cria_compromisso(horario_compromisso_t hc, int id, char *descricao)
{
   compromisso_t *novo_compromisso = malloc(sizeof(compromisso_t));

   if (novo_compromisso == NULL)
      return NULL; /* caso tenha falha na alocacao de memoria */

   novo_compromisso->id = id;
   novo_compromisso->inicio = hc.ini_h;
   novo_compromisso->fim = hc.fim_h;

   /* aloca memoria para descricao e copia o conteudo */
   novo_compromisso->descricao = malloc(strlen(descricao) + 1);

   if (novo_compromisso->descricao == NULL) {
      free(novo_compromisso);
      return NULL; /* caso tenha falha na alocacao de memoria */
   }

   strcpy(novo_compromisso->descricao, descricao);
   return novo_compromisso;
}

/* destroi a descricao de um compromisso */
void destroi_descricao_compromisso(compromisso_t* compr)
{
   if (compr != NULL && compr->descricao != NULL) {
      free(compr->descricao);
      compr->descricao = NULL;
   }
}

/* Libera toda memoria associada a agenda. */
void destroi_agenda(agenda_t *agenda) {
    mes_t *atual_mes = agenda->ptr_mes_atual;
    mes_t *prox_mes;

    while (atual_mes != NULL) {
        prox_mes = atual_mes->prox;
        free(atual_mes->dias);
        free(atual_mes);
        atual_mes = prox_mes;
    }

    agenda->ptr_mes_atual = NULL;
    agenda->mes_atual = 0;
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

   while (atual != NULL) {
      /* verificar a intersecao entre os horarios de inicio e fim */
      if (compr->fim > atual->inicio && compr->inicio < atual->fim)
         return -1; /* intersecao */

      /* proximo compromisso */
      anterior = atual;
      atual = atual->prox;
   }

   /* inserir o novo compromisso na posição correta na lista ordenada */
   if (anterior == NULL) {
      /* novo compromisso será o primeiro da lista */
      agenda->ptr_mes_atual = compr;
   } else {
      /* novo compromisso será inserido após o compromisso anterior */
      anterior->prox = compr;
   }

   compr->prox = atual; /* para ajustar os ponteiros */
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

   compromisso_t *atual = agenda->ptr_mes_atual;
   compromisso_t *anterior = NULL;

   while (atual != NULL) {
      if (atual == compr) {
         if (anterior == NULL) {
            agenda->ptr_mes_atual = atual->prox;
         } else {
            anterior->prox = atual->prox;
         }

         destroi_descricao_compromisso(atual);
         free(atual);
         return 1;
      }

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

   printf("Agenda do mês %d:\n", agenda->mes_atual);

   compromisso_t *compr = agenda->ptr_mes_atual;

   while (compr != NULL) {
      printf("Dia %d: %s\n", compr->inicio, compr->descricao);
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

   /* verifica se esta alocado */
   if (agenda->ptr_mes_atual != NULL)
      free(agenda->ptr_mes_atual);

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
   if (compr == NULL) {
      return NULL;
   } else {
      return compr->prox;
   }
}

/* As funcoes abaixo sao usadas para acessar os membros da struct compromisso
   obtidos com a funcao prox_compr. */
horario_compromisso_t hc_compr(compromisso_t *compr)
{
   
}

int id_compr(compromisso_t *compr)
{
   return compr->id;
}

char *descricao_compr(compromisso_t *compr)
{
   return compr->descricao;
}
