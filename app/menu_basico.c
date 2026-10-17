#include <stdio.h>
#include "menus.h"
#include <stdlib.h>
#include "basico.h"

static long demo = 0;

static void mostrar_vetor(const Dados *d){
    printf("  Vetor (%zu valores):", d->n);
    for (size_t i = 0; i < d->n; i++) printf(" %g", d->vetor[i]);
    printf("\n");
}

static void mostrar_resultado(const char *nome, be_status st, double resultado){
    printf("  %-25s", nome);
    if (st == BE_OK) printf("%.6g\n", resultado);
    else printf("erro: %s\n", be_status_mensagem(st));
}

static void mostrar_moda(Dados *d){
    size_t qtd = 0;
    double *modas = NULL;
    be_status st = be_moda(d->vetor, d->n, &qtd, &modas);

    printf("  Moda(s):                 ");
    if (st != BE_OK) {
        printf("erro: %s\n", be_status_mensagem(st));
        free(modas);
        return;
    }

    for (size_t i = 0; i < qtd; i++)
        printf("%s%g", i == 0 ? "" : ", ", modas[i]);
    printf("\n");
    free(modas);
}

static void mostrar_todas(Dados *d){
    double resultado = 0.0;
    be_status st;

    st = be_media(d->vetor, d->n, &resultado);
    mostrar_resultado("Média:", st, resultado);

    st = be_mediana(d->vetor, d->n, &resultado);
    mostrar_resultado("Mediana:", st, resultado);

    mostrar_moda(d);

    st = be_variancia(d->vetor, d->n, &resultado);
    mostrar_resultado("Variância:", st, resultado);

    st = be_desvio_padrao(d->vetor, d->n, &resultado);
    mostrar_resultado("Desvio padrão:", st, resultado);

    st = be_quartil_1(d->vetor, d->n, &resultado);
    mostrar_resultado("Primeiro quartil:", st, resultado);

    st = be_quartil_2(d->vetor, d->n, &resultado);
    mostrar_resultado("Segundo quartil:", st, resultado);

    st = be_quartil_3(d->vetor, d->n, &resultado);
    mostrar_resultado("Terceiro quartil:", st, resultado);
}

void menu_basico(Dados *d){
    if (!demo && ensure_vector(d) != 0) return;

    long opcao = -1;
    while (opcao != 0){
        if (!demo){
            print_header("Estatística básica");
            printf("  1) Informar/substituir o vetor\n");
            printf("  2) Mostrar o vetor\n");
            printf("  3) Média\n");
            printf("  4) Mediana\n");
            printf("  5) Moda\n");
            printf("  6) Variância\n");
            printf("  7) Desvio padrão\n");
            printf("  8) Primeiro quartil\n");
            printf("  9) Segundo quartil\n");
            printf(" 10) Terceiro quartil\n");
            printf(" 11) Todas as medidas\n");
            printf("  0) Voltar\n");
            print_rule();

            if (read_long("> ", &opcao) != 0) return;
            printf("\n");
        } else {
            opcao = 11;
        }

        double resultado = 0.0;
        be_status st;

        switch (opcao){
            case 1:
                if (read_vector(d) == 0)
                    printf("\n  Vetor gravado (%zu valores).\n", d->n);
                break;
            case 2:
                mostrar_vetor(d);
                break;
            case 3:
                st = be_media(d->vetor, d->n, &resultado);
                mostrar_resultado("Média:", st, resultado);
                break;
            case 4:
                st = be_mediana(d->vetor, d->n, &resultado);
                mostrar_resultado("Mediana:", st, resultado);
                break;
            case 5:
                mostrar_moda(d);
                break;
            case 6:
                st = be_variancia(d->vetor, d->n, &resultado);
                mostrar_resultado("Variância:", st, resultado);
                break;
            case 7:
                st = be_desvio_padrao(d->vetor, d->n, &resultado);
                mostrar_resultado("Desvio padrão:", st, resultado);
                break;
            case 8:
                st = be_quartil_1(d->vetor, d->n, &resultado);
                mostrar_resultado("Primeiro quartil:", st, resultado);
                break;
            case 9:
                st = be_quartil_2(d->vetor, d->n, &resultado);
                mostrar_resultado("Segundo quartil:", st, resultado);
                break;
            case 10:
                st = be_quartil_3(d->vetor, d->n, &resultado);
                mostrar_resultado("Terceiro quartil:", st, resultado);
                break;
            case 11:
                mostrar_todas(d);
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

void demo_basico(void){
    double exemplo[] = {2, 4, 4, 5, 7, 9, 10, 12, 15, 20, 35};
    Dados d = {0};

    d.vetor = exemplo;
    d.n = sizeof exemplo / sizeof exemplo[0];

    print_header("Demonstração: estatística básica");
    demo = 1;
    menu_basico(&d);
    demo = 0;
}
