#include "FFI.h"
#include "KVM.h"
#include "Object.h"
#include <string.h>
#include <stdlib.h>

int MapExternFunc(struct KVM* kvm, char* libName, char* functionName)
{
	struct Type* delegateType;
	struct Method* delegateMethod;

	// Define the delegate type for the library if it doesn't already exist.
	if (delegateType = KVM_GetType(kvm, libName) == NULL)
		delegateType = KVM_DefineType(kvm, libName);

	// Ensure the method delegate doesn't already exist.
	if (Type_FindMethod(delegateType, functionName) != NULL)
		return 0; // Return because the method is already defined
	
	// Create the new method definition with only one operation
	delegateMethod = malloc(sizeof(struct Method));
	delegateMethod->LenOperations = 1;
	delegateMethod->LocalSize = 1;
	delegateMethod->StackSize = 1;
	delegateMethod->Name = functionName;
	
	delegateMethod->Operations = malloc(sizeof(int) * 1);
	delegateMethod->Operands = malloc(sizeof(void*) * 1);
	delegateMethod->Operands[0] = malloc(sizeof(char) * strlen(functionName));

	delegateMethod->Operations[0] = CALL_EXTERN; // Op for an external library call
	delegateMethod->Operands[0] = functionName;

	KVM_DefineMethod(kvm, libName, delegateMethod);

	return 1; // Return Success
}

struct Object MarshalCType(struct KVM* kvm, void* data, char* type) // TODO: Finish
{
	struct Object object;
	
	// We are manually checking characters for more speed.
	if (type[0] == 'c') // char
		object = Object_New(KVM_GetType(kvm, "System.Char"));
	else if (type[0] == 'i') // int
		object = Object_New(KVM_GetType(kvm, "System.Int"));

	object.BoxedValue = data;
	return object;
}