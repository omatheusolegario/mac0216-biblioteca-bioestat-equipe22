#include "../include/diagnostico.h"
#include <stddef.h>

be_status be_sensibilidade(const be_tabela2x2 *t, double *sens)
{
    // Tratamento de erros
    if(t == NULL || sens == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->a + t->c) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    if(t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    // Calcula o resultado
    *sens = ((double) t->a)/(t->a + t->c);
    return BE_OK;
}

be_status be_especificidade(const be_tabela2x2 *t, double *esp)
{
    // Tratamento de erros
    if(t == NULL || esp == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->b + t->d) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    if(t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    // Calcula o resultado
    *esp = ((double) t->d)/(t->b + t->d);
    return BE_OK;
}

be_status be_vpp(const be_tabela2x2 *t, double *vpp)
{
    // Tratamento de erros
    if(t == NULL || vpp == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->a + t->b) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    if(t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    // Calcula o resultado
    *vpp = ((double) t->a)/(t->a + t->b);
    return BE_OK;
}

be_status be_vpn(const be_tabela2x2 *t, double *vpn)
{
    // Tratamento de erros
    if(t == NULL || vpn == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->c + t->d) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    if(t->a < 0 || t->b < 0 || t->c < 0 || t->d < 0) return BE_ERRO_VALOR_INVALIDO;

    // Calcula o resultado
    *vpn = ((double) t->d)/(t->c + t->d);
    return BE_OK;
}
