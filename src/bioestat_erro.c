#include "bioestat_erro.h"

const char *be_status_mensagem(be_status status)
{
    switch (status) {
    case BE_OK:                    return "sucesso";
    case BE_ERRO_PONTEIRO_NULO:    return "ponteiro nulo recebido";
    case BE_ERRO_VALOR_INVALIDO:   return "valor de entrada invalido";
    case BE_ERRO_DIVISAO_POR_ZERO: return "divisao por zero no calculo";
    case BE_ERRO_MEMORIA:          return "erro de memória";
    case BE_ERRO_NAO_IMPLEMENTADO: return "funcao ainda nao implementada";
    }
    return "erro desconhecido";
}
