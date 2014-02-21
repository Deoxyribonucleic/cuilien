#include "cpu.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>

cpu_handle cpu_init(memory_t* memory)
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

void cpu_read_instruction(cpu_handle cpu, c_addr address, struct cpu_instruction* output)
{
	output->operation = mem_read_short(cpu->memory, address, true);
	output->op1.flags = mem_read_byte(cpu->memory, address + 2, true);
	output->op2.flags = mem_read_byte(cpu->memory, address + 3, true);
	output->op1.value = mem_read_long(cpu->memory, address + 4, true);
	output->op2.value = mem_read_long(cpu->memory, address + 8, true);
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

