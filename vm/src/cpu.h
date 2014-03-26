#ifndef _CPU_H_
#define _CPU_H_

#include "mem.h"
#include "operand.h"

#define INSTRUCTION_LENGTH	12

typedef struct cpu
{
	struct
	{
		c_word a, b, c, d, ip;
	} reg;

	memory_t* memory;
} cpu_t;

struct cpu_instruction
{
	c_short operation;
	operand_t op1, op2;
};

typedef cpu_t* cpu_handle;
typedef void (*cpu_function)(cpu_handle, c_byte* op1, c_byte* op2, size_t size);

cpu_handle cpu_init(memory_t* memory);
void cpu_free(cpu_handle cpu);

void cpu_execute(cpu_handle cpu, struct cpu_instruction* instruction);
void cpu_read_instruction(cpu_handle cpu, c_addr address, struct cpu_instruction* output);

// Deprecated:
/*c_byte* cpu_resolve_parameter(cpu_handle cpu, c_byte flags, c_word value);
void cpu_write_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* src, size_t size);
void cpu_read_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* dest, size_t size);*/

#endif
