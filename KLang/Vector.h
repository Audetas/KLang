#ifndef VECTOR_H
#define VECTOR_H

// Struct "Vector" contains the data and metrics of a expandable array of void pointers.
struct Vector
{
	int Length; // Number of positions in the entire Vector, occupied or not.
	int Population; // Number of occupied positions.
	void** Storage; // Raw storage.
};

struct Vector Vector_New(int size); // Creates new Vector of void pointers.
void Vector_Destroy(struct Vector* vector); // Properly frees a Vector.

void* Vector_Get(struct Vector* vector, int index); // Get pointer value at index "index".
void* Vector_Set(struct Vector* vector, int index, void* elem); // Set the pointer at index "index".
void Vector_Push(struct Vector* vector, void* elem); // Add a new element to the end of the Vector.
void* Vector_Pop(struct Vector* vector); // Remove the last element in the Vector.
void Vector_Remove(struct Vector* vector, int amount); // Remove "amount" amount of items from the top of the vector.
void Vector_Resize(struct Vector* vector); // Used interally to increase the size of the Vector when it has been filled.

#endif