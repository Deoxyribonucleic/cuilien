#include "cpu.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>

cpu_handle cpu_init(mem_handle memory)
{
	error_clear();

	if(memory == NULL)
	{
		error_last = ERR_CPU_INVALID_MEMORY;
		return NULL;
	}

	cpu_handle cpu = malloc(sizeof(struct cpu_t));

	if(cpu == NULL)
	{
		error_last = ERR_CPU_HOST_OOM;
		return NULL;
	}

	cpu->reg.a = 0;
	cpu->reg.b = 0;
	cpu->reg.c = 0;
	cpu->reg.d = 0;

	cpu->reg.ip = 0;

	cpu->memory = memory;

	return cpu;
}

void cpu_free(cpu_handle cpu)
{
	if(cpu)
		free(cpu);
}

void cpu_read_instruction(cpu_handle cpu, c_byte* address, struct cpu_instruction* output)
{
	output->operation = mem_read_short(address);
	output->op1.flags = mem_read_byte(address + 2);
	output->op2.flags = mem_read_byte(address + 3);
	output->op1.value_ptr = address + 4;
	output->op2.value_ptr = address + 8;
}


void cpu_write_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* src, size_t size)
{
	if((flags & (PF_REGISTER | PF_DEREFERENCE)) == 0)
	{
		// Parameter is not an l-value and cannot be written to. Fail silently.
		return;
	}

	if(flags & PF_REGISTER)
	{
		
	}
}

