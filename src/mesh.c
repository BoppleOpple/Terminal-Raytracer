#include "mesh.h"
#include "stringUtils.h"
#include <stdio.h>

MESH createMesh(TRIANGLE *tris, int triCount) {

}

MESH meshFromOBJ(const char *filepath) {
	char buffer[256];
	LIST triList = listCreate();
	LIST fileArgs;
	FILE *file = fopen(filepath, "r");

	while (fgets(buffer, sizeof(buffer), file)) {
		fileArgs = splitSpaces(buffer);
		listPrint(&fileArgs, "%s");
		listClear(&fileArgs);
	}

}