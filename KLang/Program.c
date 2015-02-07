#include <stdlib.h>
#include <stdio.h>
#include "KVM.h"
#include "Method.h"
#include "Type.h"
#include "Vector.h"
#include "Object.h"


int main()
{
	struct KVM* kvm = KVM_Alloc();
	struct Context* context = Context_Alloc(kvm);

	struct Method* methodMain = Method_Alloc("Main", 1, 1);
	methodMain->Operations[0] = CALL_LOCAL;
	methodMain->Operands[0] = malloc(sizeof(int));
	methodMain->Operands[0] = 1;

	struct Method* methodTest = Method_Alloc("Test", 1, 1);
	methodTest->Operations[0] = PUSH_INT;
	methodTest->Operands[0] = malloc(sizeof(int));
	methodTest->Operands[0] = 69;

	KVM_DefineType(kvm, "int");
	struct Type* typeDebug = KVM_DefineType(kvm, "Debug");
    KVM_DefineMethod(kvm, "Debug", methodMain);
	KVM_DefineMethod(kvm, "Debug", methodTest);

	struct Object* callInstance = Object_Alloc(typeDebug);

	int code = KVM_Execute(context, callInstance, methodMain);

	printf("[i] Finished execution with exit code %d.\n", code);
	
	getchar();
	return 0;
}