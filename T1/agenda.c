/*inclui as libs e funcoes que serao usadas*/
#include <stdio.h>
#include <stdlib.h>
#include "libagenda.h"

int main()
{
 /*   ESQUELETO DO PROGRAMA
---------------------

/* Inicialização */
/*- Criar os 30 FUNCS com os seguintes parâmetros:
    lideranca = ALEAT(0,100)
    experiencia = ALEAT(20,100)

- Criar as 100 TAREFAS com os seguintes parâmetro:
    tempo_conclusao = ALEAT(600,800)
    dificuldade = ALEAT(30,80)

*/
/* Marcar todas reuniões */
/*- Para cada mês de 1 até 12:
    - Marcar 100 reuniões:
        - Escolher aleatoriamente um líder entre os funcionários cuja 
          liderança esteja entre 30 e 70.
        - Criar uma nova reunião com os seguintes parâmetros:
            - Hora de início (hc.ini_h): um valor aleatório entre 8 e 12.
            - Minuto de início (hc.ini_m): um valor aleatório múltiplo 
              de 15 entre 0 e 45.
            - Hora de término (hc.fim_h): hora de início mais um valor 
              aleatório entre 1h e 4h.
            - Minuto de término (hc.fim_m): mesmo minuto de início.
            - Dia: um valor aleatório entre 1 e 31.
            - ID: um número aleatório entre 0 e TAREFAS-1, que representa 
              uma tarefa a ser discutida na reunião.
            - Descrição: uma string descrevendo a reunião (VER_ADIANTE).
        - Se o líder tem disponibilidade em sua agenda nos horários 
          escolhidos:
            - Sortear ALEAT(2,6) membros (funcionários) 
            - Para cada membro verificar 
              se liderança líder > liderança membro +ALEAT(-20,10)  
                - Se sim, tentar marcar a reunião na agenda do membro
            - Se nenhum dos membros puder participar, remova a reunião da 
              agenda do líder.

Aqui acaba a inicializacao das agendas e tarefas.

/* Realizar todas as reuniões marcadas*/
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
              (limitar em 100)
 */
}