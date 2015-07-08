#include "cpu.h"
#include "error.h"
#include "instr.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

c_cpu_handle c_cpu_init()
{
	c_build_instruction_vector();
	c_error_clear();

	/*if(memory == NULL)
	{
		error_last = ERR_CPU_INVALID_MEMORY;
		return NULL;
	}*/

	c_cpu_handle cpu = malloc(sizeof(c_cpu_t));

	if(cpu == NULL)
	{
		c_error_last = C_ERR_CPU_HOST_OOM;
		return NULL;
	}

	cpu->context = NULL;
	memset(&cpu->ivt, 0, sizeof(c_interrupt_vector_table_t));

	cpu->halted = true;

	return cpu;
}

void c_cpu_free(c_cpu_handle cpu)
{
	if(cpu)
		free(cpu);
}

void c_cpu_step(c_cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	struct c_cpu_instruction instruction;
	c_cpu_fetch(cpu, cpu->context->reg.ip, &instruction);
	cpu->context->reg.ip += INSTRUCTION_LENGTH;
	c_cpu_execute(cpu, &instruction);
}

void c_cpu_fetch(c_cpu_handle cpu, c_addr address, struct c_cpu_instruction* output)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	output->operation = c_mem_read_short(cpu->context->memory, address, true);
	output->op1.flags = c_mem_read_byte(cpu->context->memory, address + 2, true);
	output->op2.flags = c_mem_read_byte(cpu->context->memory, address + 3, true);
	output->op1.value = c_mem_read_long(cpu->context->memory, address + 4, true);
	output->op2.value = c_mem_read_long(cpu->context->memory, address + 8, true);
}

void c_cpu_execute(c_cpu_handle cpu, struct c_cpu_instruction* instruction)
{
	assert(cpu != NULL);

	if(instruction->operation >= C_INSTRUCTION_VECTOR_LENGTH ||
			c_instruction_vector[instruction->operation] == 0)
	{
		DEBUG_PRINTF("[cpu] invalid opcode: %04x\n", instruction->operation);
		return;
	}

	c_instruction_vector[instruction->operation](cpu, &instruction->op1, &instruction->op2);
}


bool c_cpu_get_flag(c_cpu_handle cpu, int flag)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	return cpu->context->reg.flags & flag;
}

void c_cpu_set_flag(c_cpu_handle cpu, int flag)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	cpu->context->reg.flags |= flag;
}

void c_cpu_clear_flag(c_cpu_handle cpu, int flag)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	cpu->context->reg.flags &= ~flag;
}


void c_cpu_jump(c_cpu_handle cpu, c_addr target)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);

	//printf("control flow: 0x%08x => 0x%08x\n", cpu->context->reg.ip, target);
	cpu->context->reg.ip = target;
}


void c_cpu_push_long(c_cpu_handle cpu, c_long value)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	cpu->context->reg.sp -= sizeof(c_long);
	c_mem_write_long(cpu->context->memory, cpu->context->reg.sp, value);
}

void c_cpu_push_short(c_cpu_handle cpu, c_short value)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	cpu->context->reg.sp -= sizeof(c_short);
	c_mem_write_short(cpu->context->memory, cpu->context->reg.sp, value);
}

void c_cpu_push_byte(c_cpu_handle cpu, c_byte value)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	cpu->context->reg.sp -= sizeof(c_byte);
	c_mem_write_byte(cpu->context->memory, cpu->context->reg.sp, value);
}


c_long c_cpu_pop_long(c_cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	c_long value = c_mem_read_long(cpu->context->memory, cpu->context->reg.sp, false);
	cpu->context->reg.sp += sizeof(c_long);
	return value;
}

c_short c_cpu_pop_short(c_cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	c_short value = c_mem_read_short(cpu->context->memory, cpu->context->reg.sp, false);
	cpu->context->reg.sp += sizeof(c_short);
	return value;
}

c_byte c_cpu_pop_byte(c_cpu_handle cpu)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	c_byte value = c_mem_read_byte(cpu->context->memory, cpu->context->reg.sp, false);
	cpu->context->reg.sp += sizeof(c_byte);
	return value;
}


void c_cpu_call(c_cpu_handle cpu, c_addr subroutine)
{
	assert(cpu != NULL);
	assert(cpu->context != NULL);
	assert(cpu->context->memory != NULL);

	DEBUG_PRINTF("calling subroutine at 0x%08x\n", subroutine);
	c_cpu_push_long(cpu, cpu->context->reg.ip);
	c_cpu_jump(cpu, subroutine);
}

void c_cpu_return(c_cpu_handle cpu)
{
	assert(cpu != NULL);

	c_addr return_address = c_cpu_pop_long(cpu);
	DEBUG_PRINTF("returning from subroutine to 0x%08x\n", return_address);
	c_cpu_jump(cpu, return_address);
}
