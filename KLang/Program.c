#include <stdlib.h>
#include <stdio.h>
#include "KVM.h"
#include "Method.h"
#include "Type.h"
#include "Vector.h"
#include "Object.h"


int main()
{
	struct KVM kvm = KVM_New();
	struct Context context = Context_New(&kvm);

	struct Method methodMain = Method_New("Main", 1, 1);
	methodMain.Operations[0] = CALL_STATIC;
	methodMain.Operands[0] = malloc(sizeof(char) * 5);
	methodMain.Operands[0] = "Test";


	KVM_DefineType(&kvm, "Debug.Test");
	KVM_DefineMethod(&kvm, methodMain.Name, &methodMain);
	
	getch();
	return 0;
}