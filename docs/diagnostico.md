# Módulo de testes diagnósticos

# Visão geral do Módulo
O objetivo desse módulo é avaliar a qualidade de um teste diagnóstico. Para todas as funções, é dada uma tabela 2x2 que cruza a condição real da pessoa com o resultado do teste.

# O que foi desenvolvido
- Funções de diagnóstico
    - be_sensibilidade: Calcula a proporção das pessoas indentificadas como positivas, dentre as que estão verdadeiramente doente. Assim, avalia a capacidade do teste indentificar quem tem a doença.
    - be_especificidade: Calcula a proporção das pessoas indentificadas como negativas, dentre as que verdadeiramente não estão doentes. Assim, avalia a capacidade do teste indentificar quem não tem a doença.
    - be_vpp: Valor preditivo positivo - calcula a probabilidade da pessoa estar doente, dado que o teste deu positivo.
    - be_vpn: Valor preditivo negatico - calcula a probabilidade da pessoa não estar doente, dado que o teste deu negativo.

# Decisões de arquitetura e design
- Todas as funções do módulo utilizam a struct be_tabela2x2 criada no módulo epidemiologia.
- Como o retorno de cada função é o status de se houve erro ou não, cada função também recebe um ponteiro e escreve nesse endereço o valor calculado pela função.

# Tratamento de erros e casos de borda
- Todas as funções do módulo verificam, antes de fazer o cálculo, se:
    - Tem algum ponteiro nulo.
    - Tem algum valor negativo na tabela.
    - O cálculo exigiria uma divisão por zero.
- Em qualquer um desses casos, é retornado um código de erro apropriado.

# Dificuldades encontradas
- Inicialmente, não tinhamos determinado como o tratamento de erro seria feito. As funções tiveram que ser adaptadas para retornar o be_status e escrever o resultado em um endereço de memória dado por um ponteiro.

# Testes
- Existem testes para cada função desse módulo em tests/test_diagnostico.c, além de testes com valores normais, tem testes que verificam se as funções retornam o código de erro apropriado em casos com entrada inválida.