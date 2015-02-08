#include <stdio.h>

#include "vm/vm.h"
#include "vm/args.h"
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


void test_interrupt(cpu_t* cpu)
{
	printf("Test interrupt reporting in :) Value of A: %d", cpu->context->reg.a);
}

void print_vector_info(vector_t* vector)
{
	printf("Element size: %d | Size: %d | Capacity: %d\n", vector->element_size, vector->size, vector->capacity);
	int i;
	printf("Listing:\n");
	for(i=0; i<vector->size; ++i)
	{
		char* string;
		vector_get(vector, i, &string);
		printf("* %p: %s\n", string, string);
	}
	printf("---\n");
}

int main(int argc, char** args)
{
	struct args_info arguments;
	args_parse(args, argc, &arguments);

	printf("Program: %s\n", arguments.program);

	int error;

	// create two processes
	process_t processes[2];
	memset(processes, 0, sizeof(process_t) * 2);

	processes[0].context.memory = mem_init(1024*1024*10); // 10 MB
	if(error_last)
	{
		error_print(error_last);
		return 1;
	}

	processes[1].context.memory = mem_init(1024*1024*10); // 10 MB
	if(error_last)
	{
		error_print(error_last);
		return 1;
	}

	cpu_handle cpu = cpu_init();
	if(error_last)
	{
		error_print(error_last);
		return 1;
	}

	// Build interrupt vector table
	interrupt_handler_t interrupt_handlers[] = { test_interrupt };
	cpu->ivt.handlers = interrupt_handlers;
	cpu->ivt.length = 1;

	scheduler_t* scheduler = scheduler_init(cpu);
	void* process_addr = &processes[0];
	vector_push_back(&scheduler->processes, &process_addr);
	process_addr = &processes[1];
	vector_push_back(&scheduler->processes, &process_addr);

	if((error = vm_init(&arguments)))
		error_print(error);


	page_info_t shared_info = { PAGE_WRITE, false };
	c_byte* shared_mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	//page_map(&memory->page_table, 0, &shared_info, shared_mem);


	// Load test program into memory
	c_addr programStart = 0xff000000;
	mem_load_file(processes[0].context.memory, arguments.program, programStart);
	mem_load_file(processes[1].context.memory, arguments.program, programStart);


	// load brainfuck program into memory at 0xffe00000
	mem_load_file(processes[0].context.memory, "vmtest/programs/99bottles.bf", 0xffe00000);
	mem_load_file(processes[1].context.memory, "vmtest/programs/99bottles.bf", 0xffe00000);

	// Initialize instruction pointers
	processes[0].context.reg.ip = programStart;
	processes[1].context.reg.ip = programStart;
	
	cpu->context = &processes[0].context;

	// Step through the instructions until HALT is reached
	cpu->halted = false;
	while(!cpu->halted)
	{
		scheduler_tick(scheduler);

		cpu_step(cpu);
		DEBUG_PRINTF("--\n");
	}

	free(shared_mem);

	scheduler_free(scheduler);
	cpu_free(cpu);
	mem_free(processes[0].context.memory);
	mem_free(processes[1].context.memory);

	return 0;
}
