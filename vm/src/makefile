bin: main.o args.o error.o vm.o mem.o cpu.o instr.o vector.o page.o
	gcc main.o args.o error.o vm.o mem.o cpu.o instr.o vector.o page.o -g -o bin

main.o: main.c
	gcc -g -c main.c

args.o: args.c
	gcc -g -c args.c

error.o: error.c
	gcc -g -c error.c

vm.o: vm.c
	gcc -g -c vm.c

mem.o: mem.c
	gcc -g -c mem.c

cpu.o: cpu.c
	gcc -g -c cpu.c

instr.o: instr.c
	gcc -g -c instr.c

vector.o: vector.c
	gcc -g -c vector.c

page.o: page.c
	gcc -g -c page.c

clean:
	-rm *.o
	-rm bin
	-rm *~

clean-compile:
	-rm *.o
	-rm *~
