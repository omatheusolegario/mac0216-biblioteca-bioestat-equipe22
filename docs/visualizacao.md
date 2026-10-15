# Módulo de visualização

# Visão geral do Módulo
O objetivo desse módulo é oferecer suporte a geração de gráficos tanto rapidamente no terminal através de ASCII, quanto de maneira mais polida por arquivos vetoriais (SVG/HTML), para visualização no navegador.

# O que foi desenvolvido
- Structs: 
    - Canvas, para agrupar o ponteiro da matriz 2D, as dimensões e offset dela, reduzindo a complexidade na hora de manipular os desenhos em ascii.
    - Quartis, para agrupar os 3 quartis necessários para o boxplot e também reduzir a complexidade na hora de manipular esses dados dentro das funções.

- Funções de desenho em ASCII:
    - be_dispersao_ascii: Desenha o gráfico de dispersão, mapeando pares (Xi, Yi). Desenha esses pontos com o caracter '*' na matriz 2d, ajusta as medidas de acordo com a escala da matriz e gera uma legenda com o tamanho da amostra, e os range de valores que cada variável assume.
    - be_histograma_ascii: Desenha um histograma, baseado nos dados e número de intervalos inseridos. Preenche as barras com '#' na matriz 2d, ajusta as medidas de acordo com a escala da matriz e gera uma legenda com informações de cada intervalo.
    - be_boxplot_ascii: Desenha um boxplot, baseado nos dados e nos quartis inseridos. Preenche os "bigodes", a caixa principal, a mediana e desenha os outliers com o caracter '*'. Ajusta todas medidas de acordo com a escala da matriz e gera uma legenda com os valores de cada ponto crítico do boxplot.
    
- Funções de desenho em SVG/HTML:
    - be_dispersao_svg: Gera um arquivo html de nome escolhido contendo o gráfico de dispersão, mapeando pares (Xi, Yi). Desenha esses pontos com a tag `<circle>` e gera uma legenda mais completa que sua versão ASCII. Os eixos são desenhados através da tag `<line>`, usando valores da função para localizá-los adequadamente. Isso também vale para as demais tags.
    - be_histograma_svg: Gera um arquivo html de nome escolhido contendo o histograma, baseado nos dados e número de intervalos inseridos. Usa a tag `<rect>` para gerar as barras, usando valores da função para localizá-los adequadamente. Isso também vale para as demais tags. Também conta com rótulos adequados como densidade de frequência no eixo y, e intervalos no eixo x.
    - be_boxplot_svg: Gera um arquivo de html de nome escolhido contendo o boxplot, baseado nos dados e nos quartis inseridos. Utiliza a tag `<rect>`, `<line>,` `<circle>`, para a caixa principal, eixos, bigodes, linhas e outliers, que por sua vez usam valores da função para localizá-los adequadamente. Isso também vale para as demais tags. Também conta com os a localização no eixo x de cada ponto crítico escrito nesses locais no gráfico.
    
- Funcões auxiliares (privadas):
    - esvaziaCanvas: função que limpa a matriz 2D de chars onde o gráfico fica totalmente.
    - desenhaEixos: função que desenha os eixos na matriz 2D de chars.
    - imprimeCanvas: função que imprime no terminal a matriz 2D do gráfico.
    - achaMin: função que acha o valor mínimo num array dado.
    - achaMax: função que acha o valor máximo num array dado.
    - normaliza: função que pega um valor, o maior valor, o menor valor, calcula qual porcentagem relativa do valor dada a amplitude de valores disponíveis, e multiplica pela escala inserida, assim normalizando um valor à matriz por exemplo.
    
# Decisões de arquitetura e design

- O módulo funciona de modo completamente independente, necessitando apenas dos dados de entradas das funções, e devolvendo o canvas ou svg pronto.
- Os eixos x e y são dinâmicos, isto é, se movem de acordo com a origem quando necessário, para uma representação menos confusa dos valores.
- Integração com a convenção global dessa biblioteca de tratamento de erros, o enum be_status, fazendo cada função retornar um status (como BE_OK), facilitando a localização de um erro em sua ocorrência, ou de um sucesso.

# Tratamento de erros e casos de borda

- Todos ponteiros que entram nas funções desse módulo possuem checagem para saber se eles são referências reais ou apenas nulas, evitando que o C retorne um erro mais crítico de memória.
- Sempre que existe a possibilidade de ocorrer uma divisão por zero, esses casos são ou evitados no começo da função, ou contornados com checagem individual de valores e atribuição de um valor válido.
- Para uso de doubles, onde preciso existe uma checagem de precisão, para evitar por exemplo que um vetor seja acessado através de um índice não inteiro.
- Sempre que criado um arquivo logo em seguida é verificado se a criação realmente ocorreu, antes de qualquer operação dentro dele.

# Dificuldades encontradas

- Problemas com o uso de calloc e com a liberação da memória no momento errado.
- Muitos problemas com a não inicialização de variáveis, que por ficar com lixo na memória acabam causando valores indesejados.
- Inviabilidade de dentro da matriz escrever os valores dos pontos ou colocar rótulos nos eixos, a alternativa foi simplesmente legendar esses gráficos ASCII.
- Se adaptar da visão do desenho na mão para o desenho em coordenadas x e y tanto no ASCII quanto no SVG, e tornar isso de certa forma abstrato para qualquer valor ficar no lugar certo.
- Fazer os eixos serem relativos a posição da origem quando necessário, pois exige um recálculo geral.
- Confusão que a altura são as linhas (o eixo y é escrito a partir de um x fixo) e a largura as colunas (o eixo x é escrito a partir de um y fixo).
- Acertar os caracteres para uma visualização limpa no ASCII e no SVG acertar as cores e formas.

# Testes

- Existem testes unitários em tests/test_visualizacao.c, utilizando assert, de cada uma das funções, testando valores de borda, nulos, se para valores válidos os gráficos ASCII são desenhados corretamente e se para o SVG os arquivos estão sendo criados corretamente e não estão vazios.