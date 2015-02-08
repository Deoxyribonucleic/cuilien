#include "cpu.h"
#include "error.h"
#include "instr.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

cpu_handle cpu_init()
{
	build_instruction_vector();
	error_clear();

	/*if(memory == NULL)
	{
		error_last = ERR_CPU_INVALID_MEMORY;
		return NULL;
	}*/

	cpu_handle cpu = malloc(sizeof(cpu_t));

	if(cpu == NULL)
	{
		error_last = ERR_CPU_HOST_OOM;
		return NULL;
	}

	cpu->context = NULL;
	memset(&cpu->ivt, 0, sizeof(interrupt_vector_table_t));

	cpu->halted = true;

	return cpu;
}

void cpu_free(cpu_handle cpu)
{
	if(cpu)
		free(cpu);
}

void cpu_step(cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	struct cpu_instruction instruction;
	cpu_fetch(cpu, cpu->context->reg.ip, &instruction);
	cpu->context->reg.ip += INSTRUCTION_LENGTH;
	cpu_execute(cpu, &instruction);
}

void cpu_fetch(cpu_handle cpu, c_addr address, struct cpu_instruction* output)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	output->operation = mem_read_short(cpu->context->memory, address, true);
	output->op1.flags = mem_read_byte(cpu->context->memory, address + 2, true);
	output->op2.flags = mem_read_byte(cpu->context->memory, address + 3, true);
	output->op1.value = mem_read_long(cpu->context->memory, address + 4, true);
	output->op2.value = mem_read_long(cpu->context->memory, address + 8, true);
}

void cpu_execute(cpu_handle cpu, struct cpu_instruction* instruction)
{
	assert(cpu != NULL);

	if(instruction->operation >= INSTRUCTION_VECTOR_LENGTH ||
			instruction_vector[instruction->operation] == 0)
	{
		DEBUG_PRINTF("[cpu] invalid opcode: %04x\n", instruction->operation);
		return;
	}

	instruction_vector[instruction->operation](cpu, &instruction->op1, &instruction->op2);
}


bool cpu_get_flag(cpu_handle cpu, int flag)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	return cpu->context->reg.flags & flag;
}

void cpu_set_flag(cpu_handle cpu, int flag)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	cpu->context->reg.flags |= flag;
}

void cpu_clear_flag(cpu_handle cpu, int flag)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	cpu->context->reg.flags &= ~flag;
}


void cpu_jump(cpu_handle cpu, c_addr target)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	//printf("control flow: 0x%08x => 0x%08x\n", cpu->context->reg.ip, target);
	cpu->context->reg.ip = target;
}


void cpu_push_long(cpu_handle cpu, c_long value)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	cpu->context->reg.sp -= sizeof(c_long);
	mem_write_long(cpu->context->memory, cpu->context->reg.sp, value);
}

void cpu_push_short(cpu_handle cpu, c_short value)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	cpu->context->reg.sp -= sizeof(c_short);
	mem_write_short(cpu->context->memory, cpu->context->reg.sp, value);
}

void cpu_push_byte(cpu_handle cpu, c_byte value)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	cpu->context->reg.sp -= sizeof(c_byte);
	mem_write_byte(cpu->context->memory, cpu->context->reg.sp, value);
}


c_long cpu_pop_long(cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	c_long value = mem_read_long(cpu->context->memory, cpu->context->reg.sp, false);
	cpu->context->reg.sp += sizeof(c_long);
	return value;
}

c_short cpu_pop_short(cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	c_short value = mem_read_short(cpu->context->memory, cpu->context->reg.sp, false);
	cpu->context->reg.sp += sizeof(c_short);
	return value;
}

c_byte cpu_pop_byte(cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	c_byte value = mem_read_byte(cpu->context->memory, cpu->context->reg.sp, false);
	cpu->context->reg.sp += sizeof(c_byte);
	return value;
}


void cpu_call(cpu_handle cpu, c_addr subroutine)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	DEBUG_PRINTF("calling subroutine at 0x%08x\n", subroutine);
	cpu_push_long(cpu, cpu->context->reg.ip);
	cpu_jump(cpu, subroutine);
}

void cpu_return(cpu_handle cpu)
{
	assert(cpu != NULL);

	c_addr return_address = cpu_pop_long(cpu);
	DEBUG_PRINTF("returning from subroutine to 0x%08x\n", return_address);
	cpu_jump(cpu, return_address);
}
