/*
 *  ESQUELETO da Aplicação cliente que voou implementar
 */
#include <stdio.h>
#include "epidemiologia.h"


int main(void)
{
    be_tabela2x2 tabela = {4, 5, 7, 1};
    double re;
    be_risco_expostos(&tabela, &re);
    
    be_status st = be_risco_expostos(&tabela, &re);
    printf("RE: %f\n STATUS: %d\n", re, st);
    
    return 0;
}
