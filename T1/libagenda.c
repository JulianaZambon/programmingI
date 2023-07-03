#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libagenda.h"

/* Cria uma agenda vazia:
   O mes_atual deve ser inicializado com 1; ptr_mes_atual deve ser definido. */
agenda_t* cria_agenda()
{
   agenda_t *nova_agenda = malloc(sizeof(agenda_t));  /* aloca memoria para a agenda */

   /* Alocar memória para a estrutura mes_t */
   nova_agenda->ptr_mes_atual = malloc(sizeof(mes_t));
   nova_agenda->mes_atual = 1; /* mes_atual deve ser inicializado com 1 */

   if (nova_agenda->ptr_mes_atual == NULL) {
      free(nova_agenda);
      return NULL;
   }

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

   novo_compromisso->inicio = (hc.ini_h * 60 )+ hc.ini_m; /* converte o horario para minutos */
   novo_compromisso->fim = (hc.fim_h * 60) + hc.fim_m; /* converte o horario para minutos */
   novo_compromisso->id = id;
   novo_compromisso->descricao = malloc(sizeof(char) * (strlen(descricao) + 1));

   if (novo_compromisso->descricao == NULL) {
      free(novo_compromisso);
      return NULL; /* caso tenha falha na alocacao de memoria */
   }

   strcpy(novo_compromisso->descricao, descricao); /* copia a descricao para o compromisso */
   novo_compromisso->prox = NULL; /* prox deve ser inicializado com NULL */
   return novo_compromisso;
}

/* destroi a descricao de um compromisso */
void destroi_descricao_compromisso(compromisso_t* compr)
{
   if (compr == NULL)
      return;

   free(compr->descricao);
   compr->descricao = NULL;
}

/* destroi um compromisso */
void destroi_compromisso(compromisso_t* compr)
{
   if (compr == NULL)
      return;

   destroi_descricao_compromisso(compr);
   free(compr);
   compr = NULL;
}

/* Libera toda memoria associado a agenda. */
void destroi_agenda(agenda_t* agenda)
{
   if (agenda == NULL) /* caso a agenda esteja vazia */
      return;

   mes_t *atual = agenda->ptr_mes_atual;
   dia_t* atual_dia = atual->dias;
   dia_t* prox_dia = atual_dia->prox;
   while (atual_dia != NULL) {
      compromisso_t* atual_compromisso = atual_dia->comprs;
         while (atual_compromisso != NULL) {
            /* libera a memoria alocada para os compromissos */
            compromisso_t* prox_compromisso = atual_compromisso->prox;
            destroi_compromisso(atual_compromisso);
            atual_compromisso = prox_compromisso;
         }
      /* libera a memoria alocada para os dias */
      free(atual_dia);
      atual_dia = prox_dia;
      prox_dia = atual_dia->prox;
   }

   /* libera a memoria alocada para a agenda */
   free(atual);
   free(agenda);
   agenda->mes_atual = 0;
   agenda->ptr_mes_atual = NULL;
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

   /* verifica se o dia informado como parametro existe */
   if (dia < 1 || dia > 31)
      return 0;

   /* verifica se o compromisso tem interseccao com outro */
   mes_t *atual = agenda->ptr_mes_atual;

   while (atual->dias != NULL) {
      dia_t *dia_atual = atual->dias;
      
      while (dia_atual != NULL) {
         /* verifica se é o dia desejado */
         if (dia_atual->dia == dia) {
            compromisso_t *atual_compromisso = dia_atual->comprs;
            
            /* verifica se há interseção com outros compromissos */
            while (atual_compromisso != NULL) {
               if (atual_compromisso->inicio < compr->fim && compr->inicio < atual_compromisso->fim)
                  return -1;
               
               atual_compromisso = atual_compromisso->prox;
            }
            
            /* insere o compromisso no final da lista de compromissos */
            compr->prox = dia_atual->comprs;
            dia_atual->comprs = compr;
            
            return 1;
         }
         
         dia_atual = dia_atual->prox;
      }

      atual = atual->prox;
   }

   /* cria um novo dia e insere o compromisso */
   mes_t *novo_mes = malloc(sizeof(mes_t));
   if (novo_mes == NULL)
      return 0;  /* Erro na alocação de memória */

   novo_mes->dias = malloc(sizeof(dia_t));
   if (novo_mes->dias == NULL)
      return 0;  /* Erro na alocação de memória */

   novo_mes->dias->dia = dia;
   novo_mes->dias->comprs = compr;
   novo_mes->prox = agenda->ptr_mes_atual;
   agenda->ptr_mes_atual = novo_mes;

   /* Sucesso */
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

   /* verifica se o dia informado como parametro existe */
   if (dia < 1 || dia > 31)
      return 0;

   mes_t *atual = agenda->ptr_mes_atual;

   while (atual->dias != NULL) {
      dia_t *dia_atual = atual->dias;
      
      while (dia_atual != NULL) {
         /* verifica se é o dia desejado */
         if (dia_atual->dia == dia) {
            compromisso_t *atual_compromisso = dia_atual->comprs;
            compromisso_t *ant_compromisso = NULL;
            
            /* procura o compromisso */
            while (atual_compromisso != NULL) {
               if (atual_compromisso == compr) {
                  /* remove o compromisso da lista */
                  if (ant_compromisso == NULL)
                     dia_atual->comprs = atual_compromisso->prox;
                  else
                     ant_compromisso->prox = atual_compromisso->prox;
                  
                  /* libera a memoria alocada para o compromisso */
                  destroi_compromisso(atual_compromisso);
                  
                  return 1;
               }
               
               ant_compromisso = atual_compromisso;
               atual_compromisso = atual_compromisso->prox;
            }
            
            return 0;
         }
         
         dia_atual = dia_atual->prox;
      }

      atual = atual->prox;
   }

   return 0;
}

/* Imprime a agenda do mes atual */
void imprime_agenda_mes(agenda_t *agenda)
{
   if (agenda == NULL || agenda->ptr_mes_atual == NULL || agenda->ptr_mes_atual->dias == NULL)
      return;

   mes_t *atual = agenda->ptr_mes_atual;
   dia_t *atual_dia = atual->dias;
   compromisso_t *atual_compromisso = atual_dia->comprs;

   /* imprime o mes atual */
   printf("M %.2d\n", atual->mes);

   /* imprime os dias do mes atual */
   while (atual_dia != NULL) {
      printf("D %.2d\n", atual_dia->dia);

      /* imprime os compromissos do dia atual */
      while (atual_compromisso != NULL) {
         horario_compromisso_t hc = hc_compr(atual_compromisso);
         printf("C %.2d:%.2d %.2d:%.2d %.2d %s\n", hc.ini_h, hc.ini_m, hc.fim_h, hc.fim_m, 
         atual_compromisso->id, atual_compromisso->descricao);
         atual_compromisso = atual_compromisso->prox;
      }

      atual_dia = atual_dia->prox;
   }
}

/* Retorna o mes atual da agenda. */
/*OK*/
int mes_atual_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return 0;

   return agenda->mes_atual;
}

/* Ajusta o mes_atual para 1; caso o mes esteja alocado, ptr_mes_atual
 * apontara para o mes 1, caso contrario para NULL. */
/*OK*/
void prim_mes_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return;

   agenda->mes_atual = 1; /* mes_atual deve ser inicializado com 1 */

   if (agenda->ptr_mes_atual != NULL) /* caso o mes esteja alocado */
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

   agenda->mes_atual++; /* incrementa mes_atual */

   if (agenda->ptr_mes_atual != NULL) /* caso o mes esteja alocado */
      agenda->ptr_mes_atual = agenda->ptr_mes_atual->prox;
   else 
      agenda->ptr_mes_atual = NULL;

   return agenda->mes_atual;
}

/* Analogo ao prox_mes_agenda porem decrementa mes_atual. */
int ant_mes_agenda(agenda_t *agenda)
{
   if (agenda == NULL)
      return 0;

   agenda->mes_atual--; /* decrementa mes_atual */

   if (agenda->ptr_mes_atual != NULL) /* caso o mes esteja alocado */
      agenda->ptr_mes_atual = agenda->ptr_mes_atual->ant;
   else 
      agenda->ptr_mes_atual = NULL;

   return agenda->mes_atual;
}

/* Retorna um ponteiro para a lista ligada de compromissos de um dia do mes
   ou NULL se vazia. A lista de compromissos retornada pode ser percorrida
   usando-se a funcao prox_compr. */
compromisso_t *compr_agenda(agenda_t *agenda, int dia)
{
   if (agenda == NULL)
      return NULL;

   /* verifica se o dia informado como parametro existe */
   if (dia < 1 || dia > 31)
      return NULL;

   mes_t *atual = agenda->ptr_mes_atual;

   while (atual->dias != NULL) {
      dia_t *dia_atual = atual->dias;
      
      while (dia_atual != NULL) {
         /* verifica se é o dia desejado */
         if (dia_atual->dia == dia)
            return dia_atual->comprs;
         
         dia_atual = dia_atual->prox;
      }

      atual = atual->prox;
   }

   return NULL;
}

/* Retorna o primeiro compromisso da lista de compromissos compr e avanca
 * para o prox. Retorna NULL se a lista esta vazia, ou seja, sem compromissos. */
/*OK*/
compromisso_t *prox_compr(compromisso_t *compr)
{
   if (compr == NULL)
      return NULL;

   return compr->prox;
}

/*------------------------------------------------------------------------*/
/* As funcoes abaixo sao usadas para acessar os membros da struct compromisso
   obtidos com a funcao prox_compr. */
   /*OK*/
horario_compromisso_t hc_compr(compromisso_t *compr)
{
   horario_compromisso_t hc;
   hc.ini_h = compr->inicio / 60;
   hc.ini_m = compr->inicio % 60;
   hc.fim_h = compr->fim / 60;
   hc.fim_m = compr->fim % 60;
   return hc;
}

/*OK*/
int id_compr(compromisso_t *compr)
{
   return compr->id;
}

/*OK*/
char *descricao_compr(compromisso_t *compr)
{
   return compr->descricao;
}
