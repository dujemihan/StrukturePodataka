#include <errno.h>
#include <stdlib.h>

#include "list.hpp"

typedef struct node {
	long data;
	node* next;
} node;

node* newNode(long const data) {
	node* newNode = (node*)calloc(1, sizeof(node));

	if (!newNode)
		return NULL;

	newNode->data = data;
	return newNode;
}

errno_t pushNode(node* const head, node* const newNode) {
	node* current = head;

	if (!head || !newNode)
		return EINVAL;

	newNode->next = head->next;
	head->next = newNode;

	current->next = newNode;
	return 0;
}

errno_t popNode(node* const head, long *ret) {
	node* target = head->next;

	if (!head || !target) {
		if (ret)
			*ret = 0;
		return EINVAL;
	}

	if (ret)
		*ret = target->data;

	head->next = target->next;
	free(target);
	target = NULL;
	return 0;
}

int freeList(node* head)
{
	int n = 0;

	while (head) {
		node* next = head->next;
		free(head);
		head = next;
		n++;
	}

	return n;
}