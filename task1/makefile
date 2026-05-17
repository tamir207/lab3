all: task1

task1: start.o task1.o util.o
	ld -m elf_i386 start.o task1.o util.o -o task1

start.o: start.s
	nasm -f elf32 start.s -o start.o

task1.o: task1.s
	nasm -f elf32 task1.s -o task1.o

util.o: util.c util.h
	gcc -m32 -g -Wall -ansi -c -nostdlib -fno-stack-protector -o util.o util.c

.PHONY: clean

clean:
	rm -f *.o task1