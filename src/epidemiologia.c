#include "epidemiologia.h"

/*
 * ESQUELETO: as funções abaixo ainda não calculam nada.
 * Elas existem para que tudo compile e os testes rodem (e falhem!).
 * Implementar cada uma e ver o teste correspondente passar é o
 * "teste que falha inicialmente" exigido pelo enunciado.
 */

be_status be_tabela_criar(long a, long b, long c, long d, be_tabela2x2 *saida)
{
    (void)a; (void)b; (void)c; (void)d; (void)saida;
    return BE_ERRO_NAO_IMPLEMENTADO;
}

be_status be_risco_relativo(const be_tabela2x2 *t, double *rr)
{
    (void)t; (void)rr;
    return BE_ERRO_NAO_IMPLEMENTADO;
}

be_status be_odds_ratio(const be_tabela2x2 *t, double *or_)
{
    (void)t; (void)or_;
    return BE_ERRO_NAO_IMPLEMENTADO;
}

be_status be_sensibilidade(const be_tabela2x2 *t, double *sens)
{
    (void)t; (void)sens;
    return BE_ERRO_NAO_IMPLEMENTADO;
}

be_status be_especificidade(const be_tabela2x2 *t, double *espec)
{
    (void)t; (void)espec;
    return BE_ERRO_NAO_IMPLEMENTADO;
}
