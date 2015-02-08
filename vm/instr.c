#include "instr.h"

#include "cpu.h"
#include "operand.h"
#include "debug.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

instruction_function instruction_vector[INSTRUCTION_VECTOR_LENGTH];
bool instruction_vector_built = false;

void NOP(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	DEBUG_PRINTF("NOP\n");
}

void HALT(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	DEBUG_PRINTF("HALT\n");
	cpu->halted = true;
}

void INT(cpu_t* cpu, operand_t const* index_operand, operand_t const* op2)
{
	DEBUG_PRINTF("INT\n");
	c_byte index = operand_read_value(cpu, index_operand);

	if(index < cpu->ivt.length)
		cpu->ivt.handlers[index](cpu);
}

void MOV(cpu_t* cpu, operand_t const* dest, operand_t const* src)
{
	DEBUG_PRINTF("MOV\n");

	c_word src_val = operand_read_value(cpu, src);
	c_word dst_val = operand_read_value(cpu, dest);

	operand_write_value(cpu, dest, src_val);

	c_word dst_val_new = operand_read_value(cpu, dest);
}

void JMP(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("JMP\n");
	cpu_jump(cpu, operand_read_value(cpu, operand));
}

void INC(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("INC\n");
	c_word value = operand_read_value(cpu, operand);
	operand_write_value(cpu, operand, value+1);
}

void DEC(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("DEC\n");
	c_word value = operand_read_value(cpu, operand);
	operand_write_value(cpu, operand, value-1);
}

void ADD(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	DEBUG_PRINTF("ADD\n");

	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, op1_val + op2_val);
}

void SUB(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	DEBUG_PRINTF("SUB\n");

	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, op1_val - op2_val);
}

void MUL(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	DEBUG_PRINTF("MUL\n");

	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, op1_val * op2_val);
}

void DIV(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("DIV\n");

	c_word op2_val = operand_read_value(cpu, op2);
	c_word op1_val = operand_read_value(cpu, op1);

	operand_write_value(cpu, op1, (c_word)(op1_val / op2_val));
	cpu->context->reg.a = op1_val % op2_val;
}

void SHOW(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("SHOW\n");
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

void TEST(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("TEST\n");
	c_word value = operand_read_value(cpu, operand);

	cpu->context->reg.flags = 0;

	if(value == 0)
	{
		cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		cpu_set_flag(cpu, FLAG_GT);
	}
}

void STEST(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("STEST\n");

	c_sword value = operand_read_value(cpu, operand);

	cpu->context->reg.flags = 0;

	if(value == 0)
	{
		cpu_set_flag(cpu, FLAG_ZERO);
	}
	else if(value > 0)
	{
		cpu_set_flag(cpu, FLAG_GT);
	}
	else
	{
		cpu_set_flag(cpu, FLAG_LT);
	}
}

void CMP(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("CMP\n");

	c_word value1 = operand_read_value(cpu, op1);
	c_word value2 = operand_read_value(cpu, op2);

	cpu->context->reg.flags = 0;

	if(value1 == value2)
	{
		cpu_set_flag(cpu, FLAG_EQUAL);
	}
	else if(value1 > value2)
	{
		cpu_set_flag(cpu, FLAG_GT);
	}
	else
	{
		cpu_set_flag(cpu, FLAG_LT);
	}
}

void SCMP(cpu_t* cpu, operand_t const* op1, operand_t const* op2)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("SCMP\n");

	c_sword value1 = operand_read_value(cpu, op1);
	c_sword value2 = operand_read_value(cpu, op2);

	cpu->context->reg.flags = 0;

	if(value1 == value2)
	{
		cpu_set_flag(cpu, FLAG_EQUAL);
	}
	else if(value1 > value2)
	{
		cpu_set_flag(cpu, FLAG_GT);
	}
	else
	{
		cpu_set_flag(cpu, FLAG_LT);
	}
}

void JZ(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JZ\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(cpu_get_flag(cpu, FLAG_ZERO))
	{
		cpu_jump(cpu, target);
	}
}

void JNZ(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JNZ\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(!cpu_get_flag(cpu, FLAG_ZERO))
	{
		cpu_jump(cpu, target);
	}
}

void JEQ(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JEQ\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(cpu_get_flag(cpu, FLAG_EQUAL))
	{
		cpu_jump(cpu, target);
	}
}

void JNEQ(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JNEQ\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(!cpu_get_flag(cpu, FLAG_EQUAL))
	{
		cpu_jump(cpu, target);
	}
}


void JGT(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JGT\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(cpu_get_flag(cpu, FLAG_GT))
	{
		cpu_jump(cpu, target);
	}
}
void JNGT(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JNGT\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(!cpu_get_flag(cpu, FLAG_GT))
	{
		cpu_jump(cpu, target);
	}
}

void JLT(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JLT\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(cpu_get_flag(cpu, FLAG_LT))
	{
		cpu_jump(cpu, target);
	}
}
void JNLT(cpu_t* cpu, operand_t const* target_operand, operand_t const* unused)
{
	DEBUG_PRINTF("JNLT\n");
	c_word target = operand_read_value(cpu, target_operand);

	if(!cpu_get_flag(cpu, FLAG_LT))
	{
		cpu_jump(cpu, target);
	}
}

void AND(cpu_t* cpu, operand_t const* operand1, operand_t const* operand2)
{
	DEBUG_PRINTF("AND\n");
	c_word value1 = operand_read_value(cpu, operand1);
	c_word value2 = operand_read_value(cpu, operand2);

	operand_write_value(cpu, operand1, value1 & value2);
}
void OR(cpu_t* cpu, operand_t const* operand1, operand_t const* operand2)
{
	DEBUG_PRINTF("OR\n");
	c_word value1 = operand_read_value(cpu, operand1);
	c_word value2 = operand_read_value(cpu, operand2);

	operand_write_value(cpu, operand1, value1 | value2);
}
void XOR(cpu_t* cpu, operand_t const* operand1, operand_t const* operand2)
{
	DEBUG_PRINTF("XOR\n");
	c_word value1 = operand_read_value(cpu, operand1);
	c_word value2 = operand_read_value(cpu, operand2);

	operand_write_value(cpu, operand1, value1 ^ value2);
}
void NOT(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("NOT\n");
	operand_write_value(cpu, operand, ~operand_read_value(cpu, operand));
}

void PUSH(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("PUSH\n");
	c_word value = operand_read_value(cpu, operand);

	switch(operand_get_size(operand))
	{
	case sizeof(c_long):
		cpu_push_long(cpu, value);
		return;
	case sizeof(c_short):
		cpu_push_short(cpu, value);
		return;
	case sizeof(c_byte):
		cpu_push_byte(cpu, value);
		return;
	}
}
void POP(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("POP\n");
	switch(operand_get_size(operand))
	{
	case sizeof(c_long):
		operand_write_value(cpu, operand, cpu_pop_long(cpu));
		return;
	case sizeof(c_short):
		operand_write_value(cpu, operand, cpu_pop_short(cpu));
		return;
	case sizeof(c_byte):
		operand_write_value(cpu, operand, cpu_pop_byte(cpu));
		return;
	}
}

void CALL(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("CALL\n");
	c_word address = operand_read_value(cpu, operand);
	cpu_call(cpu, address);
}

void RET(cpu_t* cpu, operand_t const* unused1, operand_t const* unused2)
{
	DEBUG_PRINTF("RET\n");
	cpu_return(cpu);
}

void PUTC(cpu_t* cpu, operand_t const* operand, operand_t const* unused)
{
	DEBUG_PRINTF("PUTC\n");
	char character = (char)operand_read_value(cpu, operand);
	putchar(character);
}

void build_instruction_vector()
{
	if(instruction_vector_built)
		return;

	memset(instruction_vector, 0, sizeof(instruction_function) * INSTRUCTION_VECTOR_LENGTH);
	instruction_function* set = instruction_vector;

	set[INSTR_NOP]  = NOP;
	set[INSTR_HALT] = HALT;
	set[INSTR_INT]  = INT;
	set[INSTR_MOV]  = MOV;

	set[INSTR_JMP]  = JMP;

	set[INSTR_INC]  = INC;
	set[INSTR_DEC]  = DEC;

	set[INSTR_ADD]  = ADD;
	set[INSTR_SUB]  = SUB;
	set[INSTR_MUL]  = MUL;
	set[INSTR_DIV]  = DIV;

	set[INSTR_SHOW] = SHOW;

	set[INSTR_CMP]  = CMP;
	set[INSTR_SCMP] = SCMP;
	set[INSTR_TEST] = TEST;
	set[INSTR_STEST]= STEST;

	set[INSTR_JZ]   = JZ;
	set[INSTR_JNZ]  = JNZ;
	set[INSTR_JEQ]  = JEQ;
	set[INSTR_JNEQ] = JNEQ;
	set[INSTR_JGT]  = JGT;
	set[INSTR_JNGT] = JNGT;
	set[INSTR_JLT]  = JLT;
	set[INSTR_JNLT] = JNLT;

	set[INSTR_AND]  = AND;
	set[INSTR_OR]   = OR;
	set[INSTR_XOR]  = XOR;
	set[INSTR_NOT]  = NOT;

	set[INSTR_PUSH] = PUSH;
	set[INSTR_POP]  = POP;

	set[INSTR_CALL] = CALL;
	set[INSTR_RET]  = RET;

	set[INSTR_PUTC] = PUTC;

	instruction_vector_built = true;
}
