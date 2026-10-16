#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "basico.h"
#include "teste.h"

typedef be_status (*funcao_numerica)(const double *, size_t, double *);

static const struct {
    const char *nome;
    funcao_numerica funcao;
} FUNCOES[] = {
    {"be_media", be_media},
    {"be_mediana", be_mediana},
    {"be_quartil_1", be_quartil_1},
    {"be_quartil_2", be_quartil_2},
    {"be_quartil_3", be_quartil_3},
    {"be_variancia", be_variancia},
    {"be_desvio_padrao", be_desvio_padrao},
};
#define N_FUNCOES (sizeof FUNCOES / sizeof FUNCOES[0])

// Chamas as sete funções de FUNCOES com os mesmos dados e confere cada uma.
// esperados segue a ordem de FUNCOES.
static void confere_todas(const char *caso, const double *dados, size_t n, const double *esperados){
    char desc[120];
    for (size_t i = 0; i < N_FUNCOES; i++){
        double r = NAN;
        be_status status = FUNCOES[i].funcao(dados, n, &r);
        // snprintf escreve em desc, VERIFICA dá print
        snprintf(desc, sizeof desc, "%s: %s devolve BE_OK", caso, FUNCOES[i].nome);
        VERIFICA(status == BE_OK, desc);
        snprintf(desc, sizeof desc, "%s: %s = %g", caso, FUNCOES[i].nome, esperados[i]);
        VERIFICA_PROXIMO(r, esperados[i], desc);
    }
}

// Chama be_moda e a confere.
// be_moda tem entradas diferente das demais.
static void confere_moda(const char *caso, const double *dados, size_t n, const double *esperadas, size_t qtd_esperada){
    char desc[120];
    size_t qtd = 0;
    double *modas = NULL;
    be_status status = be_moda(dados, n, &qtd, &modas);

    snprintf(desc, sizeof desc, "%s: be_moda devolve BE_OK", caso);
    VERIFICA(status == BE_OK, desc);
    snprintf(desc, sizeof desc, "%s: be_moda encontra %zu valor(es)", caso, qtd_esperada);
    VERIFICA(qtd == qtd_esperada, desc);

    if (status == BE_OK && qtd == qtd_esperada){
        for (size_t i = 0; i < qtd; i++){
            snprintf(desc, sizeof desc, "%s: moda[%zu] = %g", caso, i, esperadas[i]);
            VERIFICA_PROXIMO(modas[i], esperadas[i], desc);
        }
    }
    free(modas); // Seguro mesmo se for NULL.
}

static void teste_entrada_normal(void){
    printf("Entrada normal\n");

    // n par, dados desordenado de propósito
    double dados[] = {9, 2, 5, 4, 7, 4, 5, 4};
    double esperados[] = {5.0, 4.5, 4.0, 4.5, 5.5, 4.0, 2.0};
    confere_todas("n par", dados, 8, esperados);

    double moda[] = {4.0};
    confere_moda("n par", dados, 8, moda, 1);

    VERIFICA(dados[0] == 9 && dados[1] == 2 && dados[7] == 4, "os dados de entrada não foram alterados");

    // n ímpar
    double impar[] = {1, 2, 3, 4, 5};
    double esperados_impar[] = {3.0, 3.0, 2.0, 3.0, 4.0, 2.0, sqrt(2.0)};
    confere_todas("n impar", impar, 5, esperados_impar);

    // valores negativos
    double negativos[] = {-5, -1, -3};
    double esperados_neg[] = {-3.0, -3.0, -4.0, -3.0, -2.0, 8.0 / 3.0, sqrt(8.0 / 3.0)};
    confere_todas("negativos", negativos, 3, esperados_neg);

}

static void teste_casos_limite(void){
    printf("Casos limite\n");

    // somente um elemento
    double um[] = {7};
    double esperados_um[] = {7.0, 7.0, 7.0, 7.0, 7.0, 0.0, 0.0};
    confere_todas("n = 1", um, 1, esperados_um);
    confere_moda("n = 1", um, 1, um, 1);

    // todos os valores iguais
    double iguais[] = {7, 7, 7, 7};
    double esperados_iguais[] = {7.0, 7.0, 7.0, 7.0, 7.0, 0.0, 0.0};
    confere_todas("todos iguais", iguais, 4, esperados_iguais);
    confere_moda("todos iguais", iguais, 4, um, 1);

    // dois elementos
    double dois[] = {10, 20};
    double esperados_dois[] = {15.0, 15.0, 12.5, 15.0, 17.5, 25.0, 5.0};
    confere_todas("n = 2", dois, 2, esperados_dois);
    confere_moda("n = 2 sem repeticao", dois, 2, dois, 2)

    // moda com empate e moda sem repetição em dados desordenados
    double empate[] = {1, 1, 2, 2, 3};
    double modas_empate[] = {1, 2};
    confere_moda("empate", empate, 5, modas_empate, 2);

    double sem_repeticao[] = {3, 1, 2};
    double modas_sem_rep[] = {1, 2, 3};
    confere_moda("sem repeticao", sem_repeticao, 3, modas_sem_rep, 3);

    // vetor grande
    double grande[1000];
    for (size_t i = 0; i < 1000; i++)
        grande[i] = (double)(i + 1);
    double esperados_grande[] = {500.5, 500.5, 250.75, 500.5, 750.25, 83333.25, sqrt(83333.25)};
    confere_todas("n = 1000", grande, 1000, esperados_grande);
}

static void teste_ponteiro_nulo(void){
    printf("Entrada com ponteiro nulo\n");

    double dados[] = {1, 2, 3, 4};
    size_t n = 4;
    char desc[120];

    // FUNCOES
    for (size_t i = 0; i < N_FUNCOES; i++){
        double r = -1.0; // Valor "sentinela": deve continuar intacto.

        snprintf(desc, sizeof desc, "%s(NULL, ...) devolve BE_ERRO_PONTEIRO_NULO", FUNCOES[i].nome);
        VERIFICA(FUNCOES[i].funcao(NULL, n, &r) == BE_ERRO_PONTEIRO_NULO, desc);

        snprintf(desc, sizeof desc, "%s(..., NULL) devolve BE_ERRO_PONTEIRO_NULO", FUNCOES[i].nome);
        VERIFICA(FUNCOES[i].funcao(dados, n, NULL) == BE_ERRO_PONTEIRO_NULO, desc);

        snprintf(desc, sizeof desc, "%s não altera a saída em caso de erro", FUNCOES[i].nome);
        VERIFICA(r == -1.0, desc);
    }

    // MODA
    size_t qtd = 7;
    double *modas = NULL;
    VERIFICA(be_moda(NULL, n, &qtd, &modas) == BE_ERRO_PONTEIRO_NULO, "be_moda com dados NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(be_moda(dados, n, NULL, &modas) == BE_ERRO_PONTEIRO_NULO, "be_moda com qtd NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(be_moda(dados, n, &qtd, NULL) == BE_ERRO_PONTEIRO_NULO, "be_moda com modas NULL devolve BE_ERRO_PONTEIRO_NULO");
    VERIFICA(qtd == 7 && modas == NULL, "be_moda não altera as saídas em caso de erro");
}

static void teste_valor_invalido(void){
    printf("Entrada com valor invalido\n");

    double dados[] = {1, 2, 3, 4};
    char desc[120];

    // FUNCOES
    for (size_t i = 0; i < N_FUNCOES; i++){
        double r = -1.0; // Valor "sentinela": deve continuar intacto.

        snprintf(desc, sizeof desc, "%s com n = 0 devolve BE_ERRO_VALOR_INVALIDO", FUNCOES[i].nome);
        VERIFICA(FUNCOES[i].funcao(dados, 0, &r) == BE_ERRO_VALOR_INVALIDO, desc);

        snprintf(desc, sizeof desc, "%s não altera a saída em caso de erro", FUNCOES[i].nome);
        VERIFICA(r == -1.0, desc);
    }

    // MODA
    size_t qtd = 7;
    double *modas = NULL;
    VERIFICA(be_moda(dados, 0, &qtd, &modas) == BE_ERRO_VALOR_INVALIDO, "be_moda com n = 0 devolve BE_ERRO_VALOR_INVALIDO");
    VERIFICA(qtd == 7 && modas == NULL, "be_moda não altera as saídas em caso de erro");
}

int main(void){
    teste_entrada_normal();
    teste_casos_limite();
    teste_ponteiro_nulo();
    teste_valor_invalido();
    return RESULTADO_TESTES();
}
