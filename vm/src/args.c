#include "args.h"

#include <string.h>

void args_parse(char** args, int argc, struct args_info* out)
{
	strcpy(out->program, "test.cx");
}
