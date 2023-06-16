/*inclui as libs e funcoes que serao usadas*/
#include <stdio.h>
#include <string.h> /*para usar sprintf*/
#include <stdlib.h>
#include <time.h>
#include "libagenda.h"

#define TAREFAS 100
#define MES 12

/*struct para o funcionario*/
/*30 funcionarios*/
typedef struct
{
  int lideranca;
  int experiencia;
} funcionario;

/*struct para as tarefas*/
/*100 tarefas*/
typedef struct
{
  int tempo_conclusao;
  int dificuldade;
} tarefa;

/*struct para as reunioes*/
typedef struct
{
  int hc_ini_h;        /*hora inicio*/
  int hc_ini_m;        /*minuto inicio*/
  int hc_fim_h;        /*hora fim*/
  int hc_fim_m;        /*minuto fim*/
  int dia;             /*dia*/
  int id;              /*id tarefa*/
  char descricao[100]; /*descricao*/
} reuniao;

/*function para gerar min e max random */
int aleatorio(int min, int max)
{
  return min + rand() % (max - min + 1);
}

/*Se o líder tem disponibilidade em sua agenda nos horários*/
int verificaDisponibilidade(reuniao reunioes[], int indice_membro, int hora_ini, int minuto_ini, int hora_fim, int minuto_fim, int dia)
{
  for (int i = 0; i < TAREFAS; i++)
  {
    reuniao reuniao = reunioes[i];
    if (i != indice_membro && reuniao.dia == dia)
    {
      if (!((hora_fim < reuniao.hc_ini_h) || (hora_ini > reuniao.hc_fim_h) ||
            (hora_fim == reuniao.hc_ini_h && minuto_fim <= reuniao.hc_ini_m) ||
            (hora_ini == reuniao.hc_fim_h && minuto_ini >= reuniao.hc_fim_m)))
      {
        return 0; /*membro nao disponivel*/
      }
    }
  }
  return 1; /*membro disponivel*/
}

/*Para cada membro verificar:
 se liderança líder > liderança membro +ALEAT(-20,10) */
int verificaLideranca(funcionario lider, funcionario membro)
{
  int limite_superior = lider.lideranca;
  int limite_inferior = lider.lideranca - 20;
  return (membro.lideranca >= limite_inferior && membro.lideranca <= limite_superior);
}

int main()
{

  /*------------------------------------------------------------------------*/
  /* Inicialização */

  /*inicializa o gerador random*/
  srand(time(NULL));

  /*array de 30 funcionarios*/
  funcionario funcionarios[30];
  /*array de 100 tarefas*/
  tarefa tarefas[100];
  /*array de reunioes*/
  reuniao reunioes[TAREFAS];

  /*parametros dos funcionarios*/
  for (int i = 0; i < 30; i++)
  {
    funcionarios[i].lideranca = aleatorio(0, 100);
    funcionarios[i].experiencia = aleatorio(20, 100);
  }

  /*parametros das tarefas*/
  for (int i = 0; i < 100; i++)
  {
    tarefas[i].tempo_conclusao = aleatorio(600, 800);
    tarefas[i].dificuldade = aleatorio(30, 80);
  }
  /*------------------------------------------------------------------------*/
  /* Marcar todas reuniões */

  /*Para cada mês de 1 até 12:
    - Marcar 100 reuniões:*/

  int mes_atual;
  for (mes_atual = 1; mes_atual <= MES; mes_atual++)
  {
    for (int i = 0; i < 100; i++)
    {

      /*  - Escolher aleatoriamente um líder entre os funcionários cuja
            liderança esteja entre 30 e 70.*/
      int lider;

      do
      {
        lider = aleatorio(0, 29);                                                         /*aleatorio*/
      } while (funcionarios[lider].lideranca < 30 || funcionarios[lider].lideranca > 70); /*lideranca*/

      /*- Criar uma nova reunião com os seguintes parâmetros:
                  - Hora de início (hc.ini_h): um valor aleatório entre 8 e 12.
                  - Minuto de início (hc.ini_m): um valor aleatório múltiplo
                    de 15 entre 0 e 45.
                  - Hora de término (hc.fim_h): hora de início mais um valor
                    aleatório entre 1h e 4h.
                  - Minuto de término (hc.fim_m): mesmo minuto de início.
                  - Dia: um valor aleatório entre 1 e 31.
                  - ID: um número aleatório entre 0 e TAREFAS-1, que representa
                    uma tarefa a ser discutida na reunião.*/

      reunioes[i].hc_ini_h = aleatorio(8, 12);
      reunioes[i].hc_ini_m = aleatorio(0, 3) * 15;
      reunioes[i].hc_fim_h = reunioes[i].hc_ini_h + aleatorio(1, 4);
      reunioes[i].hc_fim_m = reunioes[i].hc_ini_m;
      reunioes[i].dia = aleatorio(1, 31);
      reunioes[i].id = aleatorio(0, TAREFAS - 1);

      /* - Descrição: uma string descrevendo a reunião.*/
      sprintf(reunioes[i].descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", lider, reunioes[i].dia, mes_atual,
              reunioes[i].hc_ini_h, reunioes[i].hc_ini_m, reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].id);

      /* - Se o líder tem disponibilidade em sua agenda nos horários
                escolhidos:
                  - Sortear ALEAT(2,6) membros (funcionários) */

      int num_membros = aleatorio(2, 6);
      int membros[num_membros];

      /*sorteio dos membros*/
      for (int j = 0; j < num_membros; j++)
      {
        int membro;
        do
        {
          membro = aleatorio(0, 29);
        } while (membro == lider);

        membros[j] = membro;
      }
      /*verifica a lideranca e disponibilidade*/
      int marcada = 0;
      for (int j = 0; j < num_membros; j++)
      {
        int membro = membros[j];

        if (verificaLideranca(funcionarios[lider], funcionarios[membro]))
        {
          if (verificaDisponibilidade(reunioes, membro, reunioes[i].hc_ini_h, reunioes[i].hc_ini_m,
                                      reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].dia))
          {
            marcada = 1; /*pode marcar a reuniao*/
            break;
          }
        }
      }
    }
    /*------------------------------------------------------------------------*/
    /* Realizar todas as reuniões marcadas*/
  }
  return 0;
}