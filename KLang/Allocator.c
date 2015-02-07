#include <stdlib.h>
#include <string.h>
#include "Allocator.h"

char* Alloc_char(char value)
{
	char* c = malloc(sizeof(char));
	*c = value;

	return c;
}

int* Alloc_int(int value)
{
	int* i = malloc(sizeof(int));
	*i = value;

	return i;
}

float* Alloc_float(float value)
{
	float* f = malloc(sizeof(float));
	*f = value;

	return f;
}

double* Alloc_double(double value)
{
	double* d = malloc(sizeof(double));
	*d = value;

	return d;
}

char* Alloc_cstring(char* value)
{
	char* s = malloc(sizeof(char) * strlen(value));
	strcpy(s, value);

	return s;
}
