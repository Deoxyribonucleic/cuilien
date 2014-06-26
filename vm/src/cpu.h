#ifndef _CPU_H_
#define _CPU_H_

#include "mem.h"
#include "operand.h"

#define INSTRUCTION_LENGTH	12

#define FLAG_ZERO	1
#define FLAG_EQUAL	2
#define FLAG_GT		4
#define FLAG_LT		8
#define FLAG_ERROR	16

typedef struct cpu
{
	struct
	{
		c_reg a, b, c, d, ip, flags;
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

bool cpu_get_flag(cpu_handle cpu, int flag);
void cpu_set_flag(cpu_handle cpu, int flag);
void cpu_clear_flag(cpu_handle cpu, int flag);

void cpu_jump(cpu_handle cpu, c_addr target);

// Deprecated:
/*c_byte* cpu_resolve_parameter(cpu_handle cpu, c_byte flags, c_word value);
void cpu_write_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* src, size_t size);
void cpu_read_parameter(cpu_handle cpu, c_byte flags, c_word value, c_byte* dest, size_t size);*/

#endif
