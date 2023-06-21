/*inclui as libs e funcoes que serao usadas*/
#include <stdio.h>
#include <stdlib.h>
#include "libagenda.h"

int main()
{
    /*criar uma nova agenda*/
    agenda_t *agenda = cria_agenda();

    /*criar compromissos*/
    horario_compromisso_t horario1 = {10, 0};
    compromisso_t *compromisso1 = cria_compromisso(horario1, 1, "compromisso 1");

    horario_compromisso_t horario2 = {5, 0};
    compromisso_t *compromisso2 = cria_compromisso(horario2, 2, "compromisso 2");

    horario_compromisso_t horario3 = {15, 0};
    compromisso_t *compromisso3 = cria_compromisso(horario3, 3, "compromisso 3");

    horario_compromisso_t horario4 = {9, 0};
    compromisso_t *compromisso4 = cria_compromisso(horario4, 4, "compromisso 4");

    /*marcar os compromissos na agenda*/
    marca_compromisso_agenda(agenda, 1, compromisso1);
    marca_compromisso_agenda(agenda, 2, compromisso2);
    marca_compromisso_agenda(agenda, 3, compromisso3);
    marca_compromisso_agenda(agenda, 4, compromisso4);

    /*imprimir a agenda do mes atual*/
    printf("Agenda do mês atual:\n");
    imprime_agenda_mes(agenda);

    /*desmarcar um compromisso*/
    desmarca_compromisso_agenda(agenda, 4, compromisso4);

    /*atualizar agenda*/
    printf("Agenda após desmarcar o compromisso 4:\n");
    imprime_agenda_mes(agenda);

    /*adicionar mais compromissos*/
    horario_compromisso_t horario5 = {14, 0};
    compromisso_t *compromisso5 = cria_compromisso(horario5, 5, "compromisso 5");
    marca_compromisso_agenda(agenda, 5, compromisso5);

    horario_compromisso_t horario6 = {8, 30};
    compromisso_t *compromisso6 = cria_compromisso(horario6, 6, "compromisso 6");
    marca_compromisso_agenda(agenda, 6, compromisso6);

    /*imprimir a agenda de um dia especifico*/
    printf("agenda do dia 5:\n");
    imprime_agenda_dia(agenda, 5);

    /*buscar um compromisso pelo id*/
    int id = 3;
    compromisso_t *compromisso = busca_compromisso_id(agenda, id);
    if (compromisso != NULL) {
        printf("compromisso encontrado (id %d): %s\n", id, descricao_compr(compromisso));
    } else {
        printf("nenhum compromisso encontrado com o id %d\n", id);
    }

    /*atualizar um compromisso existente*/
    horario_compromisso_t novoHorario = {16, 30};
    atualiza_compromisso_agenda(agenda, 5, novoHorario, "compromisso 5 atualizado");

    /*imprimir a agenda da semana 2*/
    printf("agenda da semana 2:\n");
    imprime_agenda_semana(agenda, 2);

    /*desmarcar todos os compromissos de uma data especifica*/
    desmarca_compromisso_data(agenda, 15);

    /*imprimir a agenda do mes atual apos desmarcar a data 15*/
    printf("agenda apos desmarcar a data 15:\n");
    imprime_agenda_mes(agenda);

    /*liberacao de memoria*/
    destroi_agenda(&agenda);

    return 0;
}