#ifndef _C_OPERAND_H_
#define _C_OPERAND_H_

#include "types.h"

#include <stddef.h>

#define C_PF_REGISTER     8
#define C_PF_DEREFERENCE  4
#define C_PF_SIZE_MASK    3

#define C_PF_SIZE_WORD    3
#define C_PF_SIZE_SHORT   1
#define C_PF_SIZE_BYTE    0

#define C_OP_REG_A        1
#define C_OP_REG_B        2
#define C_OP_REG_C        3
#define C_OP_REG_D        4
#define C_OP_REG_IP       5
#define C_OP_REG_SP       6
#define C_OP_REG_FLAGS    7

typedef struct c_operand
{
	c_byte flags;
	c_word value;
} c_operand_t;

typedef struct c_cpu c_cpu_t;

size_t c_operand_get_size(c_operand_t const* op);

void c_operand_read(c_cpu_t* cpu, c_operand_t const* op, c_byte* out, size_t size);
void c_operand_write(c_cpu_t* cpu, c_operand_t const* op, c_byte const* data, size_t size);

c_word c_operand_read_value(c_cpu_t* cpu, c_operand_t const* op);
void c_operand_write_value(c_cpu_t* cpu, c_operand_t const* op, c_word value);

#endif
