#include "cpu.h"
#include "error.h"
#include "instr.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

cpu_handle cpu_init(memory_t* memory)
{
	build_instruction_vector();
	error_clear();

	if(memory == NULL)
	{
		error_last = ERR_CPU_INVALID_MEMORY;
		return NULL;
	}

	cpu_handle cpu = malloc(sizeof(cpu_t));

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
	cpu->reg.sp = 0;

	cpu->reg.flags = 0;

	cpu->memory = memory;

	return cpu;
}

void cpu_free(cpu_handle cpu)
{
	if(cpu)
		free(cpu);
}

void cpu_step(cpu_handle cpu)
{
	struct cpu_instruction instruction;
	cpu_read_instruction(cpu, cpu->reg.ip, &instruction);
	cpu->reg.ip += INSTRUCTION_LENGTH;
	cpu_execute(cpu, &instruction);
}

void cpu_read_instruction(cpu_handle cpu, c_addr address, struct cpu_instruction* output)
{
	output->operation = mem_read_short(cpu->memory, address, true);
	output->op1.flags = mem_read_byte(cpu->memory, address + 2, true);
	output->op2.flags = mem_read_byte(cpu->memory, address + 3, true);
	output->op1.value = mem_read_long(cpu->memory, address + 4, true);
	output->op2.value = mem_read_long(cpu->memory, address + 8, true);
}

void cpu_execute(cpu_handle cpu, struct cpu_instruction* instruction)
{
	assert(instruction->operation < INSTRUCTION_VECTOR_LENGTH);
	assert(instruction_vector[instruction->operation] != 0);

	instruction_vector[instruction->operation](cpu, &instruction->op1, &instruction->op2);
}


bool cpu_get_flag(cpu_handle cpu, int flag)
{
	return cpu->reg.flags & flag;
}

void cpu_set_flag(cpu_handle cpu, int flag)
{
	cpu->reg.flags |= flag;
}

void cpu_clear_flag(cpu_handle cpu, int flag)
{
	cpu->reg.flags &= ~flag;
}


void cpu_jump(cpu_handle cpu, c_addr target)
{
	printf("control flow: 0x%08x => 0x%08x\n", cpu->reg.ip, target);
	cpu->reg.ip = target;
}


void cpu_push_long(cpu_handle cpu, c_long value)
{
	cpu->reg.sp -= sizeof(c_long);
	mem_write_long(cpu->memory, cpu->reg.sp, value);
}

void cpu_push_short(cpu_handle cpu, c_short value)
{
	cpu->reg.sp -= sizeof(c_short);
	mem_write_short(cpu->memory, cpu->reg.sp, value);
}

void cpu_push_byte(cpu_handle cpu, c_byte value)
{
	cpu->reg.sp -= sizeof(c_byte);
	mem_write_byte(cpu->memory, cpu->reg.sp, value);
}


c_long cpu_pop_long(cpu_handle cpu)
{
	c_long value = mem_read_long(cpu->memory, cpu->reg.sp, false);
	cpu->reg.sp += sizeof(c_long);
	return value;
}

c_short cpu_pop_short(cpu_handle cpu)
{
	c_short value = mem_read_short(cpu->memory, cpu->reg.sp, false);
	cpu->reg.sp += sizeof(c_short);
	return value;
}

c_byte cpu_pop_byte(cpu_handle cpu)
{
	c_byte value = mem_read_byte(cpu->memory, cpu->reg.sp, false);
	cpu->reg.sp += sizeof(c_byte);
	return value;
}


void cpu_call(cpu_handle cpu, c_addr subroutine)
{
	printf("calling subroutine at 0x%08x\n", subroutine);
	cpu_push_long(cpu, cpu->reg.ip);
	cpu_jump(cpu, subroutine);
}

void cpu_return(cpu_handle cpu)
{
	c_addr return_address = cpu_pop_long(cpu);
	printf("returning from subroutine to 0x%08x\n", return_address);
	cpu_jump(cpu, return_address);
}
