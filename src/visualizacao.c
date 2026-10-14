#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/visualizacao.h"

#define LINHAS 15
#define COLUNAS 50

typedef struct{
    char * ptr_canvas;
    int linhas;
    int colunas;
    int offset;
} Canvas;

static void esvaziaCanvas(Canvas canvas_ascii)
{
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    for (int i = 0; i < canvas_ascii.linhas; i++)
    {
        for (int j = 0; j < canvas_ascii.colunas; j++)
        {
            canvas[i][j] = ' ';
        }
    }
}

static void desenhaEixos(Canvas canvas_ascii)
{

   char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    for (int i = 0; i < canvas_ascii.linhas; i++)
    {
        canvas[i][0] = '|';
    }
    for (int j = 0; j < canvas_ascii.colunas; j++)
    {
        canvas[canvas_ascii.linhas - canvas_ascii.offset][j] = '-';
    }
    canvas[canvas_ascii.linhas - canvas_ascii.offset][0] = '+';
}

static void imprimeCanvas(Canvas canvas_ascii)
{
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    for (int i = 0; i < canvas_ascii.linhas; i++)
    {
        for (int j = 0; j < canvas_ascii.colunas; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

static double achaMin(const double *x, const int colunas)
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

static double achaMax(const double *x, const int colunas)
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

static int normaliza(double valor, double min, double max, const int range_maximo)
{
    return round(((valor - min) / (max - min)) * (range_maximo - 1));
}


void dispersao_ascii(const double *X, const double *Y, const int tamanho, Canvas canvas_ascii)
{
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;
    double min_x = achaMin(X, tamanho);
    double max_x = achaMax(X, tamanho);
    double min_y = achaMin(Y, tamanho);
    double max_y = achaMax(Y, tamanho);

    esvaziaCanvas(canvas_ascii);
    desenhaEixos(canvas_ascii);

    for (int i = 0; i < tamanho; i++)
    {
        int coluna = canvas_ascii.offset + normaliza(X[i], min_x, max_x, canvas_ascii.colunas - canvas_ascii.offset -1);
        int linha_bruta = normaliza(Y[i], min_y, max_y, canvas_ascii.linhas - canvas_ascii.offset -1);
        int linha = (canvas_ascii.linhas - canvas_ascii.offset - 1) - linha_bruta;
        canvas[linha][coluna] = '*';
    }
    imprimeCanvas(canvas_ascii);
}

void histograma_ascii(const double *dados, const int tamanho, const int num_bins, Canvas canvas_ascii)
{
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;
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

    esvaziaCanvas(canvas_ascii);
    desenhaEixos(canvas_ascii);

    for (int i = 0; i < num_bins; i++)
    {
        int y = normaliza(densidade_frequencia[i], 0, densidade_max, canvas_ascii.linhas -canvas_ascii.offset-1);
        int x = canvas_ascii.offset + normaliza(i, 0, num_bins - 1, canvas_ascii.colunas - canvas_ascii.offset -1);
        for (int j = 0; j < y; j++)
        {
            int altura_atual = (canvas_ascii.linhas - canvas_ascii.offset-1) - j;
            canvas[altura_atual][x] = '#';
        }
    }

    free(frequencia_absoluta);

    imprimeCanvas(canvas_ascii);
}

void boxplot_ascii(const double *dados, const int tamanho, const double q1, const double med, const double q3, Canvas canvas_ascii)
{
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

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
    double vmax = achaMax(dados, tamanho);
    if (vmin == vmax)
    {
        vmax = vmin + 1.0;
    }

    int x_min = canvas_ascii.offset + normaliza(min, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_q1 = canvas_ascii.offset + normaliza(q1, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_med = canvas_ascii.offset + normaliza(med, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_q3 = canvas_ascii.offset + normaliza(q3, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_max = canvas_ascii.offset + normaliza(max, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);

    int y = (canvas_ascii.linhas - canvas_ascii.offset-1) / 2; // centro das linhas

    esvaziaCanvas(canvas_ascii);
    desenhaEixos(canvas_ascii);

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

    canvas[y][x_q1] = '[';
    canvas[y][x_q3] = ']';

    for (int i = x_q1 + 1; i < x_q3; i++)
    {
        canvas[y][i] = '=';
    }
    canvas[y][x_med] = '|';

    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] < li || dados[i] > ls)
        {
            int x_normalizado = canvas_ascii.offset + normaliza(dados[i], vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
            canvas[y][x_normalizado] = '*';
        }
    }

    imprimeCanvas(canvas_ascii);
}

void dispersao_svg(const char *nome_arquivo, const double *X, const double *Y, const int tamanho)
{
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL)
        return;

    double min_x = achaMin(X, tamanho);
    double max_x = achaMax(X, tamanho);
    double min_y = achaMin(Y, tamanho);
    double max_y = achaMax(Y, tamanho);

    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura - (2 * margem);
    double altura_util = altura - (2 * margem);

    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, margem, margem, altura - margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura - margem, largura - margem, altura - margem);

    for (int i = 0; i < tamanho; i++)
    {
        double x = margem+ normaliza(X[i], min_x, max_x, largura_util - margem);
        double y = altura - margem - normaliza(Y[i], min_y, max_y, altura_util - margem);
        fprintf(f, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"4\" fill=\"#006080\" />\n", x, y);
    }

    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);
}

void histograma_svg(const char *nome_arquivo, const double *dados, const int tamanho, const int num_bins)
{
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
    double largura_util = largura - (2 * margem);
    double altura_util = altura - (2 * margem);

    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL)
        return;
    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, margem, margem, altura - margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura - margem, largura - margem, altura - margem);

    double width = largura_util / num_bins;

    for (int i = 0; i < num_bins; i++)
    {
        double h = normaliza(densidade_frequencia[i], 0, densidade_max, altura_util - margem);
        double y = altura - margem - h;
        double x = margem + (i * width);

        fprintf(f, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.0f\" height=\"%.2f\"fill=\"#006080\" stroke=\"black\" stroke-width=\"1\"/>\n", x, y, width, h);
    }

    free(frequencia_absoluta);
    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);
}

void boxplot_svg(const char *nome_arquivo, const double *dados, const int tamanho, const double q1, const double med, const double q3)
{

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
    double vmax = achaMax(dados, tamanho);
    if (vmin == vmax)
    {
        vmax = vmin + 1.0;
    }

    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura - (2 * margem);
    double altura_util = altura - (2 * margem);

    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL)
        return;
    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, margem, margem, altura - margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura - margem, largura - margem, altura - margem);

    double x_min = margem + normaliza(min, vmin, vmax, largura_util);
    double x_q1 = margem + normaliza(q1, vmin, vmax, largura_util);
    double x_med = margem + normaliza(med, vmin, vmax, largura_util);
    double x_q3 = margem + normaliza(q3, vmin, vmax, largura_util);
    double x_max = margem + normaliza(max, vmin, vmax, largura_util);

    double y = altura_util / 2; // centro das linhas

    fprintf(f, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.0f\" height=\"%.2d\"fill=\"#006080\" stroke=\"black\" stroke-width=\"1\"/>\n", x_q1, y - 10, (x_q3 - x_q1), 20);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_min, y, x_q1, y);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_q3, y, x_max, y);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_min, y + 10, x_min, y - 10);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_max, y + 10, x_max, y - 10);

    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_med, y + 10, x_med, y - 10);

    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] < li || dados[i] > ls)
        {
            double x_normalizado = margem + normaliza(dados[i], vmin, vmax, largura_util);
            fprintf(f, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"4\" fill=\"transparent\" stroke=\"black\" stroke-width=\"2\"/>\n", x_normalizado, y);
        }
    }

    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);
}

int main(int argc, char const *argv[])
{
    char matriz_canvas[LINHAS][COLUNAS];
    int tamanho = 6;
    double X[] = {1, 2, 3, 4, 5, 6};
    double Y[] = {6, 5, 4, 3, 2, 1};
    double Z[] = {1, 2, 3, 4, 5, 3, 2, 10, 10, 10};
    double A[] = {2, 4, 5, 6, 8, 9, 10, 11, 12, 13, 35};
    char nome_arquivo[] = "ab.html";
    Canvas canvas;
    canvas.ptr_canvas= &matriz_canvas[0][0];
    canvas.linhas= LINHAS;
    canvas.colunas= COLUNAS;

    // dispersao_svg(nome_arquivo, X, Y, tamanho);
    // histograma_svg(nome_arquivo, Z, 10, 2);
    // histograma_ascii(Z, 10, 2, canvas);
    // dispersao_ascii(X,Y,tamanho,canvas);
    boxplot_ascii(A, 11, 5.5, 9.0, 11.5, canvas);
    boxplot_svg(nome_arquivo, A, 11, 5.5, 9.0, 11.5);
    return 0;
}
