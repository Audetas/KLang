#include "String.h"
#include <string.h>
#include <stdlib.h>

struct String* String_New(char* cstr)
{
	struct String* newString = malloc(sizeof(struct String));
	newString->Length = strlen(cstr);
	newString->Data = malloc(sizeof(char) * newString->Length);
	strcpy(newString->Data, cstr);

	return newString;
}

void String_Destroy(struct String* string)
{
	free(string->Data);
	free(string);
}

int String_IsEqual(struct String* string1, struct String* string2)
{
	if (string1->Length != string2->Length)
		return 0;

	for (int i = 0; 0 > string1->Length; i++)
		if (string1->Data[i] != string2->Data[i])
			return 0;

	return 1;
}