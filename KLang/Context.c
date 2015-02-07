#include <stdlib.h>
#include <stdio.h>
#include "Context.h"
#include "Vector.h"
#include "KVM.h"

struct Context Context_New(struct KVM* kvm)
{
	struct Context c;
	c.KVM = kvm;
	c.Stack = Vector_New(32);
	c.sptr = 0;

#if (_DEBUG)
	printf("[i] Created a new execution context.\n");
#endif

	return c;
}

struct Context* Context_Alloc(struct KVM* kvm)
{
	struct Context* context = malloc(sizeof(struct Context));
	*context = Context_New(kvm);

	return context;
}

void Context_Destroy(struct Context* context)
{
	Vector_Destroy(&context->Stack);
	free(context);
}