/** 
 *  @file epidemiologia.h
 *  @brief modulo das funçoes estatisticas de epidemiologia
 * 
 *  o modelo padrão de entrada para as funçoes 
 *  é a "tabela2x2", que tem o seguinte formato:
 *  
 * ---------------------  Doente  | Não doente
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
    long a; /**<Doente e Exposto/Teste+ */
    long b; /**<Não doente e Exposto/Teste+ */
    long c; /**<Doente e Não exposto/Teste- */
    long d; /**<Não doente e Não exposto/Teste- */
} be_tabela2x2;

#endif /* EPIDEMIOLOGIA_H */

