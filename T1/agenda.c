/* inclui as libs e funcoes que serao usadas */
#include <stdio.h>
#include <string.h> /* para usar sprintf */
#include <stdlib.h>
#include <time.h>
#include "libagenda.h"

#define TAREFAS 100 /* qntd tarefas */
#define MES 12      /* qntd de meses */
#define FUNCIONARIOS 30 /* qntd de funcionarios */

/*------------------------------------------------------------------------*/

/* struct para o funcionario */
typedef struct
{
  int lideranca;
  int experiencia;
} Funcionario;

/* struct para as tarefas */
typedef struct
{
  int tempo_conclusao;
  int dificuldade;
} Tarefa;

/* struct para as reunioes */
typedef struct
{
  int hc_ini_h;                      /* hora inicio */
  int hc_ini_m;                      /* minuto inicio */
  int hc_fim_h;                      /* hora fim */
  int hc_fim_m;                      /* minuto fim */
  int dia;                           /* dia */
  int id;                            /* id tarefa */
  char descricao[100];               /* descricao */
  int disponibilidade[FUNCIONARIOS]; /* disponibilidade dos membros */
  int marcada;                       /* marcar/referir-se ao status da reunião */
} Reuniao;


/*------------------------------------------------------------------------*/

/* função ALEAT(MIN,MAX) que gera inteiros aleatórios entre MIN e MAX. */
int ALEAT(int MIN, int MAX)
{
  return MIN + rand() % (MAX - MIN + 1);
}

/* Escolher aleatoriamente um líder entre os funcionários cuja 
          liderança esteja entre 30 e 70. */
int escolherLider(Funcionario funcionarios[], int num_funcionarios)
{
  int candidatos[num_funcionarios]; // Lista de índices dos funcionários elegíveis
  int qtde_candidatos = 0;          // Quantidade de candidatos elegíveis

  // Encontrar os funcionários elegíveis e armazenar seus índices na lista de candidatos
  for (int i = 0; i < num_funcionarios; i++) {
    /* liderança entre 30 e 70 */
    if (funcionarios[i].lideranca >= 30 && funcionarios[i].lideranca <= 70) {
      candidatos[qtde_candidatos] = i;
      qtde_candidatos++;
    }
  }

  /* Se há funcionários elegíveis*/
  if (qtde_candidatos == 0) 
    return -1; /* Nenhum líder elegível encontrado */

  /* Gerar um índice aleatório dentro do intervalo dos candidatos */
  int indice_aleatorio = ALEAT(0, qtde_candidatos - 1);
  /* Acessar o funcionário correspondente ao índice aleatório na lista de candidatos */
  int indice_lider = candidatos[indice_aleatorio];

  return indice_lider;
}

/* Se o líder tem disponibilidade em sua agenda nos horários */
int verificaDisponibilidade(Reuniao reunioes[], int lider, int hc_ini_h, int hc_ini_m, 
                              int hc_fim_h, int hc_fim_m, int dia, int id_tarefa) 
{
    /* Verifica a disponibilidade do líder em relação às reuniões existentes */
    for (int i = 0; i < TAREFAS; i++) {
        Reuniao reuniao = reunioes[i];
         
        /* Verifica se a reunião já está agendada para o líder */
        if (lider == reuniao.id) {
            /* Verifica se há conflito de horários */
            if ((hc_ini_h >= reuniao.hc_ini_h && hc_ini_h <= reuniao.hc_fim_h) ||
                (hc_fim_h >= reuniao.hc_ini_h && hc_fim_h <= reuniao.hc_fim_h)) {
                /* Verifica se os minutos também estão em conflito */
                if (hc_ini_m == reuniao.hc_ini_m || hc_fim_m == reuniao.hc_fim_m) {
                    return 0;  /* Líder indisponível devido a conflito de horários */
                }
            }
        }
    }
    
    /* Se não houver conflito de horários, a reunião pode ser agendada */
    return 1;
}

/*  - Para cada membro verificar 
              se liderança líder > liderança membro +ALEAT(-20,10) */
int verificaLideranca(Funcionario lider, Funcionario membros[]) 
{
    for (int i = 0; i < FUNCIONARIOS; i++) {
        int lideranca_membro = membros[i].lideranca;
        int aleatorio = ALEAT(-20, 10);  /* Gera um valor aleatório entre -20 e 10 */

        if (lider.lideranca <= lideranca_membro + aleatorio) {
            return 0;  /* Liderança do líder não é maior que a liderança do membro mais o valor aleatório */
        }
    }
    
    return 1;  /* Liderança do líder é maior que a liderança de todos os membros */
}

/* Informações finais da saida do programa */
void imprimirResumoFinal(int qtde_tarefas_tempo_restante_zero, int qtdes_reunioes_realizadas)
{
  printf("REUNIOES REALIZADAS %d\n", qtdes_reunioes_realizadas);
  printf("TAREFAS CONCLUIDAS %d\n", qtde_tarefas_tempo_restante_zero);
}

/*------------------------------------------------------------------------*/

int main()
{
/*------------------------------------------------------------------------*/
  srand(time(NULL));

  Funcionario* funcionarios = malloc(FUNCIONARIOS * sizeof(Funcionario));
  Tarefa* tarefas = malloc(TAREFAS * sizeof(Tarefa));
  Reuniao* reunioes = malloc(MES * TAREFAS * sizeof(Reuniao));
  Reuniao* reunioes_realizadas = malloc(TAREFAS * sizeof(Reuniao));

  /* Inicializar contadores */
  int qtdes_reunioes_realizadas = 0;
  int qtde_tarefas_tempo_restante_zero = 0;

  for (int i = 0; i < FUNCIONARIOS; i++) {
    funcionarios[i].lideranca = ALEAT(0, 100);
    funcionarios[i].experiencia = ALEAT(20, 100);
  }

  for (int T = 0; T < TAREFAS; T++) {
    tarefas[T].tempo_conclusao = ALEAT(600, 800);
    tarefas[T].dificuldade = ALEAT(30, 80);
  }
  
/*------------------------------------------------------------------------*/

/* Para cada mês de 1 até 12 */
  int mes_atual;
  for (mes_atual = 1; mes_atual <= MES; mes_atual++) {
    printf("M %d\n", mes_atual);
    
    /* Marcar 100 reuniões */
    for (int i = 0; i < TAREFAS; i++) {
      int lider;
/*------------------------------------------------------------------------*/   
      reunioes[i].hc_ini_h = ALEAT(8, 12);
      reunioes[i].hc_ini_m = ALEAT(0, 3) * 15;
      reunioes[i].hc_fim_h = reunioes[i].hc_ini_h + ALEAT(1, 4);
      reunioes[i].hc_fim_m = reunioes[i].hc_ini_m;
      reunioes[i].dia = ALEAT(1, 31);
      reunioes[i].id = ALEAT(0, TAREFAS - 1); 

      reunioes[i].marcada = 0; /* Primeiramente a reunião não está marcada*/
/*------------------------------------------------------------------------*/      
      /* Descrição da reunião */
      sprintf(reunioes[i].descricao, "REUNIR L %.2d %.2d/%.2d %.2d:%.2d %.2d:%.2d T %.2d", lider, reunioes[i].dia, mes_atual,
              reunioes[i].hc_ini_h, reunioes[i].hc_ini_m, reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].id);
      printf("%s\n", reunioes[i].descricao);
      
      /* Se o líder tem disponibilidade em sua agenda nos horários escolhidos: */
      int disponivel_lider = verificaDisponibilidade(reunioes, lider, reunioes[i].hc_ini_h, reunioes[i].hc_ini_m,
                                                    reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].dia, reunioes[i].id);
      
      /* Se o líder estiver disponível */
      if (disponivel_lider) {
        /* Sortear ALEAT(2,6) membros (funcionários) */
        int num_membros = ALEAT(2, 6);
        int membros[num_membros];
        int algum_membro_disponivel = 0; // Variável para verificar se algum membro está disponível
        
        for (int j = 0; j < num_membros; j++) {
          membros[j] = ALEAT(0, FUNCIONARIOS - 1);
          
          /* Para cada membro, verificar se liderança líder > liderança membro +ALEAT(-20,10) */
          if (verificaLideranca(funcionarios[lider], funcionarios)) {
            /* Tentar marcar a reunião na agenda do membro */
            int disponivel_membro = verificaDisponibilidade(reunioes, membros[j], reunioes[i].hc_ini_h, reunioes[i].hc_ini_m,
                                                            reunioes[i].hc_fim_h, reunioes[i].hc_fim_m, reunioes[i].dia, reunioes[i].id);
            if (disponivel_membro) { /* Marcar a reunião na agenda do membro */
              /* MARCAR */
              reunioes[i].marcada = 1; /* Marcar a reunião */
              printf("%.2d: OK\n", membros[j]);
              algum_membro_disponivel = 1; // Definir a variável como 1 para indicar que pelo menos um membro está disponível
            } else {
              printf("%.2d: IN\n", membros[j]);
            }
          }
        }
        
        printf("\tMEMBROS\n");
        
        if (!algum_membro_disponivel) {
          printf("VAZIA\n");
        }
      } else {
        printf("\tLIDER INDISPONIVEL\n");
      }
      
      /* Se nenhum dos membros puder participar, remova a reunião da agenda do líder */
      if (!disponivel_lider && !reunioes[i].marcada)
        reunioes[i].marcada = 0; /* Desmarcar a reunião */
      }

/*------------------------------------------------------------------------*/
      /* Imprimir a saída após realizar a reunião */
      for (int T = 0; T < TAREFAS; T++) {
        printf("%.2d/%.2d F %.2d: %s\n", reunioes[i].dia, mes_atual, funcionarios, descricao_compr);
        
        /* Se o tempo de conclusão da tarefa <= 0, imprimir "CONCLUÍDA" */
        if (tarefas[T].tempo_conclusao <= 0) {
          printf("CONCLUÍDA\n");
        } else {
          /* Senão, imprimir informações da tarefa */
          printf("\tT %.2d D %.2d TCR %.2d\n", T, tarefas[T].dificuldade, tarefas[T].tempo_conclusao);
        }
        /* Verificar se o tempo restante para concluir a tarefa é igual a zero */
          if (tarefas[T].tempo_conclusao == 0) {
            qtde_tarefas_tempo_restante_zero++;
          }
        }
      /* Verificar se a reunião foi realizada */
      if (reunioes[i].realizada) {
        reunioes_realizadas[qtdes_reunioes_realizadas] = reunioes[i];
        qtdes_reunioes_realizadas++;
      }
    }
     
/*------------------------------------------------------------------------*/
/* Realizar todas as reuniões marcadas*/
/* Voltar para o mês 1 da agenda para "TRABALHAR" */
  int min_trab = 60;
  for (int mes_atual = 0; mes_atual < MES; mes_atual++) {
      /* Para cada dia entre 1 e 31 */
      for (int dia = 1; dia <= 31; dia++) {
          /* Para cada funcionário X */
          for (int X = 0; X < FUNCIONARIOS; X++) {
              /* Obter lista de compromissos para o dia e funcionário */
              Reuniao compromissos = funcionarios[X].[mes_atual][dia - 1];

              /* Para cada compromisso */
              for (int T = 0; T < TAREFAS; T++) {
                  /* Verificar se a tarefa ainda não foi concluída */
                  if (tarefas[T].tempo_conclusao > 0) {
                      /* Reduzir o tempo restante para concluir a tarefa de acordo com a fórmula */
                      double reducao_tempo = min_trab * (funcionarios[X].experiencia / 100.0) * ((100 - tarefas[T].dificuldade) / 100.0);
                      tarefas[T].tempo_conclusao -= reducao_tempo;

                      /* Verificar se o tempo restante para concluir a tarefa é menor ou igual a zero */
                      if (tarefas[T].tempo_conclusao <= 0)
                          tarefas[T].tempo_conclusao = 0;
                      
                      /* Incrementar a experiência do funcionário em uma unidade (limitar em 100) */
                      funcionarios[X].experiencia++;
                      if (funcionarios[X].experiencia > 100) {
                          funcionarios[X].experiencia = 100; /* limite */
                      }
                  }
              }
          }
      }
  }
/*------------------------------------------------------------------------*/
    imprimirResumoFinal(qtde_tarefas_tempo_restante_zero, qtdes_reunioes_realizadas);

    /* Desalocar memória */
    free(funcionarios);
    free(tarefas);
    free(reunioes);
    free(reunioes_realizadas);

    return 0;
}