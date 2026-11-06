#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.hpp"

#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define DEBUG(fmt, ...)	fprintf(stderr, "%s.%d: " fmt, __FILE__, __LINE__, __VA_ARGS__)
#else
#define DEBUG(fmt, ...)
#endif

static int printNumber(int const data);
static int printOperator(int const data);

int main(int argc, char* argv[])
{
	char* buffer = NULL;
	char* filename = argv[1];
	node* numbers = newNode(0);
	char* ctx = NULL;
	FILE* fp = NULL;
	size_t length = 0;
	errno_t err = 0;
	long result = 0;

	if (!numbers)
		return -ENOMEM;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return -EINVAL;
	}

	err = fopen_s(&fp, filename, "r");
	if (err) {
		fprintf(stderr, "Error opening file %s\n", filename);
		return -err;
	}

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buffer = (char*)calloc(length + 1, sizeof(*buffer));
	if (!buffer) {
		fclose(fp);
		return -ENOMEM;
	}
	fread(buffer, sizeof(*buffer), length, fp);
	fclose(fp);
	
	for(char* token = strtok_s(buffer, " ", &ctx); token; token = strtok_s(NULL, " ", &ctx)) {
		char* end = NULL;
		long number = strtol(token, &end, 0);

		if (end != token) {
			node* _new = newNode(number);

			if (!_new)
				return -ENOMEM;

			pushNode(numbers, _new);
		}
		else {
			long a = 0, b = 0;

			err = popNode(numbers, &b);
			if (err) {
				fprintf(stderr, "%s: failed to get a number, check input!", __func__);
				return -err;
			}

			err = popNode(numbers, &a);
			if (err) {
				fprintf(stderr, "%s: failed to get a number, check input!", __func__);
				return -err;
			}

			switch (*token) {
			case '+': {
				DEBUG("%ld + %ld = %ld\n", a, b, a + b);
				node* _new = newNode(a + b);
				pushNode(numbers, _new);
				break;
			}
			case '-': {
				DEBUG("%ld - %ld = %ld\n", a, b, a - b);
				node* _new = newNode(a - b);
				pushNode(numbers, _new);
				break;
			}
			case '*': {
				DEBUG("%ld * %ld = %ld\n", a, b, a * b);
				node* _new = newNode(a * b);
				pushNode(numbers, _new);
				break;
			}
			case '/': {
				DEBUG("%ld / %ld = %ld\n", a, b, a / b);
				node* _new = newNode(a / b);
				pushNode(numbers, _new);
				break;
			}
			case '^': {
				DEBUG("%ld ^ %ld = %ld\n", a, b, (long)powl(a, b));
				node* _new = newNode((long)powl(a, b));
				pushNode(numbers, _new);
				break;
			}
			}
		}
	}

	err = popNode(numbers, &result);
	if (err) {
		fprintf(stderr, "No result, check input!\n");
		DEBUG("Destroyed %d item(s)\n", freeList(numbers));
		return -EINVAL;
	}

	err = popNode(numbers, NULL);
	if (!err) {
		fprintf(stderr, "Excess numbers, check input!\n");
		DEBUG("Destroyed %d item(s)\n", freeList(numbers));
		return -EINVAL;
	}

	printf("%ld\n", result);
	DEBUG("Destroyed %d item(s)\n", freeList(numbers));

	return 0;
}