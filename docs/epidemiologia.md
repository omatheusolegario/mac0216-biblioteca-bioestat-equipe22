# Módulo de epidemiologia
Thiago Assumpção Baisch - 11229110

# Visão geral do Módulo
O objetivo desse módulo é comparar o risco de adoecer entre um grupo exposto e um grupo não exposto a algum fator. Para todas as funções, é dada uma tabela 2x2 que cruza a exposição da pessoa com o fato dela estar doente ou não.

A tabela segue o formato abaixo (o mesmo usado na documentação do código):

|                    | Doente | Não doente |
|--------------------|--------|------------|
| Exposto/Teste+     | a      | b          |
| Não exposto/Teste- | c      | d          |

# O que foi desenvolvido
- Structs:
    - be_tabela2x2, para agrupar as quatro contagens (a, b, c, d) da tabela. Os campos são long, porque são contagens de pessoas e podem ser grandes.

- Funções de epidemiologia:
    - be_preencher_tabela: Preenche uma be_tabela2x2 a partir de quatro contagens, validando que nenhuma é negativa.
    - be_risco_expostos: Calcula a probabilidade de adoecer entre os expostos (incidência cumulativa dos expostos), a / (a + b).
    - be_risco_naoexpostos: Calcula a probabilidade de adoecer entre os não expostos, c / (c + d).
    - be_risco_relativo: Calcula o risco relativo (RR), risco dos expostos dividido pelo risco dos não expostos. RR = 1 significa sem associação, RR > 1 a exposição aumenta o risco e RR < 1 a exposição protege.
    - be_odds_ratio: Calcula a razão de chances (OR), (a * d) / (b * c).
    - be_diferenca_riscos: Calcula a diferença de riscos (DR), risco dos expostos menos risco dos não expostos. Diz quantos casos a mais (ou a menos) a exposição causa em termos absolutos.

- Menu da aplicação (app/menu_epidemiologia.c):
    - menu_epidemiologia: Menu interativo para informar a tabela e calcular cada medida, ou todas de uma vez.
    - demo_epidemiologia: Demonstração usada pelo --demo, com a tabela {20, 80, 10, 90}.

# Decisões de arquitetura e design
- A be_tabela2x2 é a entrada padrão de todo o módulo. Ela também é reutilizada pelo módulo de diagnóstico, por isso ela fica definida em epidemiologia.h.
- Todas as funções seguem a convenção da biblioteca: o retorno é o be_status e o resultado é gravado em um ponteiro de saída (double *saida).
- A tabela é passada como const be_tabela2x2 *, já que as funções só leem os valores. Só be_preencher_tabela escreve na struct.
- be_risco_relativo e be_diferenca_riscos reaproveitam be_risco_expostos e be_risco_naoexpostos, em vez de repetir as fórmulas. Se uma das chamadas internas falha, o status dela é repassado.
- Nas divisões entre contagens, o numerador é convertido para double antes de dividir. Sem isso, a divisão seria inteira e o resultado ficaria errado (4 / 9 daria 0).
- No odds ratio, a e d (e b e c) são convertidos para double antes da multiplicação. Como são long, o produto a * d poderia estourar para contagens grandes.
- be_diferenca_riscos não testa divisão por zero diretamente, isso já é feito dentro das funções de risco.
- Os resultados só são escritos em *saida quando está tudo certo, então em caso de erro a saída fica como estava.

# Tratamento de erros e casos de borda
- Todas as funções verificam, nessa ordem: ponteiros nulos, valores negativos na tabela e divisão por zero.
- Divisão por zero em cada função:
    - be_risco_expostos: a + b == 0 (grupo dos expostos vazio).
    - be_risco_naoexpostos: c + d == 0 (grupo dos não expostos vazio).
    - be_risco_relativo: a + b == 0, c + d == 0 ou c == 0. O último caso é quando o risco dos não expostos é zero, ou seja, o denominador da razão.
    - be_odds_ratio: b == 0 ou c == 0.
    - be_diferenca_riscos: a + b == 0 ou c + d == 0.
- Não há erro de memória, já que esse módulo não aloca memória.

- Casos de borda tratados:
    - Tabela com zeros: be_preencher_tabela aceita 0 em qualquer célula.
    - a == 0: Risco dos expostos é 0. RR e OR valem 0 (se o outro grupo permite o cálculo).
    - b == 0: Risco dos expostos é 1. RR é válido, já que b não está no denominador do RR. Já o OR devolve erro de divisão por zero.
    - c == 0: Risco dos não expostos é 0. RR e OR devolvem erro de divisão por zero.
    - Riscos iguais: RR vale 1 e DR vale 0.
    - Exposição protetora: DR negativa e RR menor que 1 são resultados válidos, não são erros.
    - Grupo vazio (a + b == 0 ou c + d == 0): Divisão por zero nas medidas que dependem do risco daquele grupo.
    - Contagens muito grandes: OR calculado sem overflow.
    - Saídas intactas em caso de erro.

# Dificuldades encontradas
- Retorno das funções ocupado pelo be_status. As funções tiveram que ser adaptadas para gravar o resultado em um ponteiro de saída.
- Descobrir exatamente quando o RR e o OR têm divisão por zero. No RR, o denominador é o risco dos não expostos, então só c == 0 (e não b == 0) causa problema. No OR é o contrário, tanto b quanto c zerados dão erro. Foi preciso pensar caso a caso.
- Divisão inteira. Com os campos sendo long, esquecer de converter para double faz o risco virar 0 ou 1.
- Overflow no odds ratio com contagens grandes, que resolvemos convertendo para double antes de multiplicar.
- Decidir o que é erro e o que é só um resultado incomum. Por exemplo, DR negativa é válida (exposição protetora) e não deve devolver erro.

# Testes
- Existem testes para cada função desse módulo em tests/test_epidemiologia.c.
- Os testes abordam:
    - Casos com entrada normal, usando uma tabela de referência {20, 80, 10, 90} (risco dos expostos 0,2; risco dos não expostos 0,1; RR = 2; OR = 2,25; DR = 0,1) e uma tabela com DR negativa;
    - Casos de borda (a, b ou c zerados, riscos iguais, risco 0 e risco 1, contagens grandes);
    - Casos com ponteiro nulo (tabela e saída);
    - Casos com valor inválido (valor negativo em cada uma das células);
    - Casos de divisão por zero (grupos vazios, b == 0, c == 0);
- Nos testes de erro, a variável de saída começa com o valor -9.0, para verificar que ela não foi modificada quando a função devolve erro.
