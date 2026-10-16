#include <stddef.h>
#include "teste.h"
#include "diagnostico.h"
#include "epidemiologia.h"

int main()
{
    double resultado = 0;
    be_status status;
    be_tabela2x2 tabela;
    // testes de be_sensibilidade
    // valor normal
    tabela = (be_tabela2x2){1, 2, 3, 4};
    status = be_sensibilidade(&tabela, &resultado);
    VERIFICA(status == BE_OK, "sensibilidade: retorna BE_OK");
    VERIFICA_PROXIMO(resultado, 1.0 / (1.0 + 3.0), "sensibilidade: devolve o valor correto");
    // teste com valor 0
    tabela = (be_tabela2x2){0, 1, 0, 2};
    status = be_sensibilidade(&tabela, &resultado);
    VERIFICA(status == BE_ERRO_DIVISAO_POR_ZERO, "sensibilidade: retorna erro de divisão por zero");

    // testes de be_especificidade
    // valor normal
    tabela = (be_tabela2x2){5, 100, 3, 400};
    status = be_especificidade(&tabela, &resultado);
    VERIFICA(status == BE_OK, "especificidade: retorna BE_OK");
    VERIFICA_PROXIMO(resultado, 400.0 / (100.0 + 400.0), "especificidade: devolve o valor correto");
    // teste com valor negativo
    tabela = (be_tabela2x2){-3, 1415, 926535, 89793238};
    status = be_especificidade(&tabela, &resultado);
    VERIFICA(status == BE_ERRO_VALOR_INVALIDO, "especificidade: retorna erro de valor invalido");

    // testes de be_vpp
    // valor normal
    tabela = (be_tabela2x2){7777, 777, 77, 7};
    status = be_vpp(&tabela, &resultado);
    VERIFICA(status == BE_OK, "vpp: retorna BE_OK");
    VERIFICA_PROXIMO(resultado, 7777.0 / (7777.0 + 777.0), "vpp: devolve o valor correto");
    // teste com ponteiro nulo
    double *npt = NULL;
    status = be_vpp(&tabela, npt);
    VERIFICA(status == BE_ERRO_PONTEIRO_NULO, "vpp: retorna erro de ponteiro nulo");

    // testes de be_vpn
    // valor normal
    tabela = (be_tabela2x2){27, 1828, 1828, 459045};
    status = be_vpn(&tabela, &resultado);
    VERIFICA(status == BE_OK, "vpn: retorna BE_OK");
    VERIFICA_PROXIMO(resultado, 459045.0 / (1828 + 459045.0), "vpn: devolve o valor correto");
    // valor limite que ainda dá certo
    tabela = (be_tabela2x2){0, 0, 0, 1};
    status = be_vpn(&tabela, &resultado);
    VERIFICA(status == BE_OK, "vpn: retorna BE_OK");
    VERIFICA_PROXIMO(resultado, 1.0 / (0.0 + 1.0), "vpn: devolve o valor correto");

    return RESULTADO_TESTES();
}