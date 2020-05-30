all: lab08

lab08: mapa.o dfs.o
	riscv32-unknown-elf-ld mapa.o dfs.o -o lab08
mapa.o: mapa.s
	riscv32-unknown-elf-as mapa.s -o mapa.o
mapa.s: mapa.c
	riscv32-unknown-elf-gcc mapa.c -S -o mapa.s
dfs.o: dfs.s
	riscv32-unknown-elf-as dfs.s -o dfs.o

clean:
	rm -rf dfs.o mapa.o mapa.s lab08
