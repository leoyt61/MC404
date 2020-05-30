all : lab05.o
	riscv32-unknown-elf-ld -g lab05.o -o lab05
lab05.o: lab05.s
	riscv32-unknown-elf-as -g lab05.s -o lab05.o

clean:
	rm -rf lab05.o lab05
