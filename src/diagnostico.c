#include "../include/diagnostico.h"
#include <stddef.h>

be_status be_sensibilidade(const be_tabela2x2 *t, double *sens)
{
    if(t == NULL || sens == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->a + t->c) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    *sens = ((double) t->a)/(t->a + t->c);
    return BE_OK;
}

be_status be_especificidade(const be_tabela2x2 *t, double *esp)
{
    if(t == NULL || esp == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->b + t->d) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    *esp = ((double) t->d)/(t->b + t->d);
    return BE_OK;
}

be_status be_vpp(const be_tabela2x2 *t, double *vpp)
{
    if(t == NULL || vpp == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->a + t->b) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    *vpp = ((double) t->a)/(t->a + t->b);
    return BE_OK;
}

be_status be_vpn(const be_tabela2x2 *t, double *vpn)
{
    if(t == NULL || vpn == NULL) return BE_ERRO_PONTEIRO_NULO;
    if((t->c + t->d) == 0) return BE_ERRO_DIVISAO_POR_ZERO;
    *vpn = ((double) t->d)/(t->c + t->d);
    return BE_OK;
}
