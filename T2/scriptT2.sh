#!/bin/bash
export LC_NUMERIC="en_US.UTF-8"

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
    #campo de status (coluna 10) do arquivo resultado.csv
    cut -d',' -f10 resultado.csv | sort | uniq -c |
    while read -r count status; do
        lower_status=$(echo "$status" | tr '[:upper:]' '[:lower:]')
        if [ "$lower_status" != "status" ]; then
            if [ "$lower_status" = "matriculado" ]; then
                count=$(grep -wc "$status" resultado.csv)
            fi
            printf "%s indivíduo(s) com status '%s'\n" "$count" "$lower_status"
        fi
    done
}

#3)qual o maximo de vezes que um individuo cursou a discplina antes de ser aprovado? 
#quantos individuos possuem o mesmo numero maximo de vezes cursadas ate a aprovacao?
function aprovacao() {
    # $() eh executado e seu resultado eh atribuido a variavel

    #extrair as matrículas dos registros aprovados
    matriculas=$(awk -F',' '$10 == "Aprovado" {print $1}' resultado.csv)

    #inicializa as var para rastrear o maximo de vezes cursadas e o numero de individuos com esse maximo
    max_cursadas=0
    count_max_cursadas=0

    #percorre as matriculas e conta o numero de vezes cursadas antes da aprovacao
    for matricula in $matriculas; do
        num_cursadas=$(grep -c "^$matricula," resultado.csv)
        if ((num_cursadas > max_cursadas)); then
            max_cursadas=$num_cursadas
            count_max_cursadas=1
        elif ((num_cursadas == max_cursadas)); then
            ((count_max_cursadas++))
        fi
    done

    printf "maximo de vezes cursadas antes da aprovacao: %s\n" "$max_cursadas"
    printf "numero de indivíduos com o mesmo número maximo de vezes cursadas: %s\n" "$count_max_cursadas"

            #-c faz com que o uniq exiba o numero de ocorrencias de cada linha
}

#4)qual a porcentagem de aprovacaoo/reprovacao por ano?
#imprime as porcentagens em 0
function porcentagem_aprovacao_reprovacao() {
  while IFS=',' read -r count ano status; do
        total_alunos=$(grep -c ",$ano" resultado.csv)
        
        # Verifica se o total de alunos é igual a zero
        # Evita divisão por zero
        if [ "$total_alunos" -eq 0 ]; then
            printf "%s: nenhum aluno registrado.\n" "$ano"
            continue
        fi

        porcent_aprovados=$(awk -F',' -v ano="$ano" -v total_alunos="$total_alunos" '$2 == ano && $10 == "Aprovado" {count++} END {if (total_alunos > 0) printf "%.2f", (count / total_alunos) * 100; else printf "0"}' resultado.csv)
        porcent_reprovados=$(awk -F',' -v ano="$ano" -v total_alunos="$total_alunos" '$2 == ano && $10 == "Reprovado" {count++} END {if (total_alunos > 0) printf "%.2f", (count / total_alunos) * 100; else printf "0"}' resultado.csv)
        
        printf "%s: Aprovados: %.2f%%, Reprovados: %.2f%%\n" "$ano" "$porcent_aprovados" "$porcent_reprovados"
    done < <(cut -d',' -f2,5,10 resultado.csv | sort | uniq -c)
}



#5)qual eh a media de nota dos aprovados (no periodo total e por ano)?
# ta imprimindo a media em 0!
function media_nota_aprovados() {
    declare -A notas
    declare -A count
    local total_media=0
    local total_alunos=0

    while IFS=',' read -r ano nota status; do
        nota=${nota/,/.}  # Substituir vírgula por ponto
        notas[$ano]=$(awk -v nota="$nota" -v total="${notas[$ano]}" 'BEGIN { printf "%.2f", total + nota }')
        count[$ano]=$((${count[$ano]} + 1))
    done < <(grep 'Aprovado' resultado.csv | cut -d',' -f5,6,10)

    for ano in "${!notas[@]}"; do
        local media=$(awk -v total="${notas[$ano]}" -v count="${count[$ano]}" 'BEGIN { printf "%.2f", total / count }')
        printf "%s: Média de nota dos aprovados: %.2f\n" "$ano" "$media"
        total_media=$(awk -v total_media="$total_media" -v count="${count[$ano]}" -v media="$media" 'BEGIN { printf "%.2f", total_media + (media * count) }')
        total_alunos=$(($total_alunos + ${count[$ano]}))
    done

    local media_total=$(awk -v total_media="$total_media" -v total_alunos="$total_alunos" 'BEGIN { printf "%.2f", total_media / total_alunos }')
    printf "Média de nota dos aprovados no período total: %.2f\n" "$media_total"
}




#6)qual eh a media de nota dos reprovados por nota (periodo total e ano)?
function media_nota_reprovados() {
    declare -A notas
    declare -A count

    while IFS=',' read -r ano nota status; do
        if [[ $status == "Reprovado" ]]; then
            nota=${nota/,/.}
            if [[ -z ${notas[$ano]} ]]; then
                notas[$ano]=$nota
            else
                notas[$ano]=$(echo "${notas[$ano]} + $nota" | bc)
            fi
            count[$ano]=$((${count[$ano]} + 1))
        fi
    done < resultado.csv

    for ano in "${!notas[@]}"; do
        media=$(echo "scale=2; ${notas[$ano]} / ${count[$ano]}" | bc)
        printf "%s: Média de nota dos reprovados: %.2f\n" "$ano" "$media"
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
    while IFS=',' read -r ano status count; do
        total_alunos=$(grep -c ",$ano" resultado.csv)
        porcent_evasoes=$(grep -c ",$ano,Cancelado" resultado.csv)
        porcent_evasoes=$(awk "BEGIN { printf \"%.2f\", ($porcent_evasoes / $total_alunos) * 100 }")
        printf "%s: Porcentagem de evasões: %.2f%%\n" "$ano" "$porcent_evasoes"
    done < <(cut -d',' -f10,5 resultado.csv | sort | uniq -c)
}

#9)como os anos de pandemia impactaram no rendimento dos estudantes em relacao aos anos anteriores? 
#calcule em percentual o rendimento dos aprovados, a taxa de cancelamento e de reprovacoes. 
#considere como anos de pandemia os anos de 2020 e 2021. 
#(EXEMPLO: qual o percentual de aumento ou diminuicao de notas, frequencias, aprovacoes/reprovacoes e cancelamentos).
function rendimento_pandemia() {
     total_aprovados=$(grep -c 'Aprovado' resultado.csv)
    total_cancelamentos=$(grep -c 'Cancelado' resultado.csv)
    total_reprovados=$(grep -c 'Reprovado' resultado.csv)
    
    total_pandemia=$(grep -cE '2020|2021' resultado.csv)
    aprovados_pandemia=$(grep -cE '2020|2021' resultado.csv | grep -c 'Aprovado')
    cancelamentos_pandemia=$(grep -cE '2020|2021' resultado.csv | grep -c 'Cancelado')
    reprovados_pandemia=$(grep -cE '2020|2021' resultado.csv | grep -c 'Reprovado')
    
    percent_aprovados=$(awk "BEGIN { printf \"%.2f\", ($aprovados_pandemia / $total_pandemia) * 100 }")
    percent_cancelamentos=$(awk "BEGIN { printf \"%.2f\", ($cancelamentos_pandemia / $total_pandemia) * 100 }")
    percent_reprovados=$(awk "BEGIN { printf \"%.2f\", ($reprovados_pandemia / $total_pandemia) * 100 }")
    
    printf "Rendimento dos aprovados durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_aprovados"
    printf "Taxa de cancelamento durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_cancelamentos"
    printf "Taxa de reprovação durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_reprovados"
}


#10)compare a volta as aulas hibrida (2022 periodo 1) com os anos de pandemia e 
#os anos anteriores em relacao as aprovacoes, reprovacoes, mediana das notas e cancelamentos.
function comparacao() {
    #aprovacoes, reprovacoes e cancelamentos em 2022.1
    aprovados_2022=$(grep -c '1,2022,Aprovado' resultado.csv)
    reprovados_2022=$(grep -c '1,2022,Reprovado' resultado.csv)
    cancelamentos_2022=$(grep -c '1,2022,Cancelado' resultado.csv)
    
    #aprovacoes, reprovacoes e cancelamentos na pandemia (2020 e 2021)
    aprovados_pandemia=$(grep -cE '2020|2021,Aprovado' resultado.csv)
    reprovados_pandemia=$(grep -cE '2020|2021,Reprovado' resultado.csv)
    cancelamentos_pandemia=$(grep -cE '2020|2021,Cancelado' resultado.csv)
    
    #aprovacoes, reprovacoes e cancelamentos nos anos anteriores a 2022
    aprovados_anteriores=$(grep -vcE '2020|2021|2022' resultado.csv | grep -c 'Aprovado')
    reprovados_anteriores=$(grep -vcE '2020|2021|2022' resultado.csv | grep -c 'Reprovado')
    cancelamentos_anteriores=$(grep -vcE '2020|2021|2022' resultado.csv | grep -c 'Cancelado')
    
    #mediana das notas em 2022.1
    notas_2022=$(grep '1,2022' resultado.csv | cut -d',' -f7 | sort -n)
    total_notas_2022=$(echo "$notas_2022" | wc -l)
    metade=$((total_notas_2022 / 2))
    if ((total_notas_2022 % 2 == 0)); then
        mediana_2022=$(echo "$notas_2022" | sed -n "$metade p")
    else
        mediana_2022=$(echo "$notas_2022" | sed -n "$((metade + 1)) p")
    fi
    
    printf "Comparacao de 2022 periodo 1 com anos de pandemia e anos anteriores:\n"
    printf "Aprovacoes:\n"
    printf "2022 periodo 1: %d\n" "$aprovados_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$aprovados_pandemia"
    printf "Anos anteriores: %d\n" "$aprovados_anteriores"
    printf "\n"
    printf "Reprovacoes:\n"
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


#main-----------------------------------------------------------------------------------

main() {
    clear
    printf "Analise de Dados do Curso de Algoritmos e Estrutura de Dados 1\n\n"
    printf "1) Remover 2º semestre de 2022\n"
    printf "2) Numero de individuos por status\n"
    printf "3) Maximo de vezes cursadas antes da aprovacao\n"
    printf "4) Porcentagem de aprovação/reprovação por ano\n"
    printf "5) Media de nota dos aprovados por ano\n"
    printf "6) Media de nota dos reprovados por ano\n"
    printf "7) Media de frequencia dos reprovados\n"
    printf "8) Porcentagem de evasoes\n"
    printf "9) Rendimento durante os anos de pandemia (2020 e 2021) comparado com anos anteriores\n"
    printf "10) Comparacao da aula hibrida com os anos anteriores\n" 
    printf "11) Sair\n\n"

    printf "Selecione uma opcao: "
    read -r option

    case $option in
        1) remove_2semestre ;;
        2) status ;;
        3) aprovacao ;;
        4) porcentagem_aprovacao_reprovacao ;;
        5) media_nota_aprovados ;;
        6) media_nota_reprovados ;;
        7) media_frequencia_reprovados_nota ;;
        8) porcentagem_evasoes ;;
        9) rendimento_pandemia;;
        10) comparacao;;
        11) exit ;;
        *) printf "opcao invalida\n" ;;
    esac

    printf "\npressione qualquer tecla para voltar"
    read -n 1 -s
    main
}

#execucao -----------------------------------------------------------------------------------------

if [ -f "historico-alg1_SIGA_ANONIMIZADO.csv" ]; then
    main
else
    printf "arquivo historico-alg1_SIGA_ANONIMIZADO.csv nao encontrado.\n"
fi