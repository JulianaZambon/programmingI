#!/bin/bash

#funcoes

#1)remova todas as linhas que facam parte do 2o semestre de 2022 (periodo 2, ano 2022)
function remove_2semestre() {
    grep -v '2022,2' historico-alg1_SIGA_ANONIMIZADO.csv > resultado.csv
}

#"status" diz a situação de cada individuo (se cancelou a materia, foi aprovado, reprovou, etc.).
#2)para cada status, calcule o número de individuos unicos naquele status
function status() {
    cut -d',' -f10 resultado.csv | sort | uniq -c
}

#funcao principal do script
function main() {
    #chama as funcoes
}

#para chamar a funcao principal
main