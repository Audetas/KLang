#include <stdlib.h>
#include "KVM.h"
#include "Object.h"
#include "Method.h"

struct KVM KVM_New()
{
	struct KVM kvm;
	kvm.Types = Vector_New(1);
	kvm.ExternLibCache = Vector_New(1);

	return kvm;
}

void KVM_Destroy(struct KVM* kvm)
{
	Vector_Destroy(&kvm->Types);
	Vector_Destroy(&kvm->ExternLibCache);
	
	free(kvm);
}



int KVM_Execute(struct KVM* kvm, struct Object* this, struct Method* method)
{
	struct Object* locals = malloc(sizeof(struct Object) * method->LocalSize);
	struct Object* stack = malloc(sizeof(struct Object) * method->StackSize);

	int sptr = -1;

	for (int i = 0; i < method->LenOperations; i++)
	{
		switch (method->Operations[i])
		{
		}
	}
}



struct Type* KVM_DefineType(struct KVM* kvm, char* typeName)
{
	struct Type* type = malloc(sizeof(struct Type));
	type->Name = typeName;
	type->Members = Vector_New(1);
	type->Methods = Vector_New(1);

	Vector_Push(&kvm->Types, type);
	return type;
}

char* KVM_DefineMember(struct KVM* kvm, char* typeName, char* memberName)
{
	Vector_Push(&KVM_GetType(kvm, typeName)->Members, memberName);
	return memberName;
}

struct Method* KVM_DefineMethod(struct KVM* kvm, char* typeName, struct Method* method)
{
	Vector_Push(&KVM_GetType(kvm, typeName)->Methods, method);
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