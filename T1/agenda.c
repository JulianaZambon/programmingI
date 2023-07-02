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
      printf("%s\n", descricao);

      compromisso = cria_compromisso(hc, id_tarefa, descricao); /* Criar compromisso */

      /* Se o líder tem disponibilidade em sua agenda nos horários escolhidos */
      marcar = marca_compromisso_agenda(lider->agenda, dia, compromisso);

      if (marcar == 1) {

        /* Sortear ALEAT(2,6) membros (funcionários)
          - Para cada membro verificar
            se liderança líder > liderança membro +ALEAT(-20,10)
              - Se sim, tentar marcar a reunião na agenda do membro
          - Se nenhum dos membros puder participar, remova a reunião da
            agenda do líder.*/

        aleatorio = ALEAT(2, 6);
        control = 0; /* Controla se algum membro pode participar */
        printf("\tMEMBROS:");
        for (int i = 0; i < aleatorio; i++) {
          membro = &funcionarios[ALEAT(0, FUNCIONARIOS - 1)];
          if (lider->lideranca > membro->lideranca + ALEAT(-20, 10)) {
            marcar = marca_compromisso_agenda(membro->agenda, dia, compromisso);
            if (marcar == 1)
              control = 1;
            printf("%.2d, %s", membro->id, marcar == 1 ? "OK" : "IN"); /* OK se marcar = 1, IN se marcar = 0 */
          }
        }
        if (control == 0) {
          desmarca_compromisso_agenda(lider->agenda, dia, compromisso); 
          printf("\tVAZIA \n");
        }
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
void realizarReuniao()
{
  int qtde_reunioes_realizadas, qtde_tarefas_tempo_restante_zero, min_trab;
  qtde_reunioes_realizadas = 0;
  qtde_tarefas_tempo_restante_zero = 0;
  Funcionario *funcionario;
  compromisso_t *compromisso;
  Tarefa *tarefa;

  /*- Voltar para o mês 1 da agenda para "TRABALHAR":
  - Para cada dia entre 1 e 31 e para cada funcionário X 
      - Obter lista de compromissos e para cada compromisso
          - Se a tarefa[T] ainda não foi concluída 
            (tarefas[T].tempo_conclusao > 0):
              - Reduzir o tempo restante para concluir a tarefa de acordo com a
                seguinte fórmula: 
                  tarefas[T].tempo_conclusao -= min_trab * (funcs[X].experiencia / 100.0) * ((100 - tarefas[T].dificuldade) / 100.0);

              - Se o tempo restante para concluir a tarefa for menor ou igual 
                a zero:
                  tarefas[T].tempo_conclusao = 0;

              - Incrementar a experiência do funcionário em uma unidade 
                (limitar em 100)*/            

  for (int mes_atual = 1; mes_atual <= MES; mes_atual++) { /* Para cada mês de 1 até 12 */
    for (int dia = 1; dia <= 31; dia++) { /* Para cada dia entre 1 e 31 */
      for (int i = 0; i < FUNCIONARIOS; i++) { /* Para cada funcionário X*/
        funcionario = &funcionario[i]; /* Obter lista de compromissos */
        compromisso = funcionario->agenda->ptr_mes_atual->dias->comprs; /* Para cada compromisso */

        while (compromisso != NULL) { /* Se a tarefa[T] ainda não foi concluída */
          tarefa = &tarefa[compromisso->id]; 

          if (tarefa->tempo_conclusao > 0) { /* Reduzir o tempo restante para concluir a tarefa */
            /* Fórmula fornecida */
            min_trab = (compromisso->fim - compromisso->inicio) * 60; /* minutos trabalhados*/
            tarefa->tempo_conclusao -= min_trab * (funcionario->experiencia / 100.0) * ((100 - tarefa->dificuldade) / 100.0);

            if (tarefa->tempo_conclusao <= 0) { /* Se o tempo restante para concluir a tarefa for menor ou igual a zero */
              tarefa->tempo_conclusao = 0; /* Tempo restante para concluir a tarefa = 0 */
              funcionario->experiencia++; /* Incrementar a experiência do funcionário em uma unidade */
              qtde_tarefas_tempo_restante_zero++; /* Incrementar a quantidade de tarefas com tempo restante igual a zero */

              if (funcionario->experiencia > 100)
                funcionario->experiencia = 100; /* Limitar em 100 */
            }
          }
          qtde_reunioes_realizadas++; /* Incrementar a quantidade de reuniões realizadas */
          compromisso = compromisso->prox; /* Próximo compromisso */
        }
      }
    }
  }
  /* Infos finais */
  printf("REUNIOES REALIZADAS %.2d", qtde_reunioes_realizadas);
  printf("TAREFAS CONCLUIDAS %.2d", qtde_tarefas_tempo_restante_zero);
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