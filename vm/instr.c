#include "instr.h"

#include "cpu.h"
#include "operand.h"
#include "debug.h"

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

c_instruction_function c_instruction_vector[C_INSTRUCTION_VECTOR_LENGTH];
struct c_instr_info c_instruction_info[C_INSTRUCTION_VECTOR_LENGTH];
bool instruction_vector_built = false;

void NOP(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	DEBUG_PRINTF("NOP\n");
}

void HALT(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	DEBUG_PRINTF("HALT\n");
	cpu->halted = true;
}

void INT(c_cpu_t* cpu, c_operand_t const* index_operand, c_operand_t const* op2)
{
	DEBUG_PRINTF("INT\n");
	c_byte index = c_operand_read_value(cpu, index_operand);

	if(index < cpu->ivt.length && cpu->ivt.handlers[index] != NULL)
		cpu->ivt.handlers[index](cpu);
}

void MOV(c_cpu_t* cpu, c_operand_t const* dest, c_operand_t const* src)
{
	DEBUG_PRINTF("MOV\n");

	c_word src_val = c_operand_read_value(cpu, src);
	c_word dst_val = c_operand_read_value(cpu, dest);

	c_operand_write_value(cpu, dest, src_val);

	c_word dst_val_new = c_operand_read_value(cpu, dest);
}

void JMP(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JMP\n");
	c_cpu_jump(cpu, c_operand_read_value(cpu, operand));
}

void INC(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("INC\n");
	c_word value = c_operand_read_value(cpu, operand);
	c_operand_write_value(cpu, operand, value+1);
}

void DEC(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("DEC\n");
	c_word value = c_operand_read_value(cpu, operand);
	c_operand_write_value(cpu, operand, value-1);
}

void ADD(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	DEBUG_PRINTF("ADD\n");

	c_word op2_val = c_operand_read_value(cpu, op2);
	c_word op1_val = c_operand_read_value(cpu, op1);

	c_operand_write_value(cpu, op1, op1_val + op2_val);
}

void SUB(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	DEBUG_PRINTF("SUB\n");

	c_word op2_val = c_operand_read_value(cpu, op2);
	c_word op1_val = c_operand_read_value(cpu, op1);

	c_operand_write_value(cpu, op1, op1_val - op2_val);
}

void MUL(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	DEBUG_PRINTF("MUL\n");

	c_word op2_val = c_operand_read_value(cpu, op2);
	c_word op1_val = c_operand_read_value(cpu, op1);

	c_operand_write_value(cpu, op1, op1_val * op2_val);
}

void DIV(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("DIV\n");

	c_word op2_val = c_operand_read_value(cpu, op2);
	c_word op1_val = c_operand_read_value(cpu, op1);

	if(op2_val == 0)
	{
		cpu->context->reg.flags = FLAG_ERROR;
		return;
	}

	c_operand_write_value(cpu, op1, (c_word)(op1_val / op2_val));
	cpu->context->reg.a = op1_val % op2_val;
}

void SHOW(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("SHOW\n");
	c_word value = c_operand_read_value(cpu, operand);

	switch(c_operand_get_size(operand))
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

void TEST(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("TEST\n");
	c_word value = c_operand_read_value(cpu, operand);

	cpu->context->reg.flags = 0;

	if(value == 0)
	{
		c_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else
	{
		c_cpu_set_flag(cpu, FLAG_GT);
	}
}

void STEST(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("STEST\n");

	c_sword value = c_operand_read_value(cpu, operand);

	cpu->context->reg.flags = 0;

	if(value == 0)
	{
		c_cpu_set_flag(cpu, FLAG_ZERO);
	}
	else if(value > 0)
	{
		c_cpu_set_flag(cpu, FLAG_GT);
	}
	else
	{
		c_cpu_set_flag(cpu, FLAG_LT);
	}
}

void CMP(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("CMP\n");

	c_word value1 = c_operand_read_value(cpu, op1);
	c_word value2 = c_operand_read_value(cpu, op2);

	cpu->context->reg.flags = 0;

	if(value1 == value2)
	{
		c_cpu_set_flag(cpu, FLAG_EQUAL);
	}
	else if(value1 > value2)
	{
		c_cpu_set_flag(cpu, FLAG_GT);
	}
	else
	{
		c_cpu_set_flag(cpu, FLAG_LT);
	}
}

void SCMP(c_cpu_t* cpu, c_operand_t const* op1, c_operand_t const* op2)
{
	assert(cpu->context != NULL);
	DEBUG_PRINTF("SCMP\n");

	c_sword value1 = c_operand_read_value(cpu, op1);
	c_sword value2 = c_operand_read_value(cpu, op2);

	cpu->context->reg.flags = 0;

	if(value1 == value2)
	{
		c_cpu_set_flag(cpu, FLAG_EQUAL);
	}
	else if(value1 > value2)
	{
		c_cpu_set_flag(cpu, FLAG_GT);
	}
	else
	{
		c_cpu_set_flag(cpu, FLAG_LT);
	}
}

void JZ(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JZ\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(c_cpu_get_flag(cpu, FLAG_ZERO))
	{
		c_cpu_jump(cpu, target);
	}
}

void JNZ(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JNZ\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(!c_cpu_get_flag(cpu, FLAG_ZERO))
	{
		c_cpu_jump(cpu, target);
	}
}

void JEQ(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JEQ\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(c_cpu_get_flag(cpu, FLAG_EQUAL))
	{
		c_cpu_jump(cpu, target);
	}
}

void JNEQ(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JNEQ\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(!c_cpu_get_flag(cpu, FLAG_EQUAL))
	{
		c_cpu_jump(cpu, target);
	}
}


void JGT(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JGT\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(c_cpu_get_flag(cpu, FLAG_GT))
	{
		c_cpu_jump(cpu, target);
	}
}
void JNGT(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JNGT\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(!c_cpu_get_flag(cpu, FLAG_GT))
	{
		c_cpu_jump(cpu, target);
	}
}

void JLT(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JLT\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(c_cpu_get_flag(cpu, FLAG_LT))
	{
		c_cpu_jump(cpu, target);
	}
}
void JNLT(c_cpu_t* cpu, c_operand_t const* target_operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("JNLT\n");
	c_word target = c_operand_read_value(cpu, target_operand);

	if(!c_cpu_get_flag(cpu, FLAG_LT))
	{
		c_cpu_jump(cpu, target);
	}
}

void AND(c_cpu_t* cpu, c_operand_t const* operand1, c_operand_t const* operand2)
{
	DEBUG_PRINTF("AND\n");
	c_word value1 = c_operand_read_value(cpu, operand1);
	c_word value2 = c_operand_read_value(cpu, operand2);

	c_operand_write_value(cpu, operand1, value1 & value2);
}
void OR(c_cpu_t* cpu, c_operand_t const* operand1, c_operand_t const* operand2)
{
	DEBUG_PRINTF("OR\n");
	c_word value1 = c_operand_read_value(cpu, operand1);
	c_word value2 = c_operand_read_value(cpu, operand2);

	c_operand_write_value(cpu, operand1, value1 | value2);
}
void XOR(c_cpu_t* cpu, c_operand_t const* operand1, c_operand_t const* operand2)
{
	DEBUG_PRINTF("XOR\n");
	c_word value1 = c_operand_read_value(cpu, operand1);
	c_word value2 = c_operand_read_value(cpu, operand2);

	c_operand_write_value(cpu, operand1, value1 ^ value2);
}
void NOT(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("NOT\n");
	c_operand_write_value(cpu, operand, ~c_operand_read_value(cpu, operand));
}

void PUSH(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("PUSH\n");
	c_word value = c_operand_read_value(cpu, operand);

	switch(c_operand_get_size(operand))
	{
	case sizeof(c_long):
		c_cpu_push_long(cpu, value);
		return;
	case sizeof(c_short):
		c_cpu_push_short(cpu, value);
		return;
	case sizeof(c_byte):
		c_cpu_push_byte(cpu, value);
		return;
	}
}
void POP(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("POP\n");
	switch(c_operand_get_size(operand))
	{
	case sizeof(c_long):
		c_operand_write_value(cpu, operand, c_cpu_pop_long(cpu));
		return;
	case sizeof(c_short):
		c_operand_write_value(cpu, operand, c_cpu_pop_short(cpu));
		return;
	case sizeof(c_byte):
		c_operand_write_value(cpu, operand, c_cpu_pop_byte(cpu));
		return;
	}
}

void CALL(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("CALL\n");
	c_word address = c_operand_read_value(cpu, operand);
	c_cpu_call(cpu, address);
}

void RET(c_cpu_t* cpu, c_operand_t const* unused1, c_operand_t const* unused2)
{
	DEBUG_PRINTF("RET\n");
	c_cpu_return(cpu);
}

void PUTC(c_cpu_t* cpu, c_operand_t const* operand, c_operand_t const* unused)
{
	DEBUG_PRINTF("PUTC\n");
	char character = (char)c_operand_read_value(cpu, operand);
	putchar(character);
}

#define SET(a, b) { c_instruction_vector[C_INSTR_##a] = a; c_instruction_info[C_INSTR_##a].name = #a; c_instruction_info[C_INSTR_##a].operands = b; }

void c_build_instruction_vector()
{
	if(instruction_vector_built)
		return;

	memset(c_instruction_vector, 0, sizeof(c_instruction_function) * C_INSTRUCTION_VECTOR_LENGTH);
	memset(c_instruction_info, 0, sizeof(struct c_instr_info) * C_INSTRUCTION_VECTOR_LENGTH);

	SET(NOP, 0);
	SET(HALT, 0);
	SET(INT, 1);
	SET(MOV, 2);

	SET(JMP, 1);

	SET(INC, 1);
	SET(DEC, 1);

	SET(ADD, 2);
	SET(SUB, 2);
	SET(MUL, 2);
	SET(DIV, 2);

	SET(SHOW, 1);

	SET(CMP, 2);
	SET(SCMP, 2);
	SET(TEST, 1);
	SET(STEST, 1);

	SET(JZ, 1);
	SET(JNZ, 1);
	SET(JEQ, 1);
	SET(JNEQ, 1);
	SET(JGT, 1);
	SET(JNGT, 1);
	SET(JLT, 1);
	SET(JNLT, 1);

	SET(AND, 2);
	SET(OR, 2);
	SET(XOR, 2);
	SET(NOT, 1);

	SET(PUSH, 1);
	SET(POP, 1);

	SET(CALL, 1);
	SET(RET, 0);

	SET(PUTC, 1);

	instruction_vector_built = true;
}

#undef SET

