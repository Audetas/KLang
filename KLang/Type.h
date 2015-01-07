#ifndef TYPE_H
#define TYPE_H

#include "Vector.h"

// Struct "Type" is used to contain the definition of a type.
// It also will hold meta data for reflection.
// Each type known to the virtual machine is backed by ONE instance of this struct.

struct Type 
{
	char* Name; //Example: System.Int

	struct Vector Methods; // Holds instances of struct "Method" representing all known methods for the type.
	struct Vector Members; // Holds the type's member names. Used strictly for reflection.
};

#endif