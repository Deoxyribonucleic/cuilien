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
		c_reg a, b, c, d, ip, sp, flags;
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

void cpu_push_long(cpu_handle cpu, c_long value);
void cpu_push_short(cpu_handle cpu, c_short value);
void cpu_push_byte(cpu_handle cpu, c_byte value);

c_long cpu_pop_long(cpu_handle cpu);
c_short cpu_pop_short(cpu_handle cpu);
c_byte cpu_pop_byte(cpu_handle cpu);

void cpu_call(cpu_handle cpu, c_addr subroutine);
void cpu_return(cpu_handle);

#endif
