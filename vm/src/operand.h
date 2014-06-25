#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "types.h"

#include <stddef.h>

#define PF_REGISTER		8
#define PF_DEREFERENCE	4
#define PF_SIZE_MASK	3

#define PF_SIZE_WORD	3
#define PF_SIZE_SHORT	1
#define PF_SIZE_BYTE	0

#define OP_REG_A		1
#define OP_REG_B		2
#define OP_REG_C		3
#define OP_REG_D		4
#define OP_REG_IP		5
#define OP_REG_FLAGS	6

typedef struct operand
{
	c_byte flags;
	c_word value;
} operand_t;

typedef struct cpu cpu_t;

size_t operand_get_size(operand_t const* op);

void operand_read(cpu_t* cpu, operand_t const* op, c_byte* out, size_t size);
void operand_write(cpu_t* cpu, operand_t const* op, c_byte const* data, size_t size);

c_word operand_read_value(cpu_t* cpu, operand_t const* op);
void operand_write_value(cpu_t* cpu, operand_t const* op, c_word value);

#endif
