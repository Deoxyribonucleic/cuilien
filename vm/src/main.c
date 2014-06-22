#include <stdio.h>

#include "vm.h"
#include "args.h"
#include "mem.h"
#include "error.h"
#include "cpu.h"
#include "vector.h"

#include <stdlib.h>

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

	memory_t* memory = mem_init(2050);
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

	//mem_read_short(mem_resolve(memory, 0x104));

	page_info_t shared_info = { PAGE_WRITE, false };
	c_byte* shared_mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	//page_map(&memory->page_table, 0, &shared_info, shared_mem);

	// Write test program into memory
	c_addr programStart = 0xff000000;
	mem_write_short(memory,	programStart+ 0, 0x0003);
	mem_write_short(memory,	programStart+ 2, 0x030B);
	mem_write_long(memory,	programStart+ 4, 0x00000001);
	mem_write_long(memory,	programStart+ 8, 0x00000001);

	mem_write_short(memory,	programStart+12, 0x0003);
	mem_write_short(memory,	programStart+14, 0x030B);
	mem_write_long(memory,	programStart+16, 0x00000002);
	mem_write_long(memory,	programStart+20, 0x00000000);

	mem_write_short(memory,	programStart+24, 0x0003);
	mem_write_short(memory,	programStart+26, 0x0B0B);
	mem_write_long(memory,	programStart+28, 0x00000002);
	mem_write_long(memory, 	programStart+32, 0x00000001);

	mem_write_short(memory,	programStart+36, 0x0003);
	mem_write_short(memory,	programStart+38, 0x070B);
	mem_write_long(memory,	programStart+40, 0x00000003);
	mem_write_long(memory,	programStart+44, 0x00000666);

	mem_write_short(memory,	programStart+48, 0x0003);
	mem_write_short(memory,	programStart+50, 0x0F0B);
	mem_write_long(memory,	programStart+52, 0x00000001);
	mem_write_long(memory,	programStart+56, 0x00000003);

	mem_write_short(memory,	programStart+60, 0x0002);
	mem_write_short(memory,	programStart+62, 0x0000);
	mem_write_long(memory,	programStart+64, 0x00000000);
	mem_write_long(memory,	programStart+68, 0x00000000);

	mem_write_short(memory,	programStart+72, 0x0001);
	mem_write_short(memory,	programStart+74, 0x0000);
	mem_write_long(memory,	programStart+76, 0x00000000);
	mem_write_long(memory,	programStart+80, 0x00000000);
	
	// Write some fun data to play with
	mem_write_long(memory, 0x666, 1337);
	mem_write_long(memory, 1337, 0xdeadbeef);
	
	// Initialize registers to identifiable values
	cpu->reg.a = 1000;
	cpu->reg.b = 2000;
	cpu->reg.c = 3000;
	cpu->reg.d = 4000;

	// Move instruction pointer to start of program and step through the 7 instructions
	cpu->reg.ip = programStart;
	int i;
	for(i = 0; i<7; ++i)
	{
		cpu_step(cpu);
	}
	
	free(shared_mem);

	cpu_free(cpu);
	mem_free(memory);

	return 0;
}
