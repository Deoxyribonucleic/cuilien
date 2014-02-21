#include <stdio.h>

#include "vm.h"
#include "args.h"
#include "mem.h"
#include "error.h"
#include "cpu.h"
#include "vector.h"

void print_vector_info(vector_t* vector)
{
	printf("Element size: %d | Size: %d | Capacity: %d\n", vector->element_size, vector->size, vector->capacity);
	int i;
	printf("Listing:\n");
	for(i=0; i<vector->size; ++i)
	{
		char* string;
		vector_get(vector, i, &string);
		printf("* %s\n", string);
	}
	printf("---\n");
}

int main(int argc, char** args)
{
	vector_t argv;
	vector_create(sizeof(char*), 0, &argv);
	print_vector_info(&argv);

	int arg;
	for(arg = 0; arg < argc; ++arg)
	{
		vector_push_back(&argv, &args[arg]);
		print_vector_info(&argv);
	}

	char hello[] = "HELLO! :D";
	char* hello_ptr = hello;
	vector_insert(&argv, &hello_ptr, 2);
	vector_insert(&argv, &hello_ptr, argv.size + 1);
	print_vector_info(&argv);
	vector_remove(&argv, 3);
	print_vector_info(&argv);

	vector_free(&argv);

	return 0;

	struct args_info arguments;
	args_parse(args, argc, &arguments);

	printf("Program: %s\n", arguments.program);

	int error;

	mem_handle memory = mem_init(0xfff);
	if(error_last)
	{
		error_print(error_last);
		return 1;
	}

	cpu_handle cpu = cpu_init(memory);
	if(error_last)
	{
		error_print(error_last);
		return 1;
	}

	if(error = vm_init(&arguments))
		error_print(error);

	mem_read_long(mem_resolve(memory, 0x104));

	c_long testVariable = 1337;
	mem_write_long(mem_resolve(memory, 0x0666), testVariable);
	c_long read1 = mem_read_long(mem_resolve(memory, 0x666));
	c_short read2 = mem_read_short(mem_resolve(memory, 0x666));

	printf("Wrote: %d\nRead (long): %d\nRead (short): %d\n", testVariable, read1, read2);

	cpu_free(cpu);
	mem_free(memory);

	return 0;
}
