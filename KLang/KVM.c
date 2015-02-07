#include <stdlib.h>
#include <stdio.h>
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

	KVM_DefineType(&kvm, "static"); // static object is used as a container for static methods and members.

#if (_DEBUG)
	printf("[i] Initialized a new virtual machine.\n");
#endif

	return kvm;
}

struct KVM* KVM_Alloc()
{
	struct KVM* kvm = malloc(sizeof(struct KVM));
	*kvm = KVM_New();

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

int KVM_Execute(struct Context* c, struct Object* t, struct Method* m)
{
#if (_DEBUG)
	printf("[i] Began execution of %s.%s at offset %d.\n", t->Type->Name, m->Name, c->sptr);
#endif

	struct Object** locals = malloc(sizeof(struct Object*) * m->LocalSize);
	//struct Object** stack = (struct Object**)c->Stack.Storage;
	struct Vector* stack = &c->Stack;

	for (int i = 0; i < m->LenOperations; i++)
	{
		switch (m->Operations[i])
		{
			case PUSH_CHAR:
			{
				struct Object* o = malloc(sizeof(struct Object));
				*o = Object_New(KVM_GetType(c->KVM, "char"));
				o->BoxedValue = m->Operands[i];
				Vector_Push(stack, o);
				
#if (_DEBUG)
				printf("[i] Pushed [Boxed]char '%c' at offset %d.\n", (char)m->Operands[i], c->sptr);
#endif
				break;
			}
			
			case PUSH_INT:
			{
				struct Object* o = malloc(sizeof(struct Object));
				struct Type* type = KVM_GetType(c->KVM, "int");
				*o = Object_New(type);
				o->BoxedValue = m->Operands[i];
				Vector_Push(stack, o);
				
#if (_DEBUG)
				printf("[i] Pushed [Boxed]int %d at offset %d.\n", (int)m->Operands[i], stack->Pos);
#endif
				break;
			}
			
			case POP:
			{
				struct Object* poppedObject = Vector_Pop(stack);
#if (_DEBUG)
				printf("[i] Popped reference to a %s off the stack.\n", poppedObject->Type->Name);
#endif
				break;
			}

			case CALL_LOCAL:
			{
				int returnPointer = stack->Pos;
				int methodIndex = (int)m->Operands[i];
				struct Method* method = Vector_Get(&t->Type->Methods, methodIndex);

				KVM_Execute(c, t, method);

				stack->Pos = returnPointer + 1;
				break;
			}
			
			case CALL_INSTANCE:
			{
				int returnPointer = stack->Pos;
				int methodIndex = *(int*)m->Operands[i];
				struct Object* callingObject = Vector_Pop(stack);
				struct Method* method = Vector_Get(&callingObject->Type->Methods, methodIndex);

				KVM_Execute(c, callingObject, method);
				
				stack->Pos = returnPointer + 1;
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
	struct Type* type = KVM_GetType(kvm, typeName);

	if (type == NULL)
	{
		printf("[ERR] Unable to define member %s for type %s, it does not exist.\n", memberName, typeName);
		return memberName;
	}
	Vector_Push(&type->Members, memberName);

#if (_DEBUG)
	printf("[i] Defined member %s for type %s.\n.", memberName, typeName);
#endif

	return memberName;
}

struct Method* KVM_DefineMethod(struct KVM* kvm, char* typeName, struct Method* method)
{

	struct Type* type = KVM_GetType(kvm, typeName);

	if (type == NULL)
	{
		printf("[ERR] Unable to define method %s for type %s, it does not exist.\n", method->Name, typeName);
		return method;
	}

	Vector_Push(&type->Methods, method);

#if (_DEBUG)
	printf("[i] Defined method %s for type %s.\n", method->Name, typeName);
#endif

	return method;
}

struct Type* KVM_GetType(struct KVM* kvm, char* typeName)
{
	for (int i = 0; i < kvm->Types.Pos; i++)
	{
		struct Type* type = (struct Type*)kvm->Types.Data[i];

		if (!strcmp(type->Name, typeName))
			return type;
	}

	return NULL;
}