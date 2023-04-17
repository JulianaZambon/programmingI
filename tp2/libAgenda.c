/* 
 * Feito por Marcos Castilho em 06/04/2023
 * para a disciplina CI1001 - Programacao 1
 * obs.: a funcao obtemDiaDoAno foi feita
 * pelo prof. Andre Gregio.
 */

#include <stdio.h>
#include <time.h>
#include "libAgenda.h"

#define LIVRE 0
#define OCUPADA 1

int obtemDiaDoAno(struct data d){
    struct tm tipodata={0};
    time_t segundos;
    int dia_do_ano;

    tipodata.tm_mday = d.dia;
    tipodata.tm_mon = d.mes - 1;
    tipodata.tm_year = d.ano - 1900;
    tipodata.tm_isdst = -1;
    tipodata.tm_hour = 0;

    /* converte data para epoca, isto eh, segundos desde 1970 */
    segundos = mktime(&tipodata);

    /* converte epoca em data, obtendo assim automaticamente
     * o campo 'dia do ano' (tm_yday) que sera o indice do
     * vetor necessario para marcar um compromisso */
    tipodata = *localtime(&segundos);

    /* da reconversao da data, obtem o dia do ano, um numero
     * entre 0 e 364 */
    dia_do_ano = tipodata.tm_yday;

    return dia_do_ano;
}

/* Daqui para frente eh com voces! */

/* Inicializa a agenda do ano corrente, onde cada hora de cada dia deve ter o 
 * valor 0 para indicar que nao ha um compromisso marcado. 
 * Retorna uma agenda livre */
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

/* Funcao para calculo do dia do ano, que sera o indice para o vetor da agenda
 * O codigo sera fornecido pelos professores e nao devera ser alterado.
 * Obs: o codigo fornecido para o tp2 eh diferente do fornecido no tp1! */
int obtemDiaDoAno(struct data d)
{

}

/* Dado um compromisso, retorna a hora definida */
int obtemHora(struct compromisso *compr)
{
   return compr -> hora_compr; 
}

/* Retorna o ano atribuido a uma agenda criada */
int obtemAno(struct agenda *ag)
{
    return ag -> ano;
}

/* Le um compromisso do teclado (dia, mes, ano e hora, nesta ordem) 
 * Devolve o compromisso no parametro e retorna 1 se o compromisso
 * eh valido ou 0 caso contrario */
int leCompromisso(struct agenda *ag, struct compromisso *compr)
{
    scanf("%d", &compr -> data_compr.dia);
    scanf("%d", &compr -> data_compr.mes);
    scanf("%d", &compr -> data_compr.ano);
    scanf("%d", &compr -> hora_compr);


}

/* Valida um data lida do usuario; 
 * Retorna 1 se a data for valida e 0 caso contrario */
int validaData(struct agenda *ag, struct data *d)
{

}

/* Valida uma hora lida do usuario; 
 * Retorna 1 se a hora for valida e 0 caso contrario */
int validaHora(struct compromisso *compr)
{

}

/* Retorna 0 se data e horario já estiverem ocupados, ou 1 caso contrario */
int verificaDisponibilidade(struct agenda *ag, struct compromisso *compr)
{

}

/* Esta funcao considera que o comprimisso eh valido e a agenda esta livre
 * para da data/hora fornecidos, portanto quem chama esta funcao tem que
 * garantir estas informacoes. Portanto, a funcao simplesmente muda o valor
 * da hora do compromisso de livre para ocupado */
void marcaCompromisso(struct agenda *ag, struct compromisso *compr)
{

}

/* Mostra as datas e horas de todos os compromissos marcados na agenda.
 * Se a agenda nao tiver compromissos agendados nao imprime nada */
void listaCompromissos(struct agenda *ag)
{
    
}
