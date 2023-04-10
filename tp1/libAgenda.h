#include <stdlib.h>
#define HORAS_DO_DIA 24
#define DIAS_DO_ANO 365

/* estrutura para representar uma data, composta por dia, mês e ano */
struct data {
	int dia;
	int mes;
	int ano;
};

/* estrutura para representar um compromisso, isto é, uma data e uma hora */
struct compromisso {
	struct data data_compr;	/* dia, mês, ano */
	int hora_compr; /* hora do compromisso entre 0 e 23 */
};

/* estrutura para representar um dia do ano, composto por 24 horas */
struct dias {
	int horas[HORAS_DO_DIA];
};

/* estrutura para representar uma agenda de um dado ano, com 365 dias de 24 horas */
struct agenda {
	int ano;
	struct dias agenda_do_ano[DIAS_DO_ANO]; /* vetor agenda do ano, a ser modificado em ano bissexto */
};

/* função com implementação dada pelos professores para cálculo do dia do 
 * ano, que servirá de índice para o vetor da agenda */
int obtemDiaDoAno(struct data d);

/* inicializa a agenda do ano corrente, onde cada hora de cada dia deve ter o 
 * valor 0 para indicar que não há um compromisso marcado. Retorna uma agenda
 * livre */ 
struct agenda criaAgenda(int ano);

/* lê um compromisso passado pelo usuário, com data completa e hora pretendida. 
 * O compromisso deve ser validado, isto é, a data deve estar correta, 
 * considerando os dias de cada mês e o ano da agenda criada, assim como a  
 * a hora, que deve ser entre 0 e 23, sem frações. 
 * Retorna um compromisso válido. */
struct compromisso leCompromisso();

/* dado um compromisso, retorna a hora definida */
int obtemHora(struct compromisso compr);

/* retorna o ano atribuído a uma agenda criada */
int obtemAno(struct agenda ag);

/* Valida uma data lida do usuário; utilizar como função auxiliar da função
 * leCompromisso(). Retorna 0 se a data for inválida */
int validaData(struct data d, struct agenda ag);

/* Retorna 0 se data e horário já estiverem ocupados, 1 se data e horário
 * estiverem livres */
int verificaDisponibilidade(struct compromisso compr, struct agenda ag);

/* Dada uma agenda e um compromisso válido, isto é, com data/hora válidos, 
 * hora livre e dentro do ano da agenda, muda o valor da hora do compromisso
 * de 0 (livre) para 1 (ocupado). Retorna a nova agenda com o compromisso
 * marcado. */
struct agenda marcaCompromisso(struct agenda ag, struct compromisso compr);

/* mostra as datas e horas de todos os compromissos marcados na agenda */
void listaCompromissos(struct agenda ag);