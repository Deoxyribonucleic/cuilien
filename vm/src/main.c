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

	//mem_read_long(mem_resolve(memory, 0x104));

	page_info_t shared_info = { PAGE_WRITE, false };
	c_byte* shared_mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	page_map(&memory->page_table, 0, &shared_info, shared_mem);

	c_long testVariable = 0x12345678;
	mem_write_long(memory, 0x0666, testVariable);
	c_long read1 = mem_read_long(memory, 0x0666, false);
	c_short read2 = mem_read_short(memory, 0x0666, false);
	c_short read3 = mem_read_short(memory, 500, false);
	c_short read4 = mem_read_short(memory, 2050, false);
	
	free(shared_mem);

	//mem_write_long(mem_resolve(&memory, 0x0666), testVariable);
	//c_long read1 = mem_read_long(&mem_resolve(memory, 0x666));
	//c_short read2 = mem_read_short(&mem_resolve(memory, 0x666));

	printf("Wrote: %x\nRead (long): %x\nRead (short): %x\n", testVariable, read1, read2);

	cpu_free(cpu);
	mem_free(memory);

	return 0;
}
