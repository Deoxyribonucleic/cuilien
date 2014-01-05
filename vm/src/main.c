#include <stdio.h>

#include "vm.h"
#include "args.h"
#include "mem.h"
#include "error.h"
#include "cpu.h"

int main(char** args, int argc)
{
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
