# Atividade de Laboratório 7

## Objetivos

O objetivo desta atividade é exercitar o uso de operações com bits, controle de fluxo, e acesso a vetores utilizando o conjunto de instruções da arquitetura RISC-V.

## Descrição

![MIDI](https://en.wikipedia.org/wiki/MIDI) (Interface Digital de Instrumentos Musicais, do inglês *Musical Instrument Digital Interface*) é um padrão de armazenamento e comunicação entre instrumentos musicais, computadores e ferramentas de áudio. Sua estrutura consiste em representar notas musicais através de uma sequência de comandos simples e de fácil reprodução. Um arquivo MIDI possui até 16 canais. A cada canal, um instrumento musical pode ser associado. A melodia é, então, representada como uma lista de notas musicais para cada canal. Cada nota possui ainda sua velocidade, frequência e tempo.

Neste laboratório, você deve implementar um tocador de músicas no formato MIDI utilizando a linguagem de montagem do RISC-V. O programa é dividido em duas partes: *Parser* e *Player*. O pseudo-código delas pode ser visto no documento ![mc404_lab07.pdf](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab07/mc404_lab07.pdf). Para facilitar o problema, nós fornecemos o arquivo base ![lab7.s](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab07/lab7.s), que já declara as estruturas de dados e implementa a parte *Player*. Assim, somente a parte *Parser* precisa ser implementada e será considerada na avaliação do laboratório.
### O *Parser*

Nesta primeira parte, você deverá ler e interpretar os dados da música. Você receberá a entrada no formato Midigram, uma representação simplificada (e mais compreensível) do formato MIDI. Sua estrutura é ilustrada abaixo.
```
Header 1 15
Program   8 71 
8040 8148 4 8 58 76
8160 8376 4 8 58 76
8400 8616 4 8 59 76
8640 8964 4 8 58 76
12000 12216 4 8 58 76
12240 12456 4 8 59 76
12480 12804 4 8 58 76
15720 15828 4 8 58 76
...
82440 82488 15 10 20 20
82440 82488 15 10 33 13
82500 82548 15 10 20 18
82500 82548 15 10 33 12
88320
```
Seu código deverá ler uma linha por vez e identificar seu tipo. No nosso exemplo:

* Header 1 15
	* Linha de cabeçalho.
	* Deve ser ignorada.
* Program 8 71
	* Linha de programa. É ela quem define qual instrumento o canal irá tocar.
		* No exemplo: O canal 8 tocará o instrumento 71 (o Fagote)
	* Contém a palavra “Program” seguida por dois números inteiros separados por espaços.
	* Formato: “ Program canal instrumento ”.
	* Você deve ler os valores de canal e de instrumento.
	* Você deve armazenar, em um vetor, o instrumento programado para o canal.
* 8040 8148 4 8 58 76
	* Linha de nota.
	* Contém sempre 6 números inteiros separados por espaços.
	* Formato: “Início Fim Track Canal Frequência Velocidade”
	* Você deverá ler os valores de Início, Fim, Canal, Frequência e Velocidade.
	* A forma de armazenamento será detalhada a seguir.
* 88320
	* Última linha.
	* Contém apenas um número inteiro.
	* Deve ser ignorada.

**Armazenamento dos canais (linhas 4 e 5 do Algoritmo 1)**

1. Use o vetor de inteiros C (já criado no arquivo de template) que possui 16 posições. Você deve inserir código para inicializá-lo com zeros.
2. Sempre que uma linha de **programa** for recebida com Canal c e instrumento i:
	* Preencha a posição c do vetor com o valor i.

**Armazenamento das notas (linhas 6 a 11 do Algoritmo 1)**

1. Use a matriz de inteiros M (já criada no arquivo de template) que possui 16 colunas e 300 mil linhas. Você deve inserir código para inicializá-la com zeros.
2. Sempre que uma linha de nota for recebida com Início tini, Fim tend, canal c, frequência f, e velocidade v:
	* Converta os valores de tini e tend para valores de tempo em milisegundos.
		* Para isso, basta aplicar a fórmula mostrada nas Linhas 7 e 8 do Algoritmo.
		* Os parâmetros BPM e ticks serão passado pela entrada padrão antes do arquivo MIDIGRAM.
		* A tabela a seguir mostra os valores de BPM e ticks para os arquivos de teste (![music.zip](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab07/music.zip)).
		* Note que, a depender de como a fórmula for aplicada, seu código poderá gerar overflow. Você deve evitar que isso ocorra. Para isso, você pode dividir um dos parâmetros (BPM ou Ticks) por 10 e multiplicar por 6000 em vez de 60000.
	* Na Matriz, preencha a coluna c e as linhas de tini até tend (considerando os valores já convertidos em milisegundos) com a tripla (f, v, d), onde d = (tend - i) e i é valor da linha atual.
		* A tripla (f, v, d) deve ser armazenada numa única palavra de memória nesta ordem.
		* Para isso, f deve ser armazenado nos bits 31..24, v nos bits 23..16, e d nos bits 15..0.

| Arquivo | BPM | Ticks |
| 1 | 105 | 240 |
| 2 | 124 | 384 |
| 3 | 78 | 192 |
| 4 |67 | 480 |
| 5 | 170 | 96 |
| 6 | 114 | 384 |
| 7 | 100 | 96 |
| 8 | 67 | 384 |

**Entrada*

A entrada será o valor dos parâmetros BPM e ticks (na mesma linha separados por um espaço), seguido pelos dados do arquivo Midigram. Para testar seu código, disponibilizamos 8 arquivos Midigram (music.zip), mas você pode gerar novos a partir de músicas no formato MIDI (veja como na última seção deste documento).

Exemplo:
```
105 240
Header 1 15
Program   8 71 
8040 8148 4 8 58 76
8160 8376 4 8 58 76
8400 8616 4 8 59 76
8640 8964 4 8 58 76
12000 12216 4 8 58 76
12240 12456 4 8 59 76
12480 12804 4 8 58 76
15720 15828 4 8 58 76
...
82440 82488 15 10 20 20
82440 82488 15 10 33 13
82500 82548 15 10 20 18
82500 82548 15 10 33 12
88320
```
**Organização e compilação do parser**

Seu código do "parser" deve ser implementado no arquivo lab7.s. Além disso:

* O código do lab7.s deve fazer uso da matriz M e do vetor C já declarados no arquivo de template. Lembre-se de inserir código para iniciar estas estruturas como indicado acima.
* O arquivo lab7.s deve ter uma função chamada "_start", que deverá conter o código do Parser.
* O rótulo da função _start deve ser "_start" e deve ser declarado como global usando a diretiva .globl.
* Ao final da função _start, seu código deve continuar executando a partir do rótulo "play".

Execute a compilação e ligação de seu código como de costume. Agora, basta executar o programa lab7 no simulador para testá-lo. Veja a seção Entrada (acima) para entender o formato de entrada.

### O *Player*

Embora a implementação dessa etapa já esteja feita no arquivo de template, a leitura dessa seção é importante para o entendimento da infraestrutura a ser utilizada. Além disso, a compreensão do algoritmo pode vir a ser necessária para o trabalho final da disciplina. Uma vez preenchidos a matriz M e o vetor C, você pode reproduzir a música seguindo o Algoritmo 2 do PDF. Para isso, o código implementa a função "play" dentro do arquivo lab7.s da seguinte forma:

* Um vetor de inteiros P com 16 posições é criado.
	* Um segundo vetor de inteiros P2, com 16 posições, é criado. Apesar do algoritmo utilizar um único vetor P para tudo, você poderia ter problemas de overflow se o fizesse.
* Em um laço infinito, o código:
	* Obtém o tempo t em milissegundos a partir do início da execução do player.
	* Percorre cada canal j da linha t da matriz M
		* Se a nota armazenada na posição M[t][j] não estiver tocando, chama o Sintetizador para reproduzi-la.

**Obtenção de tempo**

O tempo pode ser obtido através da chamada de sistema gettimeofday. No exemplo a seguir, o tempo em milissegundos é carregado em a0.
```
la a0, buffer_timeval
la a1, buffer_timerzone
li a7, 169 # chamada de sistema gettimeofday
ecall
la a0, buffer_timeval
lw t1, 0(a0) # tempo em segundos
lw t2, 8(a0) # fração do tempo em microssegundos
li t3, 1000
mul t1, t1, t3
div t2, t2, t3
add a0, t2, t1
```
**Chamando o sintetizador**

Um sintetizador de áudio é um componente capaz de imitar o som de instrumentos musicais convencionais. Em nosso simulador RISC-V, nós simulamos o uso de um sintetizador implementado em um circuito integrado e conectado ao microcontrolador pelo barramento. Para simplificar seu uso no laboratório, a syscall SynthPlay foi implementada para tratar da comunicação com o CI. **Para conectar o sintetizador ao simulador, é necessário ir à aba "External Devices", seção "Sound Synthesizer", e clicar em "Load Device"**.

Exemplo de uso da *syscall*:
```
li a0, 25          # instrumento (25: guitarra acústica de nylon) 
li a1, 0x1800      # tempo para tocar a nota (d = 0x1800)
ori a1, 0x100000   # velocidade da nota (v = 0x10)
ori a1, 0x52000000 # frequencia da nota (f = 0x52)
li a2, 1           # canal 1
li a7, 2048        # chamada de sistema
ecall
```
Note que é importante representar a tripla (f, v, d) como indicado na Parte 1 para que o código do *Player* funcione corretamente.

Agora, basta executar o programa lab7 no simulador para testá-lo.

## Para diversão!

* Tente acertar qual a música em cada um dos arquivos de exemplo.
* Reproduza outros MIDIs, além dos fornecidos (MIDIs são facilmente encontrados na internet). Para isso, instale o software midi2abc:
	* Para instalar (ubuntu): sudo apt install abcmidi
	* Para gerar o midigram: midi2abc musica.mid -midigram > musica.midigram
	* Para obter o parâmetro BPM: midi2abc musica.mid -sum | grep Tempo
	* Para obter o parâmetro ticks: od -An -j 12 -N 2 --endian=big -vd musica.mid | xargs


