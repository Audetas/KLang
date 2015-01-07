#ifndef KVM_H
#define KVM_H

#include "Type.h"
#include "Method.h"
#include "Vector.h"

enum OP // Enumeration of all possible operations that the interpreter can compute.
{
	PUSH,
	POP
};

// Struct "KVM" is a manifestation of the main components, that together, contain and execute the program.

struct KVM
{
	struct Vector Types; // Contains the corresponding struct "Type" instances for all known types.
	struct Vector ExternLibCache; // Contains the modules for all loaded external libraries.
};

struct KVM KVM_New(); // Create an instance of a KVM
void KVM_Destroy(struct KVM* kvm); // Properly free an instance of a KVM.

int KVM_Execute(struct KVM* kvm, struct Object* this, struct Method* method); // Executes the given method under the given context (object) as the caller.

struct Type* KVM_DefineType(struct KVM* kvm, char* typeName); // Adds an empty definition of type "typeName" to the database.
char* KVM_DefineMember(struct KVM* kvm, char* typeName, char* memberName); // Adds the name of a member to the known members list of type "typeName".
struct Method* KVM_DefineMethod(struct KVM* kvm, char* typeName, char* methodName); // Adds the method definition to the list of known members for type "typeName".

struct Type* KVM_GetType(struct KVM* kvm, char* typeName); // Fetch a reference to type "typeName"'s definition.

#endif