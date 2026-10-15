/**
 * @file dados.h
 * @brief Dados que o usuário carrega na CLI, compartilhados entre os menus dos módulos.
 *
 * header também interno da app (não é da biblio)
 *
 * São dois tipos de dado, opcionais:
 *   - uma tabela 2x2 (epidemiologia e testes diagnósticos);
 *   - um vetor de números (estatística básica, relaçoes e visualização).
 *
 * Não há validação dos dados. Se estiverem invalidos, apenas retorna o erro da biblioteca
 *
 */
#ifndef DADOS_H
#define DADOS_H

#include <stddef.h>
#include "epidemiologia.h"

typedef struct {
    be_tabela2x2 tabela; /**< Tabela 2x2 (só vale se tem_tabela != 0). */
    int tem_tabela;      /**< 1 se há uma tabela carregada, 0 se não. */
    double *vetor;       /**< Vetor alocado com malloc (NULL se não tiver). */
    size_t n;            /**< Quantidade de valores em vetor (0 se não tiver). */
} Dados;

/**
 * Pergunta a, b, c, d e grava a tabela.
 * Entrada: quatro inteiros >= 0 .
 * @return 0 em sucesso; -1 se der erro (a tabela anterior não muda).
 */
int read_table(Dados *d);

/**
 * Pergunta a quantidade e os valores e grava o vetor.
 * Entrada: quantidade >= 1 e números reais com ponto decimal 
 * @return 0 em sucesso; -1 se der erro (o vetor anterior não muda).
 */
int read_vector(Dados *d);

/** Se não há tabela, oferece informar agora. @return 0 se há tabela para usar, -1 se não. */
int ensure_table(Dados *d);

/** Se não há vetor, oferece informar agora. @return 0 se há vetor para usar, -1 se não. */
int ensure_vector(Dados *d);

/** Carrega uma tabela e um vetor de exemplo. @return 0, ou -1 se faltar memória. */
int load_example(Dados *d);

/** Mostra os dados carregados. */
void print_data(const Dados *d);

/** Libera a memória do vetor e deixa tudo como "não carregado". */
void dados_free(Dados *d);

#endif /* DADOS_H */
