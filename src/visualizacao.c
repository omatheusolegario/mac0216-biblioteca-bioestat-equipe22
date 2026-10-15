#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/visualizacao.h"

// Função auxiliar que limpa completamente a matriz 2d onde o desenho acontece
static void esvaziaCanvas(Canvas canvas_ascii)
{
    //Permite referenciar a matriz de forma natural
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    for (int i = 0; i < canvas_ascii.linhas; i++) for (int j = 0; j < canvas_ascii.colunas; j++) canvas[i][j] = ' ';
}

// Função auxiliar responsável por desenhar os eixos de referência x e y
static void desenhaEixos(Canvas canvas_ascii,  int pos_eixo_x, int pos_eixo_y)
{
    //Permite referenciar a matriz de forma natural
   char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    // Desenha o eixo y
    for (int i = 0; i < canvas_ascii.linhas-canvas_ascii.offset; i++) canvas[i][pos_eixo_y] = '|';
    
    // Desenha o eixo x
    for (int j = canvas_ascii.offset; j < canvas_ascii.colunas; j++) canvas[pos_eixo_x][j] = '-';
    
    // Desenha a origem
    canvas[pos_eixo_x][pos_eixo_y] = '+';
}

//Função auxiliar responsável por imprimir a matriz 2d no terminal
static void imprimeCanvas(Canvas canvas_ascii)
{
    // Permite referenciar a matriz 2d de forma natural
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    for (int i = 0; i < canvas_ascii.linhas; i++)
    {
        for (int j = 0; j < canvas_ascii.colunas; j++) printf("%c", canvas[i][j]);
        printf("\n");
    }
}

// Função auxiliar que dado um array de doubles e seu tamanho devolve o menor elemento
static double achaMin(const double *x, const int tamanho)
{
    if (x == NULL) return 0;
    double min = x[0];
    for (int i = 1; i < tamanho; i++) if (x[i] < min) min = x[i];
    return min;
}

// Função auxiliar que dado um array de doubles e seu tamanho devolve o maior elemento
static double achaMax(const double *x, const int tamanho)
{
    if (x == NULL) return 0;
    double max = x[0];
    for (int i = 1; i < tamanho; i++) if (x[i] > max) max = x[i];
    return max;
}

// Função auxiliar que dado um valor, o máximo e o mínimo, calcula a porcentagem daquele valor entre a amplitude, e depois 
// multiplica pela escala dada (arredondando para inteiro, pois é um índice de matriz)
static int normaliza(double valor, double min, double max, const int range_maximo)
{
    if (max==min) return 0;
    return round(((valor - min) / (max - min)) * (range_maximo - 1));
}


be_status be_dispersao_ascii(const double *X, const double *Y, const int tamanho, Canvas canvas_ascii)
{
    if(X== NULL || Y == NULL || canvas_ascii.ptr_canvas == NULL) return BE_ERRO_PONTEIRO_NULO;
    if(tamanho<=0 || canvas_ascii.linhas <= 0 || canvas_ascii.colunas <= 0 || canvas_ascii.offset < 0) return BE_ERRO_VALOR_INVALIDO;

    // permite referenciar a matriz de forma natural
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    // acha os valores mínimos e máximos de cada array
    double min_x = achaMin(X, tamanho);
    double max_x = achaMax(X, tamanho);
    double min_y = achaMin(Y, tamanho);
    double max_y = achaMax(Y, tamanho);

    int pos_eixo_x= canvas_ascii.linhas - canvas_ascii.offset;
    int pos_eixo_y= canvas_ascii.offset;

    // Calcula a posição da origem para traçar os eixos
    if (min_y<0 && max_y > 0) pos_eixo_x -= normaliza(0, min_y, max_y, canvas_ascii.colunas-canvas_ascii.offset-1);
    if (min_x<0 && max_x > 0) pos_eixo_y += normaliza(0, min_x, max_x, canvas_ascii.colunas-canvas_ascii.offset-1);
   
    esvaziaCanvas(canvas_ascii);
    desenhaEixos(canvas_ascii, pos_eixo_x, pos_eixo_y);

    // percorre todos itens de X e Y, normaliza suas posições no tamanho da matriz e desenha na posição (Xi,Yi)
    for (int i = 0; i < tamanho; i++)
    {
        int coluna = canvas_ascii.offset+1 + normaliza(X[i], min_x, max_x, canvas_ascii.colunas - canvas_ascii.offset -1);
        int linha = (canvas_ascii.linhas - canvas_ascii.offset - 1) - normaliza(Y[i], min_y, max_y, canvas_ascii.linhas - canvas_ascii.offset -1);
        canvas[linha][coluna] = '*';
    }

    imprimeCanvas(canvas_ascii);
    printf("Legenda\n");
    printf("    Eixo x = X | Eixo y = Y | N = %d\n", tamanho);
    printf("    X em [%.2f, %2.f]\n", min_x, max_x);     
    printf("    Y em [%.2f, %2.f]", min_y, max_y);

    return BE_OK;
}

be_status be_histograma_ascii(const double *dados, const int tamanho, const int num_bins, Canvas canvas_ascii)
{
    if(dados == NULL || canvas_ascii.ptr_canvas == NULL) return BE_ERRO_PONTEIRO_NULO;
    if(tamanho<=0 || num_bins < 0 || canvas_ascii.linhas <= 0 || canvas_ascii.colunas <= 0 || canvas_ascii.offset < 0) return BE_ERRO_VALOR_INVALIDO;
    
    // Permite referenciar a matriz 2d de forma natural
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    // Calcula os valores maximos e minimos do array
    double vmax = achaMax(dados, tamanho);
    double vmin = achaMin(dados, tamanho);

    // Calcula a largura dos intervalos
    double largura_bins = (vmax-vmin);
    if (num_bins > 0) largura_bins /= num_bins;
    if (largura_bins == 0) largura_bins = 1.0;

    // Aloca um array dinamico zerado no heap para a frequência absoluta
    double *frequencia_absoluta = calloc(num_bins, sizeof *frequencia_absoluta);
    if (frequencia_absoluta == NULL) return BE_ERRO_PONTEIRO_NULO;

    // Para todo item de dados, acha o intervalo que ele pertence e incrementa a frequência absoluta daquele intervalo
    for (int i = 0; i < tamanho; i++)
    {
        int index = (int)((dados[i] - vmin) / largura_bins);
        if (index >= num_bins) index = num_bins - 1;
        if (index<0) index=0;
        frequencia_absoluta[index]++;
    }

    // Para cada intervalo dado sua frequência calcula a densidade de frequência (eixo y do gráfico)
    double densidade_frequencia[num_bins];
    for (int i = 0; i < num_bins; i++) densidade_frequencia[i] = (frequencia_absoluta[i] / tamanho) / largura_bins;

    double densidade_max = achaMax(densidade_frequencia, num_bins);

    esvaziaCanvas(canvas_ascii);
    desenhaEixos(canvas_ascii,canvas_ascii.linhas -canvas_ascii.offset,canvas_ascii.offset);

    // Para cada intervalo, acha um y que é a densidade de frequência correspondente, e um x relativo ao número de intervalos
    // e para cada j até chegar em y preenche com # para formar uma barra
    for (int i = 0; i < num_bins; i++)
    {
        int y = normaliza(densidade_frequencia[i], 0, densidade_max, canvas_ascii.linhas -canvas_ascii.offset-1);
        int x = canvas_ascii.offset+1 + normaliza(i, 0, num_bins - 1, canvas_ascii.colunas - canvas_ascii.offset-1);
        for (int j = 0; j < y; j++)
        {
            int altura_atual = (canvas_ascii.linhas - canvas_ascii.offset-1) - j;
            canvas[altura_atual][x] = '#';
        }
    }

    imprimeCanvas(canvas_ascii);
    printf("Legenda\n");

    for (int i =0; i< num_bins; i++)printf("    Bin %d [%.2f, %.2f): Densidade = %.2f | Freq = %0.f\n", i+1, 
        vmin+(largura_bins*i), vmin+(largura_bins*(i+1)), densidade_frequencia[i], frequencia_absoluta[i]);
    
    free(frequencia_absoluta);
    
    return BE_OK;
}

be_status be_boxplot_ascii(const double *dados, const int tamanho, Quartis q, Canvas canvas_ascii)
{   
    if(dados == NULL || canvas_ascii.ptr_canvas == NULL) return BE_ERRO_PONTEIRO_NULO;
    if(tamanho<=0 || canvas_ascii.linhas <= 0 || canvas_ascii.colunas <= 0 || canvas_ascii.offset < 0) return BE_ERRO_VALOR_INVALIDO;

    // Permite referenciar a matriz de forma natural
    char (*canvas)[canvas_ascii.colunas] = (char (*)[canvas_ascii.colunas])canvas_ascii.ptr_canvas;

    // Simplifica a escrita na função dessas variáveis, deixando a leitura mais limpa também
    double q1 = q.q1;
    double med = q.q2;
    double q3 = q.q3;

    // Inicia os valores mínimos e máximos como os quartis já conhecidos
    double min = q1;
    double max = q3;

    // Calcula o limite superior e inferior do boxplot
    double j = q3 - q1;
    double ls = q3 + 1.5 * j;
    double li = q1 - 1.5 * j;

    // Para cada item no array, acha o mínimo e o máximo (dentro dos limites)
    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] >= li && dados[i] < min) min = dados[i];
        if (dados[i] <= ls && dados[i] > max) max = dados[i];
    }

    // Acha os valores mínimos e máximos do array todo
    double vmin = achaMin(dados, tamanho);
    double vmax = achaMax(dados, tamanho);

    if (vmin == vmax) vmax = vmin + 1.0;

    // Calcula todos os x relativos a matriz das medidas necessárias para desenhar o boxplot
    int x_min = canvas_ascii.offset+1 + normaliza(min, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_q1 = canvas_ascii.offset+1 + normaliza(q1, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_med = canvas_ascii.offset+1 + normaliza(med, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_q3 = canvas_ascii.offset+1 + normaliza(q3, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
    int x_max = canvas_ascii.offset+1 + normaliza(max, vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);

    // Acha um y no centro das linhas, pois o boxplot não depende do y
    int y = (canvas_ascii.linhas - canvas_ascii.offset-1) / 2;

    // calcula a posicao do eixo y relativa a origem
    int pos_eixo_y = canvas_ascii.offset;
    if (vmin<0 && vmax > 0) pos_eixo_y += normaliza(0, vmin, vmax, canvas_ascii.colunas-canvas_ascii.offset-1);
 
    esvaziaCanvas(canvas_ascii);
    desenhaEixos(canvas_ascii, canvas_ascii.linhas - canvas_ascii.offset, pos_eixo_y);

    // Desenha os "bigodes"
    canvas[y][x_min] = '|';
    canvas[y][x_max] = '|';

    // Desenha o traço do boxplot do menor valor dentro dos limites até o q1
    for (int i = x_min + 1; i < x_q1; i++) canvas[y][i] = '-';
    
    // Desenha o traço do boxplot do q3 até o maior valor dentro dos limites
    for (int i = x_q3 + 1; i < x_max; i++) canvas[y][i] = '-';

    // Desenha as bordas da caixa central
    canvas[y][x_q1] = '[';
    canvas[y][x_q3] = ']';

    // Preenche a caixa central
    for (int i = x_q1 + 1; i < x_q3; i++) canvas[y][i] = '=';
   
    // Marca a mediana
    canvas[y][x_med] = '|';

    // Para cada item no array fora dos limites, desenha um asterisco no seu lugar relativo
    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] < li || dados[i] > ls)
        {
            int x_normalizado = canvas_ascii.offset+1 + normaliza(dados[i], vmin, vmax, canvas_ascii.colunas - canvas_ascii.offset-1);
            canvas[y][x_normalizado] = '*';
        }
    }

    imprimeCanvas(canvas_ascii);
    printf("Legenda\n");
    printf("x_min: %.2f\nq1: %.2f\nmed: %.2f\nq3: %.2f\nx_max: %.2f\noutliers: ", min, q1, med, q3, max);
    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] < li || dados[i] > ls)
        {
            printf("%.2f,", dados[i]);
        }
    }

    return BE_OK;
}

be_status be_dispersao_svg(const char *nome_arquivo, const double *X, const double *Y, const int tamanho)
{
    if (nome_arquivo == NULL || X == NULL || Y == NULL) return BE_ERRO_PONTEIRO_NULO;
    if (tamanho<=0) return BE_ERRO_VALOR_INVALIDO;

    // Cria o arquivo html com nome de nome_arquivo e atribui ele ao ponteiro f
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) return BE_ERRO_VALOR_INVALIDO;

    // Acha os valores mínimos e máximos de X e Y
    double min_x = achaMin(X, tamanho);
    double max_x = achaMax(X, tamanho);
    double min_y = achaMin(Y, tamanho);
    double max_y = achaMax(Y, tamanho);

    // Define os tamanhos do quadrante onde o svg vai desenhar
    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura - (2 * margem);
    double altura_util = altura - (2 * margem);

    // Tags de abertura do html e svg
    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);

    double pos_eixo_x= altura-margem;
    double pos_eixo_y= margem;

    // Calcula a posição da origem para traçar os eixos
    if (min_y<0 && max_y > 0) pos_eixo_x -= normaliza(0, min_y, max_y, altura_util-margem);
    if (min_x<0 && max_x > 0) pos_eixo_y += normaliza(0, min_x, max_x, largura_util-margem);
    
    // Desenha os eixos x e y respectivamente
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, pos_eixo_x, largura - margem,pos_eixo_x);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", pos_eixo_y, margem, pos_eixo_y, altura - margem);
  
    // Para cada item nos arrays, acha sua posição relativa dentro do quadrante e desenha um circulo com suas coordenadas embaixo
    for (int i = 0; i < tamanho; i++)
    {
        double x = margem+ normaliza(X[i], min_x, max_x, largura_util - margem);
        double y = altura - margem - normaliza(Y[i], min_y, max_y, altura_util - margem);
        fprintf(f, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"4\" fill=\"#006080\" />\n", x, y);
        fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">(%.2f, %.2f)</text>\n",x , y-20, X[i], Y[i]);
    }

    // Rótulos dos eixos
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%s</text>\n", largura-margem,pos_eixo_x+15,"X");
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%s</text>\n", pos_eixo_y-15, margem,"Y");
    
    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);
    
    return BE_OK;
}

be_status be_histograma_svg(const char *nome_arquivo, const double *dados, const int tamanho, const int num_bins)
{
    if (nome_arquivo == NULL || dados == NULL) return BE_ERRO_PONTEIRO_NULO;  
    if (tamanho <= 0 || num_bins <0) return BE_ERRO_VALOR_INVALIDO;

    // Acha os valores máximos e minímos do array
    double vmax = achaMax(dados, tamanho);
    double vmin = achaMin(dados, tamanho);

    // Calcula a largura dos intervalos
    double largura_bins = (vmax-vmin);
    if (num_bins > 0) largura_bins /= num_bins;
    if (largura_bins == 0) largura_bins = 1.0;

    // Aloca um array dinamico zerado no heap para a frequência absoluta
    double *frequencia_absoluta = calloc(num_bins, sizeof *frequencia_absoluta);
    if (frequencia_absoluta == NULL) return BE_ERRO_PONTEIRO_NULO;

    // Para todo item de dados, acha o intervalo que ele pertence e incrementa a frequência absoluta daquele intervalo
    for (int i = 0; i < tamanho; i++)
    {
        int index = (int)((dados[i] - vmin) / largura_bins);
        if (index >= num_bins) index = num_bins - 1;
        if (index<0) index=0;
        frequencia_absoluta[index]++;
    }

    // Para cada intervalo dado sua frequência calcula a densidade de frequência (eixo y do gráfico)
    double densidade_frequencia[num_bins];
    for (int i = 0; i < num_bins; i++) densidade_frequencia[i] = (frequencia_absoluta[i] / tamanho) / largura_bins;

    double densidade_max = achaMax(densidade_frequencia, num_bins);

    // Define os tamanhos do quadrante onde o svg vai desenhar
    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura - (2 * margem);
    double altura_util = altura - (2 * margem);

    // Cria o arquivo html com nome de nome_arquivo e atribui ele ao ponteiro f
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) return BE_ERRO_VALOR_INVALIDO;

    // Cabeçalho do html e do svg
    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);

    // Desenha os eixos x e y
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura - margem, largura - margem, altura - margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, margem, margem, altura - margem);

    // Calcula a largura de cada intervalo
    double width = largura_util / num_bins;

    // Para cada intevalo, acha a altura, o x e o y relativos ao quadrante e desenha um retângulo com altura
    // da densidade de frequência, e escreve no eixo y ela e no eixo x o intervalo
    for (int i = 0; i < num_bins; i++)
    {
        double h = normaliza(densidade_frequencia[i], 0, densidade_max, altura_util - margem);
        double y = altura - margem - h;
        double x = margem + (i * width);

        fprintf(f, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.0f\" height=\"%.2f\"fill=\"#006080\" stroke=\"black\" stroke-width=\"1\"/>\n", x, y, width, h);
        fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", margem/2, y, densidade_frequencia[i]);
            fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">[%.2f, %.2f]</text>\n", x+(width/2), altura-margem+15, vmin+(largura_bins*i), vmin+largura_bins*(i+1));
    }

    // Rótulos dos eixos
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%s</text>\n", largura-margem,altura-margem+15,"x");
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"right\">%s</text>\n", margem-15, margem-10,"y = densidade de frequência");
  
    free(frequencia_absoluta);
    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);

    return BE_OK;
}

be_status be_boxplot_svg(const char *nome_arquivo, const double *dados, const int tamanho, Quartis q)
{
    if (nome_arquivo == NULL || dados == NULL) return BE_ERRO_PONTEIRO_NULO;  
    if (tamanho <= 0) return BE_ERRO_VALOR_INVALIDO;

    // Simplifica a escrita na função dessas variáveis, deixando a leitura mais limpa também
    double q1 = q.q1;
    double med = q.q2;
    double q3 = q.q3;

    // Inicia os valores mínimos e máximos como os quartis já conhecidos
    double min = q1;
    double max = q3;

    // Calcula o limite superior e inferior do boxplot
    double j = q3 - q1;
    double ls = q3 + 1.5 * j;
    double li = q1 - 1.5 * j;

    // Para cada item no array, acha o mínimo e o máximo (dentro dos limites)
    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] >= li && dados[i] < min) min = dados[i];
        if (dados[i] <= ls && dados[i] > max) max = dados[i];
    }

    // Acha os valores mínimos e máximos do array todo
    double vmin = achaMin(dados, tamanho);
    double vmax = achaMax(dados, tamanho);

    if (vmin == vmax) vmax = vmin + 1.0;

    // Define os tamanhos do quadrante onde o svg vai desenhar
    double largura = 800.0;
    double altura = 600.0;
    double margem = 60.0;
    double largura_util = largura - (2 * margem);
    double altura_util = altura - (2 * margem);

    // Cria o arquivo html com nome de nome_arquivo e atribui ele ao ponteiro f
    FILE *f = fopen(nome_arquivo, "w");
    if (f == NULL) return BE_ERRO_VALOR_INVALIDO;

    // Cabeçalho do html e svg
    fprintf(f, "<!DOCTYPE html>\n<html>\n<body>\n");
    fprintf(f, "<svg width=\"%.0f\" height=\"%.0f\" style=\"background:#ffffff; border: 1px solid #ccc;\">\n", largura, altura);

    // Calcula a posicao do eixo y relativa a origem
    double pos_eixo_y = margem;
    if (vmin<0 && vmax > 0) pos_eixo_y += normaliza(0, vmin, vmax, largura_util-margem);
 
    // Desenha os eixos x e y, respectivamente
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", margem, altura - margem, largura - margem, altura - margem);
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", pos_eixo_y, altura-margem, pos_eixo_y, margem);

    // Acha as posições de cada medida necessária para o desenho relativas ao quadrante do svg
    double x_min = margem + normaliza(min, vmin, vmax, largura_util);
    double x_q1 = margem + normaliza(q1, vmin, vmax, largura_util);
    double x_med = margem + normaliza(med, vmin, vmax, largura_util);
    double x_q3 = margem + normaliza(q3, vmin, vmax, largura_util);
    double x_max = margem + normaliza(max, vmin, vmax, largura_util);
    double x_ls = margem + normaliza(ls, vmin, vmax, largura_util);
    double x_li = margem + normaliza(li, vmin, vmax, largura_util);

    // Define um y no meio do eixo y
    double y = altura_util / 2;

    //Desenha a caixa principal do boxplot
    fprintf(f, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.0f\" height=\"40\" fill=\"#006080\" stroke=\"black\" stroke-width=\"1\"/>\n", x_q1, y - 20, (x_q3 - x_q1));
   
    // Desenha a linha do menor valor dentro dos limites até o q1
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_min, y, x_q1, y);
   
    // Desenha a linha do q3 até o maior valor dentro dos limites
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_q3, y, x_max, y);
   
    // Desenha o bigode no menor valor dentro dos limites
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_min, y + 20, x_min, y - 20);
   
    // Desenha o bigode no maior valor dentro dos limites
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_max, y + 20, x_max, y - 20);
   
    // Desenha o traço da mediana
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\"/>\n", x_med, y + 20, x_med, y - 20);
   
    // Desenha um pontilhado no limite superior
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\" stroke-dasharray=\"5,5\"/>\n", x_ls, margem, x_ls, altura-margem);
   
    // Desenha um pontilhado no limite inferior
    fprintf(f, "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"black\" stroke-width=\"2\" stroke-dasharray=\"5,5\"/>\n", x_li, margem, x_li, altura-margem);

    // Escreve o menor valor dentro dos limites abaixo dele no gráfico
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", x_min,  y + 40, min);
   
    // Escreve o maior valor dentro dos limites abaixo dele no gráfico
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", x_max,  y + 40, max);
   
    // Escreve o valor de q1 acima dele no gráfico
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", x_q1,  y - 40, q1);
   
    // Escreve o valor da mediana abaixo dela no gráfico
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", x_med,  y + 40, med);
   
    //Escreve o valor de q3 acima dele no gráfico
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", x_q3,  y - 40, q3);
   
    //Escreve o valor do limite superior no eixo x
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">LS: %.2f</text>\n", x_ls,  altura-margem+15, ls);
   
    // Escreve o valor do limite inferior no eixo x
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">LI: %.2f</text>\n", x_li,  altura-margem+15, li);
    
    // Para cada valor do array fora dos limites traça uma circunferência no lugar
    for (int i = 0; i < tamanho; i++)
    {
        if (dados[i] < li || dados[i] > ls)
        {
            double x_normalizado = margem + normaliza(dados[i], vmin, vmax, largura_util);
            fprintf(f, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"4\" fill=\"transparent\" stroke=\"black\" stroke-width=\"2\"/>\n", x_normalizado, y);
            fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%.2f</text>\n", x_normalizado, y+40, dados[i]);
        }
    }

    // Rótulo do eixo x
    fprintf(f, "<text x=\"%.2f\" y=\"%.2f\" font-family=\"sans-serif\" font-size=\"12\" fill=\"black\" text-anchor=\"middle\">%s</text>\n", largura-margem,altura-margem+15,"Valores de X");

    fprintf(f, "</svg> \n</body>\n</html>\n");
    fclose(f);

    return BE_OK;
}