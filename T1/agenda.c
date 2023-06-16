/*inclui as libs e funcoes que serao usadas*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libagenda.h"

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

/*function para gerar min e max random */
int aleatorio(int min, int max)
{
  return min + rand() % (max - min + 1);
}

int main()
{
  /* Inicialização */

  /*inicializa o gerador random*/
  srand(time(NULL));

  /*array de 30 funcionarios*/
  funcionario funcionarios[30];
  /*array de 100 tarefas*/
  tarefa tarefas[100];

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

  /* Marcar todas reuniões */

  /* Realizar todas as reuniões marcadas*/
}