/**
 * @file diagnostico.h
 * @brief Testes diagnósticos.
 * Usa a tabela de contingência 2x2 definida em epidemiologia.h
 * @verbatim
Convenção da tabela (linhas = exposição ou teste; colunas = doença):
                    Doente    Não doente
Exposto / Teste+    a           b
Não exp. / Teste-   c           d @endverbatim
 *
 * @author Matheus Moreira Cabral
 */

#ifndef DIAGNOSTICO_H
#define DIAGNOSTICO_H

#include "bioestat_erro.h"
#include "epidemiologia.h"

/**
 * @brief Capacidade do teste de identificar corretamente quem tem a doença
 * @param t Tabela de contingência do teste
 * @param sens Ponteiro para onde o valor da sensibilidade será escrito
 * @pre a+c != 0
 * @return BE_OK caso não ocorra erros, 
 * BE_ERRO_PONTEIRO_NULO se t ou sens forem nulos
 * BE_ERRO_DIVISAO_POR_ZERO se a+c == 0
 * @post sens aponta para o valor da sensibilidade a/(a+c)
 */
be_status be_sensibilidade(const be_tabela2x2 *t, double *sens);

/**
 * @brief Capacidade do teste de identificar corretamente quem não tem a doença
 * @param t Tabela de contingência do teste
 * @param esp Ponteiro para onde o valor da especificidade será escrito
 * @pre b+d != 0
 * @return BE_OK caso não ocorra erros, 
 * BE_ERRO_PONTEIRO_NULO se t ou esp forem nulos
 * BE_ERRO_DIVISAO_POR_ZERO se b+d == 0
 * @post esp aponta para o valor da especificidade d/(b+d)
 */
be_status be_especificidade(const be_tabela2x2 *t, double *esp);

/**
 * @brief Probabilidade da pessoa realmente ter a doença quando o resultado é positivo
 * @param t Tabela de contingência do teste
 * @param vpp Ponteiro para onde o valor preditivo positivo será escrito
 * @pre a+b != 0
 * @return BE_OK caso não ocorra erros, 
 * BE_ERRO_PONTEIRO_NULO se t ou esp forem nulos
 * BE_ERRO_DIVISAO_POR_ZERO se a+b == 0
 * @post vpp aponta para o valor preditivo positivo a/(a+b)
 */
be_status be_vpp(const be_tabela2x2 *t, double *vpp);

/**
 * @brief Probabilidade da pessoa realmente não ter a doença quando o resultado é negativo
 * @param t Tabela de contingência do teste
 * @param vpn Ponteiro para onde o valor preditivo negativo será escrito
 * @pre c+d != 0
 * @return BE_OK caso não ocorra erros, 
 * BE_ERRO_PONTEIRO_NULO se t ou esp forem nulos
 * BE_ERRO_DIVISAO_POR_ZERO se c+d == 0
 * @post vpn aponta para o valor preditivo negativo d/(c+d)
 */
be_status be_vpn(const be_tabela2x2 *t, double *vpn);

#endif
