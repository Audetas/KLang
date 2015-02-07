#include "Method.h"
#include <stdlib.h>

struct Method Method_New(char* name, int localSize, int numOperations)
{
	struct Method m;
	m.Name = name;
	m.LocalSize = localSize;
	m.StackSize = 0;
	m.LenOperations = numOperations;
	m.Operations = malloc(sizeof(int) * numOperations);
	m.Operands = malloc(sizeof(void*) * numOperations);

	return m;
}

struct Method* Method_Alloc(char* name, int localSize, int numOperations)
{
	struct Method* method = malloc(sizeof(struct Method));
	*method = Method_New(name, localSize, numOperations);
	return method;
}