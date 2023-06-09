# Programação I (CI1001)
Trabalhos solicitados na disciplina de programação I (ci1001).

### Trabalho Pequeno 2 
Este trabalho é uma variante de implementaçãoo do mesmo problema do
TP1, o desafio  é aprender a passar parâmetros usando ponteiros (endereços)
para evitar cópias de estruturas que podem ser complexas e ocupar muito
espaço na STACK.
<br /> 
### Trabalho Pequeno 3
Este trabalho consiste na implementação do programa libpilha.c com base
no libpilha.h fornecido.
Sua implementaçãoo deve usar uma lista ligada com nodo cabeça que
implementa a política LIFO (Last In First Out), isto é, uma política que
implementa um Tipo Abstrato de Dados Pilha.
O desafio é aprender a usar alocação dinâmica e manter a lista sempre
ligada, sem perder ponteiros.
<br /> 
### Trabalho Pequeno 3b (parenteses) 
Implementar um programa que verifica se uma expressão aritimética digitada pelo usuário está com 
os parênteses balanceados, considerando que existem alguns tipos de “parênteses” tais como os próprio
parênteses ((, )), colchetes ([ ]), chaves ({, }), dentre outros.
Não é necessário fazer a conta, é só para verificar os parênteses. Basta
não fazer nada com os números lidos.
O programa vai ler uma expressão da entrada padrão e verificar se os
símbolos de precedencia (parentesis, colchetes e chaves) foram corretamente
utilizados
<br /> 
### Trabalho Pequeno 4 
Este trabalho consiste na implementação do programa libfila.c com base
no libfila.h fornecido.
Sua implementação deve usar uma lista ligada com nodo cabeça que
implementa a política FIFO (First In First Out), isto  é, uma política que
implementa um Tipo Abstrato de Dados Fila.
O desafio é aprender a usar alocaçãoo dinâmica e manter a lista sempre
ligada, sem perder ponteiros. Desta vez, observe que o tipo fila contém dois
apontadores para a lista ligada, um que aponta para a cabeça da lista (início
dela) e outro que aponta sempre para a cauda da lista (final dela).
<br /> 
### Preparatório 1 para o T1
Este trabalho preparatório consiste na implementação do programa liblista_ordenada.c
com base no liblista_ordenada.h fornecido e de um testador para ele
como programa main.Os desafios são:
• Aprender a inserir e remover ordenadamente;
• Lidar com a diferença entre o nodo conter um int e agora ele conter
um apontador para int.
Para testar sua implementação, construa um arquivo testa_lista_ordenada.c 
<br />
### Preparatório 2 para o T1
Este trabalho preparatório consiste na implementaçãoo do programa liblista_circular.c
com base no liblista_circular.h fornecido e de um testador para ele
como programa main.
Basicamente, consiste em modificar o preparatório 1 para implementar
uma lista circular duplamente encadeada. Tome as decisões que quiser neste
preparatório.
DICA: ao criar a lista, crie um nodo adicional para ser um sentinela. Se
este sentinela for sempre o primeiro da lista sua implementação será mais
simples.
Os desafios são:
• Aprender a implementar uma lista circular duplamente encadeada com
sentinela.
Para testar sua implementaçãoo, construa um arquivo testa_lista_circular.c 
<br />
### T1 - parte 01
Este trabalho consiste na implementação do programa libagenda.c com
base no libagenda.h fornecido.
Basicamente, consiste em adaptar os seus programas feitos nos prepa ratórios 1 e 2, implementando as listas ligadas ordenadas (uma simples e
outra dupla circular), modificadas para atender às novas especificações.
Os desafios são:
• Adaptar os códigos já feitos;
• Implementar as funções adicionais.
