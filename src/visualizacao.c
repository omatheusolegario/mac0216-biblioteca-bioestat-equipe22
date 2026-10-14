#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/visualizacao.h"

#define LINHAS 15
#define COLUNAS 50

void esvaziaCanvas(char *ptr_canvas, int linhas, int colunas)
{
    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            canvas[i][j] = ' ';
        }
    }
}

void desenhaEixos(char *ptr_canvas, int linhas, int colunas)
{

    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

    for (int i = 0; i < linhas; i++)
    {
        canvas[i][0] = '|';
    }
    for (int j = 0; j < colunas; j++)
    {
        canvas[linhas - 1][j] = '-';
    }
    canvas[linhas - 1][0] = '+';
}

void imprimeCanvas(char *ptr_canvas, int linhas, int colunas)
{
    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

double achaMin(double *x, int colunas)
{
    double min = x[0];
    for (int i = 1; i < colunas; i++)
    {
        if (x[i] < min)
        {
            min = x[i];
        }
    }
    return min;
}

double achaMax(double *x, int colunas)
{
    double max = x[0];
    for (int i = 1; i < colunas; i++)
    {
        if (x[i] > max)
        {
            max = x[i];
        }
    }
    return max;
}

int normaliza(double valor, double min, double max, int range_maximo)
{
    return round(((valor - min) / (max - min)) * (range_maximo - 1));
}

void dispersao_ascii(double *X, double *Y, int tamanho, char *ptr_canvas, int linhas, int colunas)
{
    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;
    double min_x = achaMin(X, tamanho);
    double max_x = achaMax(X, tamanho);
    double min_y = achaMin(Y, tamanho);
    double max_y = achaMax(Y, tamanho);

    esvaziaCanvas(ptr_canvas, linhas, colunas);
    desenhaEixos(ptr_canvas, linhas, colunas);

    for (int i = 0; i < tamanho; i++)
    {
        int coluna = 1 + normaliza(X[i], min_x, max_x, colunas - 2);
        int linha_bruta = normaliza(Y[i], min_y, max_y, linhas - 2);
        int linha = (linhas - 2) - linha_bruta;
        canvas[linha][coluna] = '*';
    }
    imprimeCanvas(ptr_canvas, linhas, colunas);
}

void histograma_ascii(double *dados, int tamanho, int num_bins, char *ptr_canvas, int linhas, int colunas)
{
    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;
    double vmax = achaMax(dados, tamanho);
    double vmin = achaMin(dados, tamanho);
    double largura_bins = (vmax - vmin) / num_bins;
    if (largura_bins == 0)
    {
        largura_bins = 1.0;
    }

    double *frequencia_absoluta;
    frequencia_absoluta = calloc(num_bins, sizeof *frequencia_absoluta);
    if (frequencia_absoluta == NULL)
    {
        return;
    }

    for (int i = 0; i < tamanho; i++)
    {
        int index = (int)((dados[i] - vmin) / largura_bins);
        if (index >= num_bins)
        {
            index = num_bins - 1;
        }
        frequencia_absoluta[index]++;
    }

    double densidade_frequencia[num_bins];

    for (int i = 0; i < num_bins; i++)
    {
        densidade_frequencia[i] = (frequencia_absoluta[i] / tamanho) / largura_bins;
    }

    double densidade_max = achaMax(densidade_frequencia, num_bins);

    esvaziaCanvas(ptr_canvas, linhas, colunas);
    desenhaEixos(ptr_canvas, linhas, colunas);

    for (int i = 0; i < num_bins; i++)
    {
        int y = normaliza(densidade_frequencia[i], 0, densidade_max, linhas - 2);
        int x = 1 + normaliza(i, 0, num_bins - 1, colunas - 2);
        for (int j = 0; j < y; j++)
        {
            int altura_atual = (linhas - 2) - j;
            canvas[altura_atual][x] = '#';
        }
    }

    free(frequencia_absoluta);

    imprimeCanvas(ptr_canvas, linhas, colunas);
}

void boxplot_ascii(double *dados, int tamanho, double q1, double med, double q3, char *ptr_canvas, int linhas, int colunas)
{
    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

    double min = q1;
    double max = q3;

    double j = q3 - q1;
    double ls = q3 + 1.5 * j;
    double li = q1 - 1.5 * j;

    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] >= li && dados[i] < min)
        {
            min = dados[i];
        }
        if (dados[i] <= ls && dados[i] > max)
        {
            max = dados[i];
        }
    }

    double vmin = achaMin(dados, tamanho);
    double vmax= achaMax(dados, tamanho);
    if(vmin==vmax){vmax =vmin+1.0;}

    int x_min = 1 + normaliza(min, vmin, vmax, colunas - 2);
    int x_q1 = 1 + normaliza(q1, vmin, vmax, colunas - 2);
    int x_med = 1 + normaliza(med, vmin, vmax, colunas - 2);
    int x_q3 = 1 + normaliza(q3, vmin, vmax, colunas - 2);
    int x_max = 1 + normaliza(max, vmin, vmax, colunas - 2);

    int y = (linhas - 2) / 2; // centro das linhas

    esvaziaCanvas(ptr_canvas, linhas, colunas);
    desenhaEixos(ptr_canvas, linhas, colunas);

    canvas[y][x_min] = '|';
    canvas[y][x_max] = '|';

    for (int i = x_min + 1; i < x_q1; i++)
    {
        canvas[y][i] = '-';
    }
    for (int i = x_q3 + 1; i < x_max; i++)
    {
        canvas[y][i] = '-';
    }
    
    canvas[y][x_q1]='[';
    canvas[y][x_q3]=']';

    for (int i = x_q1+1; i < x_q3; i++)
    {
        canvas[y][i] = '=';
    }
    canvas[y][x_med] = '|';

    for (int i=0; i< tamanho; i++){
        if(dados[i] < li || dados[i]> ls){
            int x_normalizado = 1+ normaliza(dados[i], vmin, vmax, colunas-2);
            canvas[y][x_normalizado]='*';
        }
    }
}

void dispersao_svg(char* nome_arquivo, double *X, double *Y, int tamanho){
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) return;

    double min_x = achaMin(X, tamanho);
    double max_x = achaMax(X, tamanho);
    double min_y = achaMin(Y, tamanho);
    double max_y = achaMax(Y, tamanho);

    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura -(2* margem);
    double altura_util = altura - (2* margem);

    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);
    
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, margem, margem, altura-margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura-margem, largura-margem, altura-margem);

    for (int i = 0; i < tamanho; i++)
    {
        double x = margem+20 + normaliza(X[i], min_x, max_x, largura_util-margem);
        double y = altura-margem-20-normaliza(Y[i], min_y, max_y, altura_util-margem);
        fprintf(f, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"4\" fill=\"#006080\" />\n", x, y);
    }   

    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);
}

void histograma_svg(char* nome_arquivo, double* dados, int tamanho, int num_bins){
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) return;

    double vmax = achaMax(dados, tamanho);
    double vmin = achaMin(dados, tamanho);
    double largura_bins = (vmax - vmin) / num_bins;
    if (largura_bins == 0)
    {
        largura_bins = 1.0;
    }

    double *frequencia_absoluta;
    frequencia_absoluta = calloc(num_bins, sizeof *frequencia_absoluta);
    if (frequencia_absoluta == NULL)
    {
        return;
    }

    for (int i = 0; i < tamanho; i++)
    {
        int index = (int)((dados[i] - vmin) / largura_bins);
        if (index >= num_bins)
        {
            index = num_bins - 1;
        }
        frequencia_absoluta[index]++;
    }

    double densidade_frequencia[num_bins];

    for (int i = 0; i < num_bins; i++)
    {
        densidade_frequencia[i] = (frequencia_absoluta[i] / tamanho) / largura_bins;
    }

    double densidade_max = achaMax(densidade_frequencia, num_bins);

    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura -(2* margem);
    double altura_util = altura - (2* margem);

    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);
    
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, margem, margem, altura-margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura-margem, largura-margem, altura-margem);

    double width = largura_util/num_bins;
    
    for (int i = 0; i < num_bins; i++)
    {
        double h = normaliza(densidade_frequencia[i], 0, densidade_max, altura_util-margem);
        double y = altura-margem-h;
        double x = margem + (i* width);
      
        fprintf(f, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.0f\" height=\"%.2f\"fill=\"#006080\" stroke=\"black\" stroke-width=\"1\"/>\n", x,y,width,h);
    }

    free(frequencia_absoluta);
    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);
}

int main(int argc, char const *argv[])
{   
    char canvas[LINHAS][COLUNAS];
    int tamanho = 6;
    double X[]={1,2,3,4,5,6};
    double Y[]={6,5,4,3,2,1};
    double Z[]={1,2,3,4,5,3,2,10,10,10};
    char nome_arquivo[]="ab.html";
    //dispersao_svg(nome_arquivo, X, Y, tamanho);
    histograma_svg(nome_arquivo, Z, 10, 2);
    histograma_ascii(Z, 10, 2, &canvas[0][0], LINHAS, COLUNAS);
   //dispersao_ascii(X,Y,tamanho,&canvas[0][0],LINHAS,COLUNAS);
    return 0;
}
