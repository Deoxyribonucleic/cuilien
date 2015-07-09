#include "operand.h"
#include "error.h"
#include "util.h"
#include "mem.h"
#include "cpu.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

size_t c_operand_get_size(c_operand_t const* op)
{
	return (op->flags & C_PF_SIZE_MASK) + 1;
}

size_t c_operand_get_operation_size(c_operand_t const* op1, c_operand_t const* op2)
{
	return min_size(c_operand_get_size(op1), c_operand_get_size(op2));
}

c_word c_get_operand_register_value(c_cpu_t* cpu, c_operand_t const* op)
{
	switch(op->value)
	{
	case C_OP_REG_A:
		return cpu->context->reg.a;
	case C_OP_REG_B:
		return cpu->context->reg.b;
	case C_OP_REG_C:
		return cpu->context->reg.c;
	case C_OP_REG_D:
		return cpu->context->reg.d;
	case C_OP_REG_IP:
		return cpu->context->reg.ip;
	case C_OP_REG_SP:
		return cpu->context->reg.sp;
	case C_OP_REG_FLAGS:
		return cpu->context->reg.flags;
	default:
		return 0x00000000;
	}
}

void c_set_operand_register_value(c_cpu_t* cpu, c_operand_t const* op, c_word value)
{
	switch(op->value)
	{
	case C_OP_REG_A:
		cpu->context->reg.a = value;
		break;
	case C_OP_REG_B:
		cpu->context->reg.b = value;
		break;
	case C_OP_REG_C:
		cpu->context->reg.c = value;
		break;
	case C_OP_REG_D:
		cpu->context->reg.d = value;
		break;
	case C_OP_REG_IP:
		cpu->context->reg.ip = value;
		break;
	case C_OP_REG_SP:
		cpu->context->reg.sp = value;
		break;
	case C_OP_REG_FLAGS:
		cpu->context->reg.flags = value;
		break;
	default:;
	}
}

// Lets the compiler take care of any big-/ little endian issues
static inline void sized_copy_by_cast(void* dst, size_t dst_size, void const* src, size_t src_size)
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

void read_register_pointer(c_cpu_t* cpu, c_operand_t const* op, c_byte* out, size_t size)
{
	c_addr pointer = c_get_operand_register_value(cpu, op);
	c_mem_read_value(cpu->context->memory, pointer, out, size, false);
}

void read_register(c_cpu_t* cpu, c_operand_t const* op, c_byte* out, size_t size)
{
	c_word value = c_get_operand_register_value(cpu, op);
	sized_copy_by_cast(out, size, &value, size);
}

void read_value_pointer(c_cpu_t* cpu, c_operand_t const* op, c_byte* out, size_t size)
{
	c_mem_read_value(cpu->context->memory, op->value, out, size, false);
}

void read_value(c_cpu_t* cpu, c_operand_t const* op, c_byte* out, size_t size)
{
	sized_copy_by_cast(out, size, &op->value, size);
}

void c_operand_read(c_cpu_t* cpu, c_operand_t const* op, c_byte* out, size_t size)
{
	c_error_clear();
	
	if(op->flags & C_PF_REGISTER && op->flags & C_PF_DEREFERENCE)
	{
		read_register_pointer(cpu, op, out, size);
	}
	else if(op->flags & C_PF_REGISTER)
	{
		read_register(cpu, op, out, size);
	}
	else if(op->flags & C_PF_DEREFERENCE)
	{
		read_value_pointer(cpu, op, out, size);
	}
	else
	{
		read_value(cpu, op, out, size);
	}
}

c_word c_operand_read_value(c_cpu_t* cpu, c_operand_t const* op)
{
	c_word value = 0;
	c_operand_read(cpu, op, (c_byte*)&value, c_operand_get_size(op));
	return value;
}

void write_register_pointer(c_cpu_t* cpu, c_operand_t const* op, c_byte const* data, size_t size)
{
	c_addr pointer = c_get_operand_register_value(cpu, op);
	c_mem_write_value(cpu->context->memory, pointer, data, size);
}

void write_register(c_cpu_t* cpu, c_operand_t const* op, c_byte const* data, size_t size)
{
	c_word value;
	sized_copy_by_cast(&value, sizeof(value), data, size);
	c_set_operand_register_value(cpu, op, value);
}

void write_value_pointer(c_cpu_t* cpu, c_operand_t const* op, c_byte const* data, size_t size)
{
	c_mem_write_value(cpu->context->memory, op->value, data, size);
}

void c_operand_write(c_cpu_t* cpu, c_operand_t const* op, c_byte const* data, size_t size)
{
	c_error_clear();
	
	if(op->flags & C_PF_REGISTER && op->flags & C_PF_DEREFERENCE)
	{
		write_register_pointer(cpu, op, data, size);
	}
	else if(op->flags & C_PF_REGISTER)
	{
		write_register(cpu, op, data, size);
	}
	else if(op->flags & C_PF_DEREFERENCE)
	{
		write_value_pointer(cpu, op, data, size);
	}
	else
	{
		DEBUG_PRINTF("(not an l-value)\n");
	}
}

void c_operand_write_value(c_cpu_t* cpu, c_operand_t const* op, c_word value)
{
	c_operand_write(cpu, op, (c_byte*)&value, c_operand_get_size(op));
}

