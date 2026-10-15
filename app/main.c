/*
 *  Aplicação cliente da libbioestat: menu interativo com os módulos do grupo.
 *  Usa APENAS headers públicos (include/) e os headers internos da aplicação (app/).
 *
 *  Uso:  ./bin/app_estatica          menu interativo
 *        ./bin/app_estatica --demo   demonstração sem pedir nada (usada pelo make run)
 */
#include <stdio.h>
#include <string.h>
#include "entrada.h"
#include "menus.h"

static void demonstracao(void){
    demo_epidemiologia();
    // Adicione aqui a demonstração do seu módulo
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

    long opcao = -1;

    while (opcao != 0) {
        printf("=== libbioestat ===\n");
        printf("1) Epidemiologia\n");
        // Adicione aqui a opção do seu módulo
        printf("0) Sair\n");

        // Se a entrada acabar (EOF), encerra o programa
        if (le_long("> ", &opcao) != 0) break;

        switch (opcao) {
            case 1: menu_epidemiologia(); break;
            // Adicione aqui o case do seu módulo
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    }

    printf("Ate logo!\n");
    return 0;
}
