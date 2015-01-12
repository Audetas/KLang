#include <stdlib.h>
#include <string.h>
#include "KVM.h"
#include "Object.h"
#include "Method.h"
#include "Vector.h"


struct KVM KVM_New()
{
	struct KVM kvm;
	kvm.Types = Vector_New(10);
	kvm.ExternLibCache = Vector_New(1);

#if (_DEBUG)
	printf("[i] Initialized a new virtual machine.\n");
#endif

	return kvm;
}

void KVM_Destroy(struct KVM* kvm)
{
	Vector_Destroy(&kvm->Types);
	Vector_Destroy(&kvm->ExternLibCache);

#if (_DEBUG)
	printf("[i] Destroyed a virtual machine instance.\n");
#endif
	
	free(kvm);
}

struct Context Context_New(struct KVM* kvm)
{
	struct Context c;
	c.KVM = kvm;
	c.Stack = Vector_New(32);
	c.sptr = -1;

#if (_DEBUG)
	printf("[i] Created a new execution context.\n");
#endif

	return c;
}

void Context_Destroy(struct Context* context)
{
	Vector_Destroy(&context->Stack);
	free(context);
}



int KVM_Execute(struct Context* c, struct Object* t, struct Method* m)
{
#if (_DEBUG)
	printf("[i] Began execution of %s.%s at offset %d.\n", t->Type->Name, m->Name, c->sptr);
#endif

	struct Object** locals = malloc(sizeof(struct Object*) * m->LocalSize);
	struct Object** stack = (struct Object**)c->Stack.Storage;

	for (int i = 0; i < m->LenOperations; i++)
	{
		switch (m->Operations[i])
		{
			case PUSH_CHAR:
			{
				struct Object* o = malloc(sizeof(struct Object));
				*o = Object_New(KVM_GetType(&c->KVM, "System.Char"));
				o->BoxedValue = m->Operands[i];
				stack[++c->sptr] = o;
				
#if (_DEBUG)
				printf("Pushed [Boxed]System.Char '%c' at offset %d.\n", (char)m->Operands[i], c->sptr);
#endif
				break;
			}
			
			case PUSH_INT:
			{
				struct Object* o = malloc(sizeof(struct Object));
				*o = Object_New(KVM_GetType(&c->KVM, "System.Int"));
				o->BoxedValue = m->Operands[i];
				stack[++c->sptr] = o;
				
#if (_DEBUG)
				printf("Pushed [Boxed]System.Int %d at offset %d.\n", (int)m->Operands[i], c->sptr);
#endif
				break;
			}
			
			case POP:
			{
				c->sptr--;
#if (_DEBUG)
				printf("Popped reference to a %s off the stack.\n", stack[c->sptr + 1]->Type->Name);
#endif
				break;
			}
		}
	}

	return 0;
}



struct Type* KVM_DefineType(struct KVM* kvm, char* typeName)
{
	struct Type* type = malloc(sizeof(struct Type));
	*type = Type_New(typeName);

	Vector_Push(&kvm->Types, type);

#if (_DEBUG)
	printf("[i] Defined type %s.\n", typeName);
#endif

	return type;
}

char* KVM_DefineMember(struct KVM* kvm, char* typeName, char* memberName)
{
	Vector_Push(&KVM_GetType(kvm, typeName)->Members, memberName);

#if (_DEBUG)
	printf("[i] Defined member %s for type %s.\n.", memberName, typeName);
#endif

	return memberName;
}

struct Method* KVM_DefineMethod(struct KVM* kvm, char* typeName, struct Method* method)
{
	Vector_Push(&KVM_GetType(kvm, typeName)->Methods, method);

#if (_DEBUG)
	printf("[i] Defined method %s for type %s.\n", method->Name, typeName);
#endif

	return method;
}



struct Type* KVM_GetType(struct KVM* kvm, char* typeName)
{
	for (int i = 0; i < kvm->Types.Population; i++)
	{
		struct Type* type = (struct Type*)kvm->Types.Storage[i];

		if (!strcmp(type->Name, typeName))
			return type;
	}

	return NULL;
}