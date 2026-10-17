# Módulo de estatística básica

# Visão geral do Módulo
O objetivo desse módulo é reunir as medidas básicas de estatística sobre um conjunto de números reais.
Como ele contém essas medidas básicas, esse módulo é utilizado como um módulo de apoio, em especial, para o módulo de relações.

# O que foi desenvolvido
- Funções de estatística básica:
    - be_media: Calcula a média.
    - be_mediana: Calcula a mediana (quantil de ordem 0,5).
    - be\_quartil\_1: Calcula o primeiro quartil (quantil de ordem 0,25).
    - be\_quartil\_2: Calcula o segundo quartil (quantil de ordem 0,5).
    - be\_quartil\_3: Calcula o terceiro quartil (quantil de ordem 0,75).
    - be_variancia: Calcula a variância.
    - be\_desvio\_padrao: Calcula o desvio padrão (raiz quadrada da variância).
    - be_moda: Calcula a moda. 

- Funções auxiliares (privadas):
    - comparar: Compara dois double.
    - dados_ordenados: Devolve uma cópia ordenada do vetor de entrada.
    - quantil: Calcula o quantil de ordem p por interpolação linear. p vale entre 0 e 1.
    - tam_sequencia: Verifica o tamanho da sequência de valores iguais que começa a partir de determinada posição do vetor já ordenado.

# Decisões de arquitetura e design
- Mediana, quartis e moda precisam dos dados ordenados, mas o parâmetro é const double *. Por conta disso, dados_ordenados ordena uma cópia.
- Utiliza-se a função auxiliar quantil para os quartils e mediana, com p diferente, para evitar código repetido e garantir que a mediana e o segundo quartil coincidam. Para o cálculo, o método escolhido foi a interpolação linear.
- A variância calculada é a variância populacional (divide-se por n).
- Funções como be_\media e be\_variancia são reaproveitadas.
- be_moda passa duas vezes pelo vetor ordenado. A primeira passada descobre a maior frequência e quantos valores a possuem. Com esses dados, aloca-se um vetor com o tamanho exato. A segunda passada preenche esse vetor.
- Como a moda pode ser mais que um valor, be_moda devolve um vetor alocado com malloc em *\modas e a quantidade de valores em \*qtd.
- be\_moda somente aloca. Quem chama be\_moda deve liberar.

# Tratamento de erros e casos de borda
- Antes dos cálculos serem realizados, os ponteiros e então os valores são verificados.
- Não há erro de divisão por zero, já que n == 0 é rejeitado na verificação de valores.
- Há a verificação de erro de memória sempre que há uso de malloc.

- Casos de borda tratados
    - n == 1: Média, mediana, quartis e moda são o próprio valor. Variância e desvio padrão valem 0.
    - Todos os valores iguais (n == 1 cai dentro desse caso): Variância e desvio padrão valem 0. Moda é esse valor.
    - n == 2: Quartis são interpolados.
    - Moda com empate: Moda é todos os valores de maior frequência, ordenados, com cada valor distinto aparecendo uma única vez.
    - Moda sem repetições: Moda é todos os valores, ordenados.
    - Valores negativo
    - Vetor grande
    - Dados desordenados
    - Saídas intactas em caso de erro

# Dificuldades encontradas
- Retorno das funções ocupado pelo be_status. Não era mais possível retornar o resultado diretamente, foi necessário criar um parâmetro de saída e trabalhar com ponteiros.
- Moda com vários valores.

# Testes
- Os testes abordam:
    - Casos com entrada normal;
    - Casos de borda;
    - Casos com ponteiro nulo;
    - Casos com valor inválido;
- Em test\_basico há a utilização de um struct e funções auxiliares. Como todas as funções do módulo, menos be\_moda possuem mesmos parâmetros, elas são verificadas juntas.
- be_moda é testada separadamente.
