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
    node* after = head->next ? head->next : head;

    while (after->next)
        after = after->next;

    return listInsert(after, data);
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

/// <summary>
/// 
/// </summary>
/// <param name="head"></param>
/// <param name="target"></param>
/// <returns>Node before the target one</returns>
static node* listFind(node* const head, student const* const target)
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

errno_t listInsertBefore(node* const head, student const* const data, student const* const before)
{
    node* bBefore = NULL, * _new = NULL;
    errno_t err = 0;

    bBefore = listFind(head, before);
    if (!bBefore)
        return NOT_FOUND;

    _new = (node*)calloc(1, sizeof(*_new));
    if (!_new)
        return errno;

    err = memcpy_s(&_new->data, sizeof(student), data, sizeof(*data));
    if (!err) {
        _new->next = bBefore->next;
        bBefore->next = _new;
    }

    return err;
}

errno_t listInsertAfter(node* const head, student const* const data, student const* const after)
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
            int result = strcmp(bMin->next->data.surname, inner->next->data.surname);
            if (result == 0)
                result = strcmp(bMin->next->data.name, inner->next->data.name);

            if (result > 0) {
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

int listSave(node const *head, char const* const filename)
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

    fprintf(fp, FILE_HEADER);

    head = head->next;
    while (head) {
        fprintf(fp, "%s%c%s%c%hu\n", head->data.surname, DELIM, head->data.name, DELIM, head->data.year);
        head = head->next;
    }

    fclose(fp);
    return 0;
}

int listLoad(node** head, char const* const filename)
{
    char buf[BUFSIZ] = { 0 };
    char fmt[BUFSIZ] = { 0 };
    node* newHead = NULL;
    FILE* fp = NULL;
    errno_t err = 0;
    
    err = fopen_s(&fp, filename, "r");
    if (err) {
        perror("Failed to open file for reading");
        return err;
    }

    if (!fgets(buf, BUFSIZ, fp)) {
        fclose(fp);
        return BAD_FILE;
    }

    if (strcmp(buf, FILE_HEADER) != 0) {
        fclose(fp);
        return BAD_FILE;
    }

    err = listInitialize(&newHead);
    if (err) {
        perror("Failed to init list");
        fclose(fp);
        return -err;
    }

    snprintf(fmt, sizeof(fmt), "%%%d[^%c]%c%%%d[^%c]%c%%hu\n",
        NAME_LEN - 1, DELIM, DELIM, NAME_LEN - 1, DELIM, DELIM);

    student a = { 0 };
    while (fscanf_s(fp, fmt, a.surname, NAME_LEN, a.name, NAME_LEN, &a.year) == 3) {
        err = listAppend(newHead, &a);
        if (err)
            perror("Failed to insert element");
    }

    *head = newHead;
    fclose(fp);
    return 0;
}