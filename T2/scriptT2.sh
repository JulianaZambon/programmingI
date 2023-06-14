#!/bin/bash

#funcoes --------------------------------------------------------------------

#1)remova todas as linhas que facam parte do 2o semestre de 2022 (periodo 2, ano 2022)
function remove_2semestre() {
    grep -v '2022,2' historico-alg1_SIGA_ANONIMIZADO.csv > resultado.csv
}

#"status" diz a situação de cada individuo (se cancelou a materia, foi aprovado, reprovou, etc.).
#2)para cada status, calcule o numero de individuos unicos naquele status
function status() {
    cut -d',' -f10 resultado.csv | sort | uniq -c
}

#3)qual o maximo de vezes que um individuo cursou a discplina antes de ser aprovado? 
#quantos individuos possuem o mesmo numero maximo de vezes cursadas ate a aprovacao?
function aprovacao() {
    #f1,5 matricula, periodo e o ano
    cut -d',' -f1,4,5 resultado.csv | grep 'Aprovado' | cut -d',' -f1,3 | 
    sort | uniq -c | sort -nr | head -n 1
            #-c faz com que o uniq exiba o numero de ocorrencias de cada linha
}

#4)qual a porcentagem de aprovacaoo/reprovacaoo por ano?

#5)qual eh a media de nota dos aprovados (no periodo total e por ano)?

#6)qual eh a media de nota dos reprovados por nota (periodo total e ano)?

#7)qual eh a media da frequencia dos reprovados por nota (periodo total e por ano)?

#8)qual a porcentagem de evasoes (total e anual)?

#9)como os anos de pandemia impactaram no rendimento dos estudantes em relacao aos anos anteriores? 
#calcule em percentual o rendimento dos aprovados, a taxa de cancelamento e de reprovacoes. 
#considere como anos de pandemia os anos de 2020 e 2021. 
#(EXEMPLO: qual o percentual de aumento ou diminuicao de notas, frequencias, aprovacoes/reprovacoes e cancelamentos).

#10)compare a volta as aulas hibrida (2022 periodo 1) com os anos de pandemia e 
#os anos anteriores em relacao as aprovacoes, reprovacoes, mediana das notas e cancelamentos.

#funcao principal do script -------------------------------------------------
function main() {
    #chama as funcoes
}

#para chamar a funcao principal ---------------------------------------------
main