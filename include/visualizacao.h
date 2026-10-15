/**
 * @file visualizacao.h
 * @brief Módulo responsável pelo desenho de gráficos no terminal em ASCII e em SVG/HTML.
 * @author Matheus Guimarães Olegario
 * @date 2026-10-14
 */
/**
 * @brief Struct responsável por agrupar tudo relacionado a matriz onde será desenhada o gráfico em ASCII
 */
typedef struct{
    char * ptr_canvas; /**< Ponteiro para uma matriz 2D de caracteres*/
    int linhas; /**< Representa a quantidade de linhas da matriz */
    int colunas; /**< Representa a quantidade de colunas da matriz */
    int offset; /**< A distância dos eixos x e y da matriz em relação a linha mais abaixo e a borda esquerda do terminal */
} Canvas;

/**
 * @brief Desenha no terminal um gráfico de dispersão entre duas variáveis de mesmo tamanho.
 */
void dispersao_ascii(const double *X, const double *Y, const int tamanho, Canvas canvas_ascii);
