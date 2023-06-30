/* inclui as libs e funcoes que serao usadas */
#include <stdio.h>
#include <string.h> /* para usar sprintf */
#include <stdlib.h>
#include <time.h>
#include "libagenda.h"

#define TAREFAS 100 /* qntd tarefas */
#define REUNIOES 100 /* qntd reunioes */
#define MES 12      /* qntd de meses */
#define FUNCIONARIOS 30 /* qntd de funcionarios */

/*------------------------------------------------------------------------*/

/* struct para o funcionario */
typedef struct
{
  int id;
  int lideranca;
  int experiencia;
  agenda_t *agenda
} Funcionario;

/* struct para as tarefas */
typedef struct
{
  int id;
  int tempo_conclusao;
  int dificuldade;
} Tarefa;

/*------------------------------------------------------------------------*/

/* função ALEAT(MIN,MAX) que gera inteiros aleatórios entre MIN e MAX. */
int ALEAT(int MIN, int MAX)
{
  return MIN + rand() % (MAX - MIN + 1);
}
/*------------------------------------------------------------------------*/
/* Inicialização dos vetores */
void iniciarFuncionarios (Funcionario *funcionarios)
{
   for (int i = 0; i < FUNCIONARIOS; i++) {
    funcionarios[i].agenda = cria_agenda(); /* Inicializar a agenda do funcionário */
    funcionarios[i].id = i;
    funcionarios[i].lideranca = ALEAT(0, 100);
    funcionarios[i].experiencia = ALEAT(20, 100);
  }
}

void iniciarTarefas (Tarefa *tarefas)
{
  for (int T = 0; T < TAREFAS; T++) {
    tarefas[T].id = T;
    tarefas[T].tempo_conclusao = ALEAT(600, 800);
    tarefas[T].dificuldade = ALEAT(30, 80);
  }
}
/*------------------------------------------------------------------------*/

/* Escolher aleatoriamente um líder entre os funcionários cuja 
          liderança esteja entre 30 e 70. */
Funcionario *escolherLider(Funcionario *funcionarios)
{
  Funcionario *lider;
  lider = &funcionarios[ALEAT(0, FUNCIONARIOS - 1)];

  while (lider->lideranca < 30 || lider->lideranca > 70)
    lider = &funcionarios[ALEAT(0, FUNCIONARIOS - 1)];

  return lider;
}

/*------------------------------------------------------------------------*/
/* Marcar reuniões */
int marcarReunioes ()
{
  Funcionario *lider;
  compromisso_t *compromisso;
  horario_compromisso_t hc;
  int dia;
  int id_tarefa;
  char descricao[100];

  /* Para cada mês de 1 até 12 */
  for (int mes_atual = 1; mes_atual <= MES; mes_atual++) {
    printf("M %.2d\n", mes_atual);
    /* Marcar 100 reuniões */
    for (int R = 0; R < REUNIOES; R++) {
    
      hc.ini_h = ALEAT(8, 12);
      hc.ini_m = ALEAT(0, 3) * 15;
      hc.fim_h = hc.ini_h + ALEAT(1, 4);
      hc.fim_m = hc.ini_m;
      dia = ALEAT(1, 31);
      id_tarefa = ALEAT(0, TAREFAS - 1);
      sprintf(descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", lider->id, dia, mes_atual,
              hc.ini_h, hc.ini_m, hc.fim_h, hc.fim_m, id_tarefa);

      escolherLider(funcionarios);
      compromisso = cria_compromisso(hc, id_tarefa, descricao); /* Criar compromisso */  

      /* Se o líder tem disponibilidade em sua agenda nos horários escolhidos */
      marca_compromisso_agenda(lider->agenda, dia, compromisso);

      /* Sortear ALEAT(2,6) membros (funcionários) 
            - Para cada membro verificar 
              se liderança líder > liderança membro +ALEAT(-20,10)  
                - Se sim, tentar marcar a reunião na agenda do membro
            - Se nenhum dos membros puder participar, remova a reunião da 
              agenda do líder.*/
    }
  }
}

/*------------------------------------------------------------------------*/
int main()
{
  srand(time(NULL));

  Funcionario funcionarios[FUNCIONARIOS];
  Tarefa tarefas[TAREFAS];

  iniciarFuncionarios(funcionarios);
  iniciarTarefas(tarefas);

  return 0;
}