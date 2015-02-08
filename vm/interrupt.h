#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <memory.h>

struct cpu;
typedef void(*interrupt_handler_t)(struct cpu* cpu);

typedef struct interrupt_vector_table
{
	interrupt_handler_t* handlers;
	size_t length;
} interrupt_vector_table_t;

#endif

