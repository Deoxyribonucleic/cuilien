#include <stdio.h>

#include "vm/mem.h"
#include "vm/error.h"
#include "vm/cpu.h"
#include "vm/vector.h"
#include "vm/scheduler.h"
#include "vm/process.h"
#include "vm/debug.h"
#include "vm/interrupt.h"

#include <stdlib.h>
#include <string.h>


void test_interrupt(c_cpu_t* cpu)
{
	printf("Test interrupt reporting in :) Value of A: %d", cpu->context->reg.a);
}

void print_vector_info(c_vector_t* vector)
{
	printf("Element size: %lu | Size: %lu | Capacity: %lu\n", vector->element_size, vector->size, vector->capacity);
	int i;
	printf("Listing:\n");
	for(i=0; i<vector->size; ++i)
	{
		char* string;
		c_vector_get(vector, i, &string);
		printf("* %p: %s\n", string, string);
	}
	printf("---\n");
}

int main(int argc, char** args)
{
	char* const program = "test.cx";

	printf("Program: %s\n", program);

	int error;

	// create two processes
	c_process_t processes[2];
	memset(processes, 0, sizeof(c_process_t) * 2);

	processes[0].context.memory = c_mem_init(1024*1024*10); // 10 MB
	if(c_error_last)
	{
		c_error_print(c_error_last);
		return 1;
	}

	processes[1].context.memory = c_mem_init(1024*1024*10); // 10 MB
	if(c_error_last)
	{
		c_error_print(c_error_last);
		return 1;
	}

	c_cpu_handle cpu = c_cpu_init();
	if(c_error_last)
	{
		c_error_print(c_error_last);
		return 1;
	}

	// Build interrupt vector table
	c_interrupt_handler_t interrupt_handlers[] = { test_interrupt };
	cpu->ivt.handlers = interrupt_handlers;
	cpu->ivt.length = 1;

	c_scheduler_t* scheduler = c_scheduler_init(cpu);
	void* process_addr = &processes[0];
	c_vector_push_back(&scheduler->processes, &process_addr);
	process_addr = &processes[1];
	c_vector_push_back(&scheduler->processes, &process_addr);

	c_page_info_t shared_info = { C_PAGE_WRITE, false };
	c_byte* shared_mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	//page_map(&memory->page_table, 0, &shared_info, shared_mem);


	// Load test program into memory
	c_addr programStart = 0xff000000;
	c_mem_load_file(processes[0].context.memory, program, programStart);
	c_mem_load_file(processes[1].context.memory, program, programStart);


	// load brainfuck program into memory at 0xffe00000
	c_mem_load_file(processes[0].context.memory, "vmtest/programs/99bottles.bf", 0xffe00000);
	c_mem_load_file(processes[1].context.memory, "vmtest/programs/99bottles.bf", 0xffe00000);

	// Initialize instruction pointers
	processes[0].context.reg.ip = programStart;
	processes[1].context.reg.ip = programStart;
	
	cpu->context = &processes[0].context;

	// Step through the instructions until HALT is reached
	cpu->halted = false;
	while(!cpu->halted)
	{
		c_scheduler_tick(scheduler);

		c_cpu_step(cpu);
		DEBUG_PRINTF("--\n");
	}

	free(shared_mem);

	c_scheduler_free(scheduler);
	c_cpu_free(cpu);
	c_mem_free(processes[0].context.memory);
	c_mem_free(processes[1].context.memory);

	return 0;
}

