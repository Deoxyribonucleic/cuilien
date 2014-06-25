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
	case OP_REG_IP:
		return cpu->reg.ip;
	case OP_REG_FLAGS:
		return cpu->reg.flags;
	default:
		return 0x00000000;
	}
}

void set_operand_register_value(cpu_t* cpu, operand_t const* op, c_word value)
{
	switch(op->value)
	{
	case OP_REG_A:
		cpu->reg.a = value;
		break;
	case OP_REG_B:
		cpu->reg.b = value;
		break;
	case OP_REG_C:
		cpu->reg.c = value;
		break;
	case OP_REG_D:
		cpu->reg.d = value;
		break;
	case OP_REG_IP:
		cpu->reg.ip = value;
		break;
	case OP_REG_FLAGS:
		cpu->reg.flags = value;
		break;
	default:;
	}
}

// Lets the compiler take care of any big-/ little endian issues
inline void sized_copy_by_cast(void* dst, size_t dst_size, void const* src, size_t src_size)
{	
	c_word value;
	
	switch(src_size)
	{
	case 1:
		value = *(c_byte*)src;
		break;
	case 2:
		value = *(c_short*)src;
		break;
	case 4:
		value = *(c_word*)src;
		break;
	default:;
	}
	
	switch(dst_size)
	{
	case 1:
		*(c_byte*)dst = value;
		return;
	case 2:
		*(c_short*)dst = value;
		return;
	case 4:
		*(c_word*)dst = value;
		return;
	default:;
	}
}

void read_register_pointer(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	c_addr pointer = get_operand_register_value(cpu, op);
	mem_read_value(cpu->memory, pointer, out, size, false);
}

void read_register(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	c_word value = get_operand_register_value(cpu, op);
	sized_copy_by_cast(out, size, &value, size);
}

void read_value_pointer(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	mem_read_value(cpu->memory, op->value, out, size, false);
}

void read_value(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size)
{
	sized_copy_by_cast(out, size, &op->value, size);
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

c_word operand_read_value(cpu_t* cpu, operand_t const* op)
{
	c_word value = 0;
	operand_read(cpu, op, (c_byte*)&value, operand_get_size(op));
	return value;
}

void write_register_pointer(cpu_t* cpu, operand_t const* op, c_byte const* data, size_t size)
{
	c_addr pointer = get_operand_register_value(cpu, op);
	mem_write_value(cpu->memory, pointer, data, size);
}

void write_register(cpu_t* cpu, operand_t const* op, c_byte const* data, size_t size)
{
	c_word value;
	sized_copy_by_cast(&value, sizeof(value), data, size);
	set_operand_register_value(cpu, op, value);
}

void write_value_pointer(cpu_t* cpu, operand_t const* op, c_byte const* data, size_t size)
{
	mem_write_value(cpu->memory, op->value, data, size);
}

void operand_write(cpu_t* cpu, operand_t const* op, c_byte const* data, size_t size)
{
	error_clear();
	
	if(op->flags & PF_REGISTER && op->flags & PF_DEREFERENCE)
	{
		write_register_pointer(cpu, op, data, size);
	}
	else if(op->flags & PF_REGISTER)
	{
		write_register(cpu, op, data, size);
	}
	else if(op->flags & PF_DEREFERENCE)
	{
		write_value_pointer(cpu, op, data, size);
	}
	else
	{
		printf("(not an l-value)\n");
	}
}

void operand_write_value(cpu_t* cpu, operand_t const* op, c_word value)
{
	operand_write(cpu, op, (c_byte*)&value, operand_get_size(op));
}