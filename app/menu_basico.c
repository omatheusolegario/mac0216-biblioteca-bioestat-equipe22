#include <stdio.h>
#include "menus.h"

/*
 * Esqueleto do módulo "estatistica basica" (responsável: Gustavo).
 *
 * Este arquivo é seu: troque o conteúdo das duas funções pelo menu e pela demo do seu
 * módulo. Os nomes e as assinaturas já estão combinados com o main.c e o menus.h,
 * então não mude.
 *
 * Como eu fiz no menu_epidemiologia.c:
 *   1. #include o header público do seu módulo (include/basico.h), nunca o .c;
 *   2. no menu, comece com  if (ensure_vector(d) != 0) return;
 *      (usa o dado que o usuário carregou; se não tiver, pergunta se quer informar agora);
 *   3. pra ler do teclado use read_long / read_double (estão no menus.h);
 *   4. print_header, opções com 2 espaços de recuo, print_rule, laço com switch,
 *      opção 0 = voltar. Os erros vêm da biblioteca: mostre com be_status_mensagem(st);
 *   5. não precisa validar a entrada na CLI: o contrato da sua função já faz isso.
 */

void menu_basico(Dados *d){
    (void)d;   // tire esta linha quando usar o parâmetro
    print_header("Estatistica basica");
    printf("\n  [em construcao]\n");
}

void demo_basico(void){
    print_header("Demonstracao: estatistica basica");
    printf("\n  [em construcao]\n");
}
