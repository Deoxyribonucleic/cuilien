bin: main.o args.o error.o vm.o mem.o
	gcc main.o args.o error.o vm.o mem.o -o bin

main.o: main.c
	gcc -c main.c

args.o: args.c
	gcc -c args.c

error.o: error.c
	gcc -c error.c

vm.o: vm.c
	gcc -c vm.c

mem.o: mem.c
	gcc -c mem.c
