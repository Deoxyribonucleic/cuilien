#include "operand.h"
#include "error.h"
#include "util.h"
#include "mem.h"
#include "cpu.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

size_t operand_get_size(operand_t const* op)
{
	return (op->flags & PF_SIZE_MASK) + 1;
}

size_t operand_get_operation_size(operand_t const* op1, operand_t const* op2)
{
	return min_size(operand_get_size(op1), operand_get_size(op2));
}

c_word get_operand_register_value(cpu_t* cpu, operand_t const* op)
{
	printf("reading value of register %d...\n", op->value);

	switch(op->value)
	{
	case OP_REG_A:
		return cpu->reg.a;
	case OP_REG_B:
		return cpu->reg.b;
	case OP_REG_C:
		return cpu->reg.c;
	case OP_REG_D:
		return cpu->reg.d;
	default:
		return 0x00000000;
	}
}

// I'm thinking the compiler should handle taking care of little/big endian problems here, yes?
inline void sized_copy_by_cast(void* dest, void* src, size_t size)
{
	c_word value = get_operand_register_value(cpu, op);

	switch(size)
	{
	case 1:
		*(c_byte)dest = *(c_byte)src;
		return;
	case 2:
		*(c_short)dest = *(c_short)src;
		return;
	case 4:
		*(c_word)dest = *(c_word)src;
		return;
	default:
	}
}

void read_register_pointer(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	c_addr pointer = get_operand_register_value(cpu, op);
	mem_read_value(cpu->memory, pointer, out, size);
}

void read_register(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	c_word value = get_operand_register_value(cpu, op);
	sized_copy_by_cast(out, &value, size);
}

void read_value_pointer(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	mem_read_value(cpu->memory, op->value, out, size);
}

void read_value(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	sized_copy_by_cast(out, &op->value, size);
}

void operand_read(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	error_clear();

	if(op->flags & PF_REGISTER && op->flags & PF_DEREFERENCE)
	{
		read_register_pointer(cpu, op, out, size);
	}
	else if(op->flags & PF_REGISTER)
	{
		read_register(cpu, op, out, size);
	}
	else if(op->flags & PF_DEREFERENCE)
	{
		read_value_pointer(cpu, op, out, size);
	}
	else
	{
		read_value(cpu, op, out, size);
	}
}

void write_register_pointer(cpu_t* cpu, operand_t const* op, c_byte* in, size_t size)
{
}
