#ifndef _ARGS_H_
#define _ARGS_H_

struct args_info
{
	char program[128];
};

void args_parse(char** args, int argc, struct args_info* out);

#endif
