#include <stddef.h>

/*
 * Módulo de estatística básica.
 *
 * dados aponta para n valores double. Esse vetor nunca é alterado.
 */

/* Média aritmética */
double media(const double *dados, size_t n);


/* Mediana
 * = Segundo quartil
 */
double mediana(const double *dados, size_t n);


/* Variância
 *
 */
double variancia(const double *dados, size_t n);


/* Desvio padrão
 * Raiz quadrada da variância
 */
double desvio_padrao(const double *dados, size_t n);


/* Moda
 * Todos os valores de maior frequência, em ordem crescente
 * - Em caso de empate devolve todos os valores empatados
 * - Caso nenhum valor se repita devolve todos os valores
 *
 * Devolve um vetor alocado com malloc e grava em *qtd quantos valores ele tem
 */
double *moda(const double *dados, size_t n, size_t *qtd);


/** Quartis
 * Ordenação necessária
 * Posição do k-ésimo quartil: ((n+1)*k)/4
 * Posição = inteiro -> quartil = valor nessa posição
 * Posição = decimal -> quartil = interpolação linear entre as duas posições adjacentes
 **/
 /* Primeiro quartil */
double quartil_1(const double *dados, size_t n);

 /* Segundo quartil */
double quartil_2(const double *dados, size_t n);

 /* Terceiro quartil */
double quartil_3(const double *dados, size_t n);
