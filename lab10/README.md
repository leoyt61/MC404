# Atividade de Laboratório 10

## Objetivos

O objetivo desta atividade é exercitar a manipulação (programação) de periféricos através de entrada e saída mapeada em endereços de memória (MMIO) utilizando a linguagem de montagem do RISC-V.

## Descrição

Neste laboratório, você realizará a mesma atividade do laboratório 9. Entretanto, você não terá mais acesso às chamadas de sistema para controlar o robô ou ler entrada. Você deverá implementar suas próprias chamadas de sistema para controlar o robô.

## Mapeamento do dispositivo em memória

Após carregar o dispositivo "Robot Vacuum Cleaner", os seguintes registradores do dispositivo estarão mapeados em endereços de memória. "Valor" representa o valor lido ou escrito no/do endereço de memória.
|Endereço |	Função|
|---------|-----------|
0xFFFF000C |	O robô irá configurar o torque dos motores 1 e 2 para, respectivamente, (valor >> 16) e (valor & 0xFFFF) N dm (Newton decímetros).|
0xFFFF0004 |	Quando atribuído valor = 0, inicia a leitura da posição e rotação do robô.<br>Quando o robô terminar de efetuar as leituras, o valor 1 é atribuído a este registrador.|
0xFFFF0008 |	Quando o valor de 0xFFFF0004 é 1, este registrador armazena: (rot << 20) \| (x << 10) \| z<br>
* rot é rotação atual do robô medida em graus, com valores entre 0 e 360.<br>
* x é o valor da posição do robô no eixo x em decímetros.<br>
* z é o valor da posição do robô no eixo z em decímetros.<br>|

## Obtenção de Coordenadas e Rotação

Para obter as coordenadas e rotação, o seguinte algoritmo pode ser executado.

* Armazena valor 0 em 0xFFFF0004;
* Lê 0xFFFF0004 até que o valor dele seja 1;
* Lê o valor de 0xFFFF0008.

**Importante**: Você deve acessar os registradores mapeados em memória apenas dentro das syscalls.

## Implementação das Syscalls

Para o controle do robô, você deve implementar as mesmas syscalls especificadas no laboratório 9. Recomendamos a leitura da especificação das chamadas no enunciado do laboratório 9.

Para simplificar o problema, não será necessário a implementação de funções de entrada e saída, ou de medição de tempo. Assim:

* Você pode assumir que o nome do ponto de destino já estará na memória, num rótulo de nome "destino".
* Não é necessário fazer qualquer impressão. Basta levar o robô ao destino escolhido, similarmente ao laboratório 9.

Você deve utilizar o arquivo ![lab10.s](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab10/lab10.s) como base para implementar sua solução. Ele implementa um tratador de interrupções/exceções que não faz nada além de retornar para a instrução seguinte àquela em que a interrupção/exceção ocorreu. Quando a instrução ecall é executada, uma exceção é gerada, e a execução é desviada para o tratador.

### Configuração da Infraestrutura e da compilação

Neste laboratório, é você quem está implementando funções que seriam do sistema operacional. Assim, é necessário desabilitar a emulação de syscalls do simulador desativando a opção "Emulate Syscalls". Além disso, o processo de compilação deverá ser realizado utilizando diretamente o montador e o ligador, como a seguir. Você não deve utilizar o GCC.
```
$ riscv32-unknown-elf-as lab10.s -o lab10.o
$ riscv32-unknown-elf-ld lab10.o -o lab10
```
