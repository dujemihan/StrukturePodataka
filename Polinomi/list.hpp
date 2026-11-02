#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>

#define NOT_FOUND   (-1)
#define BAD_FILE    (-2)

/// <summary>
/// A polynomial
/// </summary>
typedef struct polynomial {
    int coeff;
    int exp;
} polynomial;

/// <summary>
/// A linked list node
/// </summary>
typedef struct node node;

/// <summary>
/// Initializes a new list
/// </summary>
/// <param name="head">Pointer to pointer to new list</param>
/// <returns>0 on success, otherwise errno</returns>
errno_t listInitialize(node** head);
/// <summary>
/// Destroys an initialized list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <returns>Number of destroyed elements</returns>
int listFree(node* head);
/// <summary>
/// Inserts an element to the beginning of a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <param name="data">Pointer to data to insert</param>
/// <returns>0 on success, errno otherwise</returns>
errno_t listInsert(node* const head, polynomial const* const data);
/// <summary>
/// Inserts an element to the end of a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <param name="data">Pointer to data to insert</param>
/// <returns>0 on success, errno otherwise</returns>
errno_t listAppend(node* const head, polynomial const* const data);
/// <summary>
/// Searches for first occurence of a surname in the list
/// </summary>
/// <param name="result">Pointer to pointer where result will be stored</param>
/// <param name="head">Pointer to list</param>
/// <param name="surname">Surname to search for</param>
/// <returns>0 if found, NOT_FOUND otherwise</returns>
errno_t listFindExp(node** result, node* const head, int const exp);
/// <summary>
/// Removes first occurence of an element from a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <param name="target">Pointer to data to search for and remove</param>
/// <returns>0 on success, NOT_FOUND otherwise</returns>
errno_t listRemove(node* const head, polynomial const* const target);
/// <summary>
/// Prints all the elements in a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <returns>Number of printed elements</returns>
int listPrintAll(node* const head);
errno_t listInsertBefore(node* const head, polynomial const* const data, polynomial const* const before);
errno_t listInsertAfter(node* const head, polynomial const* const data, polynomial const* const after);
int listSortAscending(node* const head);
int listSave(node const* head, char const* const filename);
int listLoad(node** head, char const* const filename);
errno_t polynomialSum(node* a, node* b, node** sum);
errno_t polynomialProduct(node* a, node* b, node** product);

#endif