#include "scheduler.h"
#include "process.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


c_scheduler_t* c_scheduler_init(c_cpu_t* cpu)
{
	assert(cpu != NULL);
	
	c_scheduler_t* scheduler = malloc(sizeof(c_scheduler_t));

	c_vector_create(sizeof(c_process_t*), 0, &scheduler->processes);
	scheduler->cpu = cpu;

	scheduler->ticks_since_ctx_switch = 0;
	scheduler->current_process_index = 0;

	return scheduler;
}

void c_scheduler_free(c_scheduler_t* scheduler)
{
	assert(scheduler != NULL);
	free(scheduler);
}

void c_scheduler_tick(c_scheduler_t* scheduler)
{
	assert(scheduler != NULL);
	assert(scheduler->cpu != NULL);

	if(scheduler->ticks_since_ctx_switch >= 10)
	{
		// switch to next process
		int prev_index = scheduler->current_process_index;
		scheduler->current_process_index =
			(scheduler->current_process_index + 1) % scheduler->processes.size;

		c_vector_get(&scheduler->processes,
				scheduler->current_process_index,
				&scheduler->cpu->context);

		scheduler->ticks_since_ctx_switch = 0;
	}

	++scheduler->ticks_since_ctx_switch;
}

