#include "mesh.h"
#include "matrix.h"
#include "stringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MESH createMesh(LIST *verts, LIST *normals, LIST *tris) {
	return (MESH) {
		*verts,
		*normals,
		*tris,
		createTransform()
	};
}

MESH meshFromOBJ(const char *filepath) {
	char buffer[256];
	LIST vertList = listCreate();
	LIST normalList = listCreate();
	LIST triList = listCreate();
	LIST objLineArguments;
	FILE *file = fopen(filepath, "r");

	while (fgets(buffer, sizeof(buffer), file)) {
		objLineArguments = splitSpaces(buffer);
		
		if (objLineArguments.size <= 0)
			continue;

		if (strcmp((char *) listGetElement(&objLineArguments, 0), "v") == 0) {
			if (objLineArguments.size == 4) {
				MATRIX *newVertex = malloc(sizeof(MATRIX));

				*newVertex = createVector(
					atof((char *) listGetElement(&objLineArguments, 1)),
					atof((char *) listGetElement(&objLineArguments, 2)),
					atof((char *) listGetElement(&objLineArguments, 3))
				);

				listAppendItem(&vertList, newVertex);
			} else {
				printf("malformed obj file!\n");
				exit(1);
			}
		} else if (strcmp((char *) listGetElement(&objLineArguments, 0), "vn") == 0) {
			if (objLineArguments.size == 4) {
				MATRIX *newVertex = malloc(sizeof(MATRIX));

				*newVertex = createVector(
					atof((char *) listGetElement(&objLineArguments, 1)),
					atof((char *) listGetElement(&objLineArguments, 2)),
					atof((char *) listGetElement(&objLineArguments, 3))
				);

				listAppendItem(&normalList, newVertex);
			} else {
				printf("malformed obj file!\n");
				exit(1);
			}
		} else if (strcmp((char *) listGetElement(&objLineArguments, 0), "f") == 0) {
			if (objLineArguments.size >= 4) {
				int polygonVertices[objLineArguments.size - 1];
				// int polygonTextureVertices[objLineArguments.size - 1];
				int polygonNormals[objLineArguments.size - 1];
				for (int i = 1; i < objLineArguments.size; i++) {
					LIST vertexInfo = split(listGetElement(&objLineArguments, i), '/');
					polygonVertices[i-1] = atoi(listGetElement(&vertexInfo, 0));
					// polygonTextureVertices[i-1] = atoi(listGetElement(&vertexInfo, 1));
					polygonNormals[i-1] = atoi(listGetElement(&vertexInfo, 2));
					listClear(&vertexInfo);
				}

				for (int i = 3; i < objLineArguments.size; i++) {
					TRIANGLE *newTri = malloc(sizeof(TRIANGLE));
					newTri->vertices[0] = listGetElement(&vertList, polygonVertices[0]);
					newTri->normals[0] = listGetElement(&normalList, polygonNormals[0]);

					newTri->vertices[1] = listGetElement(&vertList, polygonVertices[i-1]);
					newTri->normals[1] = listGetElement(&normalList, polygonNormals[i-1]);

					newTri->vertices[2] = listGetElement(&vertList, polygonVertices[i]);
					newTri->normals[2] = listGetElement(&normalList, polygonNormals[i]);

					listAppendItem(&triList, newTri);
				}
			} else {
				printf("malformed obj file!\n");
				exit(1);
			}
		}

		listClear(&objLineArguments);
	}

	return createMesh(&vertList, &normalList, &triList);
}

void printMesh(MESH *m) {
	printf("Details of mesh at (%lf, %lf, %lf):\n", m->transform.translation.data[0], m->transform.translation.data[1], m->transform.translation.data[2]);
	printf("--| %i vertices\n", m->verts.size);
	printf("--| %i normals\n", m->normals.size);
	printf("--| %i tris\n", m->tris.size);
}