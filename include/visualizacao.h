/**
 * @file visualizacao.h
 * @brief Módulo responsável pelo desenho de gráficos no terminal em ASCII e em SVG/HTML.
 * @author Matheus Guimarães Olegario
 * @date 2026-10-14
 */

 #ifndef VISUALIZACAO_H
 #define VISUALIZACAO_H

 #include "bioestat_erro.h"
/**
 * @brief Struct responsável por agrupar tudo relacionado a matriz onde será desenhada o gráfico em ASCII.
 */
typedef struct{
    char * ptr_canvas; /**< Ponteiro para uma matriz 2D de caracteres.*/
    int linhas; /**< Representa a quantidade de linhas da matriz. */
    int colunas; /**< Representa a quantidade de colunas da matriz. */
    int offset; /**< A distância dos eixos x e y da matriz em relação a linha mais abaixo e a borda esquerda do terminal. */
} Canvas;

/**
 * @brief Struct responsável por agrupar os três quartis de um certo conjunto de dados.
 */
typedef struct{
    double q1; /**< Representa o primeiro quartil. */
    double q2; /**< Representa o segundo quartil (mediana). */
    double q3; /**< Representa o terceiro quartil. */
} Quartis;

/**
 * @brief Desenha no terminal um gráfico de dispersão entre dois arrays de dados de mesmo tamanho.
 * 
 * @param[in] X Vetor que agrupa valores de entrada no eixo horizontal.
 * @param[in] Y Vetor que agrupa valores de entrada no eixo vertical.
 * @param[in] tamanho Tamanho de ambos arrays.
 * @param[in, out] canvas_ascii Struct do Canvas a ser modificado para representar o gráfico de dispersão.
 * 
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se X, Y, ou ptr_canvas forem nulos,
 *                   BE_ERRO_VALOR_INVALIDO se tamanho, linhas ou colunas forem <=0 ou também offset < 0.
 * @warning Ambos vetores devem possuir o mesmo tamanho.
 */
be_status be_dispersao_ascii(const double *X, const double *Y, const int tamanho, Canvas canvas_ascii);

/**
 * @brief Desenha no terminal um histograma do array de dados fornecido.
 * 
 * @param[in] dados Vetor que agrupa os valores de entrada.
 * @param[in] tamanho Tamanho do vetor dados.
 * @param[in] num_bins Número de intervalos a serem incluídos no histograma.
 * @param[in, out] canvas_ascii Struct do Canvas a ser modificado para representar o gráfico de dispersão.
 * 
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados, ptr_canvas ou frequencia_absoluta forem nulos,
 *                   BE_ERRO_VALOR_INVALIDO se tamanho, linhas ou colunas forem <=0 ou também offset e num_bins < 0.
 */
be_status be_histograma_ascii(const double *dados, const int tamanho, const int num_bins, Canvas canvas_ascii);

/**
 * @brief Desenha no terminal um boxplot do array de dados e medidas resumo fornecidos.
 * 
 * @param[in] dados Vetor que agrupa os valores de entrada.
 * @param[in] tamanho Tamanho do vetor dados.
 * @param[in] q Struct contendo os três quartis necessários para o boxplot.
 * @param[out] canvas_ascii Struct do Canvas a ser modificado para representar o gráfico de dispersão.
 *  
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados ou ptr_canvas forem nulos,
 *                   BE_ERRO_VALOR_INVALIDO se tamanho, linhas ou colunas forem <=0 ou também offset < 0.
 * 
 */
be_status be_boxplot_ascii(const double *dados, const int tamanho, Quartis q, Canvas canvas_ascii);

/**
 * @brief Desenha em SVG/HTML um gráfico de dispersão entre dois arrays de dados de mesmo tamanho.
 * 
 * @param[in] X Vetor que agrupa valores de entrada no eixo horizontal.
 * @param[in] Y Vetor que agrupa valores de entrada no eixo vertical.
 * @param[in] tamanho Tamanho de ambos arrays.
 * 
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se X, Y ou ptr_canvas forem nulos,
 *                   BE_ERRO_VALOR_INVALIDO se tamanho <= 0 ou se falhar na criação do arquivo html.
 * 
 * @warning Ambos vetores devem possuir o mesmo tamanho.
 */
be_status be_dispersao_svg(const double *X, const double *Y, const int tamanho);

/**
 * @brief Desenha em SVG/HTML um histograma do array de dados fornecido.
 *
 * @param[in] dados Vetor que agrupa os valores de entrada.
 * @param[in] tamanho Tamanho do vetor dados.
 * @param[in] num_bins Número de intervalos a serem incluídos no histograma.
 * 
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados ou frequencia_absoluta forem nulos,
 *                   BE_ERRO_VALOR_INVALIDO se tamanho <= 0 ou num_bins < 0 ou se falhar na criação do arquivo html.
 * 
 */
be_status be_histograma_svg(const double *dados, const int tamanho, const int num_bins);

/**
 * @brief Desenha em SVG/HTML um boxplot do array de dados e medidas resumo fornecidos.
 * 
 * @param[in] dados Vetor que agrupa os valores de entrada.
 * @param[in] tamanho Tamanho do vetor dados.
 * @param[in] q Struct contendo os três quartis necessários para o boxplot.
 * 
 * @return be_status BE_OK caso não ocorra erros,
 *                   BE_ERRO_PONTEIRO_NULO se dados forem nulos,
 *                   BE_ERRO_VALOR_INVALIDO se tamanho <= 0 ou se falhar na criação do arquivo html.
 * 
 */
be_status be_boxplot_svg(const double *dados, const int tamanho, Quartis q);

#endif