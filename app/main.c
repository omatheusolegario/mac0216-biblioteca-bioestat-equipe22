/*
 *  Aplicação cliente da libbioestat: menu interativo com os módulos do grupo.
 *  Usa apenas headers públicos (include/) e os headers internos da aplicação (app/).
 *  
 *  Uso:  ./bin/app_estatica          menu interativo
 *        ./bin/app_estatica --demo   demonstração sem pedir nada (usada pelo make run, para que não ficasse esperando input)
 *
 */
#include <stdio.h>
#include <string.h>
#include "dados.h"
#include "menus.h"

// Lê um inteiro. Devolve 0 em sucesso e -1 se a entrada acabou (Ctrl+D).
// Se digitar algo que não é número, valor vira -1 e o menu mostra "Opcao invalida"
int read_long(const char *prompt, long *valor){
    char linha[64];

    printf("%s", prompt);
    if (fgets(linha, sizeof linha, stdin) == NULL) return -1;
    if (sscanf(linha, "%ld", valor) != 1) *valor = -1;
    return 0;
}

// Lê um número real (decimal com ponto: 3.5). Devolve 0 em sucesso e -1 se
// a entrada acabou ou se não for um número
int read_double(const char *prompt, double *valor){
    char linha[64];

    printf("%s", prompt);
    if (fgets(linha, sizeof linha, stdin) == NULL) return -1;
    if (sscanf(linha, "%lf", valor) != 1) return -1;
    return 0;
}

/* ---------- Visual dos menus ---------- */

void print_header(const char *titulo){
    printf("\n============================================\n");
    printf("  %s\n", titulo);
    printf("============================================\n");
}

void print_rule(void){
    printf("--------------------------------------------\n");
}

/* ---------- Menu de entrada de dados ---------- */

static void menu_entrada(Dados *d){
    long opcao = -1;

    while (opcao != 0) {
        print_header("Entrada de dados");
        printf("  1) Informar a tabela 2x2\n");
        printf("  2) Informar um vetor de numeros\n");
        printf("  3) Carregar dados de exemplo\n");
        printf("  4) Ver os dados carregados\n");
        printf("  0) Voltar\n");
        print_rule();

        if (read_long("> ", &opcao) != 0) return;
        printf("\n");

        switch (opcao) {
            case 1: if (read_table(d) == 0) printf("\n  Tabela gravada.\n"); break;
            case 2: if (read_vector(d) == 0) printf("\n  Vetor gravado (%zu valores).\n", d->n); break;
            case 3: if (load_example(d) == 0) printf("  Dados de exemplo carregados.\n"); break;
            case 4: print_data(d); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }
    }
}

/* ---------- Programa principal ---------- */

static void demonstracao(void){
    demo_epidemiologia();
    demo_diagnostico();
    demo_basico();
    demo_relacoes();
    demo_visualizacao();
}

int main(int argc, char *argv[]){
    // Criei uma demonstração básica com dados iniciais usando --demo quando roda make run.
    if (argc > 1) {
        if (argc == 2 && strcmp(argv[1], "--demo") == 0) {
            demonstracao();
            return 0;
        }
        fprintf(stderr, "Uso: %s [--demo]\n", argv[0]);
        return 1;
    }

    // Dados que o usuário carrega: todos os menus dos módulos usam estes mesmos dados
    Dados dados = {0};
    long opcao = -1;

    while (opcao != 0) {
        print_header("libbioestat - Biblioteca de Bioestatistica");
        printf("  1) Entrada de dados\n");
        printf("  2) Epidemiologia\n");
        printf("  3) Testes diagnosticos\n");
        printf("  4) Estatistica basica\n");
        printf("  5) Relacoes entre variaveis\n");
        printf("  6) Visualizacao\n");
        printf("  0) Sair\n");
        print_rule();

        // Se a entrada acabar, encerra o programa
        if (read_long("> ", &opcao) != 0) break;
        printf("\n");

        switch (opcao) {
            case 1: menu_entrada(&dados); break;
            case 2: menu_epidemiologia(&dados); break;
            case 3: menu_diagnostico(&dados); break;
            case 4: menu_basico(&dados); break;
            case 5: menu_relacoes(&dados); break;
            case 6: menu_visualizacao(&dados); break;
            case 0: break;
            default: printf("  Opcao invalida.\n");
        }
    }

    dados_free(&dados);
    printf("\nAte logo!\n");
    return 0;
}
