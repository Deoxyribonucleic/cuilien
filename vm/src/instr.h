#ifndef _INSTR_H_
#define _INSTR_H_


#define INSTR_NOP	0x0000
#define INSTR_HALT	0x0001
#define INSTR_INT	0x0002
#define INSTR_MOV	0x0003

#define INSTR_JMP	0x0004

#define INSTR_ERR	0x0005 // not implemented

#define INSTR_INC	0x0006
#define INSTR_DEC	0x0007

#define INSTR_ADD	0x000A
#define INSTR_SUB	0x000B
#define INSTR_MUL	0x000C
#define INSTR_DIV	0x000D

#define INSTR_SHOW	0x0010

#define INSTR_CMP	0x0020
#define INSTR_SCMP	0x0021
#define INSTR_TEST	0x0022
#define INSTR_STEST	0x0023
#define INSTR_JZ	0x0024
#define INSTR_JNZ	0x0025
#define INSTR_JEQ	0x0026
#define INSTR_JNEQ	0x0027
#define INSTR_JGT	0x0028
#define INSTR_JNGT	0x0029
#define INSTR_JLT	0x002A
#define INSTR_JNLT	0x002B
#define INSTR_JER	0x002C
#define INSTR_JNER	0x002D

#define INSTR_AND	0x0040
#define INSTR_OR	0x0041
#define INSTR_XOR	0x0042
#define INSTR_NOT	0x0043

#define INSTRUCTION_VECTOR_LENGTH	100

typedef struct cpu cpu_t;
typedef struct operand operand_t;
typedef void (*instruction_function)(cpu_t*, operand_t const* , operand_t const*);

extern instruction_function instruction_vector[INSTRUCTION_VECTOR_LENGTH];
void build_instruction_vector();

#endif
