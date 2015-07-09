#ifndef _INSTR_H_
#define _INSTR_H_


#define C_INSTR_NOP   0x0000
#define C_INSTR_HALT  0x0001
#define C_INSTR_INT   0x0002
#define C_INSTR_MOV   0x0003

#define C_INSTR_JMP   0x0004

#define C_INSTR_ERR   0x0005 // not implemented

#define C_INSTR_INC   0x0006
#define C_INSTR_DEC   0x0007

#define C_INSTR_ADD   0x000A
#define C_INSTR_SUB   0x000B
#define C_INSTR_MUL   0x000C
#define C_INSTR_DIV   0x000D

#define C_INSTR_SHOW  0x0010

#define C_INSTR_CMP   0x0020
#define C_INSTR_SCMP  0x0021
#define C_INSTR_TEST  0x0022
#define C_INSTR_STEST 0x0023
#define C_INSTR_JZ    0x0024
#define C_INSTR_JNZ   0x0025
#define C_INSTR_JEQ   0x0026
#define C_INSTR_JNEQ  0x0027
#define C_INSTR_JGT   0x0028
#define C_INSTR_JNGT  0x0029
#define C_INSTR_JLT   0x002A
#define C_INSTR_JNLT  0x002B
#define C_INSTR_JER   0x002C // not implemented
#define C_INSTR_JNER  0x002D // not implemented

#define C_INSTR_AND   0x0040
#define C_INSTR_OR    0x0041
#define C_INSTR_XOR   0x0042
#define C_INSTR_NOT   0x0043

#define C_INSTR_PUSH  0x0050
#define C_INSTR_POP   0x0051

#define C_INSTR_CALL  0x0060
#define C_INSTR_RET   0x0061

#define C_INSTR_PUTC  0x0070
#define C_INSTR_GETC  0x0071 // not implemented

#define C_INSTRUCTION_VECTOR_LENGTH	0x80

typedef struct c_cpu c_cpu_t;
typedef struct c_operand c_operand_t;
typedef void (*c_instruction_function)(c_cpu_t*, c_operand_t const* , c_operand_t const*);

struct c_instr_info
{
	char const* name;
	int operands;
};

extern c_instruction_function c_instruction_vector[C_INSTRUCTION_VECTOR_LENGTH];
extern struct c_instr_info c_instruction_info[C_INSTRUCTION_VECTOR_LENGTH];
void c_build_instruction_vector();

#endif
