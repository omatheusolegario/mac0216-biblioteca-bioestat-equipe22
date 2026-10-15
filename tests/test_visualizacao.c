#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/visualizacao.h"

#define LINHAS_TESTE 15
#define COLUNAS_TESTE 50

static Canvas cria_canvas_teste(char matriz[LINHAS_TESTE][COLUNAS_TESTE])
{
    Canvas c;
    c.ptr_canvas = &matriz[0][0];
    c.linhas = LINHAS_TESTE;
    c.colunas = COLUNAS_TESTE;
    c.offset = 1;
    return c;
}

void teste_nulos_e_limites()
{
    char matriz[LINHAS_TESTE][COLUNAS_TESTE];
    Canvas c = cria_canvas_teste(matriz);

    be_dispersao_ascii(NULL, NULL, 0, c);
    be_histograma_ascii(NULL, 0, -1, c);
    be_boxplot_ascii(NULL, 0, (Quartis){1, 2, 3}, c);
    be_dispersao_svg(NULL, NULL, NULL, 0);
    be_histograma_svg(NULL, NULL, 0, -1);
    be_boxplot_svg(NULL, NULL, 0, (Quartis){1, 2, 3});

    printf("[OK] Teste de nulos e limites passado com sucesso!\n");
}

void teste_dispersao_ascii()
{
    char matriz[LINHAS_TESTE][COLUNAS_TESTE];
    Canvas c = cria_canvas_teste(matriz);

    double X[] = {2, 3};
    double Y[] = {5, 6};

    be_dispersao_ascii(X, Y, 2, c);

    int n_pontos = 0;
    for (int i = 0; i < c.linhas; i++)
        for (int j = 0; j < c.colunas; j++)
            if (matriz[i][j] == '*')
                n_pontos++;
    assert(n_pontos == 2);
    printf("[OK] Teste do dispersão ascii passado com sucesso!\n");
}

void teste_histograma_ascii()
{
    char matriz[LINHAS_TESTE][COLUNAS_TESTE];
    Canvas c = cria_canvas_teste(matriz);

    double dados[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    be_histograma_ascii(dados, 10, 2, c);

    int n_barras = 0;
    for (int i = 0; i < c.linhas; i++)
        for (int j = 0; j < c.colunas; j++)
            if (matriz[i][j] == '#')
                n_barras++;
    assert(n_barras == 24);
    printf("[OK] Teste do histograma ascii passado com sucesso!\n");
}

void teste_boxplot_ascii()
{
    char matriz[LINHAS_TESTE][COLUNAS_TESTE];
    Canvas c = cria_canvas_teste(matriz);

    double X[] = {2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 20, 35};

    be_boxplot_ascii(X, 12, (Quartis){5,8.5,12}, c);

    int n_caixa = 0;
    int n_outliers=0;
    for (int i = 0; i < c.linhas; i++)
    {
        for (int j = 0; j < c.colunas; j++)
        {
            if (matriz[i][j] == '*') n_outliers++; else if (matriz[i][j] == '[' || matriz[i][j] == ']') n_caixa++;
        }
    }
    assert(n_caixa == 2);
    assert(n_outliers== 1);
    printf("[OK] Teste do boxplot ascii passado com sucesso!\n");
}

void teste_arquivos_svg(){
    double X[]={1,2,3};
    double Y[]={3,2,1};
    Quartis q = {1.5,2,2.5};
    const char *arquivo = "teste_temporario.html";

    be_dispersao_svg(arquivo, X, Y, 3);
    FILE *f = fopen(arquivo, "r");
    assert(f != NULL);
    fseek(f, 0, SEEK_END);
    assert(ftell(f)>0);
    fclose(f);
    remove(arquivo);

    be_histograma_svg(arquivo, X, 3, 2);
    f = fopen(arquivo, "r");
    assert(f != NULL);
    fseek(f, 0, SEEK_END);
    assert(ftell(f)>0);
    fclose(f);
    remove(arquivo);

    be_boxplot_svg(arquivo, X, 3, q);
    f = fopen(arquivo, "r");
    assert(f != NULL);
    fseek(f, 0, SEEK_END);
    assert(ftell(f)>0);
    fclose(f);
    remove(arquivo);

    printf("[OK] Testes de criação do SVG passados com sucesso!\n");
}

int main(){
    printf("Executando testes...\n");
    teste_nulos_e_limites();
    teste_dispersao_ascii();
    teste_histograma_ascii();
    teste_boxplot_ascii();
    teste_arquivos_svg();
    
    printf("\n[OK] Todos testes passaram com sucesso!\n");
    return 0;
}
