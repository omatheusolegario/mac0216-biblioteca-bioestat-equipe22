/**
 * @file basico.h
 * @brief Módulo de estatística básica.
 */

#ifndef BASICO_H
#define BASICO_H

#include "bioestat_erro.h"
#include <stddef.h>

/**
 * @brief Média aritmética.
 *
 * @details Soma todos os números e divide pela quantidade de números.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_media(const double *dados, size_t n, double *media);


/**
 * @brief Mediana.
 *
 * @details Mediana = Quartil 2.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_mediana(const double *dados, size_t n, double *mediana);


/**
 * @brief Variância.
 *
 * @details Divide-se a soma dos quadrados dos desvios pelo total de elementos.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_variancia(const double *dados, size_t n, double *var);


/**
 * @brief Desvio padrão.
 *
 * @details Raiz quadrada da variância.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_desvio_padrao(const double *dados, size_t n, double *dp);


/**
 * @brief Moda.
 *
 * @details Todos os valores de maior frequência, em ordem crescente.
 * Em caso de empate devolve todos os valores empatados.
 * Caso nenhum valor se repita devolve todos os valores.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 * @param[out] qtd Recebe a quantidade de valores do vetor devolvido.
 *
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados ou qtd for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status *be_moda(const double *dados, size_t n, size_t *qtd, double **modas);

/**
 * @brief Primeiro quartil.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_quartil_1(const double *dados, size_t n, double *q1);

/**
 * @brief Segundo quartil.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_quartil_2(const double *dados, size_t n, double *q2);

/**
 * @brief Terceiro quartil.
 *
 * @param[in] dados Vetor contendo os valores.
 * @param[in] n Tamanho do vetor dados.
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados for nulo,
 *                   BE_ERRO_VALOR_INVALIDO se n == 0.
 */
be_status be_quartil_3(const double *dados, size_t n, double *q3);

#endif
