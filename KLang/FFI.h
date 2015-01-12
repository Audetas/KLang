#ifndef FFI_H
#define FFI_H

#include "Method.h"
#include "KVM.h"

// Creates a delegate type with the name of the library if it doesn't exist,
// then creates a delegate KLang method to wrap the function call
// and registers them in the KVM "kvm".
int MapExternFunc(struct KVM* kvm, char* libName, char* functionName);

struct Object MarshalCType(struct KVM* kvm, void* object, char* type);

#endif