#include <stdio.h>
#include "menus.h"
#include "../include/diagnostico.h"

static long demo = 0;

void menu_diagnostico(Dados *d){
    // Pega a tabela, se não tiver.
    if(!demo) if(ensure_table(d) != 0) return;

    long opcao = -1;

    while(opcao != 0)
    {
        if(!demo){
        print_header("Testes diagnosticos");
        printf("  1) Informar a tabela 2x2\n");
        printf("  2) Mostrar a tabela\n");
        printf("  3) Sensibilidade\n");
        printf("  4) Especificidade\n");
        printf("  5) Valor preditivo positivo\n");
        printf("  6) Valor preditivo negativo\n");
        printf("  7) Todas as medidas\n");
        printf("  0) Voltar\n");
        print_rule();
        
        if (read_long("> ", &opcao) != 0) return;
        printf("\n");
        }

        int stop = 1;
        double resultado = 0;
        be_status st;

        if(demo) opcao=7;
        switch(opcao)
        {
            case 1:
                if(read_table(d) == 0) {
                    printf("\n"); 
                    stop = 2; // stop = 2 quer dizer que vai descer pro próximo case, stop vai ser 1, e vai parar no case 2, onde mostra a tabela
                } else break;
                /* fall through */
            case 7: 
                stop--; // Se for mostrar todas as medidas, passa por todos os cases 
                /* fall through */
            case 2: 
                printf("                   Doente   Nao doente\n");
                printf("  Exposto        %8ld   %10ld\n", d->tabela.a, d->tabela.b);
                printf("  Nao exposto    %8ld   %10ld\n", d->tabela.c, d->tabela.d);
                if(stop) break;
                printf("\n");
                /* fall through */
            case 3:
                st = be_sensibilidade(&d->tabela, &resultado);
                printf("  Sensibilidade:                ");
                if(st == BE_OK) printf("%.4f\n", resultado);
                else printf("erro: %s\n", be_status_mensagem(st));
                if(stop) break;
                /* fall through */
            case 4:
                st = be_especificidade(&d->tabela, &resultado);
                printf("  Especificidade:               ");
                if(st == BE_OK) printf("%.4f\n", resultado);
                else printf("erro: %s\n", be_status_mensagem(st));
                if(stop) break;
                /* fall through */
            case 5:
                st = be_vpp(&d->tabela, &resultado);
                printf("  Valor preditivo positivo:     ");
                if(st == BE_OK) printf("%.4f\n", resultado);
                else printf("erro: %s\n", be_status_mensagem(st));
                if(stop) break;
                /* fall through */
            case 6:
                st = be_vpn(&d->tabela, &resultado);
                printf("  Valor preditivo negativo:     ");
                if(st == BE_OK) printf("%.4f\n", resultado);
                else printf("erro: %s\n", be_status_mensagem(st));
                if(stop) break;
                /* fall through */
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }
        if(demo) return;
    }

}

void demo_diagnostico(void){
    Dados d = {0};
    be_status st = be_preencher_tabela(80, 10, 20, 90, &(d.tabela));
    
    print_header("Demonstracao: testes diagnosticos");
    printf("\n");

    if (st != BE_OK) {
        printf("  Erro ao criar a tabela: %s\n", be_status_mensagem(st));
        return;
    }
    
    demo = 1;
    menu_diagnostico(&d);
    demo = 0;
}
