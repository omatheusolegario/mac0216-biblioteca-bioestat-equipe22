#include <stdio.h>
#include "menus.h"
#include "../include/visualizacao.h"
#include "../include/basico.h"

#define LINHAS 15
#define COLUNAS 50

// Função auxiliar para criar o struct e atribuir os valores rapidamente
static Canvas cria_canvas(char matriz[LINHAS][COLUNAS])
{
    Canvas c;
    c.ptr_canvas = &matriz[0][0];
    c.linhas = LINHAS;
    c.colunas = COLUNAS;
    c.offset = 1;
    return c;
}
void menu_visualizacao(Dados *d){
    long opcao = -1;

    while (opcao != 0) {
        print_header("Epidemiologia");
        printf("  1) Desenhar um gráfico de dispersão em ASCII\n");
        printf("  2) Desenhar um histograma em ASCII\n");
        printf("  3) Desenhar um boxplot em ASCII\n");
        printf("  4) Desenhar um gráfico de dispersão em SVG/HTML\n");
        printf("  5) Desenhar um histograma em SVG/HTML\n");
        printf("  6) Desenhar um boxplot em SVG/HTML\n");
        print_rule();

        char matriz[LINHAS][COLUNAS];
        Canvas c = cria_canvas(matriz);

        Quartis q;
        be_quartil_1(d->vetor, d->n, &q.q1);
        be_quartil_2(d->vetor, d->n, &q.q2);
        be_quartil_3(d->vetor, d->n, &q.q3);

        // Ctrl+D: volta pro menu principal
        if (read_long("> ", &opcao) != 0) return;
        printf("\n");

        switch (opcao) {
            case 1: be_dispersao_ascii(d->vetor, d->vetor2, d->n, c); break;
            case 2: be_histograma_ascii(d->vetor, d->n, d->num_bins, c); break;
            case 3: be_boxplot_ascii(d->vetor, d->n, q, c); break;
            case 4: be_dispersao_svg(d->vetor, d->vetor, d->n); break;
            case 5: be_histograma_svg(d->vetor, d->n, d->num_bins); break;
            case 6: be_boxplot_svg(d->vetor, d->n, q); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }
    }
}

void demo_visualizacao(void){
    print_header("Demonstracao: visualizacao");
    printf("\n  [em construcao]\n");
}
