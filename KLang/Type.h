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

struct Type Type_New(char* typeName); // Create a new properly initialized instance of "Type".
void Type_Destroy(struct Type* type); // Properly free an instance of "Type".

struct Method* Type_FindMethod(struct Type* type, char* methodName); // Get the pointer to a method definition defined in the type by its name.
int Type_FindMember(struct Type* type, char* memberName); // Get the member index of a member defined in a type by its member name.

#endif