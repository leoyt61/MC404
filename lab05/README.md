# Atividade de Laboratório 5

## Objetivos

O objetivo deste laboratório é familiarizar a turma com a infraestrutura para montagem e execução de código RISC-V, que será utilizada no restante do curso. Nesta atividade, é esperado que o aluno compreenda como montar, ligar e executar, no simulador, um programa escrito em linguagem de montagem do RISC-V, além de depurar o código em linguagem de montagem.

## Descrição

Neste laboratório, você deve fazer um programa em linguagem de montagem do RISC-V que imprima seu nome seguido do seu RA na tela, na forma "Primeiro_nome - raXXXXXX". O código do programa em linguagem de montagem está disponível em ![modelo.s](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab05/modelo.s).

Note que o arquivo modelo.s está bastante comentado. Nessa atividade, **você deve alterar a string "MC404\n"** para **"Seu_nome - raXXXXXX\n" e o tamanho da string**, que é copiado para o registrador a2.

Executar um programa escrito em linguagem de montagem do RISC-V exige o uso de um simulador RISC-V, pois os computadores do laboratório possuem processadores com conjunto de instruções da família de arquiteturas x86, sendo assim incompatíveis com código RISC-V.

Desse modo, a não ser que se utilize um hardware RISC-V, é preciso executar uma sequência de passos para executar seu programa num computador da família x86.

Tal sequência, juntamente de uma breve descrição do funcionamento do simulador, está disposta na seção seguinte. Recomenda-se atenção aos passos aqui descritos, pois as etapas são necessárias para todos os futuros laboratórios.

## Simulador RISC-V

O simulador da arquitetura RISC-V usado nessa disciplina foi criado pela empresa Western Digital para simular o funcionamento do processador SweRV Core, que é utilizado no micro-controlador de mesmo nome ~[https://blog.westerndigital.com/risc-v-swerv-core-open-source/](https://blog.westerndigital.com/risc-v-swerv-core-open-source/). Sua implementação original foi escrita em C++, foi planejada para ser executada sobre a arquitetura Intel x86_64 e está disponível no ![GitHub](https://github.com/westerndigitalcorporation/swerv-ISS) sob a licensa GPLv3. Para facilitar o uso da ferramenta, nós modificamos e compilamos o simulador para JavaScript (mais especificamente, para ![WebAssembly](https://webassembly.org/) e ![asm.js](http://asmjs.org/spec/latest/), utilizando ![Emscripten](https://emscripten.org/)), de forma que ele possa ser executado diretamente no navegador. (<tspan fill="red">NOTA: Sugerimos FORTEMENTE o uso do navegador Chrome nos laboratórios</tspan>)

O micro-controlador SweRV foi planejado para executar apenas em modo máquina, sem a presença de um sistema operacional. No entanto, o simulador disponibiliza os recursos necessários para a utilização de um sistema operacional simplificado. Na primeira etapa do curso, utilizaremos um sistema operacional simulado para oferecer suporte às chamadas de sistema realizadas pela biblioteca ![Newlib](https://en.wikipedia.org/wiki/Newlib). Tal sistema implementa as syscalls mais comuns do Linux, permitindo que um programa, ao executar no simulador, escreva e leia dados de dispositivos.

Para montar um programa escrito em linguagem de montagem do RISC-V, originalmente procedemos da mesma forma que já conhecemos: usa-se um montador e em seguida um ligador (linker) para gerar código executável. Contudo, como estamos usando computadores da família x86, vamos utilizar um ambiente de compilação cruzada (cross compiling), de modo que usaremos um montador e um linker que funcionam nas famílias x86, mas que geram código para a arquitetura RISC-V.

A seguinte sequência de itens sumariza o processo:

1. Escrever um código em linguagem de montagem do RISC-V. Sugerimos utilizar o editor de texto Visual Studio Code com a extensão "RISC-V Support", que provê destacador de sintaxe *-- syntax highlighting --* para o código);
2. Montar o código escrito na etapa anterior, gerando um arquivo objeto (.o);
3. Executar o linker para converter o arquivo objeto em executável final;
4. Executar o simulador fornecendo o executável.

A seguir, as etapas serão detalhadas e exemplificadas.

## Infraestrutura

Todo o ferramental necessário para se trabalhar com o simulador, como montador, linker e depurador estão disponíveis nos laboratórios em /opt/riscv. A primeira etapa antes de utilizar as ferramentas é executar o seguinte comando:
```
export PATH=$PATH:/opt/riscv/bin
```
Esse comando inicializa variáveis de ambiente necessárias para o bom funcionamento do conjunto de ferramentas. A partir desse ponto, todos os comandos podem ser executados de qualquer diretório.

### Montagem e ligação

Para montar seu código em linguagem de montagem, use o comando:
```
riscv32-unknown-elf-as -g arquivo_de_entrada.s -o arquivo_de_saida.o
```
Note o prefixo riscv32-unknown-elf- na ferramenta as (GNU assembler) - esse prefixo indica que estamos usando um executável diferente do montador (assembler) nativo, capaz de realizar compilação cruzada. Após essa etapa, tendo o arquivo objeto em mãos, podemos executar o ligador (linker) através do seguinte comando:
```
riscv32-unknown-elf-ld -g arquivo_de_entrada.o -o arquivo_de_saida_do_ligador
```

### Simulação

Por fim, procedemos com a simulação em si.

1. Acesse a página do ![Simulador RISC-V](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/sim1/web/static/sim.html):
2. Clique em Browse e selecione o executável
3. Clique em Run para iniciar a execução.
4. Observe a saída do seu código (se houver) no painel Standard IO, no final da página
5. Encerre a execução clicando em Stop

Note que você pode passar argumentos para a ferramenta através das opções na interface gráfica:

* GDB: Habilita ou desabilita o modo de depuração
* Syscall Emulation: Habilita ou desabilita a simulação do sistema operacional que dá suporte às chamadas de sistema utilizadas pela Newlib
* Advanced Options: Permite selecionar os conjuntos de instruções suportados e passar parâmetros adicionais ao simulador.

### Depuração

Por vezes, encontrar um erro num código-fonte em linguagem de máquina não é trivial. Podemos usar a ferramenta GNU gdb para permitir a execução passo a passo do programa e encontrar o erro mais facilmente. Em geral, as etapas para se depurar um programa com o gdb são:

1. Compilar/montar o código-fonte com o parâmetro de depuração (-g) ativado;
2. Ligar o(s) arquivo(s) objeto também com o parâmetro de depuração ativado;
3. Executar o comando gdb seu_programa para invocar o gdb e começar a depuração.

No nosso caso, depuraremos uma aplicação escrita em linguagem de montagem do RISC-V num computador da família x86, logo algumas etapas adicionais são necessárias. Em primeiro lugar, é preciso compilar e ligar seu código com a flag de depuração -g ativada, como já fizemos anteriormente. Precisaremos também de um script adicional, que pode ser obtido com o seguinte comando:
```
curl http://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab05/bridge.py > /tmp/bridge.py
```
Após isso, selecione normalmente o executável no simulador, habilite a opção GDB e clique em Run. Nesse momento, o simulador será iniciado, mas irá aguardar uma conexão com o gdb na porta 5689. Para efetuar essa conexão e iniciar a simulação, é preciso executar o programa gdb no terminal com a seguinte linha de comando:
```
riscv32-unknown-elf-gdb arquivo_de_saida_do_ligador -ex 'target remote | python3 /tmp/bridge.py'
```
Esse comando conecta o gdb ao simulador; note, no navegador, que o simulador está a ponto de começar a simulação, não tendo de fato começado devido ao gdb, que está no controle da aplicação. Repare que o simulador começa a executar na primeira linha de sua função _start, então o gdb irá parar a execução para que você possa efetuar a depuração. Nesse ponto, use o comando si (abreviação de step instruction) no gdb para executar seu programa passo a passo. Pode-se usar o comando ```info register``` para ver o valor dos registradores. Note que a saída e a entrada padrão da sua aplicação continuará aparecendo no navegador, e não na janela do terminal que está executando o gdb. Mais comandos do gdb estão disponíveis no manual da ferramenta gdb em ![https://sourceware.org/gdb/current/onlinedocs/gdb](https://sourceware.org/gdb/current/onlinedocs/gdb).

Em alguns casos, o script de conexão com o GDB pode falhar em finalizar corretamente. Caso isso ocorra, você pode finaliza-lo com o comando:
```
killall python3
```

## Extra (Instalando o toolkit RISC-V em seu computador)

Este passo não é necessário, entretanto, para aqueles que tiverem interesse em instalar o compilador, o montador e o ligador GCC para o RISC-V em seu computador pessoal, segue abaixo uma sequência de comandos parar baixar o código (git clone), configurar a compilação (configure) e compilar o código (make).
```
git clone --recursive https://github.com/riscv/riscv-gnu-toolchain

cd riscv-gnu-toolchain
  
./configure --prefix=/opt/riscv --with-arch=rv32gc --with-abi=ilp32d

make
```

Os comandos configuram o software para que ele seja instalado no diretório /opt/riscv. Em alguns sistemas, este processo necessita acesso de superusuário. Neste caso, é importante você executar o comando make com permissão de superusuário (p.ex: sudo make).

**OSX:** Caso você esteja usando o sistema operacional OSX, da Apple, talvez você precise ajustar as variáveis de ambiente CC e CXX antes de executar o comando ./configure. Por exemplo:
```
export CC=gcc
export C++=g++
```
