#ifndef CONTEXT_H
#define CONTEXT_H

#include "Vector.h"
#include "KVM.h"

// Context represents a thread and holds data required for execution like the stack.
struct Context
{
	struct KVM* KVM; // Parent virtual machine.
	struct Vector Stack; // Stack for the thread.
	int sptr; // Stack pointer.
};

struct Context Context_New(struct KVM* kvm); // Create an instance of a Context.
struct Context* Context_Alloc(struct KVM* kvm); // Create an instance of a Context on the heap.
void Context_Destroy(struct Context* context); // Properly free an instance of a Context.

#endif