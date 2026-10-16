#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.hpp"

struct node {
    student data;
    struct node* next;
};

errno_t listInitialize(node** head)
{
    node* _node;

    _node = (node*)calloc(1, sizeof(*_node));
    if (!_node)
        return errno;

    *head = _node;
    return 0;
}

int listFree(node* head)
{
    node* current = head;
    node* next = head->next;
    int count = 0;

    while (next) {
        free(current);
        current = next;
        next = current->next;
        count++;
    }

    free(current);
    return count;
}

errno_t listInsert(node* const head, student const * const data)
{
    node* before = head->next;
    node* _new = NULL;
    errno_t err = 0;

    _new = (node*)calloc(1, sizeof(*_new));
    if (!_new)
        return errno;

    err = memcpy_s(&_new->data, sizeof(_new->data), data, sizeof(*data));
    if (!err) {
        _new->next = before;
        head->next = _new;
    }

    return err;
}

errno_t listAppend(node* const head, student const* const data)
{
    node* after = head->next;
    node* _new = NULL;
    errno_t err = 0;

    while (after->next)
        after = after->next;

    _new = (node*)calloc(1, sizeof(*_new));
    if (!_new)
        return errno;

    err = memcpy_s(&_new->data, sizeof(_new->data), data, sizeof(*data));
    if (!err) {
        _new->next = NULL;
        after->next = _new;
    }

    return err;
}

errno_t listFindSurname(node** result, node* const head, char const* const surname)
{
    node* current = head->next;

    while (current) {
        if (!strcmp(surname, current->data.surname)) {
            if (result)
                *result = current;
            return 0;
        }
        current = current->next;
    }

    return NOT_FOUND;
}

errno_t listRemove(node* const head, student const* const target)
{
    node* current = head->next;
    node* prev = head;

    while (current) {
        if (!memcmp(target, &current->data, sizeof(current->data))) {
            prev->next = current->next;
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }

    return NOT_FOUND;
}

int listPrintAll(node* const head)
{
    node* current = head->next;
    int count = 0;

    while (current) {
        printf("%s %s, %d.\n", current->data.name, current->data.surname, current->data.year);
        current = current->next;
        count++;
    }

    return count;
}