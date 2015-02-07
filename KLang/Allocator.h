#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "KVM.h"
#include "Method.h"
#include "Object.h"

struct KVM* Alloc_KVM();
struct Object* Alloc_Object(struct Type* type);

#endif