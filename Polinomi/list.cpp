#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.hpp"

struct node {
    polynomial data;
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

errno_t listInsert(node* const head, polynomial const* const data)
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

errno_t listAppend(node* const head, polynomial const* const data)
{
    node* after = head->next ? head->next : head;

    while (after->next)
        after = after->next;

    return listInsert(after, data);
}

errno_t listFindExp(node** result, node* const head, int const exp)
{
    node* current = head->next;

    while (current) {
        if (exp == current->data.exp) {
            if (result)
                *result = current;
            return 0;
        }
        current = current->next;
    }

    return NOT_FOUND;
}

errno_t listRemove(node* const head, polynomial const* const target)
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
        printf("%dx^%d + ", current->data.coeff, current->data.exp);
        current = current->next;
        count++;
    }

    fputs("0\n", stdout);
    return count;
}

/// <summary>
/// 
/// </summary>
/// <param name="head"></param>
/// <param name="target"></param>
/// <returns>Node before the target one</returns>
static node* listFind(node* const head, polynomial const* const target)
{
    node* current = head;

    while (current->next) {
        if (!memcmp(target, &current->next->data, sizeof(*target))) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

errno_t listInsertBefore(node* const head, polynomial const* const data, polynomial const* const before)
{
    node* bBefore = NULL, * _new = NULL;
    errno_t err = 0;

    bBefore = listFind(head, before);
    if (!bBefore)
        return NOT_FOUND;

    _new = (node*)calloc(1, sizeof(*_new));
    if (!_new)
        return errno;

    err = memcpy_s(&_new->data, sizeof(polynomial), data, sizeof(*data));
    if (!err) {
        _new->next = bBefore->next;
        bBefore->next = _new;
    }

    return err;
}

errno_t listInsertAfter(node* const head, polynomial const* const data, polynomial const* const after)
{
    node* bAfter = NULL;

    bAfter = listFind(head, after);
    if (!bAfter)
        return NULL;

    return listInsertBefore(head, data, &bAfter->next->next->data);
}

// https://postimg.cc/c6TYqxhP
int listSortAscending(node* const head)
{
    node* outer = head;
    int n = 0;

    while (outer && outer->next) {
        node* bMin = outer;
        node* min = outer->next;
        node* inner = outer;

        while (inner && inner->next) {
            if (bMin->next->data.exp > inner->next->data.exp) {
                bMin = inner;
                min = inner->next;
            }

            inner = inner->next;
        }

        if (outer->next != min) {
            node* temp = min->next;

            if (outer->next == bMin) {
                min->next = outer->next;
                outer->next = min;
                min->next->next = temp;
            }
            else {
                min->next = outer->next->next;
                bMin->next = outer->next;
                outer->next->next = temp;
                outer->next = min;
            }

            n++;
        }

        outer = outer->next;
    }

    return n;
}

int listSave(node const* head, char const* const filename)
{
    FILE* fp = NULL;
    errno_t err = 0;

    if (!head->next)
        return NOT_FOUND; // Nothing to save

    err = fopen_s(&fp, filename, "w");
    if (err) {
        perror("Failed to open file for writing");
        return err;
    }

    head = head->next;
    while (head) {
        fprintf(fp, "%d %d ", head->data.coeff, head->data.exp);
        head = head->next;
    }

    fclose(fp);
    return 0;
}

int listLoad(node** head, char const* const filename)
{
    char buf[BUFSIZ] = { 0 };
    node* newHead = NULL;
    FILE* fp = NULL;
    errno_t err = 0;

    err = fopen_s(&fp, filename, "r");
    if (err) {
        perror("Failed to open file for reading");
        return err;
    }

    err = listInitialize(&newHead);
    if (err) {
        perror("Failed to init list");
        fclose(fp);
        return -err;
    }

    polynomial a = { 0 };
    while (fscanf_s(fp, " %d %d", &a.coeff, &a.exp) == 2) {
        err = listAppend(newHead, &a);
        if (err)
            perror("Failed to insert element");
    }

    listSortAscending(newHead);

    *head = newHead;
    fclose(fp);
    return 0;
}

errno_t polynomialSum(node* a, node* b, node** sum)
{
    node* _a = NULL, * _b = NULL;
    errno_t err = 0;

    err = listInitialize(sum);
    if (err)
        return err;

    _a = a->next;
    _b = b->next;

    while (_a && _b) {
        if (_a->data.exp < _b->data.exp) {
            listAppend(*sum, &_a->data);
            _a = _a->next;
        }
        else if (_a->data.exp > _b->data.exp) {
            listAppend(*sum, &_b->data);
            _b = _b->next;
        }
        else {
            polynomial d = _a->data;
            d.coeff += _b->data.coeff;

            if (d.coeff)
                listAppend(*sum, &d);

            _a = _a->next;
            _b = _b->next;
        }
    }

    while (_a) {
        listAppend(*sum, &_a->data);
        _a = _a->next;
    }

    while (_b) {
        listAppend(*sum, &_b->data);
        _b = _b->next;
    }

    return 0;
}

errno_t polynomialProduct(node* a, node* b, node** product)
{
    errno_t err = 0;

    err = listInitialize(product);
    if (err)
        return err;

    node* _b = b;
    while (a->next) {
        while (_b->next) {
            polynomial d = a->next->data;
            node* target = NULL;

            d.coeff *= _b->next->data.coeff;
            d.exp += _b->next->data.exp;

            listFindExp(&target, *product, d.exp);
            if (target)
                target->data.coeff += d.coeff;
            else {
                listAppend(*product, &d);
            }

            _b = _b->next;
        }

        a = a->next;
        _b = b;
    }

    listSortAscending(*product);

    return 0;
}