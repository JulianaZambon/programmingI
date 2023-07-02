#!/bin/bash
export LC_NUMERIC="en_US.UTF-8"

#funcoes ------------------------------------------------------------------------------------------

#1)remova todas as linhas que facam parte do 2o semestre de 2022 (periodo 2, ano 2022)
function remove_2semestre() {
    #grep eh comando de busca
                    # resultado.csv eh o arquivo com a remocao
    grep -v '2,2022' historico-alg1_SIGA_ANONIMIZADO.csv > resultado.csv
    printf "Remoção do 2º semestre de 2022 concluída.\n"
}

#"status" diz a situação de cada individuo (se cancelou a materia, foi aprovado, reprovou, etc.).
#2)para cada status, calcule o numero de individuos unicos naquele status
function status() {
    #campo de status do arquivo
    cut -d',' -f10 historico-alg1_SIGA_ANONIMIZADO.csv | sort | uniq -c |
    while read -r count status; do
        lower_status=$(echo "$status" | tr '[:upper:]' '[:lower:]')
        if [ "$lower_status" != "status" ]; then
            if [ "$lower_status" = "matriculado" ]; then
                count=$(grep -wc "$status" historico-alg1_SIGA_ANONIMIZADO.csv)
            fi
            printf "%s indivíduo(s) com status '%s'\n" "$count" "$lower_status"
        fi
    done
}

#3)qual o maximo de vezes que um individuo cursou a discplina antes de ser aprovado? 
#quantos individuos possuem o mesmo numero maximo de vezes cursadas ate a aprovacao?
function aprovacao() {
    # $() eh executado e seu resultado eh atribuido a variavel

    #extrair os aprovados
    matriculas=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E 'Aprovado' | cut -d',' -f1)   

    #inicializa as var para rastrear o maximo de vezes cursadas e o numero de individuos com esse maximo
    max_cursadas=0
    count_max_cursadas=0

    #percorre as matriculas e conta o numero de vezes cursadas antes da aprovacao
    for matricula in $matriculas; do
        num_cursadas=$(grep -c "^$matricula," historico-alg1_SIGA_ANONIMIZADO.csv)
        if ((num_cursadas > max_cursadas)); then
            max_cursadas=$num_cursadas
            count_max_cursadas=1
        elif ((num_cursadas == max_cursadas)); then
            ((count_max_cursadas++))
        fi
    done

    printf "maximo de vezes cursadas antes da aprovacao: %s\n" "$max_cursadas"
    printf "numero de indivíduos com o mesmo número maximo de vezes cursadas: %s\n" "$count_max_cursadas"
}

#4)qual a porcentagem de aprovacao/reprovacao por ano?
function porcentagem_aprovacao_reprovacao() {
    declare -A count_aprovados
    declare -A count_reprovados
    declare -A total_alunos

    while IFS=',' read -r f5 f10; do
        status="$f10"
        ano="$f5"
        #array de aprovacao e reprovacao
        if [[ $status == "Aprovado" ]]; then
            ((count_aprovados[$ano]++))
        elif [[ $status == R-* ]]; then
            ((count_reprovados[$ano]++))
        fi
        ((total_alunos[$ano]++)) #array do total de alunos
    done < historico-alg1_SIGA_ANONIMIZADO.csv


    printf "Ano: Aprovados: Reprovados:\n"
    #percorre tanto as chaves do array count_aprovados quanto as chaves do array count_reprovados
    #faz com que garanta que 2012 esteja na saida, por ex, ja que ese ano nao tem aprovados
    for ano in $(printf '%s\n' "${!count_aprovados[@]}" "${!count_reprovados[@]}" | sort -n | uniq); do
        porcentagem_aprovados=0
        porcentagem_reprovados=0

        if [[ ${total_alunos[$ano]} -ne 0 ]]; then
            porcentagem_aprovados=$(bc <<< "scale=2; (${count_aprovados[$ano]} / ${total_alunos[$ano]}) * 100" 2>/dev/null)
            porcentagem_reprovados=$(bc <<< "scale=2; (${count_reprovados[$ano]} / ${total_alunos[$ano]}) * 100" 2>/dev/null)
        fi

        printf "%s: %.2f%% %.2f%%\n" "$ano" "$porcentagem_aprovados" "$porcentagem_reprovados"
    done
}

#5)qual eh a media de nota dos aprovados (no periodo total e por ano)?
function media_nota_aprovados() {
    declare -A notas
    declare -A count
    local total_media=0
    local total_alunos=0

    while IFS=',' read -r ano nota status; do
        nota=${nota/,/.}  # substituir vírgula por ponto para calcular certo a media depois
        #atualiza o valor da media                  #o antigo valor eh acessado e somado com a nova
        notas[$ano]=$(awk -v nota="$nota" -v total="${notas[$ano]}" 'BEGIN { printf "%.2f", total + nota }')
        #incrementa
        count[$ano]=$((${count[$ano]} + 1))
    done < <(grep 'Aprovado' historico-alg1_SIGA_ANONIMIZADO.csv | cut -d',' -f5,8,10)

    printf "Ano: Média de nota dos aprovados:\n"
    for ano in $(printf '%s\n' "${!notas[@]}" | sort -n); do
        local media=0
        if [ "${count[$ano]}" -ne 0 ]; then
            media=$(awk -v total="${notas[$ano]}" -v count="${count[$ano]}" 'BEGIN { printf "%.2f", total / count }')
        fi
        printf "%s: %.2f\n" "$ano" "$media"
        #atualiza o valor da media total
        total_media=$(awk -v total_media="$total_media" -v count="${count[$ano]}" -v media="$media" 'BEGIN { printf "%.2f", total_media + (media * count) }')
        total_alunos=$((total_alunos + ${count[$ano]}))
    done | sort -k1n #ordena a primeira coluna numericamente 

    if [ "$total_alunos" -ne 0 ]; then
        local media_total=$(awk -v total_media="$total_media" -v total_alunos="$total_alunos" 'BEGIN { printf "%.2f", total_media / total_alunos }')
        printf "Média de nota dos aprovados no período total: %.2f\n" "$media_total"
    else
        printf "Não há alunos aprovados no período.\n"
    fi
}

#5)qual eh a media de nota dos aprovados (no periodo total e por ano)?
function media_nota_aprovados() {
    declare -A notas
    declare -A count
    local total_media=0
    local total_alunos=0

    while IFS=',' read -r ano nota status; do
        nota=${nota/,/.}  # substituir vírgula por ponto para calcular certo a media depois
        #atualiza o valor da media                  #o antigo valor eh acessado e somado com a nova
        notas[$ano]=$(awk -v nota="$nota" -v total="${notas[$ano]}" 'BEGIN { printf "%.2f", total + nota }')
        #incrementa
        count[$ano]=$((${count[$ano]} + 1))
    done < <(grep 'Aprovado' historico-alg1_SIGA_ANONIMIZADO.csv | cut -d',' -f5,8,10)

    printf "Ano: Média de nota dos aprovados:\n"
    for ano in $(printf '%s\n' "${!notas[@]}" | sort -n); do
        local media=0
        if [ "${count[$ano]}" -ne 0 ]; then
            media=$(awk -v total="${notas[$ano]}" -v count="${count[$ano]}" 'BEGIN { printf "%.2f", total / count }')
        fi
        printf "%s: %.2f\n" "$ano" "$media"
        #atualiza o valor da media total
        total_media=$(awk -v total_media="$total_media" -v count="${count[$ano]}" -v media="$media" 'BEGIN { printf "%.2f", total_media + (media * count) }')
        total_alunos=$((total_alunos + ${count[$ano]}))
    done | sort -k1n #ordena a primeira coluna numericamente 

    if [ "$total_alunos" -ne 0 ]; then
        local media_total=$(awk -v total_media="$total_media" -v total_alunos="$total_alunos" 'BEGIN { printf "%.2f", total_media / total_alunos }')
        printf "Média de nota dos aprovados no período total: %.2f\n" "$media_total"
    else
        printf "Não há alunos aprovados no período.\n"
    fi
}

#a function 6 tem a mesma logica da function 5, a unica diferenca eh que ela
#considera os reprovados para realizar os calculos

#6)qual eh a media de nota dos reprovados por nota (periodo total e ano)?
function media_nota_reprovados() {
    declare -A notas
    declare -A count
    local total_media=0
    local total_alunos=0

    while IFS=',' read -r ano nota status; do
        nota=${nota/,/.}  # Substituir vírgula por ponto
        notas[$ano]=$(awk -v nota="$nota" -v total="${notas[$ano]}" 'BEGIN { printf "%.2f", total + nota }')
        count[$ano]=$((${count[$ano]} + 1))
    done < <(grep -E 'R-nota|R-freq' historico-alg1_SIGA_ANONIMIZADO.csv | cut -d',' -f5,8,10)

    for ano in "${!notas[@]}"; do
        local media=0
        if [ "${count[$ano]}" -ne 0 ]; then
            media=$(awk -v total="${notas[$ano]}" -v count="${count[$ano]}" 'BEGIN { printf "%.2f", total / count }')
        fi
        printf "%s: Média de nota dos reprovados: %.2f\n" "$ano" "$media"
        total_media=$(awk -v total_media="$total_media" -v count="${count[$ano]}" -v media="$media" 'BEGIN { printf "%.2f", total_media + (media * count) }')
        total_alunos=$((total_alunos + ${count[$ano]}))
    done | sort -k1n

    if [ "$total_alunos" -ne 0 ]; then
        local media_total=$(awk -v total_media="$total_media" -v total_alunos="$total_alunos" 'BEGIN { printf "%.2f", total_media / total_alunos }')
        printf "Média de nota dos reprovados no período total: %.2f\n" "$media_total"
    else
        printf "Não há alunos reprovados no período.\n"
    fi
}

#7)qual eh a media da frequencia dos reprovados por nota (periodo total e por ano)?
#seleciona os reprovados, analisa as frequencia e realiza o calculo da media
function media_frequencia_reprovados_nota() {
    declare -A soma_frequencias
    declare -A count_notas
    local total_soma=0
    local total_count=0

    #calcula o total de frequencia dos reprovados por nota
    while IFS=',' read -r ano frequencia status; do
        if [[ $status == R-* ]]; then
            soma_frequencias[$ano]=$(awk "BEGIN{ printf \"%.2f\", ${soma_frequencias[$ano]} + $frequencia }")
            count_notas[$ano]=$((${count_notas[$ano]} + 1))
            total_soma=$(awk "BEGIN{ printf \"%.2f\", $total_soma + $frequencia }")
            total_count=$(($total_count + 1))
        fi
    done < <(cut -d',' -f5,9,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep 'R-*')

    #calcula a media por ano
    for ano in $(echo "${!soma_frequencias[@]}" | tr ' ' '\n' | sort -n); do
        media=$(awk "BEGIN{ printf \"%.2f\", ${soma_frequencias[$ano]} / ${count_notas[$ano]} }")
        printf "%s: Média da frequência dos reprovados por nota: %.2f\n" "$ano" "$media"
    done | sort -k1n

    #realiza o calculo da media no periodo total
    media_total=$(awk "BEGIN{ printf \"%.2f\", $total_soma / $total_count }")
    printf "Média da frequência dos reprovados por nota no período total: %.2f\n" "$media_total"
}

#8)qual a porcentagem de evasoes (total e anual)?
function porcentagem_evasoes() {
    #total de alunos
    total_alunos=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | wc -l)

    #total de evasoes
    evasoes=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -c 'Cancelado')

    #porcentagem de evasoes
    percent_evasoes=$(bc <<< "scale=2; ($evasoes / $total_alunos) * 100")

    #saida
    printf "Porcentagem de evasões: %.2f%%\n" "$percent_evasoes"

    #porcentagem de evasoes por ano
    printf "Porcentagem de evasões por ano:\n"
    cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E 'Cancelado' | cut -d',' -f2 | sort | uniq -c |
    while read -r count ano; do
        printf "%s: %.2f%%\n" "$ano" "$(bc <<< "scale=2; ($count / $total_alunos) * 100")"
    done | sort -k1n
}

#9)como os anos de pandemia impactaram no rendimento dos estudantes em relacao aos anos anteriores? 
#calcule em percentual o rendimento dos aprovados, a taxa de cancelamento e de reprovacoes. 
#considere como anos de pandemia os anos de 2020 e 2021. 
#(EXEMPLO: qual o percentual de aumento ou diminuicao de notas, frequencias, aprovacoes/reprovacoes e cancelamentos).
function rendimento_pandemia() {
    #aprovacoes, reprovacoes e cancelamentos durante a pandemia (2020 e 2021)
    aprovados_pandemia=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E -c '2020|2021,Aprovado')
    reprovados_pandemia=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E -c '2020|2021,R-')
    cancelamentos_pandemia=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E -c '2020|2021,Cancelado')

    #aprovacoes, reprovacoes e cancelamentos nos anos anteriores a 2022
    aprovados_anteriores=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -Ev -c '2020|2021|2022,Aprovado')
    reprovados_anteriores=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -Ev -c '2020|2021|2022,R-')
    cancelamentos_anteriores=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -Ev -c '2020|2021|2022,Cancelado')

    #total de alunos durante a pandemia (2020 e 2021)
    total_alunos_pandemia=$((aprovados_pandemia + reprovados_pandemia + cancelamentos_pandemia))

    #total de alunos nos anos anteriores a 2022
    total_alunos_anteriores=$((aprovados_anteriores + reprovados_anteriores + cancelamentos_anteriores))

    #porcentagem de aprovados durante a pandemia (2020 e 2021)
    percent_aprovados=$(bc <<< "scale=2; ($aprovados_pandemia / $total_alunos_pandemia) * 100")

    #porcentagem de cancelamentos durante a pandemia (2020 e 2021)
    percent_cancelamentos=$(bc <<< "scale=2; ($cancelamentos_pandemia / $total_alunos_pandemia) * 100")

    #porcentagem de reprovados durante a pandemia (2020 e 2021)
    percent_reprovados=$(bc <<< "scale=2; ($reprovados_pandemia / $total_alunos_pandemia) * 100")

    #saida
    printf "Rendimento dos aprovados durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_aprovados"
    printf "Taxa de cancelamento durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_cancelamentos"
    printf "Taxa de reprovação durante a pandemia (2020 e 2021): %.2f%%\n" "$percent_reprovados"
}

#10)compare a volta as aulas hibrida (2022 periodo 1) com os anos de pandemia e 
#os anos anteriores em relacao as aprovacoes, reprovacoes, mediana das notas e cancelamentos.
function comparacao() {
    #aprovacoes, reprovacoes e cancelamentos em 2022.1
    aprovados_2022=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -c '1,2022,Aprovado')
    reprovados_2022=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -c '1,2022,R-')
    cancelamentos_2022=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -c '1,2022,Cancelado')

    #aprovacoes, reprovacoes e cancelamentos na pandemia (2020 e 2021)
    aprovados_pandemia=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E -c '2020|2021,Aprovado')
    reprovados_pandemia=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E -c '2020|2021,R-')
    cancelamentos_pandemia=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -E -c '2020|2021,Cancelado')

    #aprovacoes, reprovacoes e cancelamentos nos anos anteriores a 2022
    aprovados_anteriores=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -Ev -c '2020|2021|2022,Aprovado')
    reprovados_anteriores=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -Ev -c '2020|2021|2022,R-')
    cancelamentos_anteriores=$(cut -d',' -f4,5,10 historico-alg1_SIGA_ANONIMIZADO.csv | grep -Ev -c '2020|2021|2022,Cancelado')

    #mediana das notas em 2022.1
    notas_2022=$(awk -F',' '$4 == "1" && $5 == "2022" {print $8}' historico-alg1_SIGA_ANONIMIZADO.csv | sort -n)
    total_notas_2022=$(echo "$notas_2022" | wc -l)
    metade=$((total_notas_2022 / 2))
    if ((total_notas_2022 % 2 == 0)); then
        mediana_2022=$(echo "$notas_2022" | sed -n "$metade,$((metade + 1))p" | awk '{sum+=$1} END{printf "%.2f", sum/2}')
    else
        #caso impar
        mediana_2022=$(echo "$notas_2022" | sed -n "$((metade + 1))p")
    fi


    #saida
    printf "Comparacao de 2022 periodo 1 com anos de pandemia e anos anteriores:\n"
    printf "Aprovacoes:\n"
    printf "2022 periodo 1: %d\n" "$aprovados_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$aprovados_pandemia"
    printf "Anos anteriores: %d\n" "$aprovados_anteriores"

    printf "\nReprovacoes:\n"
    printf "2022 periodo 1: %d\n" "$reprovados_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$reprovados_pandemia"
    printf "Anos anteriores: %d\n" "$reprovados_anteriores"

    printf "\nMediana das notas em 2022 periodo 1: %.2f\n" "$mediana_2022"

    printf "\nCancelamentos:\n"
    printf "2022 periodo 1: %d\n" "$cancelamentos_2022"
    printf "Pandemia (2020 e 2021): %d\n" "$cancelamentos_pandemia"
    printf "Anos anteriores: %d\n" "$cancelamentos_anteriores"
}

#main-----------------------------------------------------------------------------------

main() {
    clear

    #especie de "menu" para facilitar a visualizacao das respostas/saidas 
    #de cada pergunta

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