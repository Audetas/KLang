#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

struct Vector Vector_New(int size) 
{
	struct Vector vector;

	vector.Size = size;
	vector.Pos = 0;
	vector.Data = calloc(size, sizeof(void*));

	return vector;
}

struct Vector* Vector_Alloc(int size)
{
	struct Vector* vector = malloc(sizeof(struct Vector));
	*vector = Vector_New(size);

	return vector;
}

void Vector_Destroy(struct Vector* vector) 
{
	free(vector->Data);
	free(vector);
}

void* Vector_Top(struct Vector* vector)
{
	return vector->Data[vector->Pos];
}

void* Vector_Get(struct Vector* vector, int index) 
{
	/*
	if (index >= vector->Pos || index < 0) 
		return NULL;
	*/

	if (index > vector->Size)
		return NULL;

	return vector->Data[index];
}

void* Vector_Set(struct Vector* vector, int index, void* elem) 
{
	void* old_elem = NULL;

	if (index < vector->Pos && index >= 0)
	{
		old_elem = vector->Data[index];
		vector->Data[index] = elem;
	}

	return old_elem;
}

void Vector_Push(struct Vector* vector, void* elem) 
{
	vector->Data[vector->Pos++] = elem;

	if (vector->Pos + 2 > vector->Size)
		Vector_Resize(vector);
}

void* Vector_Pop(struct Vector* vector) 
{
	if (vector->Pos == 0)
		return NULL;

	void* elem = vector->Data[--vector->Pos];
	//Vector_Resize(vector);
	return elem;
}

void Vector_Remove(struct Vector* vector, int amount)
{
	if (vector->Pos - amount < 0)
		vector->Pos = 0;
	else
		vector->Pos -= amount;
}

void Vector_Resize(struct Vector* vector) 
{
	void* new_storage;
	int new_length;

	if (vector->Pos == vector->Size) 
	{
		new_length = vector->Size * 2;
		new_storage = realloc(vector->Data, sizeof(void*) * vector->Size * 2);
	}
	else if ((float)vector->Pos / (float)vector->Size < 0.25)
	{
		new_length = vector->Size / 2;
		new_storage = realloc(vector->Data, sizeof(void*) * vector->Size / 2);
	}
	else
		return;

	if (new_storage == NULL) 
	{
		Vector_Destroy(vector);
		printf("Out of memory. Vector could not be expanded.");
		//TODO: Find a better way to exit.
		exit(-1);
	}

	vector->Data = new_storage;
	vector->Size = new_length;
}