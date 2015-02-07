#ifndef STRING_H
#define STRING_H

struct String
{
	char* Data;
	int Length;
};

struct String* String_New(char* cstr);
void String_Destroy(struct String* string);

int String_IsEqual(struct String* string1, struct String* string2); // 1 for equal, 0 for different

#endif