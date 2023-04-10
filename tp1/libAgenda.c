#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "libAgenda.h"
#define HORAS_DO_DIA 24
#define DIAS_DO_ANO 365

/* esta função transforma uma data lida pelo usuário para uma struct data
 * em uma data do tipo struct tm definida pela biblioteca time.h.
 * Esta data é então transformada em segundos com a função mktime, para que
 * os outros membros da struct sejam preenchidos automaticamente, e então
 * reconvertida para uma data do tipo struct tm, reescrevendo a variável
 * original. Com isso, pode-se obter o membro tm_yday, que representa o
 * dia do ano representado pela data: um inteiro entre 0 e 364 */
int obtemDiaDoAno(struct data d)
{
	struct tm tipodata;
	time_t segundos;
	int dia_do_ano;

	tipodata.tm_mday = d.dia;
	tipodata.tm_mon = d.mes - 1;
	tipodata.tm_year = d.ano - 1900;
	tipodata.tm_isdst = -1;
	tipodata.tm_hour = 0;

	/* converte data para época, isto é, segundos desde 1970 */
	segundos = mktime(&tipodata);
	/* converte época em data, obtendo assim automaticamente
	 * o campo 'dia do ano' (tm_yday) que será o índice do
	 * vetor necessário para marcar um compromisso */
	tipodata = *localtime(&segundos);
	/* da reconversão da data, obtém o dia do ano, um número
	 * entre 0 e 364 */
	dia_do_ano = tipodata.tm_yday;

	return dia_do_ano;
}

/* DAQUI PARA BAIXO É COM VOCÊS! SIGAM O ENUNCIADO E O HEADER DISPONÍVEL */

/* inicializa a agenda do ano corrente, onde cada hora de cada dia deve ter o
 * valor 0 para indicar que não há um compromisso marcado. Retorna uma agenda
 * livre */
struct agenda criaAgenda(int ano)
{
	struct agenda ag;
	ag.ano = ano;

	int i, j;

	for (i = 0; i < DIAS_DO_ANO; i++)
	{
		for (j = 0; j < HORAS_DO_DIA; j++)
		{
			ag.agenda_do_ano[i].horas[j] = 0;
		}
	}
	return ag;
}

/* lê um compromisso passado pelo usuário, com data completa e hora pretendida.
 * O compromisso deve ser validado, isto é, a data deve estar correta,
 * considerando os dias de cada mês e o ano da agenda criada, assim como a
 * a hora, que deve ser entre 0 e 23, sem frações.
 * Retorna um compromisso válido. */
struct compromisso leCompromisso()
{
	struct compromisso compr;

	printf("--> Entre com dia, mes, ano, hora:\n");

	scanf("%d", &compr.data_compr.dia);
	scanf("%d", &compr.data_compr.mes);
	scanf("%d", &compr.data_compr.ano);
	scanf("%d", &compr.hora_compr);

	return compr;
}

/* dado um compromisso, retorna a hora definida */
int obtemHora(struct compromisso compr)
{
	return compr.hora_compr;
}

/* retorna o ano atribuído a uma agenda criada */
int obtemAno(struct agenda ag)
{
	return ag.ano;
}

/* Valida uma data lida do usuário; utilizar como função auxiliar da função
 * leCompromisso(). Retorna 0 se a data for inválida */
int validaData(struct data d, struct agenda ag)
{
	if (d.ano == ag.ano)
	{

		if (d.mes > 0 && d.mes < 13)
		{

			if ((d.mes == 1 || d.mes == 3 || d.mes == 5 || d.mes == 7 || d.mes == 8 || d.mes == 10 || d.mes == 12) && d.dia > 0 && d.dia < 32)
			{
				return 1;
			}

			if ((d.mes == 4 || d.mes == 6 || d.mes == 9 || d.mes == 11) && d.dia > 0 && d.dia < 31)
			{
				return 1;
			}

			if ((d.mes == 2) && d.dia > 0 && d.dia < 29)
			{
				return 1;
			}
		}
	}
	printf("Data invalida, compromisso nao inserido\n");
	return 0;
}

/* Retorna 0 se data e horário já estiverem ocupados, 1 se data e horário
 * estiverem livres */
int verificaDisponibilidade(struct compromisso compr, struct agenda ag)
{

	if (compr.hora_compr < 0 || compr.hora_compr > 23)
	{
		printf("Hora invalida, compromisso nao inserido\n");
	}
	else if 
		(ag.agenda_do_ano[compr.data_compr.dia * compr.data_compr.mes].horas[compr.hora_compr] == 0)
		return 1;
	
	
	return 0;
}

/* Dada uma agenda e um compromisso válido, isto é, com data/hora válidos,
 * hora livre e dentro do ano da agenda, muda o valor da hora do compromisso
 * de 0 (livre) para 1 (ocupado). Retorna a nova agenda com o compromisso
 * marcado. */
struct agenda marcaCompromisso(struct agenda ag, struct compromisso compr)
{
	ag.agenda_do_ano[obtemDiaDoAno(compr.data_compr)].horas[compr.hora_compr] = 1;

	return ag;
}

/* mostra as datas e horas de todos os compromissos marcados na agenda */
void listaCompromissos(struct agenda ag)
{
	int i, j;
	for (i = 0; i < DIAS_DO_ANO; i++)
	{
		for (j = 0; j < HORAS_DO_DIA; j++)
		{
			if (ag.agenda_do_ano[i].horas[j] == 1)
				printf("dia: %d, ano: %d, hora: %d, compromisso!\n", i, ag.ano, j);
		}
	}
}
