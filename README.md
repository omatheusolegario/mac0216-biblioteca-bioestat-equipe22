# libbioestat - Biblioteca de Bioestatística e Aplicação Cliente CLI

A libbioestat é uma biblioteca modular escrita toda em C, que faz cálculos estatísticos, análise de testes diagnósticos, dados epidemiológicos e visualização de gráficos em ASCII e SVG/HTML. Aqui temos tanto uma biblioteca estática/dinâmica quanto uma aplicação CLI de terminal para utilizá-la.

---
## Estrutura do Projeto

```text
.
├── include/            # Cabeçalhos públicos da biblioteca
│   ├── basico.h        # Média, mediana, moda, variância, desvio padrão, quartis
│   ├── diagnostico.h   # Sensibilidade, especificidade, VPP, VPN
│   ├── epidemiologia.h # Risco relativo, Odds Ratio, riscos atribuíveis, tabelas 2x2
│   ├── relacoes.h      # Covariância, correlação de Pearson, regressão linear
│   └── visualizacao.h  # Geração de gráficos ASCII (Canvas) e geração SVG/HTML
├── src/                # Implementação dos módulos da biblioteca
├── app/                # Aplicação cliente interativa
│   ├── main.c          # Ponto de entrada (suporta modo interativo e `--demo`)
│   ├── dados.c/.h      # Gerenciamento da struct compartilhada e memória heap
│   ├── menus.h         # Interface e protótipos de navegação dos menus
│   └── menu_*.c        # Menus específicos de cada módulo
├── Makefile            # Automação de compilação, testes, execução e documentação
└── Doxyfile            # Configuração de geração de documentação Doxygen
```
---
# Funcionalidades
1. Epidemiologia
- Preenchimento de tabelas 2x2.
- Cálculo de Risco nos Expostos e Não Expostos.
- Risco Relativo (RR), Odds Ratio (OR) e Diferença de Riscos.

2. Testes Diagnósticos
- Sensibilidade e Especificidade.
- Valor Preditivo Positivo (VPP) e Valor Preditivo Negativo (VPN).

3. Estatística Básica
- Média, Mediana e Moda.
- Variância Amostral e Desvio Padrão.
- Quartis.

4. Relações entre Variáveis
- Covariância e Correlação Linear.
- Regressão Linear Simples.
- Previsão de y para um determinado x.

5. Visualização
- ASCII: Dispersão, Histograma e Boxplot direto no terminal.
- SVG/HTML: gráficos vetoriais que rodam no navegador, incluem Dispersão, Histograma e Boxplot.

# Compilação e execução

O projeto possui um makefile para automação da build, a seguir temos o que cada comando make faz:

## Compilar a biblioteca e a aplicação
```bash
make
```

## Executar a demonstração automatizada (--demo)
```bash
make run
```

## Executar menu interativo (CLI)
```bash
./bin/app_estatica
# ou
./bin/app_dinamica
```

## Rodar os testes unitários
```bash
make test
```

## Gerar documentação
```bash
make doc
```
## Incluir a biblioteca no sistema
```bash
# O prefixo padrão é /usr/local, escolher outro é opcional
sudo make install PREFIX=seu_caminho
```
Para instalar sem privilégios administrativos, basta fazer:
```bash
make install PREFIX=$HOME/.local
```

## Tirar a biblioteca no sistema
```bash
# O prefixo padrão é /usr/local, escolher outro é opcional
sudo make uninstall PREFIX=seu_caminho
```
Para remover sem privilégios administrativos, basta fazer:
```bash
make uninstall PREFIX=$HOME/.local
```

## Limpar arquivos compilados
```bash
make clean
```

# Tratamento de Erros

- Tratamento de erros embutido (be_status): todas funções da biblioteca retornam um código de erro padronizado através de um enum próprio, que possui mensagens de erros tratadas para cada tipo de erro.
- Alocação, liberação e tratamento de edge cases especificos para impedir acessos a locais da memória fora do programa, e problemas, como por exemplo divisão por 0.