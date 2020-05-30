all : lab06

lab06 : lab06.o
	riscv32-unknown-elf-ld -g lab06.o -o lab06
lab06.o: lab06.s
	riscv32-unknown-elf-as -g lab06.s -o lab06.o

clean:
	rm -rf lab06.o lab06
