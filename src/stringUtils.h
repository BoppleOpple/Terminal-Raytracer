#include "list.h"

#ifndef RAYTRACE_STRINGUTILS
#define RAYTRACE_STRINGUTILS

char *slice(const char *s, int start, int end);

LIST split(const char *s, char character);

LIST splitSpaces(const char *s);

#endif