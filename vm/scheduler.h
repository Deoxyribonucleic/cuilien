#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "cpu.h"
#include "vector.h"

typedef struct c_scheduler
{
	c_cpu_t* cpu;
	c_vector_t processes;
	int ticks_since_ctx_switch;
	size_t current_process_index;
} c_scheduler_t;

c_scheduler_t* c_scheduler_init(c_cpu_t* cpu);
void c_scheduler_free(c_scheduler_t* scheduler);

void c_scheduler_tick(c_scheduler_t* scheduler);

#endif

