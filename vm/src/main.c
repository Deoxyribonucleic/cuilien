#include <stdio.h>

#include "vm.h"
#include "args.h"
#include "mem.h"

int main(char** args, int argc)
{
	struct args_info arguments;
	args_parse(args, argc, &arguments);

	printf("Program: %s\n", arguments.program);

	int error;

	if(error = vm_init(&arguments))
		printf("Error: %s\n", error_tostring(error));

	mem_read_long(0x100);

	return 0;
}
