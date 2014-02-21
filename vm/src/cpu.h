#ifndef _CPU_H_
#define _CPU_H_

#include "mem.h"

#define PF_REGISTER	1
#define PF_DEREFERENCE	2
#define PF_SOURCE_SIZE	(4 | 8)

#define PF_GET_SIZE(flags) ((flags & PF_SOURCE_SIZE) + 1)

struct cpu_t
{
	struct
	{
		c_word a, b, c, d, ip;
	} reg;

	memory_t* memory;
};

struct cpu_operand
{
	c_byte flags;
	c_byte* value_ptr;
};

struct cpu_instruction
{
	c_short operation;
	struct cpu_operand op1, op2;
};

typedef struct cpu_t* cpu_handle;
typedef void (*cpu_function)(cpu_handle, c_byte* op1, c_byte* op2, size_t size);

cpu_handle cpu_init(memory_t* memory);
void cpu_free(cpu_handle cpu);

void cpu_read_instruction(cpu_handle cpu, c_addr address, struct cpu_instruction* output);
c_byte* cpu_resolve_parameter(cpu_handle cpu, c_byte flags, c_word value);

void cpu_write_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* src, size_t size);
void cpu_read_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* dest, size_t size);

#endif
