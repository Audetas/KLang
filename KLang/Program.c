#include <stdlib.h>
#include <stdio.h>
#include "KVM.h"
#include "Method.h"
#include "Type.h"
#include "Vector.h"
#include "Object.h"
#include "Allocator.h"


int main()
{
	struct KVM* kvm = KVM_Alloc();
	struct Context* context = Context_Alloc(kvm);

	struct Method* methodMain = Method_Alloc("Main", 1, 2);
	methodMain->Operations[0] = CALL_LOCAL;
	methodMain->Operations[1] = CALL_LOCAL;
	methodMain->Operands[0] = Alloc_int(1);
	methodMain->Operands[1] = Alloc_int(2);

	struct Method* methodTest = Method_Alloc("NotTest", 1, 3); // offset 1
	methodTest->Operations[0] = PUSH_INT;
	methodTest->Operations[1] = NOT;
	methodTest->Operations[2] = DEBUG_PRINT_INT;
	methodTest->Operands[0] = Alloc_int(0);

	struct Method* methodTest2 = Method_Alloc("CmpTest", 1, 4); // offset 2
	methodTest2->Operations[0] = PUSH_CHAR;
	methodTest2->Operations[1] = PUSH_CHAR;
	methodTest2->Operations[2] = CMP_BOX;
	methodTest2->Operations[3] = DEBUG_PRINT_INT;
	methodTest2->Operands[0] = Alloc_char('a');
	methodTest2->Operands[1] = Alloc_char('b');

	KVM_DefineType(kvm, "int");
	KVM_DefineType(kvm, "char");
	struct Type* typeDebug = KVM_DefineType(kvm, "Debug");
    KVM_DefineMethod(kvm, "Debug", methodMain);
	KVM_DefineMethod(kvm, "Debug", methodTest);
	KVM_DefineMethod(kvm, "Debug", methodTest2);

	struct Object* callInstance = Object_Alloc(typeDebug);

	int code = KVM_Execute(context, callInstance, methodMain);

	printf("[i] Finished execution with exit code %d.\n", code);
	
	getchar();
	return 0;
}