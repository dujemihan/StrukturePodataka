#include <stdio.h>

#include "list.hpp"

#define FILENAME_A "a.txt"
#define FILENAME_B "b.txt"

int main()
{
	node* a = NULL, * b = NULL, *c = NULL, *d = NULL;
	errno_t err = 0;

	err = listLoad(&a, FILENAME_A);
	if (err) {
		fprintf(stderr, "Failed to read %s\n", FILENAME_A);
		return err;
	}

	err = listLoad(&b, "b.txt");
	if (err) {
		fprintf(stderr, "Failed to read %s\n", FILENAME_B);
		return err;
	}

	polynomialSum(b, a, &c);
	listPrintAll(a);
	listPrintAll(b);
	listPrintAll(c);

	polynomialProduct(a, b, &d);
	listPrintAll(d);

	listFree(a);
	listFree(b);
	listFree(c);
	listFree(d);
	return 0;
}