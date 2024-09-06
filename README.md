# Paralelismo Eficiente no Caixeiro Viajante com ILS-RVND
Este projeto implementa e compara duas versões do algoritmo Iterated Local Search com Random Variable Neighborhood Descent (ILS-RVND) para o Problema do Caixeiro Viajante (PCV). O objetivo principal é otimizar o tempo de execução do algoritmo através de paralelismo, utilizando a biblioteca OpenMP.

## Estrutura do Projeto

Código-fonte: A implementação está em C++, e o código foi organizado para suportar tanto a versão original do ILS-RVND quanto a versão paralela.

Instâncias: As instâncias do problema estão localizadas na pasta instances/, seguindo o formato da biblioteca TSPLIB.

Makefile: O projeto inclui um Makefile para facilitar a compilação.


## Compilação

Para compilar o programa, basta executar o comando:

bash:
```
make
```
Isso irá gerar o executável tsp.

Execução

O programa é executado com o seguinte comando:

bash:
```
./tsp instances/<nomeDaInstancia>
```
Substitua <nomeDaInstancia> pelo nome da instância a ser resolvida, por exemplo:

bash
```
./tsp instances/berlin52.tsp
```
Os resultados da execução serão exibidos no terminal.

## Requisitos

Compilador: GCC ou outro compatível com OpenMP.

Sistema Operacional: O projeto foi testado em Linux e Windows 10, mas deve funcionar em qualquer sistema com suporte a C++ e OpenMP.
