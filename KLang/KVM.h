#ifndef KVM_H
#define KVM_H

#include "Type.h"
#include "Method.h"
#include "Vector.h"

enum OP // Enumeration of all possible operations that the interpreter can compute.
{
	PUSH_CHAR,
	PUSH_INT,
	PUSH_FLOAT,
	PUSH_DOUBLE,
	PUSH_STRING,
	PUSH_T,

	POP,
	CALL_INSTANCE,
	CALL_STATIC,
	CALL_VIRTUAL,
	CALL_EXTERN
};

// KVM is a manifestation of the main components, that together, contain and execute the program.
struct KVM
{
	struct Vector Types; // Contains the corresponding struct "Type" instances for all known types.
	struct Vector ExternLibCache; // Contains the modules for all loaded external libraries.

	struct Vector Heap; // To be used later, as memory pool instead of calling malloc.
	struct Vector HeapAllocations; // To be used later, to reference occupied slots on the heap.
};

// Context represents a thread and holds data required for execution like the stack.
struct Context
{
	struct KVM* KVM; // Parent virtual machine.
	struct Vector Stack; // Stack for the thread.
	int sptr; // Stack pointer.
};

struct KVM KVM_New(); // Create an instance of a KVM
void KVM_Destroy(struct KVM* kvm); // Properly free an instance of a KVM.

struct Context Context_New(struct KVM* kvm); // Create an instance of a Context.
void Context_Destroy(struct Context* context); // Properly free an instance of a Context.

int KVM_Execute(struct Context* c, struct Object* t, struct Method* m); // Executes the given method under the given context (object) as the caller.

struct Type* KVM_DefineType(struct KVM* kvm, char* typeName); // Adds an empty definition of type "typeName" to the database.
char* KVM_DefineMember(struct KVM* kvm, char* typeName, char* memberName); // Adds the name of a member to the known members list of type "typeName".
struct Method* KVM_DefineMethod(struct KVM* kvm, char* typeName, struct Method* method); // Adds the method definition to the list of known members for type "typeName".

struct Type* KVM_GetType(struct KVM* kvm, char* typeName); // Fetch a reference to type "typeName"'s definition.

#endif