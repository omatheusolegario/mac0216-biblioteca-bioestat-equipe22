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

    /* ---------- tabelas e variaveis usadas nos testes abaixo ---------- */

    be_tabela2x2 ref = {20, 80, 10, 90};      /* referencia: RE=0.2 RNE=0.1 RR=2 OR=2.25 DR=0.1 */
    be_tabela2x2 tx = {4, 5, 7, 1};           /* RE=4/9 RNE=7/8 (DR negativa) */
    be_tabela2x2 exp_vazio = {0, 0, 5, 5};    /* grupo dos expostos vazio */
    be_tabela2x2 nexp_vazio = {5, 5, 0, 0};   /* grupo dos nao expostos vazio */
    be_tabela2x2 neg = {1, 1, 1, -1};         /* contem valor negativo */
    double r;

    /* ---------- be_risco_expostos ---------- */

    r = -9.0;
    st = be_risco_expostos(&ref, &r);
    VERIFICA(st == BE_OK, "risco_expostos: tabela de referencia devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.2, "risco_expostos: 20/(20+80) = 0.2");

    r = -9.0;
    st = be_risco_expostos(&tx, &r);
    VERIFICA_PROXIMO(r, 4.0 / 9.0, "risco_expostos: 4/(4+5), sem divisao inteira");

    /* limites: risco 0 e risco 1 sao validos */
    be_tabela2x2 re_zero = {0, 10, 5, 5};
    be_tabela2x2 re_um = {10, 0, 5, 5};
    r = -9.0;
    st = be_risco_expostos(&re_zero, &r);
    VERIFICA(st == BE_OK, "risco_expostos: a=0 devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.0, "risco_expostos: a=0 da risco 0");
    st = be_risco_expostos(&re_um, &r);
    VERIFICA(st == BE_OK, "risco_expostos: b=0 devolve BE_OK");
    VERIFICA_PROXIMO(r, 1.0, "risco_expostos: b=0 da risco 1");

    /* erros: saida intacta (sentinela -9.0) */
    r = -9.0;
    st = be_risco_expostos(&exp_vazio, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "risco_expostos: a+b=0 devolve divisao por zero");
    VERIFICA_PROXIMO(r, -9.0, "risco_expostos: saida nao modificada em erro");
    st = be_risco_expostos(&neg, &r);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "risco_expostos: tabela com negativo devolve erro");
    st = be_risco_expostos(NULL, &r);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "risco_expostos: tabela NULL devolve erro");
    st = be_risco_expostos(&ref, NULL);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "risco_expostos: saida NULL devolve erro");

    /* ---------- be_risco_naoexpostos ---------- */

    r = -9.0;
    st = be_risco_naoexpostos(&ref, &r);
    VERIFICA(st == BE_OK, "risco_naoexpostos: tabela de referencia devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.1, "risco_naoexpostos: 10/(10+90) = 0.1");

    r = -9.0;
    st = be_risco_naoexpostos(&tx, &r);
    VERIFICA_PROXIMO(r, 7.0 / 8.0, "risco_naoexpostos: 7/(7+1), sem divisao inteira");

    /* limites: risco 0 e risco 1 sao validos */
    be_tabela2x2 rne_zero = {5, 5, 0, 10};
    be_tabela2x2 rne_um = {5, 5, 10, 0};
    r = -9.0;
    st = be_risco_naoexpostos(&rne_zero, &r);
    VERIFICA(st == BE_OK, "risco_naoexpostos: c=0 devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.0, "risco_naoexpostos: c=0 da risco 0");
    st = be_risco_naoexpostos(&rne_um, &r);
    VERIFICA(st == BE_OK, "risco_naoexpostos: d=0 devolve BE_OK");
    VERIFICA_PROXIMO(r, 1.0, "risco_naoexpostos: d=0 da risco 1");

    /* erros: saida intacta (sentinela -9.0) */
    r = -9.0;
    st = be_risco_naoexpostos(&nexp_vazio, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "risco_naoexpostos: c+d=0 devolve divisao por zero");
    VERIFICA_PROXIMO(r, -9.0, "risco_naoexpostos: saida nao modificada em erro");
    st = be_risco_naoexpostos(&neg, &r);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "risco_naoexpostos: tabela com negativo devolve erro");
    st = be_risco_naoexpostos(NULL, &r);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "risco_naoexpostos: tabela NULL devolve erro");
    st = be_risco_naoexpostos(&ref, NULL);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "risco_naoexpostos: saida NULL devolve erro");

    /* ---------- be_risco_relativo ---------- */

    r = -9.0;
    st = be_risco_relativo(&ref, &r);
    VERIFICA(st == BE_OK, "risco_relativo: tabela de referencia devolve BE_OK");
    VERIFICA_PROXIMO(r, 2.0, "risco_relativo: 0.2/0.1 = 2.0");

    r = -9.0;
    st = be_risco_relativo(&tx, &r);
    VERIFICA_PROXIMO(r, (4.0 / 9.0) / (7.0 / 8.0), "risco_relativo: (4/9)/(7/8)");

    /* limites: b=0 e valido (RR=10); a=0 e valido (RR=0) */
    be_tabela2x2 rr_b0 = {20, 0, 10, 90};
    be_tabela2x2 rr_a0 = {0, 80, 10, 90};
    r = -9.0;
    st = be_risco_relativo(&rr_b0, &r);
    VERIFICA(st == BE_OK, "risco_relativo: b=0 devolve BE_OK (b nao esta no denominador)");
    VERIFICA_PROXIMO(r, 10.0, "risco_relativo: 1.0/0.1 = 10.0");
    st = be_risco_relativo(&rr_a0, &r);
    VERIFICA(st == BE_OK, "risco_relativo: a=0 devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.0, "risco_relativo: a=0 da RR 0");

    /* erros: saida intacta (sentinela -9.0) */
    be_tabela2x2 rr_c0 = {20, 80, 0, 100};
    r = -9.0;
    st = be_risco_relativo(&rr_c0, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "risco_relativo: c=0 (risco dos nao expostos nulo) devolve erro");
    VERIFICA_PROXIMO(r, -9.0, "risco_relativo: saida nao modificada em erro");
    st = be_risco_relativo(&exp_vazio, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "risco_relativo: expostos vazio devolve erro");
    st = be_risco_relativo(&nexp_vazio, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "risco_relativo: nao expostos vazio devolve erro");
    st = be_risco_relativo(&neg, &r);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "risco_relativo: tabela com negativo devolve erro");
    st = be_risco_relativo(NULL, &r);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "risco_relativo: tabela NULL devolve erro");
    st = be_risco_relativo(&ref, NULL);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "risco_relativo: saida NULL devolve erro");

    /* ---------- be_odds_ratio ---------- */

    r = -9.0;
    st = be_odds_ratio(&ref, &r);
    VERIFICA(st == BE_OK, "odds_ratio: tabela de referencia devolve BE_OK");
    VERIFICA_PROXIMO(r, 2.25, "odds_ratio: (20*90)/(80*10) = 2.25");

    r = -9.0;
    st = be_odds_ratio(&tx, &r);
    VERIFICA_PROXIMO(r, 4.0 / 35.0, "odds_ratio: (4*1)/(5*7)");

    /* limite: a=0 e valido (OR=0) */
    r = -9.0;
    st = be_odds_ratio(&rr_a0, &r);
    VERIFICA(st == BE_OK, "odds_ratio: a=0 devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.0, "odds_ratio: a=0 da OR 0");

    /* contagens grandes: a*d nao pode estourar (long) */
    be_tabela2x2 grande = {4000000000L, 1, 1, 4000000000L};
    r = -9.0;
    st = be_odds_ratio(&grande, &r);
    VERIFICA(st == BE_OK, "odds_ratio: contagens grandes devolve BE_OK");
    VERIFICA_PROXIMO(r, 1.6e19, "odds_ratio: contagens grandes sem overflow");

    /* erros: saida intacta (sentinela -9.0) */
    be_tabela2x2 or_b0 = {20, 0, 10, 90};
    be_tabela2x2 or_c0 = {20, 80, 0, 90};
    r = -9.0;
    st = be_odds_ratio(&or_b0, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "odds_ratio: b=0 devolve divisao por zero");
    VERIFICA_PROXIMO(r, -9.0, "odds_ratio: saida nao modificada em erro");
    st = be_odds_ratio(&or_c0, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "odds_ratio: c=0 devolve divisao por zero");
    st = be_odds_ratio(&neg, &r);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "odds_ratio: tabela com negativo devolve erro");
    st = be_odds_ratio(NULL, &r);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "odds_ratio: tabela NULL devolve erro");
    st = be_odds_ratio(&ref, NULL);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "odds_ratio: saida NULL devolve erro");

    /* ---------- be_diferenca_riscos ---------- */

    r = -9.0;
    st = be_diferenca_riscos(&ref, &r);
    VERIFICA(st == BE_OK, "diferenca_riscos: tabela de referencia devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.1, "diferenca_riscos: 0.2 - 0.1 = 0.1");

    /* DR negativa e valida (exposicao protetora) */
    r = -9.0;
    st = be_diferenca_riscos(&tx, &r);
    VERIFICA(st == BE_OK, "diferenca_riscos: DR negativa devolve BE_OK");
    VERIFICA_PROXIMO(r, 4.0 / 9.0 - 7.0 / 8.0, "diferenca_riscos: 4/9 - 7/8 (negativa)");

    /* limite: riscos iguais dao DR zero */
    be_tabela2x2 iguais = {10, 10, 10, 10};
    r = -9.0;
    st = be_diferenca_riscos(&iguais, &r);
    VERIFICA(st == BE_OK, "diferenca_riscos: riscos iguais devolve BE_OK");
    VERIFICA_PROXIMO(r, 0.0, "diferenca_riscos: riscos iguais da DR 0");

    /* erros: saida intacta (sentinela -9.0) */
    r = -9.0;
    st = be_diferenca_riscos(&exp_vazio, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "diferenca_riscos: expostos vazio devolve erro");
    VERIFICA_PROXIMO(r, -9.0, "diferenca_riscos: saida nao modificada em erro");
    st = be_diferenca_riscos(&nexp_vazio, &r);
    VERIFICA(st == BE_ERRO_DIVISAO_POR_ZERO, "diferenca_riscos: nao expostos vazio devolve erro");
    st = be_diferenca_riscos(&neg, &r);
    VERIFICA(st == BE_ERRO_VALOR_INVALIDO, "diferenca_riscos: tabela com negativo devolve erro");
    st = be_diferenca_riscos(NULL, &r);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "diferenca_riscos: tabela NULL devolve erro");
    st = be_diferenca_riscos(&ref, NULL);
    VERIFICA(st == BE_ERRO_PONTEIRO_NULO, "diferenca_riscos: saida NULL devolve erro");

    return RESULTADO_TESTES();
}
