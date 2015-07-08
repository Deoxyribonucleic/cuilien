#ifndef _CPU_H_
#define _CPU_H_

#include "mem.h"
#include "operand.h"
#include "interrupt.h"

#define INSTRUCTION_LENGTH 12

#define FLAG_ZERO   1
#define FLAG_EQUAL  2
#define FLAG_GT     4
#define FLAG_LT     8
#define FLAG_ERROR  16

typedef struct c_context
{
	struct
	{
		c_reg a, b, c, d, ip, sp, flags;
	} reg;

	c_memory_t* memory;
} c_context_t;

typedef struct c_cpu
{
	c_context_t* context;
	c_interrupt_vector_table_t ivt;
	bool halted;
} c_cpu_t;

struct c_cpu_instruction
{
	c_short operation;
	c_operand_t op1, op2;
};

typedef c_cpu_t* c_cpu_handle;
typedef void (*c_cpu_function)(c_cpu_handle, c_byte* op1, c_byte* op2, size_t size);

c_cpu_handle c_cpu_init();
void c_cpu_free(c_cpu_handle cpu);

void c_cpu_step(c_cpu_handle cpu);
void c_cpu_execute(c_cpu_handle cpu, struct c_cpu_instruction* instruction);
void c_cpu_fetch(c_cpu_handle cpu, c_addr address, struct c_cpu_instruction* output);

bool c_cpu_get_flag(c_cpu_handle cpu, int flag);
void c_cpu_set_flag(c_cpu_handle cpu, int flag);
void c_cpu_clear_flag(c_cpu_handle cpu, int flag);

void c_cpu_jump(c_cpu_handle cpu, c_addr target);

void c_cpu_push_long(c_cpu_handle cpu, c_long value);
void c_cpu_push_short(c_cpu_handle cpu, c_short value);
void c_cpu_push_byte(c_cpu_handle cpu, c_byte value);

c_long c_cpu_pop_long(c_cpu_handle cpu);
c_short c_cpu_pop_short(c_cpu_handle cpu);
c_byte c_cpu_pop_byte(c_cpu_handle cpu);

void c_cpu_call(c_cpu_handle cpu, c_addr subroutine);
void c_cpu_return(c_cpu_handle);

#endif
