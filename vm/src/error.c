#include "error.h"

char* error_tostring(int code)
{
	switch(code)
	{
	case ERR_NONE:
		return "No error";
	case ERR_VM_FNF:
		return "VM: File not found";
	default:
		return "Unknown error";
	}
}
