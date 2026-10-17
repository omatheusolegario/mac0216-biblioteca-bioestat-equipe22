#include <stdio.h>
#include <stdlib.h>
#include "dados.h"
#include "menus.h"

int read_table(Dados *dados){
    long a, b, c, d;

    printf("  Informe as 4 contagens (inteiros >= 0):\n");
    if (read_long("  a (expostos doentes)         = ", &a) != 0) return -1;
    if (read_long("  b (expostos nao doentes)     = ", &b) != 0) return -1;
    if (read_long("  c (nao expostos doentes)     = ", &c) != 0) return -1;
    if (read_long("  d (nao expostos nao doentes) = ", &d) != 0) return -1;

    // Pelo contrato da biblioteca, se der erro a tabela não é alterada
    be_status st = be_preencher_tabela(a, b, c, d, &dados->tabela);

    if (st != BE_OK) {
        printf("\n  Tabela invalida: %s\n", be_status_mensagem(st));
        return -1;
    }
    dados->tem_tabela = 1;
    return 0;
}

int read_vector(Dados *dados){
    long n;

    if (read_long("  Quantos valores? (>= 1) ", &n) != 0) return -1;
    if (n<= 0) return -1;

    double *novo = malloc(n * sizeof *novo);
    if (novo == NULL) {
        printf("\n  Faltou memoria.\n");
        return -1;
    }

    for (long i = 0; i < n; i++) {
        char prompt[40];
        snprintf(prompt, sizeof prompt, "  valor %ld de %ld = ", i + 1, n);

        // Se a entrada acabar (ou não for número), libera o que já foi alocado
        if (read_double(prompt, &novo[i]) != 0) {
            free(novo);
            return -1;
        }
    }

    // Só agora, com tudo lido, troca o vetor antigo pelo novo
    free(dados->vetor);
    dados->vetor = novo;
    dados->n = (size_t)n;
    return 0;
}

int read_vector2(Dados *dados){
    long n2;

    if (read_long("  Quantos valores? (>= 1) ", &n2) != 0) return -1;
    if (n2<= 0) return -1;

    double *novo = malloc(n2 * sizeof *novo);
    if (novo == NULL) {
        printf("\n  Faltou memoria.\n");
        return -1;
    }

    for (long i = 0; i < n2; i++) {
        char prompt[40];
        snprintf(prompt, sizeof prompt, "  valor %ld de %ld = ", i + 1, n2);

        // Se a entrada acabar (ou não for número), libera o que já foi alocado
        if (read_double(prompt, &novo[i]) != 0) {
            free(novo);
            return -1;
        }
    }

    // Só agora, com tudo lido, troca o vetor antigo pelo novo
    free(dados->vetor2);
    dados->vetor2 = novo;
    dados->n2 = (size_t)n2;
    return 0;
}

int read_num_bins(Dados *dados){
    long num_bins;

    if (read_long("  Quantos intervalos? ", &num_bins) != 0) return -1;

    if (num_bins <= 0) return -1;

    dados->num_bins = (size_t)num_bins;
    return 0;
}


// Pergunta se o usuário quer informar o dado agora. Devolve 1 se sim
static int quer_informar(const char *aviso){
    long resposta = 0;

    printf("  %s Informar agora? (1 = sim, 0 = voltar)\n", aviso);
    if (read_long("> ", &resposta) != 0) return 0;
    printf("\n");
    return resposta == 1;
}

int ensure_table(Dados *dados){
    if (dados->tem_tabela) return 0;
    if (!quer_informar("Nenhuma tabela 2x2 carregada.")) return -1;
    return read_table(dados);
}

int ensure_vector(Dados *dados){
    if (dados->n > 0) return 0;
    if (!quer_informar("Nenhum vetor de numeros carregado.")) return -1;
    return read_vector(dados);
}

int ensure_vector2(Dados *dados){
    if (dados->n2 > 0) return 0;
    if (!quer_informar("Nenhum segundo vetor de numeros carregado")) return -1;
    return read_vector2(dados);
}

int ensure_num_bins(Dados *dados){
    if (dados->num_bins > 0) return 0;
    if (!quer_informar("Nenhum número de intervalos carregado.")) return -1;
    return read_num_bins(dados);
}

int load_example(Dados *dados){
    double exemplo[] = {2, 4, 4, 5, 7, 9, 10, 12, 15, 20, 35};
    double exemplo2[]= {35,20,15,12,10,9,7,5,4,4,2};
    size_t n = sizeof exemplo / sizeof exemplo[0];
    size_t n2 = sizeof exemplo2 / sizeof exemplo2[0];

    double *novo = malloc(sizeof exemplo);
    if (novo == NULL) {
        printf("  Faltou memoria.\n");
        return -1;
    }
    for (size_t i = 0; i < n; i++) novo[i] = exemplo[i];

    free(dados->vetor);
    dados->vetor = novo;
    dados->n = n;

    double *novo2 = malloc(sizeof exemplo2);
    if (novo2 == NULL) {
        printf("  Faltou memoria.\n");
        return -1;
    }
    for (size_t i = 0; i < n2; i++) novo2[i] = exemplo2[i];

    free(dados->vetor2);
    dados->vetor2 = novo2;
    dados->n2 = n2;

    be_preencher_tabela(20, 80, 10, 90, &dados->tabela);
    dados->tem_tabela = 1;

    dados->num_bins=4;
    return 0;
}

void print_data(const Dados *dados){
    if (dados->tem_tabela) {
        printf("  Tabela 2x2: a=%ld b=%ld c=%ld d=%ld\n", dados->tabela.a, dados->tabela.b,
               dados->tabela.c, dados->tabela.d);
    } else {
        printf("  Tabela 2x2: nenhuma\n");
    }

    if (dados->n > 0) {
        printf("  Vetor 1 (%zu valores):", dados->n);
        for (size_t i = 0; i < dados->n && i < 10; i++) printf(" %g", dados->vetor[i]);
        if (dados->n > 10) printf(" ...");
        printf("\n");
    } else {
        printf("  Vetor: nenhum\n");
    }
     if (dados->n2 > 0) {
        printf("  Vetor 2 (%zu valores):", dados->n2);
        for (size_t i = 0; i < dados->n2 && i < 10; i++) printf(" %g", dados->vetor2[i]);
        if (dados->n2 > 10) printf(" ...");
        printf("\n");
    } else {
        printf("  Vetor 2: nenhum\n");
    }

    if (dados->num_bins > 0){
        printf("  Número de intervalos: %zu\n", dados->num_bins);
    }else{
        printf(" Número de intervalos: nenhum\n");
    }
}

void dados_free(Dados *dados){
    free(dados->vetor);
    free(dados->vetor2);
    dados->vetor = NULL;
    dados->vetor2 = NULL;
    dados->n = 0;
    dados->n2=0;
    dados->num_bins=0;
    dados->tem_tabela = 0;
}
