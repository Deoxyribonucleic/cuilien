#include "error.h"

#include <stdio.h>

int error_last = ERR_NONE;

char* error_tostring(int code)
{
	switch(code)
	{
	case ERR_NONE:
		return "No error";
	case ERR_VM_FNF:
		return "VM: File not found";
	case ERR_MEM_HOST_OOM:
		return "Memory: host machine out of memory";
	case ERR_CPU_HOST_OOM:
		return "CPU: host machine out of memory";
	case ERR_CPU_INVALID_MEMORY:
		return "CPU: invalid memory pointer";
	default:
		return "Unknown error";
	}
}

void error_print(int code)
{
	printf("Error (%d): %s\n", code, error_tostring(code));
}

inline void error_clear()
{
	error_last = ERR_NONE;
}
