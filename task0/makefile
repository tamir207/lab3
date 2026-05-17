all: task0

task0: start.o main.o util.o
	ld -m elf_i386 start.o main.o util.o -o task0

start.o: start.s
	nasm -f elf32 start.s -o start.o

main.o: main.c util.h
	gcc -m32 -g -Wall -ansi -c -nostdlib -fno-stack-protector -o main.o main.c

util.o: util.c util.h
	gcc -m32 -g -Wall -ansi -c -nostdlib -fno-stack-protector -o util.o util.c

.PHONY: clean

clean:
	rm -f *.o task0