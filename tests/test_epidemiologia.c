#include "epidemiologia.h"
#include "teste.h"

/* Tabela de referência: a=20, b=80, c=10, d=90
 *   RR   = (20/100)/(10/100) = 2.0
 *   OR   = (20*90)/(80*10)  = 2.25
 *   Sens = 20/30            = 0.666...
 *   Esp  = 90/170           = 0.529...
 */

static void testa_criacao(void)
{
    be_tabela2x2 t;
    VERIFICA(be_tabela_criar(20, 80, 10, 90, &t) == BE_OK, "criar tabela valida");
    VERIFICA(be_tabela_criar(-1, 80, 10, 90, &t) == BE_ERRO_VALOR_INVALIDO,
             "contagem negativa e rejeitada");
    VERIFICA(be_tabela_criar(1, 1, 1, 1, NULL) == BE_ERRO_PONTEIRO_NULO,
             "saida NULL e rejeitada");
}

static void testa_risco_relativo(void)
{
    be_tabela2x2 t = {20, 80, 10, 90};
    double rr = 0.0;
    VERIFICA(be_risco_relativo(&t, &rr) == BE_OK, "RR retorna BE_OK");
    VERIFICA_PROXIMO(rr, 2.0, "RR = 2.0");

    be_tabela2x2 sem_nao_expostos_doentes = {20, 80, 0, 90};
    VERIFICA(be_risco_relativo(&sem_nao_expostos_doentes, &rr) == BE_ERRO_DIVISAO_POR_ZERO,
             "RR com c = 0 sinaliza divisao por zero");
}

static void testa_odds_ratio(void)
{
    be_tabela2x2 t = {20, 80, 10, 90};
    double or_ = 0.0;
    VERIFICA(be_odds_ratio(&t, &or_) == BE_OK, "OR retorna BE_OK");
    VERIFICA_PROXIMO(or_, 2.25, "OR = 2.25");
}

static void testa_diagnostico(void)
{
    be_tabela2x2 t = {20, 80, 10, 90};
    double s = 0.0, e = 0.0;
    VERIFICA(be_sensibilidade(&t, &s) == BE_OK, "sensibilidade retorna BE_OK");
    VERIFICA_PROXIMO(s, 20.0 / 30.0, "sensibilidade = 20/30");
    VERIFICA(be_especificidade(&t, &e) == BE_OK, "especificidade retorna BE_OK");
    VERIFICA_PROXIMO(e, 90.0 / 170.0, "especificidade = 90/170");
    VERIFICA(be_sensibilidade(NULL, &s) == BE_ERRO_PONTEIRO_NULO, "tabela NULL rejeitada");
}

int main(void)
{
    printf("Modulo epidemiologia:\n");
    testa_criacao();
    testa_risco_relativo();
    testa_odds_ratio();
    testa_diagnostico();
    return RESULTADO_TESTES();
}
