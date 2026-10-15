/**
 * @file epidemiologia.h
 * @brief Módulo 3 — medidas epidemiológicas sobre tabelas 2x2.
 *
 * Convenção da tabela (linhas = exposição ou teste; colunas = doença):
 *
 *                     Doente    Não doente
 *   Exposto / Teste+    a           b
 *   Não exp. / Teste-   c           d
 *
 * Todas as funções seguem as convenções de bioestat_erro.h.
 */
#ifndef EPIDEMIOLOGIA_H
#define EPIDEMIOLOGIA_H

#include "bioestat_erro.h"

/** Tabela de contingência 2x2 (contagens de indivíduos). */
typedef struct {
    long a; /**< Expostos (ou teste+) doentes. */
    long b; /**< Expostos (ou teste+) não doentes. */
    long c; /**< Não expostos (ou teste-) doentes. */
    long d; /**< Não expostos (ou teste-) não doentes. */
} be_tabela2x2;

/**
 * @brief Cria uma tabela 2x2 validando as contagens.
 * @param a,b,c,d Contagens das células (devem ser >= 0).
 * @param saida   Onde a tabela será gravada.
 * @return BE_OK; BE_ERRO_PONTEIRO_NULO se saida == NULL;
 *         BE_ERRO_VALOR_INVALIDO se alguma contagem for negativa.
 */
be_status be_tabela_criar(long a, long b, long c, long d, be_tabela2x2 *saida);

/**
 * @brief Risco relativo: RR = [a/(a+b)] / [c/(c+d)].
 * @return BE_OK; BE_ERRO_PONTEIRO_NULO; BE_ERRO_DIVISAO_POR_ZERO se a+b == 0,
 *         c+d == 0 ou c == 0 (risco dos não expostos nulo).
 */
be_status be_risco_relativo(const be_tabela2x2 *t, double *rr);

/**
 * @brief Odds ratio: OR = (a*d) / (b*c).
 * @return BE_OK; BE_ERRO_PONTEIRO_NULO; BE_ERRO_DIVISAO_POR_ZERO se b*c == 0.
 */
be_status be_odds_ratio(const be_tabela2x2 *t, double *or_);

/**
 * @brief Sensibilidade de um teste diagnóstico: a / (a+c).
 * @return BE_OK; BE_ERRO_PONTEIRO_NULO; BE_ERRO_DIVISAO_POR_ZERO se a+c == 0.
 */
be_status be_sensibilidade(const be_tabela2x2 *t, double *sens);

/**
 * @brief Especificidade de um teste diagnóstico: d / (b+d).
 * @return BE_OK; BE_ERRO_PONTEIRO_NULO; BE_ERRO_DIVISAO_POR_ZERO se b+d == 0.
 */
be_status be_especificidade(const be_tabela2x2 *t, double *espec);

#endif /* EPIDEMIOLOGIA_H */
