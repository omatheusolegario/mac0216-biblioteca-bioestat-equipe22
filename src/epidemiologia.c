#include "epidemiologia.h"
#include <stddef.h>


be_status be_preencher_tabela(long a, long b, long c, long d, be_tabela2x2 *saida){
    
    if (saida == NULL) return BE_ERRO_PONTEIRO_NULO;

    if (a < 0 || b < 0 || c < 0 || d < 0) return BE_ERRO_VALOR_INVALIDO;

    // Grava os valores dos parametros na struct tabela2x2
    saida->a = a;
    saida->b = b;
    saida->c = c;
    saida->d = d;

    return BE_OK;
}

be_status be_risco_expostos(const be_tabela2x2 *t, double *saida){
    // Tratamento de erros
    if (t == NULL || saida == NULL) return BE_ERRO_PONTEIRO_NULO;

    if (t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    if (t->a + t->b == 0) return BE_ERRO_DIVISAO_POR_ZERO;


    // Cálculo da formula
    *saida = (double)t->a / (t->a + t->b);

    return BE_OK;
}

be_status be_risco_naoexpostos(const be_tabela2x2 *t, double *saida){

    // Tratamento de erros
    if (t == NULL || saida == NULL) return BE_ERRO_PONTEIRO_NULO;

    if (t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    if (t->c + t->d == 0) return BE_ERRO_DIVISAO_POR_ZERO;


    // Cálculo da formula
    *saida = (double)t->c / (t->c + t->d);

    return BE_OK;

}

be_status be_risco_relativo(const be_tabela2x2 *t, double *saida){

    // Tratamento de erros
    if (t == NULL || saida == NULL) return BE_ERRO_PONTEIRO_NULO;

    if (t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    if (t->a + t->b == 0 || t->c + t->d == 0) return BE_ERRO_DIVISAO_POR_ZERO;


    // Cálculo da formula
    double RE;  // Risco expostos
    double RNE; // Risco não expostos
    be_status st1 = be_risco_expostos(t, &RE);
    be_status st2 = be_risco_naoexpostos(t, &RNE);
    
    // Verifica erros
    if (RNE == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    if (st1 != 0) return st1;
    if (st2 != 0) return st2;

    // Escreve em saida
    *saida = (double)RE / RNE;

    return BE_OK;

}

be_status be_odds_ratio(const be_tabela2x2 *t, double *saida){

    // Tratamento de erros
    if (t == NULL || saida == NULL) return BE_ERRO_PONTEIRO_NULO;

    if (t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    if (t->b * t->c == 0) return BE_ERRO_DIVISAO_POR_ZERO;

    // Cálculo da formula
    *saida = (double)(t->a * t->d) / (t->b * t->c);

    return BE_OK;

}
be_status be_diferenca_riscos(const be_tabela2x2 *t, double *saida){
    // Tratamento de erros
    if (t == NULL || saida == NULL) return BE_ERRO_PONTEIRO_NULO;

    if (t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;


    // Cálculo da formula
    double RE;  // Risco expostos
    double RNE; // Risco não expostos
    be_status st1 = be_risco_expostos(t, &RE);
    be_status st2 = be_risco_naoexpostos(t, &RNE);
    
    // Verifica erros
    if (st1 != 0) return st1;
    if (st2 != 0) return st2;

    // Escreve em saida
    *saida = RE - RNE;

    return BE_OK;
}

