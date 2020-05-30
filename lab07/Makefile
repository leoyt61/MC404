all: lab07.o
	riscv32-unknown-elf-ld -g lab07.o -o lab07
lab07.o: lab07.s
	riscv32-unknown-elf-as -g lab07.s -o lab07.o

clean:
	rm -rf lab07.o lab07
