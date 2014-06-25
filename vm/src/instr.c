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
	
	c_word src_val = operand_read_value(cpu, src);
	c_word dst_val = operand_read_value(cpu, dest);

	operand_write_value(cpu, dest, src_val);

	c_word dst_val_new = operand_read_value(cpu, dest);

	// printf("dst_val: 0x%08X\n", dst_val);
	// printf("src_val: 0x%08X\n", src_val);
	// printf("dst_val_new: 0x%08X\n", dst_val_new);
}

void INC(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	printf("INC\n");
	c_word value = operand_read_value(cpu, operand);
	operand_write_value(cpu, operand, value+1);
}

void DEC(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	printf("DEC\n");
	c_word value = operand_read_value(cpu, operand);
	operand_write_value(cpu, operand, value-1);
}

void ADD(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("ADD\n");
	
	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, op1_val + op2_val);
}

void SUB(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("SUB\n");
	
	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, op1_val - op2_val);
}

void MUL(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("MUL\n");
	
	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, op1_val * op2_val);
}

void DIV(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	printf("DIV\n");
	
	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, (c_word)(op1_val / op2_val));
	cpu->reg.a = op1_val % op2_val;
}

void SHOW(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	printf("SHOW\n");
	c_word value = operand_read_value(cpu, operand);

	switch(operand_get_size(operand))
	{
	case 1:
		printf("value (byte): %02x\n", value);
		break;
	case 2:
		printf("value (short): %04x\n", value);
		break;
	case 4:
		printf("value (long): %08x\n", value);
		break;
	}
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

	set[INSTR_INC]	= INC;
	set[INSTR_DEC]	= DEC;

	set[INSTR_ADD]	= ADD;
	set[INSTR_SUB]	= SUB;
	set[INSTR_MUL]	= MUL;
	set[INSTR_DIV]	= DIV;

	set[INSTR_SHOW]	= SHOW;

	instruction_vector_built = true;
}
