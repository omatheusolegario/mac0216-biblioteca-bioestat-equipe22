#include <stdio.h>
#include "../include/visualizacao.h"

#define LINHAS 15
#define COLUNAS 50

void esvaziaCanvas(char* ptr_canvas, int linhas, int colunas){
     char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

    for (int i =0; i < linhas; i++){
        for (int j=0; j < colunas; j++){
            canvas[i][j]= ' '; 
        }
    }
}

void desenhaEixos(char* ptr_canvas,int linhas, int colunas){

    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

    for(int i=0; i< linhas; i++){
        canvas[i][0]= '|';
    }
    for (int j =0; j< colunas; j++){
        canvas[linhas-1][j]='-';
    }
    canvas[linhas-1][0]='+';
}

void imprimeCanvas(char* ptr_canvas, int linhas, int colunas){
     char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;

     for(int i=0; i< linhas; i++){
        for (int j=0; j< colunas; j++){
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

double achaMin(double* x, int colunas){
    double min=x[0];
    for (int i=1; i< colunas; i++){
        if (x[i] < min){min = x[i];}
    }
    return min;
}

double achaMax(double* x, int colunas){
    double max=x[0];
    for (int i=1; i< colunas; i++){
        if (x[i] > max){max = x[i];}
    }
    return max;
}

int normaliza(double valor, double min, double max, int range_maximo){
    return round(((valor-min)/(max-min))* (range_maximo-1));
}

void dispersao(double* X, double* Y, int tamanho, char* ptr_canvas, int linhas, int colunas){
    char (*canvas)[colunas] = (char (*)[colunas])ptr_canvas;
    double min_x=achaMin(X, tamanho);
    double max_x=achaMax(X, tamanho);
    double min_y=achaMin(Y, tamanho);
    double max_y=achaMax(Y, tamanho);

    esvaziaCanvas(ptr_canvas, linhas, colunas);
    desenhaEixos(ptr_canvas, linhas, colunas);

    for (int i=0; i< tamanho; i++){
        int coluna = 1 + normaliza(X[i], min_x, max_x, colunas-1);
        int linha_bruta = normaliza(Y[i], min_y, max_y, linhas-1);
        int linha = (linhas-1) - linha_bruta;
        canvas[linha][coluna]= '*';
    }
}

void histograma(double* dados, int tamanho, int num_bins){
}

int main(int argc, char const *argv[])
{

    return 0;
}


