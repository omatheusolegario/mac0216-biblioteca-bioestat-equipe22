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
static double quantil(const double *dados, size_t n, double p){
    if (dados == NULL || n == 0)
        return NAN;
    double *c = dados_ordenados(dados, n);
    if (c == NULL)
        return NAN;
    double pos = p * (double)(n - 1); // Posição no vetor ordenado (base 0)
    size_t i = (size_t)pos; // Parte inteira
    double r = c[i];
    if (i + 1 < n)
        r += (pos - (double)i) * (c[i + 1] - c[i]); // Parte fracionária
    free(c);
    return r;
}

// Tamanho da sequência de valores iguais a partir de c[i] (vetor c ordenado).
static size_t tam_sequencia(const double *c, size_t n, size_t i){
    size_t j = i + 1;
    while (j < n && c[j] == c[i])
        j++;
    return j - i;
}

// FUNÇÕES DO MÓDULO ============================================================

double media(const double *dados, size_t n){
    if (dados == NULL || n == 0)
        return NAN;
    double soma = 0.0;
    for (size_t i = 0; i < n; i++)
        soma += dados[i];
    return soma / (double)n;
}

// Mediana = segundo quartil (p = 0.5 na função quantil).
double mediana(const double *dados, size_t n){
    return quartil_2(dados, n);
}

double quartil_1(const double *dados, size_t n){
    return quantil(dados, n, 0.25);
}

double quartil_2(const double *dados, size_t n){
    return quantil(dados, n, 0.5);
}

double quartil_3(const double *dados, size_t n){
    return quantil(dados, n, 0.75);
}

double variancia(const double *dados, size_t n){
    if (dados == NULL || n == 0)
        return NAN;
    double m = media(dados, n); // Média é necessária
    double soma = 0.0;
    // (Xi - Média)^2
    for (size_t i = 0, i < n; i++)
        soma += (dados[i] - m) * (dados[i] - m);
    return soma / (double)n;
}

double desvio_padrao(const double *dados, size_t n){
    return sqrt(variancia(dados, n));
}

double *moda(const double *dados, size_t n, size_t *qtd){
    if (qtd == NULL)
        return NULL;
    *qtd = 0;
    if (dados == NULL || n == 0)
        return NULL;

    double *c = dados_ordenados(dados, n);
    if (c == NULL)
        return NULL;

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

    // Guarda cada valor que atinge a maior frequência uma única vez
    // Se nenhum valor se repete, maior_freq == 1 e todos os valores entram
    double *modas = malloc(qtde * sizeof *modas);
    if (modas != NULL){
        size_t k = 0;
        i = 0;
        while (i < n){
            size_t t = tam_sequencia(c, n, i);
            if (t == maior_freq)
                modas[k++] = c[i];
            i += t;
        }
        *qtd = qtde;
    }

    free(c);
    return modas;
}
