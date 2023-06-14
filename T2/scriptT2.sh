#!/bin/bash

#funcoes ------------------------------------------------------------------------------------------

#1)remova todas as linhas que facam parte do 2o semestre de 2022 (periodo 2, ano 2022)
function remove_2semestre() {
    #grep eh comando de busca
    grep -v '2,2022' historico-alg1_SIGA_ANONIMIZADO.csv > resultado.csv
    printf "Remoção do 2º semestre de 2022 concluida.\n"
}

#"status" diz a situação de cada individuo (se cancelou a materia, foi aprovado, reprovou, etc.).
#2)para cada status, calcule o numero de individuos unicos naquele status
function status() {
            #f10 indica o campo
    cut -d',' -f10 resultado.csv | sort | uniq -c
    while read -r count status; do
        printf "%s: %s indivíduo(s) com status '%s'\n" "$count" "$status" "$(echo "$status"
        | tr '[:lower:]' '[:upper:]')"
    done
}

#3)qual o maximo de vezes que um individuo cursou a discplina antes de ser aprovado? 
#quantos individuos possuem o mesmo numero maximo de vezes cursadas ate a aprovacao?
function aprovacao() {
    #f1,5 matricula, periodo e o ano
    max_cursadas=$(cut -d',' -f1,4,5 resultado.csv | grep 'Aprovado' | cut -d',' -f1,3 |
        sort | uniq -c | sort -nr | head -n 1 | awk '{print $1}')
    count_max_cursadas=$(cut -d',' -f1,4,5 resultado.csv | grep 'Aprovado' | cut -d',' -f1,3 |
        sort | uniq -c | grep "$max_cursadas " | wc -l)
    printf "Máximo de vezes cursadas antes da aprovação: %s\n" "$max_cursadas"
    printf "Número de indivíduos com o mesmo número máximo de vezes cursadas: %s\n" "$count_max_cursadas"

            #-c faz com que o uniq exiba o numero de ocorrencias de cada linha
}

#4)qual a porcentagem de aprovacaoo/reprovacaoo por ano?
function porcentagem_aprovacao_reprovacao() {
    cut -d',' -f5,10 resultado.csv | sort | uniq -c |

    while IFS=',' read -r ano status count; do
        total_alunos=$(grep -c "$ano" resultado.csv)
        porcent_aprovados=$(grep -c "$ano,Aprovado" resultado.csv)
        porcent_reprovados=$(grep -c "$ano,Reprovado" resultado.csv)
                                #scale define a precisão decimal utilizada pelo bc
        porcent_aprovados=$(echo "scale=2; ($porcent_aprovados / $total_alunos) * 100" | bc)
        porcent_reprovados=$(echo "scale=2; ($porcent_reprovados / $total_alunos) * 100" | bc)
        printf "%s: Aprovados: %.2f%%, Reprovados: %.2f%%\n" "$ano" "$porcent_aprovados" "$porcent_reprovados"
    done
}

#5)qual eh a media de nota dos aprovados (no periodo total e por ano)?
function media_nota_aprovados() {
    cut -d',' -f5,6,10 resultado.csv | grep 'Aprovado' |
    while IFS=',' read -r ano nota status; do
        notas[$ano]=$(echo "${notas[$ano]} + $nota" | bc)
        count[$ano]=$((${count[$ano]} + 1))
    done

    for ano in "${!notas[@]}"; do
        media=$(echo "scale=2; ${notas[$ano]} / ${count[$ano]}" | bc)
        printf "%s: Media de nota dos aprovados: %.2f\n" "$ano" "$media"
    done
}

#6)qual eh a media de nota dos reprovados por nota (periodo total e ano)?
function media_nota_reprovados() {
    cut -d',' -f5,6,10 resultado.csv | grep 'Reprovado' |
    while IFS=',' read -r ano nota status; do
        notas[$ano]=$(echo "${notas[$ano]} + $nota" | bc)
        count[$ano]=$((${count[$ano]} + 1))
    done

    for ano in "${!notas[@]}"; do
        media=$(echo "scale=2; ${notas[$ano]} / ${count[$ano]}" | bc)
        printf "%s: Media de nota dos reprovados: %.2f\n" "$ano" "$media"
    done
}

#7)qual eh a media da frequencia dos reprovados por nota (periodo total e por ano)?
function media_frequencia_reprovados_nota() {
    cut -d',' -f5,7,10 resultado.csv | grep 'Reprovado' |
    while IFS=',' read -r ano frequencia status; do
        frequencias[$ano]=$(echo "${frequencias[$ano]} + $frequencia" | bc)
        count[$ano]=$((${count[$ano]} + 1))
    done

    for ano in "${!frequencias[@]}"; do
        media=$(echo "scale=2; ${frequencias[$ano]} / ${count[$ano]}" | bc)
        printf "%s: Media da frequencia dos reprovados: %.2f\n" "$ano" "$media"
    done
}

#8)qual a porcentagem de evasoes (total e anual)?
function porcentagem_evasoes() {
    cut -d',' -f5,10 resultado.csv | sort | uniq -c |
    while IFS=',' read -r ano status count; do
        total_alunos=$(grep -c "$ano" resultado.csv)
        porcent_evasoes=$(grep -c "$ano,Cancelado" resultado.csv)
        porcent_evasoes=$(echo "scale=2; ($porcent_evasoes / $total_alunos) * 100" | bc)
        printf "%s: Porcentagem de evasoes: %.2f%%\n" "$ano" "$porcent_evasoes"
    done
}

#9)como os anos de pandemia impactaram no rendimento dos estudantes em relacao aos anos anteriores? 
#calcule em percentual o rendimento dos aprovados, a taxa de cancelamento e de reprovacoes. 
#considere como anos de pandemia os anos de 2020 e 2021. 
#(EXEMPLO: qual o percentual de aumento ou diminuicao de notas, frequencias, aprovacoes/reprovacoes e cancelamentos).
function rendimento_pandemia() {
    total_aprovados=$(cut -d',' -f10 resultado.csv | grep 'Aprovado' | wc -l)
    total_cancelamentos=$(cut -d',' -f10 resultado.csv | grep 'Cancelado' | wc -l)
    total_reprovados=$(cut -d',' -f10 resultado.csv | grep 'Reprovado' | wc -l)
    
    total_pandemia=$(grep '2020\|2021' resultado.csv | wc -l)
    aprovados_pandemia=$(grep '2020\|2021' resultado.csv | grep 'Aprovado' | wc -l)
    cancelamentos_pandemia=$(grep '2020\|2021' resultado.csv | grep 'Cancelado' | wc -l)
    reprovados_pandemia=$(grep '2020\|2021' resultado.csv | grep 'Reprovado' | wc -l)
    
    #bc eh calculadora de linha de comando
                            #definir a casa decimal
    percent_aprovados=$(bc <<< "scale=2; ($aprovados_pandemia / $total_pandemia) * 100")
    percent_cancelamentos=$(bc <<< "scale=2; ($cancelamentos_pandemia / $total_pandemia) * 100")
    percent_reprovados=$(bc <<< "scale=2; ($reprovados_pandemia / $total_pandemia) * 100")
    
    printf "Rendimento dos aprovados durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_aprovados"
    printf "Taxa de cancelamento durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_cancelamentos"
    printf "Taxa de reprovação durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_reprovados"
}


#10)compare a volta as aulas hibrida (2022 periodo 1) com os anos de pandemia e 
#os anos anteriores em relacao as aprovacoes, reprovacoes, mediana das notas e cancelamentos.
function comparacao() {
    #aprovacoes, reprovacoes e cancelamentos em 2022.1
    aprovados_2022=$(grep '1,2022' resultado.csv | grep 'Aprovado' | wc -l)
    reprovados_2022=$(grep '1,2022' resultado.csv | grep 'Reprovado' | wc -l)
    cancelamentos_2022=$(grep '1,2022' resultado.csv | grep 'Cancelado' | wc -l)
    
    #aprovacoes, reprovacoes e cancelamentos na pandemia (2020 e 2021)
    aprovados_pandemia=$(grep '2020\|2021' resultado.csv | grep 'Aprovado' | wc -l)
    reprovados_pandemia=$(grep '2020\|2021' resultado.csv | grep 'Reprovado' | wc -l)
    cancelamentos_pandemia=$(grep '2020\|2021' resultado.csv | grep 'Cancelado' | wc -l)
    
    #aprovacoes, reprovacoes e cancelamentos nos anos anteriores a 2022
                                #filtra as linhas que nao contem os anos de 2020, 2021 ou 2022
    aprovados_anteriores=$(grep -v '2020\|2021\|2022' resultado.csv | grep 'Aprovado' | wc -l)
    reprovados_anteriores=$(grep -v '2020\|2021\|2022' resultado.csv | grep 'Reprovado' | wc -l)
    cancelamentos_anteriores=$(grep -v '2020\|2021\|2022' resultado.csv | grep 'Cancelado' | wc -l)
    
    #mediana das notas em 2022.1
    notas_2022=$(grep '1,2022' resultado.csv | cut -d',' -f7 | sort -n)
                                            #wc -l eh para contar o numero de linhas
    total_notas_2022=$(echo "$notas_2022" | wc -l)
    metade=$((total_notas_2022 / 2))
    if ((total_notas_2022 % 2 == 0)); then
                                            #sed para extrair uma linha especifica (no caso a mediana)
        mediana_2022=$(echo "$notas_2022" | sed -n "$metade p")
    else
        mediana_2022=$(echo "$notas_2022" | sed -n "$((metade + 1)) p")
    fi
    
    #resultados 
    printf "Comparação de 2022 periodo 1 com anos de pandemia e anos anteriores:\n"
    printf "Aprovações:\n"
    printf "2022 periodo 1: %d\n" "$aprovados_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$aprovados_pandemia"
    printf "Anos anteriores: %d\n" "$aprovados_anteriores"
    printf "\n"
    printf "Reprovações:\n"
    printf "2022 periodo 1: %d\n" "$reprovados_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$reprovados_pandemia"
    printf "Anos anteriores: %d\n" "$reprovados_anteriores"
    printf "\n"
    printf "Mediana das notas em 2022 periodo 1: %.2f\n" "$mediana_2022"
    printf "\n"
    printf "Cancelamentos:\n"
    printf "2022 periodo 1: %d\n" "$cancelamentos_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$cancelamentos_pandemia"
    printf "Anos anteriores: %d\n" "$cancelamentos_anteriores"
}


#funcao principal do script -----------------------------------------------------------------------
function main() {
    remove_2semestre
    status
    aprovacao
    porcentagem_aprovacao_reprovacao
    media_nota_aprovados
    media_nota_reprovados
    media_frequencia_reprovados_nota
    porcent_evasoes
    rendimento_pandemia
    comparacao
}

#para chamar a funcao principal -------------------------------------------------------------------
main