#ifndef METHOD_H
#define METHOD_H

#include "Vector.h"

// Struct "Method" is a wrapper for the instructions and their corresponding operands of a given method.
// It also holds the desired size of a the stack frame and locals for best optimization.

struct Method
{
	char* Name;
	int StackSize;
	int LocalSize;

	int LenOperations;
	int* Operations; // List of operations may be found in KVM.h in enum "OP".
	void** Operands; // Operands my be any literal primitive. Char, Int, Float, Double, String.
};

#endif