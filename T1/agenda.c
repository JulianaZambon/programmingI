/*inclui as libs e funcoes que serao usadas*/
#include <stdio.h>
#include <string.h> /*para usar sprintf*/
#include <stdlib.h>
#include <time.h>
#include "libagenda.h"

#define TAREFAS 100 /*qntd tarefas*/
#define MES 12      /*qntd de meses*/
#define FUNCIONARIOS 30 /*qntd de funcionarios*/

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
  int hc_ini_h;                      /*hora inicio*/
  int hc_ini_m;                      /*minuto inicio*/
  int hc_fim_h;                      /*hora fim*/
  int hc_fim_m;                      /*minuto fim*/
  int dia;                           /*dia*/
  int id;                            /*id tarefa*/
  char descricao[100];               /*descricao*/
  int disponibilidade[FUNCIONARIOS]; /* disponibilidade dos membros */
} reuniao;

/*function para gerar min e max random */
int aleatorio(int min, int max)
{
  return min + rand() % (max - min + 1);
}

/*Se o líder tem disponibilidade em sua agenda nos horários*/
int verificaDisponibilidade(reuniao reunioes[], int funcionario, int hc_ini_h, int hc_ini_m, int hc_fim_h, int hc_fim_m, int dia) {
  for (int i = 0; i < TAREFAS; i++) {
    if (reunioes[i].dia == dia &&
        ((reunioes[i].hc_ini_h < hc_fim_h && reunioes[i].hc_fim_h > hc_ini_h) ||
         (reunioes[i].hc_ini_h == hc_fim_h && reunioes[i].hc_ini_m < hc_fim_m) ||
         (reunioes[i].hc_fim_h == hc_ini_h && reunioes[i].hc_fim_m > hc_ini_m))) {
      if (reunioes[i].disponibilidade[funcionario] == 0) {
        return 0; /* Funcionário indisponível */
      }
    }
  }
  return 1; /* Funcionário disponível */
}


/*Para cada membro verificar:
 se liderança líder > liderança membro +ALEAT(-20,10) */
int verificaLideranca(funcionario lider, funcionario membro)
{
  int limite_superior = lider.lideranca;
  int limite_inferior = lider.lideranca - 20;
  return (membro.lideranca >= limite_inferior && membro.lideranca <= limite_superior);
}

/*
infos finais da saida do programa
-----
REUNIOES REALIZADAS qtdes_reunioes_realizadas
TAREFAS CONCLUIDAS qtde_tarefas_tempo_restante_zero
*/
void imprimirResumoFinal(int qtde_tarefas_tempo_restante_zero, int qtdes_reunioes_realizadas)
{
  printf("REUNIOES REALIZADAS %d\n", qtdes_reunioes_realizadas);
  printf("TAREFAS CONCLUIDAS %d\n", qtde_tarefas_tempo_restante_zero);
}

/*principal*/
int main()
{

  /*------------------------------------------------------------------------*/
  /* Inicialização */

  /*inicializa o gerador random*/
  srand(time(NULL));


  /*inicializacao das variáveis*/

  /*array de 30 funcionarios*/
  funcionario funcionarios[FUNCIONARIOS];
  /*array de 100 tarefas*/
  tarefa tarefas[TAREFAS];
  /*array de reunioes*/
  reuniao reunioes[TAREFAS];
  reuniao reunioes_realizadas[TAREFAS];
  int qtdes_reunioes_realizadas = 0;
  int qtde_tarefas_tempo_restante_zero = 0;

  /*parametros dos funcionarios*/
  for (int i = 0; i < 30; i++) {
    funcionarios[i].lideranca = aleatorio(0, 100);
    funcionarios[i].experiencia = aleatorio(20, 100);
  }

  /*parametros das tarefas*/
  for (int T = 0; T < TAREFAS; T++) {
    tarefas[T].tempo_conclusao = aleatorio(600, 800);
    tarefas[T].dificuldade = aleatorio(30, 80);
  }
  
  /*------------------------------------------------------------------------*/
  /* Marcar todas reuniões */
  /*Para cada mês de 1 até 12:
    - Marcar 100 reuniões:*/
  int mes_atual;
  for (mes_atual = 1; mes_atual <= MES; mes_atual++) {
    printf("M %d \n", mes_atual); /*imprime a agenda do mes atual*/

    for (int i = 0; i < TAREFAS; i++) {
      /*  - Escolher aleatoriamente um líder entre os funcionários cuja
            liderança esteja entre 30 e 70.*/
      int lider;
      do {
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

      /* descrição da reunião.*/
      sprintf(reunioes[i].descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", lider, reunioes[i].dia, mes_atual,
              reunioes[i].hc_ini_h, reunioes[i].hc_ini_m, reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].id);
      printf("%s", reunioes[i].descricao);

      /* - Se o líder tem disponibilidade em sua agenda nos horários escolhidos:
          - Sortear ALEAT(2,6) membros (funcionários) */
      int num_membros = aleatorio(2, 6);
      int membros[num_membros];

      /*sorteio dos membros*/
      for (int k = 0; k < num_membros; k++) {
        int membro;
        do {
          membro = aleatorio(0, 29);
        } while (membro == lider);

        membros[k] = membro;
      }

      /*
      saída na marcação das reuniões
        ------------------------------

        M mes_atual (onde m é o MES)
        REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d
        - os valores são o LIDER, DIA, mes_atual, ini_h, ini_m, fim_h, fim_m, tarefa
        - Essa string acima também será usada como descrição do compromisso

        Seguida da string de DESCRICAO acima, imprimir:

        Se o LIDER estava ocupada:
            "\tLIDER INDISPONIVEL \n"

        Caso contrário imprimir:
            "\tMEMBROS

            e para cada membro
                " %.2d:" seguido de "OK" ou "IN"
            se nenhum membro estiver disponíveis imprimir
                "VAZIA"
      */

    /* Verificar a disponibilidade do líder */
    int marcada = 0;
    int lider_disponivel = verificaDisponibilidade(reunioes, lider, reunioes[i].hc_ini_h, reunioes[i].hc_ini_m,
                                                  reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].dia);
    if (!lider_disponivel) {
        marcada = 0; /* Líder indisponível, a reunião não pode ser marcada */
        printf("\tLÍDER INDISPONÍVEL\n");
    } else {
      /* Verificar a disponibilidade dos membros */
      int membros_disponiveis = 0; /* Variável para controlar se há membros disponíveis */
      printf("%s\tMEMBROS", reunioes[i].descricao);

      for (int j = 0; j < num_membros; j++) {
        int membro = membros[j];

        if (verificaLideranca(funcionarios[lider], funcionarios[membro])) {
          if (verificaDisponibilidade(reunioes, membro, reunioes[i].hc_ini_h, reunioes[i].hc_ini_m,
                                      reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].dia)) {
            membros_disponiveis = 1; /* Pelo menos um membro está disponível */

            if (reunioes[i].disponibilidade[membro] == 1) {
              printf(" %.2d:OK", membro);
            } else {
              printf(" %.2d:IN", membro);
            }

            /* Atualizar a disponibilidade do membro como não disponível */
            reunioes[i].disponibilidade[membro] = 0;
          }
        }
      }

      if (membros_disponiveis) {
        marcada = 1; /* A reunião pode ser marcada */
      } 

      if (!membros_disponiveis) {
        printf(" VAZIA");
      }

      printf("\n");
    }

      /* se a reuniao foi marcada, adicionar às reunioes realizadas */
      if (marcada) {
        reunioes_realizadas[qtdes_reunioes_realizadas] = reunioes[i];
        qtdes_reunioes_realizadas++;
      }
    }

    /*verifica tarefas concluídas*/
    for (int i = 0; i < qtdes_reunioes_realizadas; i++) {
      reuniao reuniao = reunioes_realizadas[i];
      int tempo_conclusao = reuniao.hc_fim_h - reuniao.hc_ini_h;
      if (reuniao.hc_fim_m > reuniao.hc_ini_m) {
        tempo_conclusao++;
      }

      if (tempo_conclusao <= tarefas[reuniao.id].tempo_conclusao) {
        qtde_tarefas_tempo_restante_zero++;
      }
    }

      /*
      saída ao realizar as reuniões
      -----------------------------

      "%.2d/%.2d F %.2d: %s \n"
          os valores são: dia, mes_atual, funcionario, descricao_compromisso);

      Se o tempo de conclusão da tarefa <= 0 imprimir:
          CONCLUÍDA

      senão, imprimir:
      \tT %.2d D %.2d TCR %.2d\n
          os valores são: tarefa, tarefa_dificuldade, tarefa_tempo_conclusao
      */
/*
    for (int i = 0; i < qtdes_reunioes_realizadas; i++) {
      reuniao reuniao = reunioes_realizadas[i];
      tarefa tarefa = tarefas[reuniao.id];
    
      printf("%.2d/%.2d F %.2d: %s\n", reuniao.dia, mes_atual, funcionarios[i], reuniao.descricao);

      if (tarefa.tempo_conclusao <= 0) {
        printf("CONCLUÍDA\n");
      } else {
        printf("\tT %.2d D %.2d TCR %.2d\n", tarefas[i] , tarefa.dificuldade, tarefa.tempo_conclusao);
      }

      printf("\n");
    }
    */
  }

  /*acabou a inicialização das agendas e tarefas.*/
  /*------------------------------------------------------------------------*/
  /* Realizar todas as reuniões marcadas*/

  /*- Voltar para o mês 1 da agenda para "TRABALHAR":
  - Para cada dia entre 1 e 31 e para cada funcionário X
  - Obter lista de compromissos e para cada compromisso
  */
  int min_trab = 60; /*duração em minutos*/
  /*percorrer o mês*/
  for (int mes_atual = 1; mes_atual <= MES; mes_atual++) {
    for (int dia = 1; dia <= 31; dia++) {
      /*percorrer os funcionarios*/
      for (int X = 0; X < FUNCIONARIOS; X++) {
        /* lista de compromissos do funcionário */
        for (int T = 0; T < TAREFAS; T++) {
          /* se a tarefa[T] ainda não foi concluída */
          if (tarefas[T].tempo_conclusao > 0) {
            /* reduzir o tempo restante para concluir a tarefa de acordo com a fórmula */
            tarefas[T].tempo_conclusao -= min_trab * (funcionarios[X].experiencia / 100.0) * ((100 - tarefas[T].dificuldade) / 100.0);

            /* se o tempo restante para concluir a tarefa é menor ou igual a zero */
            if (tarefas[T].tempo_conclusao <= 0) {
              tarefas[T].tempo_conclusao = 0;
            }
            /* Incrementar a experiência do funcionário em uma unidade (limitar em 100) */
            funcionarios[X].experiencia++;
            if (funcionarios[X].experiencia > 100) {
              funcionarios[X].experiencia = 100;
            }
          }
        }
      }
    }
  }

  imprimirResumoFinal(qtde_tarefas_tempo_restante_zero, qtdes_reunioes_realizadas);
  return 0;
}