#include <stdio.h>

#include "list.hpp"

int main()
{
    student a = {
        .name = "Mate",
        .surname = "Matic",
        .year = 2006,
    };
    student b = {
        .name = "Duje",
        .surname = "Dujic",
        .year = 2007,
    };
    student c = {
        .name = "Ante",
        .surname = "Antic",
        .year = 2008,
    };
    node* head = NULL;
    errno_t err = 0;

    err = listInitialize(&head);
    if (err) {
        perror("Failed to init list");
        return -err;
    }

    //err = listAppend(head, &b);
    //if (err) {
        //printf("Failed to insert element: %d\n", err);
    //}
    err = listInsert(head, &a);
    if (err) {
        printf("Failed to insert element: %d\n", err);
    }

    err = listInsert(head, &b);
    if (err) {
        printf("Failed to insert element: %d\n", err);
    }

    err = listAppend(head, &c);
    if (err) {
        printf("Failed to insert element: %d\n", err);
    }

    student d = {
        .name = "Alpha",
        .surname = "Beta",
        .year = 2000,
    };
    err = listInsertBefore(head, &d, &a);
    if (err)
        printf("Failed to insert element: %d\n", err);

    student e = {
        .name = "Gamma",
        .surname = "Delta",
        .year = 2000,
    };
    err = listInsertAfter(head, &e, &a);
    if (err)
        printf("Failed to insert element: %d\n", err);

    printf("Printed %d items\n", listPrintAll(head));
    listSortAscending(head);
    printf("Printed %d items\n", listPrintAll(head));

    printf("Search for Matic returned %d\n", listFindSurname(NULL, head, "Matic"));
    printf("Search for Test returned %d\n", listFindSurname(NULL, head, "Test"));

    printf("List save returned %d\n", listSave(head, "list.csv"));

    printf("Removal of %s returned %d\n", a.surname, listRemove(head, &a));

    printf("Destroyed %d items\n", listFree(head));

    printf("List load returned %d\n", listLoad(&head, "list.csv"));
    listPrintAll(head);
    return 0;
}