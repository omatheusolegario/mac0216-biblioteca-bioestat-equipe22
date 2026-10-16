#include <math.h>
#include <stdio.h>
#include "relacoes.h"
#include "basico.h"

// FUNÇÕES AUXILIARES ===========================================

typedef struct{
    double media_x;
    double media_y;
    double sxx; // soma de (x - media_x)^2
    double syy; // soma de (y - media_y)^2
    double sxy; // soma de (x - media_x) * (y - media_y)
} somas;

// Calcula as somas
static be_status calcular_somas(const double *dados_x, const double *dados_y, size_t n, somas *s){
    be_status status = be_media(dados_x, n, &s->media_x);
    if (status != BE_OK) return status;

    status = be_media(dados_y, n, &s->media_y);
    if (status != BE_OK) return status;

    s->sxx = 0.0;
    s->syy = 0.0;
    s->sxy = 0.0;

    for (size_t i = 0; i < n; i++){
        double dx = dados_x[i] - s->media_x;
        double dy = dados_y[i] - s->media_y;
        s->sxx += dx * dx;
        s->syy += dy * dy;
        s->sxy += dx * dy;
    }
    return BE_OK;
}

// Verifica se todos os valores de um vetor são iguais.
// Retorna 1 se são todos iguais. Retorna 0 caso contrário.
static int constante(const double *dados, size_t n){
    for (size_t i = 1; i < n; i++)
        if (dados[i] != dados[0])
            return 0;
    return 1;
}

// Correlação = sxy / sqrt(sxx * syy)
// Se x ou y são constantes há uma divisão por zero.
static be_status calcular_corr(const double *dados_x, const double *dados_y, size_t n, const somas *s, double *r){
    double denominador = s->sxx * s->syy;
    if (constante(dados_x, n) || constante(dados_y, n) || denominador == 0.0) return BE_ERRO_DIVISAO_POR_ZERO;

    // Certificando que a correlação está no intervalo [-1, 1] devido a arredondamentos
    double valor = s->sxy / sqrt(denominador);
    if (valor > 1.0) valor = 1.0;
    if (valor < -1.0) valor = -1.0;
    *r = valor;
    return BE_OK;
}


// FUNÇÕES DO MÓDULO ==================================================

be_status be_covariancia(const double *dados_x, const double *dados_y, size_t n, double *cov){
    if (cov == NULL) return BE_ERRO_PONTEIRO_NULO;

    somas s;
    be_status status = calcular_somas(dados_x, dados_y, n, &s);
    if (status != BE_OK) return status;

    *cov = s.sxy / (double)n;
    return BE_OK;
}

be_status be_correlacao(const double *dados_x, const double *dados_y, size_t n, double *corr){
    if (corr == NULL) return BE_ERRO_PONTEIRO_NULO;

    somas s;
    be_status status = calcular_somas(dados_x, dados_y, n, &s);
    if (status != BE_OK) return status;

    return calcular_corr(dados_x, dados_y, n, &s, corr);
}

be_status be_regressao_linear(const double *dados_x, const double *dados_y, size_t n, rel_reta *reta){
    if (reta == NULL) return BE_ERRO_PONTEIRO_NULO;

    somas s;
    be_status status = calcular_somas(dados_x, dados_y, n, &s);
    if (status != BE_OK) return status;

    double corr;
    status = calcular_corr(dados_x, dados_y, n, &s, &r);
    if (status != BE_OK) return status;

    // É garantido que sxx != 0 por conta de calcular_corr
    reta->inclinacao = s.sxy / s.sxx;
    reta->intercepta = s.media_y - reta->inclinacao * s.media_x;
    reta->r2 = corr * corr;
    return BE_OK;
}

be_status be_prever(rel_reta *reta, double x, double *y_previsto){
    if (reta == NULL || y_previsto == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (!isfinite(x) || !isfinite(reta->intercepta) || !isfinite(reta->inclinacao)) return BE_ERRO_VALOR_INVALIDO;

    *y_previsto = reta->intercepta + reta->inclinacao * x;
    return BE_OK;
}
