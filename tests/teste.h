/*
 * Mini-framework de testes (sem dependências externas).
 * NÃO faz parte da biblioteca: só os programas em tests/ o incluem.
 *
 * Uso:
 *   VERIFICA(condicao, "descricao");
 *   VERIFICA_PROXIMO(obtido, esperado, "descricao");   // compara doubles
 *   return RESULTADO_TESTES();                          // no fim do main
 */
#ifndef TESTE_H
#define TESTE_H

#include <stdio.h>
#include <math.h>

static int teste_total = 0;
static int teste_falhas = 0;

#define VERIFICA(cond, desc)                                              \
    do {                                                                  \
        teste_total++;                                                    \
        if (cond) {                                                       \
            printf("  [ OK ] %s\n", desc);                                \
        } else {                                                          \
            teste_falhas++;                                               \
            printf("  [FALHA] %s  (%s:%d)\n", desc, __FILE__, __LINE__);  \
        }                                                                 \
    } while (0)

#define VERIFICA_PROXIMO(obtido, esperado, desc) \
    VERIFICA(fabs((obtido) - (esperado)) < 1e-9, desc)

#define RESULTADO_TESTES()                                                    \
    (printf("  -> %d/%d verificacoes passaram\n",                             \
            teste_total - teste_falhas, teste_total),                         \
     teste_falhas == 0 ? 0 : 1)

#endif /* TESTE_H */
