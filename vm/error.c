#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int error_last = ERR_NONE;

char* error_tostring(int code)
{
	switch(code)
	{
	case ERR_NONE:
		return "No error";
	case ERR_VM_FNF:
		return "VM: file not found";
	case ERR_MEM_HOST_OOM:
		return "Memory: host machine out of memory";
	case ERR_MEM_LIMIT_REACHED:
		return "Memory: the memory limit has been reached";
	case ERR_MEM_PF_FAILED:
		return "Memory: pagefault failed due to an unknown reason";
	case ERR_MEM_PF_INVALID:
		return "Memory: segmentation fault: invalid address";
	case ERR_MEM_NOREAD:
		return "Memory: segmentation fault: access violation (read)";
	case ERR_MEM_NOWRITE:
		return "Memory: segmentation fault: access violation (write)";
	case ERR_MEM_NOEXEC:
		return "Memory: segmentation fault: access violation (execute)";
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

inline void fatal(const char* message)
{
	fprintf(stderr, "fatal error: %s\n", message);
	abort();
}
