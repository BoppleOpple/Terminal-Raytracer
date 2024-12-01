#include "stringUtils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *slice(const char *s, int start, int end) {
	char *substring = malloc(sizeof(char) * (end - start + 1));
	memcpy(substring, s + start, sizeof(char) * (end - start));
	return substring;
}

LIST splitAtFunction(const char *s, int (fn)(const char, void *args), void *args) {
	LIST sliced = listCreate();

	int lastSpace = -1;
	for (int i = 0; i < strlen(s); i++) {
		if (fn(*(s + i), args)) {
			listAppendItem(&sliced, slice(s, lastSpace + 1, i));
			lastSpace = i;
		}
	}

	if (lastSpace != strlen(s) - 1)
		listAppendItem(&sliced, slice(s, lastSpace + 1, strlen(s)));

	return sliced;
}

int charEquality(const char a, void *b) {
	return a == *(char *) b;
}

LIST split(const char *s, char character) {
	return splitAtFunction(s, &charEquality, &character);
}

int spaceComparison(const char a, void *_b) {
	return isspace(a);
}

LIST splitSpaces(const char *s) {
	LIST sliced = splitAtFunction(s, &spaceComparison, NULL);
	for (int i = 0; i < sliced.size; i++) {
		if (*(char *) listGetElement(&sliced, i) == 0) {
			free(listPop(&sliced, i));
		}
	}
	return sliced;
}