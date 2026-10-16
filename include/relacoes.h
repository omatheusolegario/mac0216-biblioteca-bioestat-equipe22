/**
 * @file relacoes.h
 * @brief Módulo de relações entre variáveis: covariância, correlação linear e regressão linear simples.
 */

#ifndef RELACOES_H
#define RELACOES_H

#include <stddef.h>
#include "bioestat_erro.h"

/**
 * @brief Resultado de uma regressão linear simples.
 * @details Descreve a reta y = inclinação * x + intercepta
 * r2 mede quanto da variação de y a reta explica (1 = ajuste perfeito; O modelo explica toda a variabilidade dos dados)
 * r2 = correlação ao quadrado
 */
typedef struct{
    double intercepta; /**< Valor de y quando x = 0. */
    double inclinacao; /**< Variação de y a cada unidade de x. */
    double r2;         /**< Coeficiente de determinação. De 0 a 1. */
} rel_reta;

/**
 * @brief Covariância
 *
 * @param[in] dados_x Valores da primeira variável.
 * @param[in] dados_y Valores da segunda variável.
 * @param[in] n Quantidade de elementos de cada vetor.
 * @param[out] cov Recebe a covariância.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados_x, dados_y ou cov for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_covariancia(const double *dados_x, const double *dados_y, size_t n, double *cov);

/**
 * @brief Correlação linear de Pearson
 * @details Covariância dividida pelo produto dos desvios padrão.
 *
 * @param[in] dados_x Valores da primeira variável.
 * @param[in] dados_y Valores da segunda variável.
 * @param[in] n Quantidade de elementos de cada vetor.
 * @param[out] corr Recebe a correlação.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados_x, dados_y ou corr for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0,
 *                   BE_ERRO_DIVISAO_POR_ZERO se x ou y é constante (todos os valores iguais): desvio padrão é zero.
 */
be_status be_correlacao(const double *dados_x, const double *dados_y, size_t n, double *corr);

/**
 * @brief Regressão linear simples
 *
 * @param[in] dados_x Valores da primeira variável.
 * @param[in] dados_y Valores da segunda variável.
 * @param[in] n Quantidade de elementos de cada vetor.
 * @param[out] reta Recebe a reta ajustada.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados_x, dados_y ou reta for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0,
 *                   BE_ERRO_DIVISAO_POR_ZERO se x ou y é constante (todos os valores iguais): a inclinação (x) ou o r2 (y) dividiria por zero.
 */
be_status be_regressao_linear(const double *dados_x, const double *dados_y, size_t n, rel_reta *reta);

/**
 * @brief Calcula o valor de y previsto por uma reta de regressão.
 *
 * @param[in] reta Reta obtida com a regressão linear.
 * @param[in] x Valor de x para o qual se quer a previsão.
 * @param[out] y_previsto Recebe o y previsto.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se reta ou y_previsto for nulo,
 */
be_status be_prever(const rel_reta *reta, double x, double *y_previsto);

#endif
