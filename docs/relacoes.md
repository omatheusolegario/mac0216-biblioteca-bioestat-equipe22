# Módulo de relações

# Visão geral do Módulo
O objetivo desse módulo é medir a relação entre duas variáveis representadas por séries de dados pareadas.

# O que foi desenvolvido
- Structs:
    - rel_reta, para agrupar inclinacao, intercepta e r2.
    - somas, para agrupar as médias de cada variável, sxx (soma de (x - média de x)^2), syy (soma de (y - média de y)^2) e sxy (soma de (x - média de x) * (y - média de y)).

- Funções de relações:
    - be_covariancia: Calcula a covariância populacional (sxy / n).
    - be_correlacao: Calcula a correlação de Pearson (sxy / sqrt(sxx * syy))
    - be\_regressao\_linear: Faz uma regressão linear simples.
    - be_prever: Calcula o valor de y previsto por uma reta de regressão.

- Funções auxiliares (privadas):
    - calcular_somas: Valida as séries e preenche uma somas.
    - constante: Informa se todos os valores de um vetor são iguais.
    - calcular_corr: Calcula a correlação, verifica se há divisão por zero e conserta erros de arredondamento.

# Decisões de arquitetura e design
- As médias e as somas dos desvios são calculadas uma única vez em calcular_somas. Isso simplifica as fórmulas de covariância, correlação e regressão.
- O módulo básico é reutilizado para o cálculo das médias. Além de calcular as médias, be_media também valida o ponteiro e valores.
- A covariância calculada é a populacional (divide por n).
- A regressão reaproveita calcular\_corr, pois r2 é correlação ao quadrado. Por conta disso, be\_regressao\_linear não precisa tratar o caso de divisão por zero diretamente, pois isso é feito em calcular\_corr.
- constante faz uma comparação exata com valores. A comparação com as médias pode dar errado devido à aproximações.

# Tratamento de erros e casos de borda
- O ponteiro de saída é verificado primeiro. Depois, verifica-se os ponteiros, os valores e a divisão por zero, nessa ordem.
- Divisão por zero pode ocorrer quando sxx * syy é zero. Isso ocorre quando uma das duas variáveis é constante.
- Não há erro de memória, já que esse módulo não aloca memória.

- Casos de borda tratados:
    - n == 1: Covariância vale 0. Correlação e regressão devolvem erro de divisão por zero.
    - n == 2: Correlação é 1 ou -1.
    - Alguma variável é constante (n == 1 cai dentro desse caso): Covariância vale 0. Correlação e regressão devolvem erro de divisão por zero.
    - Sem relação linear: Covariância e correlação nulas.
    - Saídas intactas em caso de erro.
# Dificuldades encontradas
- Verificar o status das chamadas internas.
- Séries constantes e arredondamentos. Testar den == 0.0 não era o suficiente por conta de arredondamentos.
- Regressão com dados_y constante. r2 fica indefinido.

# Testes
- Os testes abordam:
    - Casos com entrada normal;
    - Casos de borda;
    - Casos com ponteiro nulo;
    - Casos com valor inválido;
