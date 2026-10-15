/*
 *  Aplicação cliente da libbioestat: menu interativo com os módulos do grupo.
 *  Usa APENAS headers públicos (include/) e os headers internos da aplicação (app/).
 *
 *  Uso:  ./bin/app_estatica          menu interativo
 *        ./bin/app_estatica --demo   demonstração sem pedir nada (usada pelo make run)
 */
#include <stdio.h>
#include <string.h>
#include "dados.h"
#include "entrada.h"
#include "menus.h"

static void demonstracao(void){
    demo_epidemiologia();
    demo_diagnostico();
    demo_basico();
    demo_relacoes();
    demo_visualizacao();
}

int main(int argc, char *argv[]){
    if (argc > 1) {
        if (argc == 2 && strcmp(argv[1], "--demo") == 0) {
            demonstracao();
            return 0;
        }
        fprintf(stderr, "Uso: %s [--demo]\n", argv[0]);
        return 1;
    }

    // Dados que o usuário carrega; os menus dos módulos usam estes dados
    Dados dados = {0};
    long opcao = -1;

    while (opcao != 0) {
        printf("=== libbioestat ===\n");
        printf("1) Entrada de dados\n");
        printf("2) Epidemiologia\n");
        printf("3) Testes diagnosticos\n");
        printf("4) Estatistica basica\n");
        printf("5) Relacoes entre variaveis\n");
        printf("6) Visualizacao\n");
        printf("0) Sair\n");

        // Se a entrada acabar (EOF), encerra o programa
        if (read_long("> ", &opcao) != 0) break;

        switch (opcao) {
            case 1: menu_entrada(&dados); break;
            case 2: menu_epidemiologia(&dados); break;
            case 3: menu_diagnostico(&dados); break;
            case 4: menu_basico(&dados); break;
            case 5: menu_relacoes(&dados); break;
            case 6: menu_visualizacao(&dados); break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    }

    dados_free(&dados);
    printf("Ate logo!\n");
    return 0;
}
