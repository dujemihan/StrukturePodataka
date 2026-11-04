#ifndef _LIST_HPP
#define _LIST_HPP

typedef struct node node;

node* newNode(long const data);
errno_t pushNode(node* const head, node* const new_node);
errno_t popNode(node* const head, long *ret);
int freeList(node* head);

#endif