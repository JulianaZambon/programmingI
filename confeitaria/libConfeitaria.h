//O frigobar é uma struct com dois membros:
//Prateleira de cima, que guarda até 20 bolos de pote
//Prateleira de baixo, que guarda até 10 tortinhas

//Ambas as prateleira são ponteiros para struct e devem ser 
//alocados de acordo com o tipo de doce

//Um bolo de pote é uma struct que tem tipo 
//(“m” para morango e “c” para chocolate),
//data de fabricação. O consumo deve ser imediato, isto é, no dia do pedido.

//Uma tortinha é uma struct que tem data de fabricação e dias para consumo 
//(5 dias após a data de fabricação).
//pedido do dia é uma informação passada como entrada pelo usuário e consiste na
//quantidade de bolos de pote de cada tipo e tortinhas.


#define MAX_BOLO 20
#define MAX_TORTA 10

typedef struct tipo_data 
{
    int dia;
    int mes;
    int ano;
} data;

typedef struct tipo_bolo 
{
    char sabor;
    data fabricacao;
} bolo;

typedef struct tipo_torta 
{
    data fabricacao;
    data validade;
} torta;

typedef struct tipo_frigobar
{
    bolo prateleiraCima[MAX_BOLO];
    torta prateleiraBaixo [MAX_TORTA];
} frigobar;