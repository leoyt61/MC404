# Atividade de Laboratório 9

## Objetivos

O objetivo desta atividade é introduzir a manipulação de dispositivos e exercitar o controle de um robô utilizando a linguagem de montagem do RISC-V.

## Descrição

Neste laboratório, você controlará um robô aspirador de pó utilizando chamadas de sistema. Seu objetivo será movê-lo até uma certa posição do mapa (definida pela entrada) e imprimir as coordenadas do caminho. O robô sempre começa na mesma posição e seus possíveis pontos de destino são os pontos marcados com X na imagem abaixo.
![](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab09/mapa.png)
Figura 1: Mapa.

São eles:

* Amarelo: Canto inferior esquerdo da sala, atrás da planta.
* Azul: Canto inferior direito da sala, ao lado da escada
* Verde: Embaixo da estante da sala.
* Vermelho: Atrás da porta do quarto.
* Rosa: Na entrada do banheiro.

Você deverá implementar um código em linguagem de montagem do RISC-V que lê o ponto destino da entrada e leva o robô até ele imprimindo as coordenadas do caminho.

## Entrada e Saída

A entrada será o nome do ponto de destino, seguido de uma quebra de linha.

Exemplo:
```
Azul
```	
A saída será as coordenadas do caminho percorrido pelo robô da origem até chegar no ponto destino.

* As coordenadas devem ser impressas conforme o robô as percorre.
* Elas deverão ser impressas ao menos uma e no máximo duas vezes por segundo.
* As coordenadas devem ser impressas em decímetros no formato "[s.ms] (x,z)\n", onde:
	* s é tempo em segundos;
	* ms é fração de tempo em milissegundos;
	* x é a coordenada do robô no eixo x;
	* e z é a coordenada do robô no eixo z;

Exemplo:
```
[1.0] (114, 60)
[2.0] (114, 57)
[3.0] (114, 54)
[4.0] (114, 51)
[5.0] (114, 48)
[6.0] (114, 46)
[7.0] (113, 43)
[8.0] (113, 40)
[9.0] (113, 37)
[10.0] (113, 35)
[11.0] (112, 32)
[12.0] (112, 32)
[13.0] (112, 31)
[14.0] (112, 31)
[15.0] (112, 31)
[16.0] (112, 31)
[17.0] (112, 31)
[18.0] (112, 31)
[19.0] (112, 31)
[20.0] (113, 31)
[21.0] (116, 31)
[22.0] (118, 31)
[23.0] (118, 31)
[24.0] (118, 31)
[25.0] (118, 31)
[26.0] (118, 31)
[27.0] (118, 31)
[28.0] (118, 31)
[29.0] (118, 31)
[30.0] (118, 31)
[31.0] (118, 31)
[32.0] (118, 31)
[33.0] (118, 31)
[34.0] (118, 31)
[35.0] (119, 31)
[36.0] (122, 31)
[37.0] (125, 31)
[38.0] (128, 30)
[39.0] (131, 30)
[40.0] (133, 30)
[41.0] (136, 30)
[42.0] (138, 29)
[43.0] (138, 29)
[44.0] (138, 29)
[45.0] (138, 29)
```

## Chamadas de sistema

As próximas seções descrevem as chamadas de sistema (syscalls) disponíveis para a realização desta atividade de laboratório.
### Movimentação do Robô

* Syscall número 2100
* Parâmetros:
	* a0: Tipo da operação:
		* Se a0 == 1, então o robô irá se mover para frente até que esteja a1 decímetros distante de sua posição inicial.
		* Se a0 == 2, então o robô irá rotacionar até que sua rotação seja a1 graus.
		* Se a0 == 3, então o robô irá configurar o torque dos motores 1 e 2 para, respectivamente, a1 e a2 N dm (Newton decímetros).
	* a1: Parâmetro que depende do tipo de operação. Veja a discrição de a0.
	* a2: Parâmetro que depende do tipo de operação. Veja a discrição de a0.
* Retorno: Não há.

### Obtenção de coordenadas e rotação

* Syscall número 2104
* Parâmetros: Não há.
* Retorno: a0 = (rot << 20) | (x << 10) | z
	* rot é rotação atual do robô medida em graus, com valores entre 0 e 360.
	* x é o valor da posição do robô no eixo x em decímetros.
	* z é o valor da posição do robô no eixo z em decímetros.

## Funções para leitura, impressão e cálculo de tempo

Para este laboratório, você poderá utilizar quaisquer funções das bibliotecas padrões da linguagem C que estejam disponível na ![Newlib](https://en.wikipedia.org/wiki/Newlib) e que funcionem em nosso simulador. Para isso, a compilação deverá ser feita com o gcc e seu código deverá implementar uma função chamada main. O exemplo abaixo mostra como utilizar a função printf para imprimir o retorno da syscall 2104.
```
.globl main
main:
    li  a7, 2104
    li  a0, 0
    ecall
    mv  a1, a0
    la  a0, mask
    jal printf
 
mask: .ascii "%x\n"
```
Para compilar, execute o comando:
```
riscv32-unknown-elf-gcc lab9.s -o lab9
```
Embora não seja obrigatório, sugerimos que sejam utilizadas as funções printf e scanf para escrita e leitura no terminal, além da função ![gettimeofday](http://man7.org/linux/man-pages/man2/gettimeofday.2.html) para obtenção do tempo.

## Uso da infraestrutura do simulador

**IMPORTANTE: Você deve usar a nova versão do simulador, disponibilizada em**![https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/sim3/web/static/sim.html](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/sim3/web/static/sim.html).

Para habilitar o simulador do robô **é necessário ir à aba "External Devices", seção "Robot Vacuum Cleaner", e clicar em "Load Device"**. Um novo frame se abrirá contendo o simulador gráfico. Note que o simulador captura seu cursor. Para desativar a captura e fazer uso da interface, é preciso apertar a tecla esc duas vezes.
