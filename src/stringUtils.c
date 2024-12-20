#include "stringUtils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *slice(const char *s, int start, int end) {
	// standard splitting function
	char *substring = malloc(sizeof(char) * (end - start + 1));
	
	for (int i = start; i < end; i++)
		*(substring + i - start) = *(s + i);

	*(substring + end - start) = 0;

	return substring;
}

// splits a string everywhere a function returns truthy
LIST splitAtFunction(const char *s, int (fn)(const char, void *args), void *args) {
	// init list
	LIST sliced = listCreate();

	// keep track of the beginning of the substring
	int lastFalse = -1;
	for (int i = 0; i < strlen(s); i++) {
		// add a substring to the list if true
		if (fn(*(s + i), args)) {
			listAppendItem(&sliced, slice(s, lastFalse + 1, i));

			// reset the beginning
			lastFalse = i;
		}
	}

	// append the last bit from the last false location to the end
	if (lastFalse != strlen(s) - 1)
		listAppendItem(&sliced, slice(s, lastFalse + 1, strlen(s)));

	return sliced;
}

// helper function that checks if two chars are equal
int charEquality(const char a, void *b) {
	return a == *(char *) b;
}

LIST split(const char *s, char character) {
	// call splitAtFunction with the helper
	return splitAtFunction(s, &charEquality, &character);
}

// helper function that checks if a character is a space
int spaceComparison(const char a, void *_b) {
	return isspace(a);
}

LIST splitSpaces(const char *s) {
	// call splitAtFunction with the helper
	LIST sliced = splitAtFunction(s, &spaceComparison, NULL);

	// remove empties
	for (int i = 0; i < sliced.size; i++) {
		if (*(char *) listGetElement(&sliced, i) == 0) {
			free(listPop(&sliced, i));
		}
	}

	return sliced;
}