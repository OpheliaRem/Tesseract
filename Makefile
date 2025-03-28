.PHONY: all clean run

all: kernel

kernel: clean ./build/linker_files/link.ld kasm.o kc.o
	ld -m elf_i386 -T ./build/linker_files/link.ld -o ./build/kernel ./build/kasm.o ./build/kc.o

kasm.o: kernel.asm
	nasm -f elf32 kernel.asm -o ./build/kasm.o

kc.o: kernel.c
	gcc -fno-stack-protector -m32 -c kernel.c -o ./build/kc.o

clean:
	rm -f ./build/*.o ./build/kernel

run: kernel
	qemu-system-i386 -kernel ./build/kernel
