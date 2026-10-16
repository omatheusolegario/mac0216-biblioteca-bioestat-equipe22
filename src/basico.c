#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../include/basico.h"

// FUNÇÕES AUXILIARES =========================================================

// Função para comparação.
// Retorno:
// - Negativo se a < b
// - Zero se a = b
// - Positivo se a > b
static int comparar(const void *a, const void *b){
    double x = *(const double*)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

// Retorna uma cópia ordenada de dados.
static double *dados_ordenados(const double *dados, size_t n){
    double *c = malloc(n * sizeof *c);
    if (c == NULL)
        return NULL;
    // Copia dados para c.
    memcpy(c, dados, n * sizeof *c);
    // Ordena o vetor c (cópia), utilizando a função comparar
    qsort(c, n, sizeof *c, comparar);
    return c;
}

// Quantil de ordem p (entre 0 e 1) por interpolação linear.
// p = 0.25 para o primeiro quartil e p = 0.75 para o terceiro quartil.
static be_status quantil(const double *dados, size_t n, double p, double *resultado){
    if (dados == NULL || resultado == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (n == 0) return BE_ERRO_VALOR_INVALIDO;
    double *c = dados_ordenados(dados, n);
    if (c == NULL) return BE_ERRO_MEMORIA;
    double pos = p * (double)(n - 1); // Posição no vetor ordenado (base 0)
    size_t i = (size_t)pos; // Parte inteira
    double r = c[i];
    if (i + 1 < n)
        r += (pos - (double)i) * (c[i + 1] - c[i]); // Parte fracionária
    free(c);
    *resultado = r;
    return BE_OK;
}

// Tamanho da sequência de valores iguais a partir de c[i] (vetor c ordenado).
static size_t tam_sequencia(const double *c, size_t n, size_t i){
    size_t j = i + 1;
    while (j < n && c[j] == c[i])
        j++;
    return j - i;
}

// FUNÇÕES DO MÓDULO ============================================================

be_status be_media(const double *dados, size_t n, double *media){
    if (dados == NULL || media == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (n == 0) return BE_ERRO_VALOR_INVALIDO;
    double soma = 0.0;
    for (size_t i = 0; i < n; i++)
        soma += dados[i];
    *media = soma / (double)n;
    return BE_OK;
}

// Mediana = segundo quartil (p = 0.5 na função quantil).
be_status be_mediana(const double *dados, size_t n, double *mediana){
    return quantil(dados, n, 0.5, mediana);
}

be_status be_quartil_1(const double *dados, size_t n, double *q1){
    return quantil(dados, n, 0.25, q1);
}

be_status be_quartil_2(const double *dados, size_t n, double *q2){
    return quantil(dados, n, 0.5, q2);
}

be_status be_quartil_3(const double *dados, size_t n, double *q3){
    return quantil(dados, n, 0.75, q3);
}

be_status be_variancia(const double *dados, size_t n, double *var){
    if (dados == NULL || var == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (n == 0) return BE_ERRO_VALOR_INVALIDO;
    double m = be_media(dados, n, var); // Média é necessária
    double soma = 0.0;
    // (Xi - Média)^2
    for (size_t i = 0, i < n; i++)
        soma += (dados[i] - m) * (dados[i] - m);
    *var = soma / (double)n;
    return BE_OK;
}

be_status be_desvio_padrao(const double *dados, size_t n, double *dp){
    if (dados == NULL || dp == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (n == 0) return BE_ERRO_VALOR_INVALIDO;
    *dp = sqrt(be_variancia(dados, n, dp));
    return BE_OK;
}

be_status be_moda(const double *dados, size_t n, size_t *qtd, double **modas){
    if (dados == NULL || qtd == NULL || modas == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (n == 0) return BE_ERRO_VALOR_INVALIDO;

    double *c = dados_ordenados(dados, n);
    if (c == NULL) return BE_ERRO_MEMORIA;

    // Descobre maior_freq e qtde
    size_t maior_freq = 0; // Maior frequência
    size_t qtde = 0;       // Quantos valores distintos atingem a maior frequência
    size_t i = 0;
    while (i < n){
        size_t t = tam_sequencia(c, n, i);
        if (t > maior_freq){
            maior_freq = t;
            qtde = 1;
        } else if (t == maior_freq){
            qtde++;
        }
        i += t;
    }

    double *achadas = malloc(qtde * sizeof *achadas);
    if (achadas == NULL){
        free(c);
        return BE_ERRO_MEMORIA;
    }

    // Guarda cada valor que atinge a maior frequência uma única vez
    // Se nenhum valor se repete, maior_freq == 1 e todos os valores entram
    size_t k = 0;
    i = 0;
    while (i < n){
        size_t t = tam_sequencia(c, n, i);
        if (t == maior_freq)
            achadas[k++] = c[i];
        i += t;
    }
    free(c);
    *modas = achadas;
    *qtd = qtde;
    return BE_OK;
}
