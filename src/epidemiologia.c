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
