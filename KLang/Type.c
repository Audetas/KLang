#include "Type.h"
#include "Vector.h"
#include "Method.h"
#include <string.h>
#include <stdlib.h>

struct Type Type_New(char* typeName)
{
	struct Type type;
	type.Members = Vector_New(2);
	type.Methods = Vector_New(2);
	type.Name = typeName;

	return type;
}

void Type_Destroy(struct Type* type)
{
	Vector_Destroy(&type->Members);
	Vector_Destroy(&type->Methods);
	free(type->Name);
	free(type);
}

struct Method* Type_FindMethod(struct Type* type, char* methodName)
{
	for (int i = 0; i < type->Methods.Pos; i++)
	{
		struct Method* method = (struct Method*)type->Methods.Data[i];

		if (!strcmp(method->Name, methodName))
			return method;
	}

	return NULL;
}

int Type_FindMember(struct Type* type, char* memberName)
{
	for (int i = 0; i < type->Members.Pos; i++)
	{
		char* member = (char*)type->Members.Data[i];

		if (!strcmp(member, memberName))
			return i;
	}

	return -1;
}