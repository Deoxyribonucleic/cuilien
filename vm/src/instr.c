#include "instr.h"

#include "cpu.h"
#include "operand.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

instruction_function instruction_vector[INSTRUCTION_VECTOR_LENGTH];
bool instruction_vector_built = false;

void NOP(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("NOP\n");
}
void HALT(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("HALT\n");
}
void INT(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("INT\n");
}
void MOV(cpu_t* cpu, operand_t const* dest, operand_t const* src)
{
	printf("MOV\n");
}

void build_instruction_vector()
{
	if(instruction_vector_built)
		return;

	memset(instruction_vector, 0, sizeof(instruction_function) * INSTRUCTION_VECTOR_LENGTH);
	instruction_function* set = instruction_vector;
	
	set[INSTR_NOP]	= NOP;
	set[INSTR_HALT]	= HALT;
	set[INSTR_INT]	= INT;
	set[INSTR_MOV]	= MOV;

	instruction_vector_built = true;
}
