#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>

#define NOT_FOUND   (-1)
#define BAD_FILE    (-2)

#define NAME_LEN    (20)
    
#define DELIM ','
#define FILE_HEADER "surname,name,year\n"

/// <summary>
/// A student
/// </summary>
typedef struct student {
    char name[NAME_LEN];
    char surname[NAME_LEN];
    unsigned short year;
} student;

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
errno_t listInsert(node* const head, student const* const data);
/// <summary>
/// Inserts an element to the end of a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <param name="data">Pointer to data to insert</param>
/// <returns>0 on success, errno otherwise</returns>
errno_t listAppend(node* const head, student const* const data);
/// <summary>
/// Searches for first occurence of a surname in the list
/// </summary>
/// <param name="result">Pointer to pointer where result will be stored</param>
/// <param name="head">Pointer to list</param>
/// <param name="surname">Surname to search for</param>
/// <returns>0 if found, NOT_FOUND otherwise</returns>
errno_t listFindSurname(node** result, node* const head, char const* const surname);
/// <summary>
/// Removes first occurence of an element from a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <param name="target">Pointer to data to search for and remove</param>
/// <returns>0 on success, NOT_FOUND otherwise</returns>
errno_t listRemove(node* const head, student const *const target);
/// <summary>
/// Prints all the elements in a list
/// </summary>
/// <param name="head">Pointer to target list</param>
/// <returns>Number of printed elements</returns>
int listPrintAll(node* const head);
errno_t listInsertBefore(node* const head, student const* const data, student const* const before);
errno_t listInsertAfter(node* const head, student const* const data, student const* const after);
int listSortAscending(node* const head);
int listSave(node const *head, char const* const filename);
int listLoad(node** head, char const* const filename);

#endif