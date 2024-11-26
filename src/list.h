#ifndef LIST_STRUCT
#define LIST_STRUCT
typedef struct {
	void **array;
	int maxSize;
	int size;
}LIST;
#endif

#define DEFAULT_LIST_SIZE 4

#ifndef LIST_LIST_CREATE
#define LIST_LIST_CREATE
/*
 * creates and returns an empty array list of pointers
**/
LIST listCreate();
#endif

#ifndef LIST_LIST_APPEND
#define LIST_LIST_APPEND
/*
 * takes a void pointer and adds it to the list by reference
**/
void listAppendItem(LIST *list, void *item);
#endif

#ifndef LIST_LIST_DELETE
#define LIST_LIST_DELETE
/*
 * sifts a room to the end of the array and decrements the size, then returns the item
**/
void *listPop(LIST *list, int i);
#endif

#ifndef LIST_LIST_INCLUDES
#define LIST_LIST_INCLUDES
/*
 * takes a LIST pointer and checks if it contains another pointer (unused)
**/
int listIncludesItem(LIST *list, const void *item);
#endif

#ifndef LIST_LIST_GET
#define LIST_LIST_GET
/*
 * returns the element at index i, useful for if I want to change how addressing works later
**/
void *listGetElement(LIST *list, int i);
#endif

#ifndef LIST_LIST_FREE
#define LIST_LIST_FREE
void listClear(LIST *list);
#endif

#ifndef LIST_LIST_PRINT
#define LIST_LIST_PRINT
void listPrint(LIST *list, const char *format);
#endif