/**
 * @file bioestat_erro.h
 * @brief Convenção de erros compartilhada por TODOS os módulos da libbioestat.
 *
 * Regra para nós: toda função pública que pode falhar
 *   1. retorna um be_status (BE_OK em caso de sucesso);
 *   2. devolve o resultado calculado por um ponteiro de saída.
 *
 * Exemplo de uso:
 *   double rr;
 *   be_status st = be_risco_relativo(&tab, &rr);
 *   if (st != BE_OK) { fprintf(stderr, "%s\n", be_status_mensagem(st)); }
 */
#ifndef BIOESTAT_ERRO_H
#define BIOESTAT_ERRO_H

/** Códigos de retorno das funções da biblioteca. */
typedef enum {
    BE_OK = 0,                 /**< Sucesso. */
    BE_ERRO_PONTEIRO_NULO,     /**< Algum ponteiro obrigatório veio NULL. */
    BE_ERRO_VALOR_INVALIDO,    /**< Entrada fora do domínio (ex.: contagem negativa). */
    BE_ERRO_DIVISAO_POR_ZERO,  /**< O cálculo exigiria dividir por zero. */
    BE_ERRO_NAO_IMPLEMENTADO   /**< Função ainda não implementada (uso durante o desenvolvimento). */
} be_status;

/**
 * @brief Converte um código de status em mensagem legível.
 * @param status Código retornado por qualquer função da biblioteca.
 * @return Texto constante (nunca NULL). Códigos desconhecidos retornam "erro desconhecido".
 */
const char *be_status_mensagem(be_status status);

#endif /* BIOESTAT_ERRO_H */
