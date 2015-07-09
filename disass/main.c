#include <stdio.h>

#include "vm/mem.h"
#include "vm/error.h"
#include "vm/cpu.h"
#include "vm/vector.h"
#include "vm/scheduler.h"
#include "vm/process.h"
#include "vm/debug.h"
#include "vm/interrupt.h"
#include "vm/instr.h"
#include "vm/operand.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>

void help(char** argv)
{
	fprintf(stderr, "Usage: %s [-d | -b base_address] [-s start_address] [-n instructions] file\n\n-d:\n\tload memory dump file\n\n-b base_address:\n\tbegin loading binary data at base_address (hex) (cannot be used with -d)\n\tdefault: 0x0000000C\n\n-s start_address:\n\twhere to start disassembling (hex)\n\tdefault: base_address or (0x0000000C if dump)\n\n-n instructions:\n\tnumber of instructions to disassemble (broken (broken))\n\tdefault: length of file divided by 12 (all pages if dump)\n",
			argv[0]);
}

int disassemble_operand(c_operand_t* operand, char* out)
{
	char* buf = out;
	
	/* operand size */
	switch(c_operand_get_size(operand))
	{
		case C_PF_SIZE_WORD:
			buf += sprintf(buf, "word ");
			break;
		case C_PF_SIZE_SHORT:
			buf += sprintf(buf, "short ");
			break;
		case C_PF_SIZE_BYTE:
			buf += sprintf(buf, "byte ");
			break;
		default:
			break;
	}

	if(operand->flags & C_PF_DEREFERENCE)
		buf += sprintf(buf, "[");

	/* register or value */
	if(operand->flags & C_PF_REGISTER)
	{
		switch(operand->value)
		{
			case C_OP_REG_A:
				buf += sprintf(buf, "A");
				break;
			case C_OP_REG_B:
				buf += sprintf(buf, "B");
				break;
			case C_OP_REG_C:
				buf += sprintf(buf, "C");
				break;
			case C_OP_REG_D:
				buf += sprintf(buf, "D");
				break;
			case C_OP_REG_IP:
				buf += sprintf(buf, "IP");
				break;
			case C_OP_REG_SP:
				buf += sprintf(buf, "SP");
				break;
			case C_OP_REG_FLAGS:
				buf += sprintf(buf, "FLAGS");
				break;
			default:
				buf += sprintf(buf, "?");
				break;
		}
	}
	else
	{
		buf += sprintf(buf, "0x%x", operand->value);
	}

	if(operand->flags & C_PF_DEREFERENCE)
		buf += sprintf(buf, "]");

	return buf - out;
}

int disassemble(c_addr address, struct c_cpu_instruction* instruction, char* out)
{
	char* buf = out;
	if(instruction->operation >= C_INSTRUCTION_VECTOR_LENGTH || c_instruction_info[instruction->operation].name == NULL)
	{
		return sprintf(buf, "%08x: ?", address);
	}

	struct c_instr_info info = c_instruction_info[instruction->operation];
	buf += sprintf(buf, "%08x: %s ", address, info.name);

	if(info.operands > 0)
	{
		buf += disassemble_operand(&instruction->op1, buf);
	}
	
	if(info.operands > 1)
	{
		buf += sprintf(buf, ", ");
		buf += disassemble_operand(&instruction->op2, buf);
	}

	return buf - out;
}

int main(int argc, char** argv)
{
	int opt, data_dump = 0;
	c_addr base = 12;
	c_addr start = 0;
	size_t n_instr = 0;
	size_t length = 0;

	while ((opt = getopt(argc, argv, "db:s:n:")) != -1) {
		switch (opt) {
			case 'd':
				data_dump = 1;
				break;
			case 'b':
				base = strtol(optarg, NULL, 16);
				break;
			case 's':
				start = strtol(optarg, NULL, 16);
				break;
			case 'n':
				n_instr = strtol(optarg, NULL, 10);
				break;
			default: /* '?' */
				help(argv);
				exit(EXIT_FAILURE);
		}
	}
	
	if (optind >= argc)
	{
		help(argv);
		exit(EXIT_FAILURE);
	}
	
	char const* filename = argv[optind];

	c_cpu_handle cpu = c_cpu_init();
	if(c_error_last)
	{
		c_error_print(c_error_last);
		return 1;
	}
	
	c_memory_t* memory = c_mem_init(0xFFFFFFFF); // allow allocation of entire address space
	c_context_t context;
	context.memory = memory;
	cpu->context = &context;

	int instructions_disassembled;
	int instruction_offset = start % INSTRUCTION_LENGTH;
	char buffer[128];

	if(data_dump)
	{
		c_mem_load_dump_file(memory, filename);
		if(start == 0)
			start = 0x0000000C;

		c_addr ip = start;

		int i;
		for(i = 0; i < memory->page_table.size; ++i)
		{
			c_page_t* page = (c_page_t*)c_vector_resolve(&memory->page_table, i);
			c_addr page_addr = page->id * C_PAGE_SIZE;

			if(ip > page_addr)
				ip = start;

			if(page_addr > ip)
				ip = (c_addr)ceil((double)page_addr / (double)INSTRUCTION_LENGTH) * INSTRUCTION_LENGTH + instruction_offset;

			//printf("-- %08X (%08X) --\n", page->id * C_PAGE_SIZE, ip);
			printf("-- %08X --\n", page->id * C_PAGE_SIZE);

			int first = 1;
			while(ip < (page_addr + C_PAGE_SIZE) &&
					((instructions_disassembled++) < n_instr || n_instr == 0))
			{
				struct c_cpu_instruction instr;
				c_cpu_fetch(cpu, ip, &instr);

				disassemble(ip, &instr, buffer);
				printf("%s\n", buffer);

				ip += 12;
			}
		}
	}
	else
	{
		length = c_mem_load_file(memory, filename, base);
		if(n_instr == 0)
			n_instr = length / 12;
		if(start == 0)
			start = base;

		int i;
		c_addr ip = start;
		for(i=0; i<n_instr; ++i)
		{
			struct c_cpu_instruction instr;
			c_cpu_fetch(cpu, ip, &instr);

			disassemble(ip, &instr, buffer);
			printf("%s\n", buffer);

			ip += 12;
		}
	}

	c_mem_free(memory);
	c_cpu_free(cpu);

	return 0;
}

