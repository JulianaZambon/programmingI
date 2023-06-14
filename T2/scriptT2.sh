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
    #f1,5 matricula, periodo e o ano
    # $() eh executado e seu resultado eh atribuido a variavel

    #extracao das colunas matricula, periodo e ano a partir do arquivo resultado.csv
    #filtragem dos registros aprovados
    #contagem do numero maximo de vezes cursadas antes da aprovacao
    max_cursadas=$(cut -d',' -f1,4,5 resultado.csv | grep 'Aprovado' | cut -d',' -f1,3 |
        sort | uniq -c | sort -nr | head -n 1 | awk '{print $1}')

    #contagem do número de individuos com o mesmo numero maximo de vezes cursadas
    count_max_cursadas=$(cut -d',' -f1,4,5 resultado.csv | grep 'Aprovado' | cut -d',' -f1,3 |
        sort | uniq -c | grep "$max_cursadas " | wc -l)

    printf "Máximo de vezes cursadas antes da aprovação: %s\n" "$max_cursadas"
    printf "Número de indivíduos com o mesmo número máximo de vezes cursadas: %s\n" "$count_max_cursadas"

            #-c faz com que o uniq exiba o numero de ocorrencias de cada linha
}

#4)qual a porcentagem de aprovacaoo/reprovacao por ano?
function porcentagem_aprovacao_reprovacao() {
   while IFS=',' read -r ano status count; do
        total_alunos=$(grep -c "$ano" resultado.csv)
        
        # verifica se o total de alunos eh igual a zero
        # evita divisao por zero
        if [ "$total_alunos" -eq 0 ]; then
            printf "%s: nenhum aluno registrado.\n" "$ano"
            continue
        fi

        porcent_aprovados=$(grep -c "$ano,Aprovado" resultado.csv)
        porcent_reprovados=$(grep -c "$ano,Reprovado" resultado.csv)
        
        porcent_aprovados=$(awk "BEGIN { printf \"%.2f\", ($porcent_aprovados / $total_alunos) * 100 }")
        porcent_reprovados=$(awk "BEGIN { printf \"%.2f\", ($porcent_reprovados / $total_alunos) * 100 }")
        
        printf "%s: Aprovados: %.2f%%, Reprovados: %.2f%%\n" "$ano" "$porcent_aprovados" "$porcent_reprovados"
    done < <(cut -d',' -f5,10 resultado.csv | sort | uniq -c)
}

#5)qual eh a media de nota dos aprovados (no periodo total e por ano)?
function media_nota_aprovados() {
    cut -d',' -f5,6,10 resultado.csv | grep 'Aprovado' |
    while IFS=',' read -r ano nota status; do
        notas[$ano]=$(echo "${notas[$ano]} + $nota" | bc)
        count[$ano]=$((${count[$ano]} + 1))
    done

                # @ indica que queremos todas as chaves
                # ! indica que queremos as chaves, em vez dos valores correspondentes
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
   while IFS=',' read -r ano status count; do
        total_alunos=$(grep -c "$ano" resultado.csv)
        porcent_evasoes=$(grep -c "$ano,Cancelado" resultado.csv)
        porcent_evasoes=$(awk "BEGIN { printf \"%.2f\", ($porcent_evasoes / $total_alunos) * 100 }")
        printf "%s: Porcentagem de evasões: %.2f%%\n" "$ano" "$porcent_evasoes"
    done < <(cut -d',' -f5,10 resultado.csv | sort | uniq -c)
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
        *) printf "opcaoo invalida\n" ;;
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