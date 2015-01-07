#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

struct Vector Vector_New(int size) 
{
	struct Vector vector;

	vector.Length = size;
	vector.Population = 0;
	vector.Storage = calloc(size, sizeof(void*));

	return vector;
}

void Vector_Destroy(struct Vector* vector) 
{
	free(vector->Storage);
	free(vector);
}

void* Vector_Get(struct Vector* vector, int index) 
{
	if (index >= vector->Population || index < 0) 
		return NULL;

	return vector->Storage[index];
}

void* Vector_Set(struct Vector* vector, int index, void* elem) 
{
	void* old_elem = NULL;

	if (index < vector->Population && index >= 0)
	{
		old_elem = vector->Storage[index];
		vector->Storage[index] = elem;
	}

	return old_elem;
}

void Vector_Push(struct Vector* vector, void* elem) 
{
	vector->Storage[vector->Population++] = elem;
	Vector_Resize(vector);
}

void* Vector_Pop(struct Vector* vector) 
{
	if (vector->Population == 0)
		return NULL;

	void* elem = vector->Storage[--vector->Population];
	Vector_Resize(vector);
	return elem;
}

void Vector_Resize(struct Vector* vector) 
{
	void* new_storage;
	int new_length;

	if (vector->Population == vector->Length) 
	{
		new_length = vector->Length * 2;
		new_storage = realloc(vector->Storage, sizeof(void*) * vector->Length * 2);
	}
	else if ((float)vector->Population / (float)vector->Length < 0.25)
	{
		new_length = vector->Length / 2;
		new_storage = realloc(vector->Storage, sizeof(void*) * vector->Length / 2);
	}
	else
		return;

	if (new_storage == NULL) 
	{
		Vector_Destroy(vector);
		printf("Error: Vector could not be resized!");
		exit(-1);
	}

	vector->Storage = new_storage;
	vector->Length = new_length;
}