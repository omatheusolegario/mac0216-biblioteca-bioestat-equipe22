#include <stdio.h>
#include "menus.h"
#include "relacoes.h"

static long demo = 0;

static int ensure_vectors(Dados *d){
    if (ensure_vector(d) != 0) return -1;
    if (ensure_vector2(d) != 0) return -1;
    return 0;
}

static int vetores_compativeis(const Dados *d){
    if (d->vetor == NULL || d->vetor2 == NULL || d->n == 0 || d->n2 == 0){
        printf("  Carregue os dois vetores antes de calcular as relações.\n");
        return 0;
    }
    if (d->n != d->n2){
        printf("  Os vetores precisam ter o mesmo tamanho (vetor 1: %zu; vetor 2: %zu).\n",
               d->n, d->n2);
        printf("  Ajuste os vetores em Entrada de dados ou neste menu.\n");
        return 0;
    }
    return 1;
}

static void mostrar_vetores(const Dados *d){
    printf("  Vetor X (%zu valores):", d->n);
    for (size_t i = 0; i < d->n && i<10; i++) printf(" %g", d->vetor[i]);
    if (d->n > 10) printf(" ...");
    printf("\n");

    printf("  Vetor Y (%zu valores):", d->n2);
    for (size_t i = 0; i < d->n2 && i <10; i++) printf(" %g", d->vetor2[i]);
    if (d->n > 10) printf(" ...");
    printf("\n");
}

static void mostrar_resultado(const char *nome, be_status st, double resultado){
    printf("  %-28s", nome);
    if (st == BE_OK) printf("%.6g\n", resultado);
    else printf("erro: %s\n", be_status_mensagem(st));
}

static be_status calcular_reta(Dados *d, rel_reta *reta){
    if (!vetores_compativeis(d)) return BE_ERRO_VALOR_INVALIDO;
    return be_regressao_linear(d->vetor, d->vetor2, d->n, reta);
}

static void mostrar_covariancia(Dados *d){
    double resultado = 0.0;
    be_status st;
    if (!vetores_compativeis(d)) return;
    st = be_covariancia(d->vetor, d->vetor2, d->n, &resultado);
    mostrar_resultado("Covariancia:", st, resultado);
}

static void mostrar_correlacao(Dados *d){
    double resultado = 0.0;
    be_status st;
    if (!vetores_compativeis(d)) return;
    st = be_correlacao(d->vetor, d->vetor2, d->n, &resultado);
    mostrar_resultado("Correlacao de Pearson:", st, resultado);
}

static void mostrar_regressao(Dados *d){
    rel_reta reta;
    be_status st = calcular_reta(d, &reta);

    if (st != BE_OK){
        printf("  Regressão linear: erro: %s\n", be_status_mensagem(st));
        return;
    }
    printf("  Intercepto:                  %.6g\n", reta.intercepta);
    printf("  Inclinação:                  %.6g\n", reta.inclinacao);
    printf("  Coeficiente de determinação (R^2): %.6g\n", reta.r2);
}

static void prever_y(Dados *d){
    rel_reta reta;
    double x, previsto = 0.0;
    be_status st = calcular_reta(d, &reta);

    if (st != BE_OK){
        printf("  Não foi possível calcular a regressão: %s\n", be_status_mensagem(st));
        return;
    }

    if (read_double("  Valor de x para a previsao: ", &x) != 0){
        printf("  Entrada inválida ou encerrada.\n");
        return;
    }

    st = be_prever(&reta, x, &previsto);
    mostrar_resultado("Y previsto:", st, previsto);
}

static void mostrar_todas(Dados *d){
    mostrar_covariancia(d);
    mostrar_correlacao(d);
    mostrar_regressao(d);
}

void menu_relacoes(Dados *d){
    if (!demo && ensure_vectors(d) != 0) return;

    long opcao = -1;
    while (opcao != 0){
        if (!demo){
            print_header("Relações entre variáveis");
            printf("  1) Informar/substituir o vetor X\n");
            printf("  2) Informar/substituir o vetor Y\n");
            printf("  3) Mostrar os vetores\n");
            printf("  4) Covariância\n");
            printf("  5) Correlação linear de Pearson\n");
            printf("  6) Regressão linear simples\n");
            printf("  7) Prever y para um valor de x\n");
            printf("  8) Todas as medidas\n");
            printf("  0) Voltar\n");
            print_rule();

            if (read_long("> ", &opcao) != 0) return;
            printf("\n");
        } else {
            opcao = 8;
        }

        switch (opcao){
            case 1:
                if (read_vector(d) == 0)
                    printf("\n  Vetor X gravado (%zu valores).\n", d->n);
                break;
            case 2:
                if (read_vector2(d) == 0)
                    printf("\n  Vetor Y gravado (%zu valores).\n", d->n2);
                break;
            case 3:
                mostrar_vetores(d);
                break;
            case 4:
                mostrar_covariancia(d);
                break;
            case 5:
                mostrar_correlacao(d);
                break;
            case 6:
                mostrar_regressao(d);
                break;
            case 7:
                prever_y(d);
                break;
            case 8:
                mostrar_todas(d);
                if (demo){
                    rel_reta reta;
                    double previsto;
                    be_status st = calcular_reta(d, &reta);
                    if (st == BE_OK){
                        st = be_prever(&reta, 6.0, &previsto);
                        printf("  Previsão para x = 6:          ");
                        if (st == BE_OK) printf("%.6g\n", previsto);
                        else printf("erro: %s\n", be_status_mensagem(st));
                    }
                }
                break;
            case 0:
                break;
            default:
                printf("  Opção inválida.\n");
        }

        if (demo) return;
        printf("\n");
    }
}

void demo_relacoes(void){
    double x[] = {1, 2, 3, 4, 5};
    double y[] = {2, 4, 5, 4, 6};
    Dados d = {0};

    d.vetor = x;
    d.n = sizeof x / sizeof x[0];
    d.vetor2 = y;
    d.n2 = sizeof y / sizeof y[0];

    print_header("Demonstração: relações entre variáveis");
    demo = 1;
    menu_relacoes(&d);
    demo = 0;
}
