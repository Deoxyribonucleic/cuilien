#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <memory.h>

struct c_cpu;
typedef void(*c_interrupt_handler_t)(struct c_cpu* cpu);

typedef struct c_interrupt_vector_table
{
	c_interrupt_handler_t* handlers;
	size_t length;
} c_interrupt_vector_table_t;

#endif

