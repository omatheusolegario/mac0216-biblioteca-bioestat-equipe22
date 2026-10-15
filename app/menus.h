/**
 * @file menus.h
 * @brief Funções compartilhadas da aplicação: leitura do teclado, visual dos menus,
 *        e o menu + a demo de cada módulo.
 *
 * header do app, nao faz parte do app
 *
 * Fiz a CLI com um tratamento de erros básicos, e os erros são realmente cuidados pela biblioteca... 
 * mas são aprendentados
 *
 * Dados:
 *   if (ensure_table(d) != 0) return;    // módulos que usam a tabela 2x2
 *   if (ensure_vector(d) != 0) return;   // módulos que usam um vetor de números
 *
 * Padrão dos menus
 *   print_header("Titulo"); opções com 2 espaços de recuo; print_rule();
 *   read_long("> ", &opcao); printf("\n"); resultado com 2 espaços de recuo.
 */
#ifndef MENUS_H
#define MENUS_H

#include "dados.h"

/* funções vao estar todas definidas em main.c */

/** Lê um inteiro. Devolve 0, ou -1 se a entrada acabar. Se não for número, valor vira -1. */
int read_long(const char *prompt, long *valor);

/** Lê um real (decimal com ponto). Devolve 0, ou -1 se a entrada acabar ou não for número. */
int read_double(const char *prompt, double *valor);

/** Imprime uma linha em branco e o título entre duas linhas de '='. */
void print_header(const char *titulo);

/** Imprime uma linha de '-' (separa as opções do prompt). */
void print_rule(void);

/* Um menu e uma demonstração por módulo */

/** Epidemiologia  */
void menu_epidemiologia(Dados *d);
void demo_epidemiologia(void);

/** Testes diagnósticos  */
void menu_diagnostico(Dados *d);
void demo_diagnostico(void);

/** Estatística básica */
void menu_basico(Dados *d);
void demo_basico(void);

/** Relações entre variáveis  */
void menu_relacoes(Dados *d);
void demo_relacoes(void);

/** Visualização de gráficos */
void menu_visualizacao(Dados *d);
void demo_visualizacao(void);

#endif /* MENUS_H */
