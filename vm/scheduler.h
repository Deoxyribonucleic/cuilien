#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "cpu.h"
#include "vector.h"

typedef struct scheduler
{
	cpu_t* cpu;
	vector_t processes;
	int ticks_since_ctx_switch;
	size_t current_process_index;
} scheduler_t;

scheduler_t* scheduler_init(cpu_t* cpu);
void scheduler_free(scheduler_t* scheduler);

void scheduler_tick(scheduler_t* scheduler);

#endif

