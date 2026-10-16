#include <math.h>
#include <stdio.h>
#include "relacoes.h"
#include "teste.h"

static void teste_entrada_normal(void){
    printf("Entrada normal\n");

    // Dados não lineares
    double dados_x[] = {1.0, 2.0, 3.0};
    double dados_y[] = {1.0, 2.0, 4.0};
    double cov = NAN;
    double corr = NAN;
    rel_reta reta = {NAN, NAN, NAN};
    double previsto = NAN;

    VERIFICA(be_covariancia(dados_x, dados_y, 3, &cov) == BE_OK, "be_covariancia devolve BE_OK");
    VERIFICA_PROXIMO(cov, 1.0, "covariância = 1");

    VERIFICA(be_correlacao(dados_x, dados_y, 3, &corr) == BE_OK, "be_correlacao devolve BE_OK");
    VERIFICA_PROXIMO(corr, sqrt(27.0 / 28.0), "correlação = sqrt(27/28)");

    VERIFICA(be_regressao_linear(dados_x, dados_y, 3, &reta) == BE_OK, "be_regressao_linear devolve BE_OK");
    VERIFICA_PROXIMO(reta.inclinacao, 1.5, "inclinação = 1,5");
    VERIFICA_PROXIMO(reta.intercepta, -2.0 / 3.0, "intercepto = -2/3");
    VERIFICA_PROXIMO(reta.r2, 27.0 / 28.0, "r^2 = 27/28");

    VERIFICA(be_prever(&reta, 4.0, &previsto) == BE_OK, "be_prever devolve BE_OK");
    VERIFICA_PROXIMO(previsto, 16.0 / 3.0, "previsão para x = 4 é 16/3");
}

static void teste_casos_limite(void){
    printf("Casos limite\n");

    // Apenas um elemento: covariância é zero / vetores são constantes
    double dados_x[] = {5.0};
    double dados_y[] = {9.0};
    double cov = NAN;
    double corr = -10.0;
    rel_reta reta = {11.0, 12.0, 13.0};

    VERIFICA(be_covariancia(dados_x, dados_y, 1, &cov) == BE_OK, "be_covariancia aceita n = 1");
    VERIFICA_PROXIMO(cov, 0.0, "covariância com n = 1 é zero");

    VERIFICA(be_correlacao(dados_x, dados_y, 1, &corr) == BE_ERRO_DIVISAO_POR_ZERO, "be_correlacao com n = 1 devolve BE_ERRO_DIVISAO_POR_ZERO");
    VERIFICA(corr == -10.0, "be_correlacao não altera a saída em caso de erro");

    VERIFICA(be_regressao_linear(dados_x, dados_y, 1, &reta) == BE_ERRO_DIVISAO_POR_ZERO, "be_regressao_linear com n = 1 devolve BE_ERRO_DIVISAO_POR_ZERO");
    VERIFICA(reta.intercepta == 11.0 && reta.inclinacao == 12.0 && reta.r2 == 13.0, "be_regressao_linear não altera a reta em caso de erro");

    // x = 0 deve retornar o intercepto da reta.
    rel_reta reta_simples = {7.0, 2.0, 1.0};
    double previsto = NAN;
    VERIFICA(be_prever(&reta_simples, 0.0, &previsto) == BE_OK, "be_prever aceita x = 0");
    VERIFICA_PROXIMO(previsto, 7.0, "previsão para x = 0 é o intercepto");
}

static void teste_ponteiro_nulo(void){
    printf("Entrada com ponteiro nulo\n");

    double dados_x[] = {1.0, 2.0, 3.0};
    double dados_y[] = {2.0, 4.0, 6.0};
    double saida = 123.0;
    rel_reta reta = {11.0, 12.0, 13.0};

    // Covariância
    VERIFICA(be_covariancia(NULL, dados_y, 3, &saida) == BE_ERRO_PONTEIRO_NULO, "be_covariancia com dados_x NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(saida == 123.0, "be_covariancia não altera a saída se dados_x for NULL");
    VERIFICA(be_covariancia(dados_x, NULL, 3, &saida) == BE_ERRO_PONTEIRO_NULO, "be_covariancia com dados_y NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(be_covariancia(dados_x, dados_y, 3, NULL) == BE_ERRO_PONTEIRO_NULO, "be_covariancia com cov NULL devolve BE_ERRO_PONTEIRO_NULO");

    // Correlação
    VERIFICA(be_correlacao(NULL, dados_y, 3, &saida) == BE_ERRO_PONTEIRO_NULO, "be_correlacao com dados_x NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(saida == 123.0, "be_correlacao não altera a saída se dados_x for NULL");
    VERIFICA(be_correlacao(dados_x, NULL, 3, &saida) == BE_ERRO_PONTEIRO_NULO, "be_correlacao com dados_y NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(be_correlacao(dados_x, dados_y, 3, NULL) == BE_ERRO_PONTEIRO_NULO, "be_correlacao com corr NULL devolve BE_ERRO_PONTEIRO_NULO");

    // Regressão
    VERIFICA(be_regressao_linear(NULL, dados_y, 3, &reta) == BE_ERRO_PONTEIRO_NULO, "be_regressao_linear com dados_x NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(reta.intercepta == 11.0 && reta.inclinacao == 12.0 && reta.r2 == 13.0, "be_regressao_linear preserva a reta se dados_x for NULL");
    VERIFICA(be_regressao_linear(dados_x, NULL, 3, &reta) == BE_ERRO_PONTEIRO_NULO, "be_regressao_linear com dados_y NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(be_regressao_linear(dados_x, dados_y, 3, NULL) == BE_ERRO_PONTEIRO_NULO, "be_regressao_linear com reta NULL devolve BE_ERRO_PONTEIRO_NULO");

    // Previsão
    VERIFICA(be_prever(NULL, 2.0, &saida) == BE_ERRO_PONTEIRO_NULO, "be_prever com reta NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(saida == 123.0, "be_prever não altera a saída se reta for NULL");
    VERIFICA(be_prever(&reta, 2.0, NULL) == BE_ERRO_PONTEIRO_NULO, "be_prever com y_previsto NULL devolve BE_ERRO_PONTEIRO_NULO");
}

static void teste_valor_invalido(void){
    printf("Entrada com valor inválido\n");

    double dados_x[] = {1.0, 2.0, 3.0};
    double dados_y[] = {2.0, 4.0, 6.0};
    double cov = -1.0;
    double corr = -2.0;
    rel_reta reta = {11.0, 12.0, 13.0};

    VERIFICA(be_covariancia(dados_x, dados_y, 0, &cov) == BE_ERRO_VALOR_INVALIDO, "be_covariancia com n = 0 devolve BE_ERRO_VALOR_INVALIDO");
    VERIFICA(cov == -1.0, "be_covariancia preserva a saída com n = 0");

    VERIFICA(be_correlacao(dados_x, dados_y, 0, &corr) == BE_ERRO_VALOR_INVALIDO, "be_correlacao com n = 0 devolve BE_ERRO_VALOR_INVALIDO");
    VERIFICA(corr == -2.0, "be_correlacao preserva a saída com n = 0");

    VERIFICA(be_regressao_linear(dados_x, dados_y, 0, &reta) == BE_ERRO_VALOR_INVALIDO, "be_regressao_linear com n = 0 devolve BE_ERRO_VALOR_INVALIDO");
    VERIFICA(reta.intercepta == 11.0 && reta.inclinacao == 12.0 && reta.r2 == 13.0, "be_regressao_linear preserva a reta com n = 0");

    double previsto = 456.0;
    rel_reta reta_valida = {1.0, 2.0, 1.0};
    VERIFICA(be_prever(&reta_valida, NAN, &previsto) == BE_ERRO_VALOR_INVALIDO, "be_prever com x = NAN devolve BE_ERRO_VALOR_INVALIDO");
    VERIFICA(previsto == 456.0, "be_prever preserva a saída com x inválido");
}

int main(void){
    teste_entrada_normal();
    teste_casos_limite();
    teste_ponteiro_nulo();
    teste_valor_invalido();
    return RESULTADO_TESTES();
}
