#include <stdio.h>
#include "menus.h"
/**
 * Esqueleto - veja o meu para se basear, ou não tambem kskskksks 
 * Não precisa validar entradas... a biblio já faz isso e espera-se dados razoáveis, apenas retornar o erro... 
 * 
*/
void menu_diagnostico(Dados *d){
    (void)d;   // tire esta linha quando usar o parâmetro
    print_header("Testes diagnosticos");
    printf("\n  [em construcao]\n");
}

void demo_diagnostico(void){
    print_header("Demonstracao: testes diagnosticos");
    printf("\n  [em construcao]\n");
}
