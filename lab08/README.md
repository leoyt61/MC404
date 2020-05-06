# Atividade de Laboratório 8
## Objetivos

O objetivo desta atividade é exercitar o conceito de funções e ABI na arquitetura RISC-V.

## Descrição

Robinson Lateiam é um aluno da Unicamp que sempre chega atrasado na aula de MC404. Cansado de ser chamado para resolver exercícios na lousa, ele decidiu construir um aplicativo que calculasse um caminho entre sua residência, a república Atrasolândia, e o Ciclo Básico. Após horas de planejamento, ele considerou que a melhor alternativa seria construir a interface do aplicativo em C e o algoritmo de busca de caminho utilizando a linguagem de montagem do RISC-V. Entretanto, como chegou atrasado a todas as aulas de RISC-V, ele conseguiu escrever apenas a parte em C do aplicativo e pediu a você, seu brilhante e pontual colega, que implementasse o restante.

Ajude Robinson Lateiam a ter uma desculpa a menos para se atrasar!

Você deve implementar o **algoritmo de DFS recursivo** (![https://courses.cs.washington.edu/courses/cse326/03su/homework/hw3/dfs.html](https://courses.cs.washington.edu/courses/cse326/03su/homework/hw3/dfs.html)) para encontrar um caminho entre a república e o Ciclo Básico. Para isso, você receberá, já implementado em C, a função _start() que carrega o mapa da cidade; O mapa é um plano catesiano representado por uma matriz de caracteres. **A coordenada "X" indica a linha da matriz enquanto que a coordenada "Y" indica a coluna da matriz**. uma função int daParaPassar(int x, int y) que retorna '0' caso não seja possível passar na posição (X, Y) e '1' caso seja possível; duas funções que retornam a posição atual do Robinson (int posicaoXRobinson() e int posicaoYRobinson()) e duas funções que retornam a posição onde ele quer chegar (int posicaoXLocal() e int posicaoYLocal()). No final, você deve imprimir o resultado como as posições (X, Y) do caminho em cada linha. A primeira tupla deve ser a posição do local e a última a posição do Robinson. É possível que não exista caminho, nesse caso deve ser impresso: "Não existe um caminho!". O tamanho máximo de um mapa será de 10 posições, ou seja, de 0 a 9.

Você deverá implementar a sua solução em linguagem de montagem do RISC-V e ligar com o código C fornecido. A sua solução deverá conter uma função chamada: void ajudaORobinson() que será invocada pela função main.

Obs.:

1. As movimentações do caminho podem ser na horizontal, vertical ou na diagonal com no máximo uma casa de distância.
2. Você pode pintar o mapa para saber o que já foi visitado, veja as funções int foiVisitado(int, int) e void visitaCelula(int, int) (reveja o funcionamento do DFS).
3. Reveja no lab 06 como imprimir textos e como converter números em texto utilizando as instruções do RISC-V (você pode copiar e adaptar o código).

Exemplo

Entrada:
```
0 1 2 3 4 5 6 7 8 9
X X X X X X X X X X 0
X _ _ _ X X _ X _ X 1
X _ X _ _ X _ _ _ X 2
X R X X _ _ _ X _ X 3
X _ X _ _ X _ _ _ X 4
X _ _ _ X X _ X _ X 5
X _ X _ _ X _ X _ X 6
X _ _ X _ _ _ X _ X 7
X _ _ L _ X _ _ _ X 8
X X X X X X X X X X
```
Neste exemplo, Robinson se encontra inicialmente na posição (X=3,Y=1) e deve chegar à posição (X=8,Y=3). Para isso, uma possível solução (saída do programa) é:
```
8 3
7 2
6 1
5 1
4 1 
3 1
```
## Compilação

Serão disponibilizados dois códigos: dfs.h e mapa.c. O primeiro é uma API (Application Programming Interface) que possui as definições de rotinas de suporte que serão implementadas em linguagem de montagem. Neste caso há apenas a rotina ajudaORobinson() que será chamada pela função _start() do mapa.c. Para implementar esta rotina, é preciso criar um arquivo dfs.s e implementar a rotina descrita no arquivo dfs.h.

Como visto em sala de aula, cada rotina deve ser identificada por um rótulo igual ao nome da rotina em "C". Além disso, para permitir que uma rotina implementada em um arquivo seja chamada de outro arquivo você deve sinalizar para o ligador (linker) que o rótulo que representa a rotina é um símbolo global. Para fazer isso, basta informar ao montador que o rótulo da rotina é um símbolo global através da diretiva .global. O arquivo ![example.s](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab08/example.s) apresenta um exemplo com duas rotinas onde a rotina dummy_routine1 é definida como global através do uso da diretiva .global. Observe que a outra rotina (dummy_routine2) não é declarada como global.

O segundo arquivo disponibilizado é o código C onde o programa começa a ser executado.
### Arquivos disponibilizados

* ![dfs.h](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab08/dfs.h)
* ![mapa.c](https://www.ic.unicamp.br/~edson/disciplinas/mc404/2019-2s/ab/labs/lab08/mapa.c)

### Geração do arquivo .s referente ao mapa.c:
```
riscv32-unknown-elf-gcc mapa.c -S -o mapa.s
```
### Geração dos arquivos-objeto:
```
riscv32-unknown-elf-as mapa.s -o mapa.o
riscv32-unknown-elf-as dfs.s -o dfs.o
```
### Ligação:
```
riscv32-unknown-elf-ld mapa.o dfs.o -o program
```
