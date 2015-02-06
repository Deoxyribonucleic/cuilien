#include <stdio.h>

#include "vm/vm.h"
#include "vm/args.h"
#include "vm/mem.h"
#include "vm/error.h"
#include "vm/cpu.h"
#include "vm/vector.h"
#include "vm/debug.h"

#include <stdlib.h>
#include <string.h>


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
	struct args_info arguments;
	args_parse(args, argc, &arguments);

	printf("Program: %s\n", arguments.program);

	int error;

	memory_t* memory = mem_init(1024*1024*10); // 10 MB
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


	page_info_t shared_info = { PAGE_WRITE, false };
	c_byte* shared_mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	//page_map(&memory->page_table, 0, &shared_info, shared_mem);


	// Load test program into memory
	c_addr programStart = 0xff000000;
	mem_load_file(memory, arguments.program, programStart);


	// load brainfuck program into memory at 0xffe00000
	mem_load_file(memory, "vmtest/programs/99bottles.bf", 0xffe00000);


	// Write some fun data to play with
	mem_write_long(memory, 0x666, 1337);
	mem_write_long(memory, 1337, 0xdeadbeef);


	// Create a context and initialize memory, registers
	context_t context;
	memset(&context, 0, sizeof(context));
	context.memory = memory;

	// Put identifiable values in registers
	context.reg.a = 1000;
	context.reg.b = 2000;
	context.reg.c = 3000;
	context.reg.d = 4000;

	cpu->context = &context;


	// Move instruction pointer to start of program and step through the instructions until HALT is reached
	cpu->context->reg.ip = programStart;
	cpu->halted = false;
	while(!cpu->halted)
	{
		cpu_step(cpu);
		DEBUG_PRINTF("--\n");
	}

	free(shared_mem);

	cpu_free(cpu);
	mem_free(memory);

	return 0;
}
