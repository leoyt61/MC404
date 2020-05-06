# Atividade de Laboratório 6

## Objetivos

O objetivo desta atividade é exercitar o uso de instruções aritméticas e a manipulação de entrada e saída utilizando o conjunto de instruções da arquitetura RISC-V.

## Descrição

Neste laboratório, você deve fazer um programa em linguagem de montagem do RISC-V que calcule a sua posição geográfica num plano bidimensional, baseado no tempo atual e em mensagens recebidas de 3 satélites, como ilustrados na figura a seguir.
Ilustração dos três satélites no plano cartesiano.
![](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab06/satellites.jpg)

Para simplificar o problema, assumimos que o satélite A se encontra na origem no plano cartesiano (0, 0), enquanto B e C têm posições (0, YB) e (XC, 0), respectivamente. Os satélites enviam mensagens contendo uma Marca Temporal (timestamp) continuamente através de ondas que se propagam em todas as direções numa velocidade de 3 x 108 m/s. Em um dado instante TR, você recebeu uma mensagem de cada satélite contendo os tempos TA, TB e TC. Supondo que todos os tempos estejam perfeitamente sincronizados, imprima sua coordenada (x, y) no plano cartesiano. Note que a formulação utilizada neste exercício não é realista.

## Entrada e Saída

Seu programa deve ler da entrada padrão os valores YB, XC, TA, TB, TC e TR e imprimir na saída padrão a sua coordenada (x, y).

Entrada:

* Linha 1 - Coordenadas YB, XC: Valor em metros, representado por números inteiros de 4 dígitos na base decimal e precedido pelo sinal '+' ou '-'.
* Linha 2 - Tempos TA, TB, TC e TR: Valor em nanosegundos e representado por números naturais de 4 dígitos na base decimal.

Para realizar a leitura de dados, basta utilizar a chamada de sistema 63, como no template abaixo:
```
    li a0, 0 # file descriptor = 0 (stdin)
    la a1, input_adress #  buffer
    li a2, 1 # size (lendo apenas 1 byte, mas tamanho é variável)
    li a7, 63 # syscall read (63)
    ecall

input_adress:   # buffer
```

Saída:

* Sua coordenada (x, y): Valor em metros, <ins>**aproximado**</ins>, representado por números inteiros de 4 dígitos na base decimal e precedido pelo sinal '+' ou '-'.

Observações:

* Múltiplos valores impressos ou lidos na mesma linha serão separados por um único espaçamento.
* Cada linha é finalizada com o caracter '\n'.

Exemplo

Entrada:
```
+0700 -0100
2000 0000 2240 2300
```
Saída:
```
-0088 +0016
```
## Dicas
### Geometria do problema

Existem diversas formas de resolver o problema. Aqui, sugerimos a utilização das fórmulas do círculo. Seja dA, dB e dC as distâncias entre sua posição e os satélite A, B e C, respectivamente, temos que:

**x2 + y2 = dA2 	(Eq. 1)**
**x2 + (y - YB)2 = dB2 	(Eq. 2)**
**(x - XC)2 + y2 = dC2 	(Eq. 3)**

Assim, a partir das Equações 1 e 2, temos:
**y = (dA2 + YB2 - dB2) / 2YB 	(Eq. 4)**
**x = + sqrt(dA2 - y2) OU - sqrt(dA2 - y2) 	(Eq. 5)**

Para encontrar o x correto, basta substituir os dois valores possíveis na Equação 3 e verificar qual mais se aproxima de satisfazer a igualdade.

### Implementação da Raiz Quadrada

* Sugerimos a utilização do mesmo algoritmo utilizado no laboratório 2.
* 10 Iterações são suficientes.
* Neste laboratório, aceitaremos soluções aproximadas para o problema.

### Processamento dos dados de entrada

* Os dados de entrada são representados como cadeias de caracteres na codificação ASCII. Dessa forma, o valor 2240 é representado como a cadeia de caracteres '2', '2', '4', '0', que possuem os valores numéricos 50, 50, 52, e 48. Seu código deve converter esta cadeia de caracteres para valores numéricos nos registradores. Neste exemplo, a conversão se dá por (50-48)*103 + (50-48)*102 + (52-48)*101 + (48-48)*100 = 224010.
* +!= -: Lembre-se de processar o sinal.

A imagem a seguir uma tabela com os códigos ASCII.
![](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab06/1024px-Ascii_Table-nocolor.svg.png)
Fonte: ![https://commons.wikimedia.org/wiki/File:Ascii_Table-nocolor.svg](https://commons.wikimedia.org/wiki/File:Ascii_Table-nocolor.svg)

