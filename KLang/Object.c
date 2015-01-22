#include <stdlib.h>
#include "Object.h"

struct Object Object_New(struct Type* type)
{
	struct Object object;
	object.Type = type; // The object instance will carry a reference to its definition
	object.Members = malloc(sizeof(struct Object) * type->Members.Size); // Sized to the length of known members for the type.

	return object;
}

void Object_Destroy(struct Object* object)
{
	if (object->BoxedValue != NULL) // Not all object instances box a value
		free(object->BoxedValue);

	free(object->Members); // The instance members are the only other thing that need to be freed.
	free(object);
}