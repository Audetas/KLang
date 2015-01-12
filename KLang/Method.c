#include "Method.h"

struct Method Method_New(char* name, int localSize, int numOperations)
{
	struct Method m;
	m.Name = name;
	m.LocalSize = localSize;
	m.StackSize = 0;
	m.Operations = malloc(sizeof(int) * numOperations);
	m.Operands = malloc(sizeof(void*) * numOperations);

	return m;
}