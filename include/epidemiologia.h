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
 *         BE_PONTEIRO_NULO se a saída for NULL
 *         BE_ERRO_VALOR_INVALIDO se qualquer parametro a, b, c ou d for < 0
 */
be_status be_criar_tabela(long a, long b, long c, long d, be_tabela2x2 *saida);
be_status be_risco_expostos(const be_tabela2x2 *t, double *saida);
be_status be_risco_naoexpostos(const be_tabela2x2 *t, double *saida);
be_status be_risco_relativo(const be_tabela2x2 *t, double *saida);
be_status be_odds_ratio(const be_tabela2x2 *t, double *saida);
be_status be_diferenca_riscos(const be_tabela2x2 *t, double *saida);

#endif /* EPIDEMIOLOGIA_H */

