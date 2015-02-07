#ifndef OBJECT_H
#define OBJECT_H

#include "Type.h"

// Struct "Object" is the applied form of a Type. 
// This struct is what is used in the virtual machine to store data.
struct Object
{
	struct Type* Type; // The type that this object represents.
	void* BoxedValue; // Pointer to the value this instance of Object is holding (if applicable).
	int BoxedValueSize; // Size of the boxed pointee.

	struct Object* Members; // Storage for the values of instance members.

	unsigned int References; // Count of references towards this instance. Used for garbage collection.
};

struct Object Object_New(struct Type* type); // Instantiates an Object instance from a type definition.
struct Object* Object_Alloc(struct Type* type); // Instiates an Object instance on the heap from a type definition.
struct Object* Object_Box_Alloc(struct Type* type, void* value, int lenValue); // Instiates an Object box instance on the heap from a type definition.
void Object_Destroy(struct Object* object); // Properly frees an instance of Object

#endif