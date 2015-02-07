#include "scheduler.h"
#include "process.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


scheduler_t* scheduler_init(cpu_t* cpu)
{
	assert(cpu != NULL);
	
	scheduler_t* scheduler = malloc(sizeof(scheduler_t));

	vector_create(sizeof(process_t*), 0, &scheduler->processes);
	scheduler->cpu = cpu;

	scheduler->ticks_since_ctx_switch = 0;
	scheduler->current_process_index = 0;

	return scheduler;
}

void scheduler_free(scheduler_t* scheduler)
{
	assert(scheduler != NULL);
	free(scheduler);
}

void scheduler_tick(scheduler_t* scheduler)
{
	assert(scheduler != NULL);
	assert(scheduler->cpu != NULL);

	if(scheduler->ticks_since_ctx_switch >= 10)
	{
		// switch to next process
		int prev_index = scheduler->current_process_index;
		scheduler->current_process_index =
			(scheduler->current_process_index + 1) % scheduler->processes.size;

		vector_get(&scheduler->processes,
				scheduler->current_process_index,
				&scheduler->cpu->context);

		scheduler->ticks_since_ctx_switch = 0;
	}

	++scheduler->ticks_since_ctx_switch;
}

