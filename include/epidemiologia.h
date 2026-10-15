/**
 *  @file epidemiologia.h
 *  @brief módulo das funçoes estatisticas de epidemiologia
 * 
 *  o modelo padrão de entrada para as funções 
 *  é a "tabela2x2", que tem o seguinte formato:
 *  
 *                       | Doente  | Não doente
 *  Exposto/Teste+       |   a     |     b
 *  Não Exposto/Teste-   |   c     |     d
 * 
 *  
 */
#ifndef EPIDEMIOLOGIA_H
#define EPIDEMIOLOGIA_H

#include "bioestat_erro.h"

/** Tabela 2x2 (dupla entrada) usada pelas funções de bioestatística */
typedef struct {
    long a; /**< Doente e Exposto/Teste+ */
    long b; /**< Não doente e Exposto/Teste+ */
    long c; /**< Doente e Não exposto/Teste- */
    long d; /**< Não doente e Não exposto/Teste- */
} be_tabela2x2;


/** @brief preenche uma tabela2x2, para ser usada nas análises estatisticas
 *  
 *  @param a,b,c,d contagem das células da tabela (apenas inteiros não negativos)
 *                       | Doente  | Não doente
 *  Exposto/Teste+       |   a     |     b
 *  Não Exposto/Teste-   |   c     |     d
 * 
 * @param saida é o ponteiro para a tabela, onde os parametros serão gravados
 * 
 * @return BE_OK em caso de sucesso
 *         BE_ERRO_PONTEIRO_NULO se a saída for NULL
 *         BE_ERRO_VALOR_INVALIDO se qualquer parametro a, b, c ou d for < 0
 */
be_status be_preencher_tabela(long a, long b, long c, long d, be_tabela2x2 *saida);


/**
 * @brief calcula a probabilidade de adoecer entre os expostos (incidencia cumulativa dos expostos)
 * 
 * Fórmula: a / (a + b)
 * 
 * @param t é a tabela 2x2 (dupla entrada) no formato a,b,c,d (não aceita valores negativos)
 * @param saida é um ponteiro para um double, em que ficará gravada a probabilidade
 * 
 * @return BE_OK em caso de sucesso
 *         BE_ERRO_PONTEIRO_NULO se a saida ou t for NULL
 *         BE_ERRO_VALOR_INVALIDO se a tabela conter valores negativos
 *         BE_ERRO_DIVISAO_POR_ZERO se houver divisão por 0 (a + b == 0)
 * 
 */
be_status be_risco_expostos(const be_tabela2x2 *t, double *saida);


/**
 * @brief calcula a probabilidade de adoecer entre os não expostos (incidencia cumulativa dos não expostos)
 * 
 * Fórmula: c / (c + d)
 * 
 * @param t é a tabela 2x2 (dupla entrada) no formato a,b,c,d (não aceita valores negativos)
 * @param saida é um ponteiro para um double, em que ficará gravada a probabilidade
 * 
 * @return BE_OK em caso de sucesso
 *         BE_ERRO_PONTEIRO_NULO se a saida ou t for NULL
 *         BE_ERRO_VALOR_INVALIDO se a tabela conter valores negativos
 *         BE_ERRO_DIVISAO_POR_ZERO se houver divisão por 0 (c + d == 0)
 * 
 */
be_status be_risco_naoexpostos(const be_tabela2x2 *t, double *saida);


/**
 * @brief compara ambos os riscos por divisão 
 * 
 * Formula: risco_expostos / risco_naoexpostos
 * 
 * Leitura: se RR = 2, expostos tem o dobro do risco. RR = 1 significa sem associação
 *          RR > 1 exposição aumenta o risco, RR < 1 a exposição protege. 
 * 
 * @param t é a tabela 2x2 (dupla entrada) no formato a,b,c,d (não aceita valores negativos)
 * @param saida é um ponteiro para um double, em que ficará gravado o valor
 * 
 * @return BE_OK em caso de sucesso
 *         BE_ERRO_PONTEIRO_NULO se a saida ou t for NULL
 *         BE_ERRO_VALOR_INVALIDO se a tabela conter valores negativos
 *         BE_ERRO_DIVISAO_POR_ZERO se houver divisão por 0 (se a + b == 0 ou c + d == 0 ou c == 0)
 */
be_status be_risco_relativo(const be_tabela2x2 *t, double *saida);


/**
 * @brief odds ratio (OR) é a razão entre quem adoece e não adoece no grupo
 * 
 * Formula: Odds dos expostos = a / b
 *          Odds dos não expostos = c / d
 *          OR = (a/b) / (c/d) = (a.d) / (b.c)
 * 
 * @param t é a tabela 2x2 (dupla entrada) no formato a,b,c,d (não aceita valores negativos)
 * @param saida é um ponteiro para um double, em que ficará gravado o valor
 * 
 * @return BE_OK em caso de sucesso
 *         BE_ERRO_PONTEIRO_NULO se a saida ou t for NULL
 *         BE_ERRO_VALOR_INVALIDO se a tabela conter valores negativos
 *         BE_ERRO_DIVISAO_POR_ZERO se houver divisão por 0 ((b.c) = 0)
 */
be_status be_odds_ratio(const be_tabela2x2 *t, double *saida);


/**
 * @brief diferença de riscos (DR) compara os riscos por subtração 
 * 
 * Fórmula: DR = risco_expostos - risco_naoexpostos
 * 
 * Leitura: DR = 0,10 -> a exposição está associada a 10 casos a mais a cada 100 pessoas. 
 * O RR diz o quanto proporcionalmente, o DR diz o quanto em casos reais.
 * 
 * @param t é a tabela 2x2 (dupla entrada) no formato a,b,c,d (não aceita valores negativos)
 * @param saida é um ponteiro para um double, em que ficará gravado o valor
 * 
 * @return BE_OK em caso de sucesso
 *         BE_ERRO_PONTEIRO_NULO se a saida ou t for NULL
 *         BE_ERRO_VALOR_INVALIDO se a tabela conter valores negativos
 *         BE_ERRO_DIVISAO_POR_ZERO se (a + b == 0 ou c + d == 0)
 */
be_status be_diferenca_riscos(const be_tabela2x2 *t, double *saida);

#endif /* EPIDEMIOLOGIA_H */

