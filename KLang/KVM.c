#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "KVM.h"
#include "Object.h"
#include "Method.h"
#include "Vector.h"
#include "Context.h"
#include "Allocator.h"


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

int KVM_Execute(struct Context* c, struct Object* t, struct Method* m)
{
#if (_DEBUG)
	printf("[i] Began execution of %s.%s at offset %d.\n", t->Type->Name, m->Name, c->sptr);
#endif

	struct Object** locals = malloc(sizeof(struct Object*) * m->LocalSize);
	struct Vector* stack = &c->Stack;

	for (int i = 0; i < m->LenOperations; i++)
	{
		switch (m->Operations[i])
		{
			case PUSH_CHAR:
			{
				struct Type* type = KVM_GetType(c->KVM, "char");
				struct Object* object = Object_Box_Alloc(
					type,
					m->Operands[i],
					sizeof(char));
				Vector_Push(stack, object);
				
#if (_DEBUG)
				printf("[i] Pushed [Boxed]char '%c' at offset %d.\n", *(char*)m->Operands[i], stack->Pos);
#endif
				break;
			}
			
			case PUSH_INT:
			{
				struct Type* type = KVM_GetType(c->KVM, "int");
				struct Object* object = Object_Box_Alloc(
					type, 
					m->Operands[i],
					sizeof(int));
				
				Vector_Push(stack, object);
				
#if (_DEBUG)
				printf("[i] Pushed [Boxed]int %d at offset %d.\n", *(int*)m->Operands[i], stack->Pos);
#endif
				break;
			}

			case JMP_TO:
			{
				i = (int)m->Operands[i];

				break;
			}

			case JMP_REL:
			{
				i += (int)m->Operands[i];

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
				int methodIndex = *(int*)m->Operands[i];
				struct Method* method = Vector_Get(&t->Type->Methods, methodIndex);

				KVM_Execute(c, t, method);

				stack->Pos = returnPointer + 1;
				break;
			}
			
			case CALL_INSTANCE:
			{
				int returnPointer = stack->Pos;
				int methodIndex = (int)m->Operands[i];
				struct Object* callingObject = Vector_Pop(stack); 
				struct Method* method = Vector_Get(&callingObject->Type->Methods, methodIndex);

				KVM_Execute(c, callingObject, method);
				
				stack->Pos = returnPointer + 1;
				break;
			}

			case CMP_REF:
			{
				struct Object* object1 = Vector_Pop(stack);
				struct Object* object2 = Vector_Pop(stack);

				int* eval = Alloc_int((int)(&object1 == &object2));
				struct Object* result = Object_Box_Alloc(
					KVM_GetType(c->KVM, "int"),
					(void*)eval, sizeof(int));

				break;
			}

			case CMP_BOX:
			{
				struct Object* object1 = Vector_Pop(stack);
				struct Object* object2 = Vector_Pop(stack);

				int* eval;

				if (object1->BoxedValueSize != object2->BoxedValueSize)
					eval = Alloc_int(0);
				else
					eval = Alloc_int(
						!memcmp(
							object1->BoxedValue,
							object2->BoxedValue,
							object1->BoxedValueSize));

				struct Object* result = Object_Box_Alloc(
					KVM_GetType(c->KVM, "int"),
					eval, sizeof(int));

				Vector_Push(stack, result);
				break;
			}

			case NOT:
			{
				struct Object* object = Vector_Top(stack);
				int value = *(int*)object->BoxedValue;
				*(int*)object->BoxedValue = !value;

				break;
			}

			case DEBUG_PRINT_INT:
			{
				struct Object* top = Vector_Top(stack);
				printf("[d] Stack top <int>: %d.\n", *(int*)top->BoxedValue);

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