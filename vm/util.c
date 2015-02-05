#include "util.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

size_t min_size(size_t a, size_t b)
{
	return MIN(a, b);
}

size_t max_size(size_t a, size_t b)
{
	return MAX(a, b);
}
