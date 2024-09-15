#include <stdio.h>

int main() {
	int myAge = 25;
	
	printf("%d\n", myAge);
	printf("%p\n", &myAge);

        int* ptrToMyAge = &myAge;

	printf("%p\n", ptrToMyAge);

	int** ptrToPtr = &ptrToMyAge;

	printf("%p\n", ptrToPtr);

	int** ptrToPtrTwo = ptrToPtr;

	printf("%p\n", ptrToPtrTwo);
	printf("%p\n", *ptrToPtrTwo);

	return 0;
}
