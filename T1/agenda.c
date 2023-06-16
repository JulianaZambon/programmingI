/*inclui as libs e funcoes que serao usadas*/
#include <stdio.h>
#include <stdlib.h>
#include "libagenda.h"
/*
FIRMA
-----

Você vai escrever um programa principal para simular uma empresa em que os
funcionários utilizam uma agenda para marcar reuniões entre si e resolver um
conjunto de tarefas.

Cada funcionário é caracterizado pelas seguintes informações:

    Liderança: um valor inteiro entre 0 e 100, que afeta a probabilidade de
    convocar novas reuniões.  

    Experiência: um valor inteiro entre 0 e 100, que determina o quanto 
       efetivo é o uso do tempo em reuniões para resolver problemas.

    Agenda: uma lista de compromissos.

A firma deve ter 30 funcionários, numerados de 0 a 29.

Cada tarefa é caracterizada por:

    Tempo de conclusão: a quantidade de tempo necessária para completar a
    tarefa.  

    Dificuldade: um valor entre 0 e 100 que afeta o tempo necessário para
    os funcionários concluírem a tarefa.

A firma deve ter 100 tarefas, numeradas de 0 a 99.

Em nossa simulação vamos usar vários parâmetros aleatorizados, você deve
escrever uma função ALEAT(MIN,MAX) que gera inteiros aleatórios entre 
MIN e MAX.
Ver as funções srand e rand.  

ESQUELETO DO PROGRAMA
---------------------

/* Inicialização */
/*
- Criar os 30 FUNCS com os seguintes parâmetros:
    lideranca = ALEAT(0,100)
    experiencia = ALEAT(20,100)

- Criar as 100 TAREFAS com os seguintes parâmetro:
    tempo_conclusao = ALEAT(600,800)
    dificuldade = ALEAT(30,80)


/* Marcar todas reuniões */
/*
- Para cada mês de 1 até 12:
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
/*
- Voltar para o mês 1 da agenda para "TRABALHAR":

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


SAÍDA
-----

Segue a especificação, incluindo a string de formatação para usar no 
printf ou sprintf dependendo do caso:

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


exemplo
-------

M 01
REUNIR L 07 16/01 12:30 16:30 T 47	MEMBROS: 07:IN 10:OK
REUNIR L 06 05/01 08:15 09:15 T 04	MEMBROS: 23:OK 05:OK
REUNIR L 29 11/01 08:00 09:00 T 42	MEMBROS: 23:OK 18:OK 24:OK 29:IN 13:OK
REUNIR L 29 29/01 10:30 12:30 T 39	MEMBROS: 17:OK 12:OK 23:OK 07:OK
REUNIR L 13 26/01 09:00 13:00 T 95	LIDER INDISPONIVEL 
REUNIR L 09 21/01 12:45 13:45 T 04	MEMBROS:VAZIA


saída ao realizar as reuniões
-----------------------------

"%.2d/%.2d F %.2d: %s \n"
    os valores são: dia, mes_atual, funcionario, descricao_compromisso);

Se o tempo de conclusão da tarefa <= 0 imprimir:
    CONCLUÍDA

senão, imprimir:
\tT %.2d D %.2d TCR %.2d\n
    os valores são: tarefa, tarefa_dificuldade, tarefa_tempo_conclusao


exemplo
-------

M 01
01/01 F 13: REUNIR L 29 01/01 09:15 12:15 T 09 
	T 09 D 52 TCR 578
01/01 F 14: REUNIR L 29 01/01 09:15 12:15 T 09 
	T 09 D 52 TCR 561
01/01 F 15: REUNIR L 29 01/01 09:15 12:15 T 09 
	T 09 D 52 TCR 538
01/01 F 21: REUNIR L 29 01/01 09:15 12:15 T 09 
	T 09 D 52 TCR 517
01/01 F 23: REUNIR L 29 01/01 09:15 12:15 T 09 
	T 09 D 52 TCR 503
01/01 F 29: REUNIR L 29 01/01 09:15 12:15 T 09 
	T 09 D 52 TCR 483

final
-----

REUNIOES REALIZADAS qtdes_reunioes_realizadas
TAREFAS CONCLUIDAS qtde_tarefas_tempo_restante_zero 

* ver no site exemplo completo de execução */		



int main()
{
}