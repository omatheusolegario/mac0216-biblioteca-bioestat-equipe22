/*
 *  ESQUELETO da Aplicação cliente que voou implementar
 */
#include <stdio.h>
#include "epidemiologia.h"

int main(void)
{
    be_tabela2x2 t;
    double rr;

    be_status st = be_tabela_criar(20, 80, 10, 90, &t);
    if (st == BE_OK)
        st = be_risco_relativo(&t, &rr);

    if (st != BE_OK) {
        fprintf(stderr, "Erro: %s\n", be_status_mensagem(st));
        return 1;
    }

    printf("Risco relativo: %.2f\n", rr);
    return 0;
}
