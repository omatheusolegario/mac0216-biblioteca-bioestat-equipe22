#include <stddef.h>
#include "teste.h"
#include "epidemiologia.h"

int main(void){

    /* ---------- be_preencher_tabela ---------- */

    /* caso normal: entrada valida grava os 4 campos */
    be_tabela2x2 tb = {-9, -9, -9, -9};   /* nao pode passar por acaso */
    be_status st = be_preencher_tabela(1, 2, 3, 4, &tb);
    VERIFICA(st == BE_OK, "preencher_tabela: entrada valida devolve BE_OK");
    VERIFICA(tb.a == 1, "preencher_tabela: grava a");
    VERIFICA(tb.b == 2, "preencher_tabela: grava b");
    VERIFICA(tb.c == 3, "preencher_tabela: grava c");
    VERIFICA(tb.d == 4, "preencher_tabela: grava d");

    /* limite: zero e permitido */
    be_tabela2x2 tz = {7, 7, 7, 7};
    st = be_preencher_tabela(0, 0, 0, 0, &tz);
    VERIFICA(st == BE_OK, "preencher_tabela: zeros devolve BE_OK");
    VERIFICA(tz.a == 0 && tz.b == 0 && tz.c == 0 && tz.d == 0,
             "preencher_tabela: zeros sao gravados");

    /* invalido: um valor negativo em cada parametro, saida intacta em erro */
    be_tabela2x2 t2 = {7, 7, 7, 7};
    st = be_preencher_tabela(-1, 0, 0, 0, &t2);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "preencher_tabela: a negativo devolve erro");
    st = be_preencher_tabela(0, -1, 0, 0, &t2);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "preencher_tabela: b negativo devolve erro");
    st = be_preencher_tabela(0, 0, -1, 0, &t2);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "preencher_tabela: c negativo devolve erro");
    st = be_preencher_tabela(0, 0, 0, -1, &t2);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "preencher_tabela: d negativo devolve erro");

    /* invalido: ponteiro de saida nulo */
    st = be_preencher_tabela(1, 2, 3, 4, NULL);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "preencher_tabela: saida NULL devolve erro");





    
    return RESULTADO_TESTES();
}
