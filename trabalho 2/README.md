# Trabalho 2: Sistema de software do Uóli

## Introdução

Uóli é um robô explorador que deve percorrer um terreno em um planeta inóspito em busca de seus amigos para transferir informações importantes para que a equipe possa concluir sua missão. Durante o trajeto percorrido, o Uóli deve evitar regiões de perigo e evitar a colisão com obstáculos.

Neste segundo trabalho da disciplina, você vai desenvolver todas as camadas de software responsáveis pelo controle do robô Uóli. Essas camadas, ilustradas na Figura 1, são divididas em três subcamadas: (a) Sistema Operacional UóLi (SOUL), (b) Biblioteca de Controle (BiCo) e (c) Lógica de Controle (LoCo).
![](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/t2/files/pilha-sw-uoli.png)

Figura 1: Pilha de software para controle do robô Uóli.

A subcamada SOUL é responsável pelo gerenciamento do hardware, incluindo a configuração do hardware e o tratamento de interrupções de hardware e de software. Além disso, o SOUL deve prover um conjunto de serviços para a subcamada BiCo através de chamadas de sistemas, ou syscalls. A subcamada SOUL contém código que será executado em modo supervisor e deve ser implementada em linguagem de montagem.

A subcamada BiCo é responsável por prover uma interface de programação amigável para a Lógica de Controle, a API de Controle. A subcamada BiCo também deve ser implementada em linguagem de montagem, mas seu código será executado no modo usuário e ligado com o código da subcamada LoCo com o auxílio do ligador (*linker*).

A subcamada LoCo é responsável pela lógica de controle do robô e deve invocar as funções definidas pela API de Controle e implementadas pela BiCo. A subcamada LoCo deve ser implementada em código na linguagem C e seu código será executado no modo usuário. Como informado acima, o código da LoCo deverá ser ligado ao código da BiCo com o ligador.

## Subcamada LoCo

O código da subcamada LoCo deve ser implementado em linguagem C e deve fazer uso das rotinas disponíveis na API de Controle para enviar comandos para o robô. A API está descrita no arquivo "![api_robot.h](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/t2/files/api_robot.h)".

Você desenvolverá um programa na linguagem C para a camada LoCo: o loco.c.

## Lógica de controle do programa

Com base nas coordenadas dos amigos, armazenadas na variável "friends_locations", e das posições perigosas, armazenadas na variável "dangerous_locations" (veja o arquivo ![api_robot.h](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/t2/files/api_robot.h)), a lógica de controle do programa deve:

* Movimentar o Uóli pelo terreno com o objetivo de encontrar e transmitir informações para os amigos e evitar as posições perigosas.
* O Uóli deve passar a pelo menos 5 metros do amigo para que a informação possa ser transferida.
* O Uóli deve se manter a pelo menos 10 metros de distância de posições perigosas para evitar problemas.
*Note que o Uóli não consegue subir montanhas muito íngremes. Você deve identificar estes casos e programá-lo para que ele contorne os obstáculos.
* O terreno poderá conter obstáculos, além de montanhas muito íngremes. Estes obstáculos devem ser desviados, pois a colisão com estes pode afetar a trajetória ou mesmo o funcionamento do Uóli.

Importante: O número de elementos nos arranjos "friends_locations" e "dangerous_locations" pode ser lido com o comando sizeof. Por exemplo:
```
int n = sizeof(friends_locations) / sizeof(friends_locations[0]);
```

#### Limite de Tempo

Para o sucesso da missão, é importante que o Uóli transfira a informação para todos os amigos em menos de 10 minutos e não leve mais de 3 minutos entre um amigo e outro.

## Subcamada BiCo

O código da subcamada BiCo deve implementar as rotinas da API de Controle em linguagem de montagem do RISC-V. A API está descrita no arquivo "![api_robot.h](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/t2/files/api_robot.h)". Para controlar o hardware, o código deve realizar chamadas ao sistema, ou syscalls. As syscalls são definidas abaixo.

## Subcamada SOUL

A subcamada SOUL deve gerenciar o hardware do sistema e prover serviços para a subcamada BiCo através das chamadas de sistemas.

### Tempo do sistema (system time)

O SOUL possui um relógio interno que mantém o tempo do sistema, ou system time. O tempo do sistema deve ser iniciado com 0 sempre que o sistema for (re)iniciado e o tempo deve ser incrementado de 1 100 unidade a cada milissegundo100 milissegundos. Você deve configurar o periférico General Purpose Timer (GPT) para gerar interrupções a cada 100 milissegundos e incrementar o relógio interno de uma 100 unidades a cada interrupção.

### Atendendo chamadas de sistemas (syscalls)

Você deve programar o SOUL para atender as chamadas de sistemas descritas a seguir. Seu tratador de chamadas de sistema deve analisar o valor contido no registrador a7 para identificar a chamada de sistema solicitada. Veja a atividade de ![laboratório 10](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab10/lab10.html) para mais informações sobre o tratamento de chamadas de sistemas.

#### Descrição das Syscalls

A tabela abaixo apresenta os dados das syscalls do sistema. Caso ocorra mais de um erro na execução da chamada de sistema, o código retornado deve ser o do erro de maior valor.

|Syscall 	|Parâmetros 	|Retorno|
|---------------|---------------|-------|
*read_ultrasonic_sensor*<br>Código: 16 	|- 	        |a0: Valor obtido na leitura do sensor; -1 caso nenhum objeto tenha sido detectado a menos de 600 centímetros.|
*set_servo_angles*<br>Código: 17| 	a0: id do servo a ser modificado.<br>a1: ângulo para o servo.| 	a0: -1, caso o ângulo de um dos servos seja inválido (neste caso, a operação toda deve ser cancelada e nenhum ângulo definido). -2, caso o id do servo seja inválido. Caso contrário, retorna 0.|
*set_engine_torque*<br>Código: 18 |	a0: id do motor (0 ou 1)<br>a1: torque do motor. 	|a0: -1, caso o id do motor seja inválido. 0, caso contrário. A chamada de sistema não deve verificar a validade dos valores de torque.|
*read_gps*<br>Código: 19| 	a0: Endereço do registro (com três valores inteiros) para armazenar as coordenadas (x, y, z); |	- |
*read_gyroscope*<br>Código: 20 |	a0: Endereço do registro (com três valores inteiros) para armazenar os ângulos de Euler (x, y, z); |	- |
*get_time*<br>Código: 21 |	- |	a0: tempo do sistema, em milissegundos |
*set_time<br>Código: 22 |	a0: tempo do sistema, em milissegundos 	| - |
*write*<br>Código: 64 | 	a0: Descritor do arquivo<br>a1: Endereço de memória do buffer a ser escrito.<br>a2: Número de bytes a serem escritos.| 	a0: Número de bytes efetivamente escritos. |

## Iniciando o Sistema

Ao iniciar o sistema, o SOUL deve realizar duas atividades: (a) configurar o hardware e (b) transferir a execução para a aplicação de controle no modo usuário.

### Configurando o Hardware

O SOUL deve ter uma rotina de inicialização chamada "_start" que deve ser executada quando o sistema é ligado. Esta rotina deve:

* Configurar o GPT para gerar interrupção após 1100 ms;
* Configurar o torque dos dois motores para zero;
* Configurar as articulações da cabeça do Uóli para a posição natural (Base = 31, Mid = 80, Top = 78);
* Configurar o tratamento de interrupções;

### Transferindo a execução para a aplicação de controle

Após configurar o hardware, o SOUL deve transferir a execução para a aplicação de controle. Para isso, o SOUL deve:

* Habilitar interrupções;
* Configurar a pilha do usuário e do sistema;
* Mudar para o modo usuário;
* Desviar o fluxo para a função main do programa do usuário;

## Descrição dos periféricos

Após carregar o dispositivo "Uóli", os seguintes registradores do dispositivo estarão mapeados em endereços de memória. "Valor" representa o valor lido ou escrito no/do endereço de memória.

Endereço 	Tamanho 	Função
0xFFFF0004 	word 	Quando atribuído valor = 0, inicia a leitura da posição e rotação do robô.
Quando o robô terminar de efetuar as leituras, o valor 1 é atribuído a este registrador.
0xFFFF0008 	word 	Quando o valor de 0xFFFF0004 é 1, este registrador armazena o valor da posição do robô no eixo x em decímetros.
0xFFFF000C 	word 	Quando o valor de 0xFFFF0004 é 1, este registrador armazena o valor da posição do robô no eixo y em decímetros.
0xFFFF0010 	word 	Quando o valor de 0xFFFF0004 é 1, este registrador armazena o valor da posição do robô no eixo z em decímetros.
0xFFFF0014 	word 	Quando o valor de 0xFFFF0004 é 1, este registrador armazena o ângulos de Euler que representam a rotação do robô, no formato abaixo , onde x, y e z são os ângulos nos respectivos eixos.
0xFFFF0018 	half 	A escrita neste registrador configura o torque do motor 2 do Uóli para valor N m (Newton metros).
0xFFFF001A 	half 	A escrita neste registrador configura o torque do motor 1 do Uóli para valor N m (Newton metros).
0xFFFF001C 	byte 	A escrita neste registrador configura o ângulo do servo-motor 3 (top) para valor graus.
0xFFFF001D 	byte 	A escrita neste registrador configura o ângulo do servo-motor 2 (mid) para valor graus.
0xFFFF001E 	byte 	A escrita neste registrador configura o ângulo do servo-motor 1 (base) para valor graus.
0xFFFF0020 	word 	Quando atribuído valor = 0, inicia a leitura do sensor de ultrassom.
Quando o robô terminar de efetuar as leituras, o valor 1 é atribuído a este registrador.
0xFFFF0024 	word 	Quando o valor de 0xFFFF0020 é 1, este registrador armazena o valor retornado pelo sensor de ultrassom em centímetros.
Ou, então, o valor -1 caso não hajam objetos dentro do raio de detecção.
0xFFFF0100 	word 	Se o valor escrito for 0, o GPT não irá gerar interrupções.
Se o valor escrito for maior do que 0, o GPT aguardará "valor" milissegundos e então gerará uma interrupção. Caso você deseje continuar gerando interrupções a cada "valor" milissegundos, você deve escrever "valor" novamente neste registrador durante o tratamento da interrupção.
0xFFFF0104 	byte 	Quando valor for 1, indica a existência de uma interrupção não-tratada do GPT.
Você deve atribuir valor 0 para indicar que a interrupção do GPT foi tratada (faça isso durante o tratamento da interrupção do GPT).
0xFFFF0108 	byte 	Quando atribuído valor 1, a UART inicia a transmissão do valor armazenado em 0xFFFF0109.
Quando a transmissão terminar e a UART estiver pronta para iniciar a transmissão de um novo byte, o valor 0 é atribuído ao registrador.
0xFFFF0109 	byte 	Valor a ser transmitido pela UART.
0xFFFF010A 	byte 	Quando atribuído valor 1, a UART inicia a recepção de um byte na entrada e o armazena em 0xFFFF010B. Quando o processo de recepção terminar e a UART estiver pronta para iniciar a recepção de um novo byte, o valor 0 é atribuído ao registrador.
0xFFFF010B 	byte 	Valor recebido pela UART.

### UART

A UART é um dispositivo universal de comunicação serial. No caso da plataforma do Uóli, ela está ligada ao terminal STDIN e STDOUT do simulador, portanto, dados transmitidos pela UART serão escritos no terminal de saída STDOUT e dados recebidos pela UART serão lidos do terminal STDIN.
### GPT

O General Purpose Timer, ou GPT, é um periférico que pode ser programado para gerar interrupções periódicas no processador. O GPT possui três registradores de controle associados aos endereços 0xFFFF0100, 0xFFFF0104 e 0xFFFF0108, descritos acima.

Note que, para usar o GPT, o programa pode escrever no registrador 0xFFFF0100 o valor X para que o GPT gere uma interrupção após X milissegundos. Após X misissegundos, o GPT levantará o sinal de interrupção (GPT-Int). Supondo que a CPU esteja executando código do usuário e o tratamento de interrupções esteja habilitado, a CPU desviará o fluxo de execução para o tratator de interrupções.

Durante o tratamento da interrupção do GPT, o código de tratamento da interrupção deve escrever 0 no registrador 0xFFFF0104 do GPT para indicar que a interrupção foi tratada. Após observar que o valor zero foi escrito neste registrador, o GPT abaixa o sinal de interrupção (GPT-Int). No entanto, é importante observar que o GPT é um periférico lento e ele pode demorar para abaixar o sinal GPT-Int. Em função disso, após finalizar o tratamento da interrupção e retornar ao código do usuário, é possível que o sinal do GPT-Int ainda esteja levantado. Neste caso, o processador invocará novamente o tratador de interrupção para tratar esta interrupção falsa. O código do tratador deve identificar interrupções falsas do GPT checando se o mcause indica que a interrupção foi causada pelo GPT (e não por uma syscall) e se o valor do registrador 0xFFFF0104 é zero. Caso estas condições sejam verdadeiras, a interrupção é falsa e o código do tratador de interrupção deve retornar para o código do usuário.

## Apêndice A: Hardware

A Figura 2 mostra um diagrama com os servomotores do pescoço do Uóli e seus respectivos valores limites:
![](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/t2/files/servomotores-cabeca.png)
Figura 2: Diagrama dos servomotores do pescoço do Uóli (Top, Mid e Base) e seus respectivos valores limites.

## Apêndice B: Comandos do modo Interactive

Comandos do modo interactive:
```
help []
  Print help for given command or for all commands if no command given.

run
  Run till interrupted.

until 

  Run until address or interrupted.

step []
  Execute n instructions (1 if n is missing).

peek
  Print value of resource res (one of r, f, c, m) and address addr.
  For memory (m) up to 2 addresses may be provided to define a range
  of memory locations to be printed.
  examples: peek r x1   peek c mtval   peek m 0x4096

peek pc
  Print value of the program counter.

peek all
  Print value of all non-memory resources.

poke res addr value
  Set value of resource res (one of r, c or m) and address addr.
  Examples: poke r x1 0xff  poke c 0x4096 0xabcd

disass opcode   ...
  Disassemble opcodes. Example: disass opcode 0x3b 0x8082

disass function 
  Disassemble function with given name. Example: disass func main

disass  >
  Disassemble memory locations between addr1 and addr2.

elf file
  Load elf file into simulated memory.

hex file
  Load hex file into simulated memory.

replay_file file
  Open command file for replay.

replay n
  Execute the next n commands in the replay file or all the
  remaining commands if n is missing.

replay step n
  Execute consecutive commands from the replay file until n
  step commands are executed or the file is exhausted.

reset []
  Reset hart.  If reset_pc is given, then change the reset program
  counter to the given reset_pc before resetting the hart.

quit
  Terminate the simulator.
```
