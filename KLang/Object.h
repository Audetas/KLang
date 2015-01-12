#ifndef OBJECT_H
#define OBJECT_H

#include "Type.h"

// Struct "Object" is the applied form of a Type. 
// This struct is what is used in the virtual machine to store data.
// On a 32bit system, the overhead of any given object will be 16 bytes.
// On a 64bit system, the overhead of any given object will be 28 bytes.
struct Object
{
	struct Type* Type; // The type that this object represents.
	void* BoxedValue; // Pointer to the value this instance of "Object" is holding (if applicable).

	struct Object* Members; // Storage for the values of instance members.

	unsigned int References; // Count of references towards this instance. Used for garbage collection.
};

struct Object Object_New(struct Type* type); // Instantiates an "Object" instance from a type definition.
void Object_Destroy(struct Object* object); // Properly frees an instance of "Object"

#endif