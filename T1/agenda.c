/* inclui as libs e funcoes que serao usadas */
#include <stdio.h>
#include <string.h> /* para usar sprintf */
#include <stdlib.h>
#include <time.h>
#include "libagenda.h"

#define TAREFAS 100     /* qntd tarefas */
#define REUNIOES 100    /* qntd reunioes */
#define MES 12          /* qntd de meses */
#define FUNCIONARIOS 30 /* qntd de funcionarios */

/*------------------------------------------------------------------------*/
/* struct para o funcionario */
typedef struct funcionario
{
  int id;
  int lideranca;
  int experiencia;
  agenda_t *agenda;
} Funcionario;

/* struct para as tarefas */
typedef struct tarefa
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
void iniciarFuncionarios(Funcionario *funcionarios)
{
  for (int i = 0; i < FUNCIONARIOS; i++) {
    funcionarios[i].agenda = cria_agenda(); /* Inicializar a agenda do funcionário */
    funcionarios[i].id = i;
    funcionarios[i].lideranca = ALEAT(0, 100);
    funcionarios[i].experiencia = ALEAT(20, 100);
  }
}

void iniciarTarefas(Tarefa *tarefas)
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
/* Marcar todas reuniões */
void marcarReunioes(Funcionario *funcionarios)
{
  Funcionario *lider, *membro;
  compromisso_t *compromisso;
  horario_compromisso_t hc;
  int dia, id_tarefa, marcar, aleatorio, control;
  char descricao[100];

  /* Para cada mês de 1 até 12 */
  for (int mes_atual = 1; mes_atual <= MES; mes_atual++) {
    printf("M %.2d\n", mes_atual);
    /* Marcar 100 reuniões */
    for (int R = 0; R < REUNIOES; R++) {

      hc.ini_h = ALEAT(8, 12); /* Sortear horário de início da reunião */
      hc.ini_m = ALEAT(0, 3) * 15; /* Sortear minutos de início da reunião */
      hc.fim_h = hc.ini_h + ALEAT(1, 4); /* Sortear horário de fim da reunião */
      hc.fim_m = hc.ini_m; /* Sortear minutos de fim da reunião */
      dia = ALEAT(1, 31); /* Sortear dia da reunião */
      id_tarefa = ALEAT(0, TAREFAS - 1); /* Sortear tarefa */

      lider = escolherLider(funcionarios);
      sprintf(descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", lider->id, dia, mes_atual,
              hc.ini_h, hc.ini_m, hc.fim_h, hc.fim_m, id_tarefa);
      printf("%s", descricao);

      compromisso = cria_compromisso(hc, id_tarefa, descricao); /* Criar compromisso */

      /* Se o líder tem disponibilidade em sua agenda nos horários escolhidos */
      marcar = marca_compromisso_agenda(lider->agenda, dia, compromisso);

      if (marcar == 1) {
        aleatorio = ALEAT(2, 6);
        control = 0; /* Controla se algum membro pode participar */
        printf("\tMEMBROS: ");
        for (int i = 0; i < aleatorio; i++) {
          membro = &funcionarios[ALEAT(0, FUNCIONARIOS - 1)];
          if (lider->lideranca > membro->lideranca + ALEAT(-20, 10)) {
            marcar = marca_compromisso_agenda(membro->agenda, dia, compromisso);
            if (marcar == 1)
              control = 1;
            printf("%.2d:%s", membro->id, marcar == 1 ? "OK " : "IN "); /* OK se marcar = 1, IN se marcar = 0 */
          }
        }
        if (control == 0) {
          desmarca_compromisso_agenda(lider->agenda, dia, compromisso); 
          printf("VAZIA ");
        }
        printf("\n");
      } else {
        desmarca_compromisso_agenda(lider->agenda, dia, compromisso); /* Remover compromisso */
        printf("\tLIDER INDISPONIVEL \n");
      }
    }
    /* Atualizar a agenda dos funcionários */
    for (int i = 0; i < FUNCIONARIOS; i++) {
      prox_mes_agenda(funcionarios[i].agenda); /* Avançar para o próximo mês */
    }
  }
}

/*------------------------------------------------------------------------*/
/* Realizar todas as reuniões marcadas*/
void realizarReuniao(Funcionario *funcionario, Tarefa *tarefa)
{
  int qtde_reunioes_realizadas, qtde_tarefas_tempo_restante_zero, min_trab;
  qtde_reunioes_realizadas = 0;
  qtde_tarefas_tempo_restante_zero = 0; 
  compromisso_t *compromisso;         

  for (int mes_atual = 1; mes_atual <= MES; mes_atual++) { /* Para cada mês de 1 até 12 */
    printf("M %.2d\n", mes_atual);

    for (int dia = 1; dia <= 31; dia++) { /* Para cada dia entre 1 e 31 */
      for (int i = 0; i < FUNCIONARIOS; i++) { 
        compromisso = compr_agenda(funcionario[i].agenda, dia); /* Obter lista de compromissos */

        while (compromisso != NULL) {
          Tarefa *tarefa_atual = &tarefa[compromisso->id];
 
          printf("%.2d/%.2d F %.2d: %s \n", dia, mes_atual, funcionario[i].id, compromisso->descricao);

            if (tarefa_atual->tempo_conclusao > 0) { 
              /* Fórmula fornecida */
              min_trab = (compromisso->fim - compromisso->inicio) * 60; /* minutos trabalhados*/
              tarefa_atual->tempo_conclusao -= min_trab * (funcionario[i].experiencia / 100.0) * 
                                                ((100 - tarefa_atual->dificuldade) / 100.0);

              if (tarefa_atual->tempo_conclusao <= 0) { 
                printf("\tCONCLUÍDA \n");
                tarefa_atual->tempo_conclusao = 0;
                funcionario[i].experiencia++; 
                qtde_tarefas_tempo_restante_zero++; 

                if (funcionario[i].experiencia > 100)
                  funcionario[i].experiencia = 100; /* Limitar em 100 */
              } else {
                printf("\tT %.2d D %.2d TCR %.2d\n", tarefa_atual->id, tarefa_atual->dificuldade,
                        tarefa_atual->tempo_conclusao);
              }
            } else {
              printf("\tT %.2d D %.2d TCR %.2d\n", tarefa_atual->id, tarefa_atual->dificuldade,
                      tarefa_atual->tempo_conclusao);
            }
          qtde_reunioes_realizadas++;
          compromisso = prox_compr(compromisso); /* Avançar para o próximo compromisso */
        }
      }
    }
  }
  /* Infos finais */
  printf("REUNIOES REALIZADAS %.2d\n", qtde_reunioes_realizadas);
  printf("TAREFAS CONCLUIDAS %.2d\n", qtde_tarefas_tempo_restante_zero);
}
/*------------------------------------------------------------------------*/
int main()
{
  srand(time(NULL));
  Funcionario funcionarios[FUNCIONARIOS];
  Tarefa tarefas[TAREFAS];
  iniciarFuncionarios(funcionarios);
  iniciarTarefas(tarefas);
  marcarReunioes(funcionarios);
  realizarReuniao(funcionarios, tarefas);

  return 0;
}