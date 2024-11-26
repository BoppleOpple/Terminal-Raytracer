#include "stringUtils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

LIST splitSpaces(const char *s) {
	LIST sliced = listCreate();

	int lastSpace = -1;
	for (int i = 0; i < strlen(s); i++) {
		if (isspace(*(s + i))) {
			if (lastSpace != i - 1)
				listAppendItem(&sliced, slice(s, lastSpace + 1, i));
			lastSpace = i;
		}
	}

	return sliced;
}

char *slice(const char *s, int start, int end) {
	char *substring = malloc(sizeof(char) * (end - start + 1));
	memcpy(substring, s + start, sizeof(char) * (end - start));
	return substring;
}