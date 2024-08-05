# Implementação de Tabela Hash de Dimensão Dinâmica em C

Este projeto consiste na implementação de uma Tabela Hash com Dimensão Dinâmica em C, utilizando o encadeamento exterior para tratamento de colisões. A tabela hash ajusta dinamicamente seu tamanho com base no fator de carga (limiar para extensão), assegurando eficiência na inserção e busca de registros.

## Funcionalidades

- **Tratamento de Colisões**: Utiliza encadeamento exterior.
- **Dimensionamento Dinâmico**: A tabela se ajusta dinamicamente com base no fator de carga definido.
- **Registros de Clientes**: Estrutura dos registros inclui um código de cliente e um nome.

## Estrutura dos Registros

Cada registro na tabela hash representa um cliente com as seguintes informações:

- `codCliente`: Código do cliente (inteiro).
- `nome`: Nome do cliente (String de 100 caracteres).

## Fórmula de Hash

A fórmula de hash utilizada é: [x mod (m * 2l)]
onde:
- `m` é o tamanho inicial da tabela.
- `l` é o número de vezes que a tabela já foi duplicada.
- `p` indica o próximo compartimento a ser expandido.
  

## Parâmetros de Execução

- **Dimensão da Tabela**: A tabela ajusta seu tamanho inicial `m`.
- **Fator de Carga**: O fator de carga define o limiar para a extensão da tabela.

## Compilação e Execução

Para compilar e executar o programa, siga os passos abaixo:

1. Compile o código fonte utilizando `gcc`:
    ```sh
    gcc -o hash hash.c -lm
    ```

2. Execute o programa:
    ```sh
    ./hash
    ```
