#include <stdio.h>
#include "../include/epidemiologia.h"
#include "menus.h"

// Mostra a tabela 2x2 no mesmo formato da documentação
static void mostra_tabela(const be_tabela2x2 *t){
    printf("                   Doente   Nao doente\n");
    printf("  Exposto        %8ld   %10ld\n", t->a, t->b);
    printf("  Nao exposto    %8ld   %10ld\n", t->c, t->d);
}

// Chama uma medida e mostra o resultado (ou a mensagem de erro da biblioteca)
// 'medida' é um ponteiro para função: serve pra qualquer função com essa assinatura
static void mostra_medida(const char *nome,
                          be_status (*medida)(const be_tabela2x2 *, double *),
                          const be_tabela2x2 *t){
    double resultado;
    be_status st = medida(t, &resultado);

    if (st == BE_OK) {
        printf("  %-26s %.4f\n", nome, resultado);
    } else {
        printf("  %-26s erro: %s\n", nome, be_status_mensagem(st));
    }
}

static void mostra_todas(const be_tabela2x2 *t){
    mostra_tabela(t);
    printf("\n");
    mostra_medida("Risco nos expostos:", be_risco_expostos, t);
    mostra_medida("Risco nos nao expostos:", be_risco_naoexpostos, t);
    mostra_medida("Risco relativo:", be_risco_relativo, t);
    mostra_medida("Odds ratio:", be_odds_ratio, t);
    mostra_medida("Diferenca de riscos:", be_diferenca_riscos, t);
}

void menu_epidemiologia(Dados *d){
    // Aqui só preciso da tabela 2x2 (se não tiver, pergunta se quer informar agora)
    if (ensure_table(d) != 0) return;

    long opcao = -1;

    while (opcao != 0) {
        print_header("Epidemiologia");
        printf("  1) Informar a tabela 2x2\n");
        printf("  2) Mostrar a tabela\n");
        printf("  3) Risco nos expostos\n");
        printf("  4) Risco nos nao expostos\n");
        printf("  5) Risco relativo\n");
        printf("  6) Odds ratio\n");
        printf("  7) Diferenca de riscos\n");
        printf("  8) Todas as medidas\n");
        printf("  0) Voltar\n");
        print_rule();

        // Ctrl+D: volta pro menu principal
        if (read_long("> ", &opcao) != 0) return;
        printf("\n");

        switch (opcao) {
            case 1: if (read_table(d) == 0) { printf("\n"); mostra_tabela(&d->tabela); } break;
            case 2: mostra_tabela(&d->tabela); break;
            case 3: mostra_medida("Risco nos expostos:", be_risco_expostos, &d->tabela); break;
            case 4: mostra_medida("Risco nos nao expostos:", be_risco_naoexpostos, &d->tabela); break;
            case 5: mostra_medida("Risco relativo:", be_risco_relativo, &d->tabela); break;
            case 6: mostra_medida("Odds ratio:", be_odds_ratio, &d->tabela); break;
            case 7: mostra_medida("Diferenca de riscos:", be_diferenca_riscos, &d->tabela); break;
            case 8: mostra_todas(&d->tabela); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }
    }
}

void demo_epidemiologia(void){
    be_tabela2x2 tabela;
    be_status st = be_preencher_tabela(20, 80, 10, 90, &tabela);

    print_header("Demonstracao: epidemiologia");
    printf("\n");

    if (st != BE_OK) {
        printf("  Erro ao criar a tabela: %s\n", be_status_mensagem(st));
        return;
    }

    mostra_todas(&tabela);
}
