#include "mesh.h"
#include "matrix.h"
#include "stringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MESH *createMesh() {
	MESH *m = malloc(sizeof(MESH));
	*m = (MESH) {
		listCreate(),
		listCreate(),
		listCreate(),
		createTransform()
	};
	return m;
}

MESH *meshFromOBJ(const char *filepath) {
	char buffer[256];
	MESH *mesh = createMesh();
	LIST objLineArguments;
	FILE *file = fopen(filepath, "r");

	printf("file: %s\n\n", filepath);
	while (fgets(buffer, sizeof(buffer), file)) {
		objLineArguments = splitSpaces(buffer);
		
		if (objLineArguments.size <= 0)
			continue;

		char *arg = (char *) listGetElement(&objLineArguments, 0);

		if (strcmp(arg, "v") == 0) {
			if (objLineArguments.size == 4) {
				MATRIX *newVertex = createVector(
					atof((char *) listGetElement(&objLineArguments, 1)),
					atof((char *) listGetElement(&objLineArguments, 2)),
					atof((char *) listGetElement(&objLineArguments, 3))
				);

				listAppendItem(&mesh->verts, newVertex);
			} else {
				printf("malformed obj file!\n");
				exit(1);
			}
		} else if (strcmp(arg, "vn") == 0) {
			if (objLineArguments.size == 4) {
				MATRIX *newVertex = createVector(
					atof((char *) listGetElement(&objLineArguments, 1)),
					atof((char *) listGetElement(&objLineArguments, 2)),
					atof((char *) listGetElement(&objLineArguments, 3))
				);

				listAppendItem(&mesh->normals, newVertex);
			} else {
				printf("malformed obj file!\n");
				exit(1);
			}
		} else if (strcmp(arg, "f") == 0) {
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

				for (int i = 2; i < objLineArguments.size - 1; i++) {
					TRIANGLE *newTri = malloc(sizeof(TRIANGLE));
					newTri->vertices[0] = listGetElement(&mesh->verts, polygonVertices[0] - 1);
					newTri->vertices[1] = listGetElement(&mesh->verts, polygonVertices[i-1] - 1);
					newTri->vertices[2] = listGetElement(&mesh->verts, polygonVertices[i] - 1);

					newTri->normal = listGetElement(&mesh->normals, polygonNormals[i] - 1);

					listAppendItem(&mesh->tris, newTri);
				}
			} else {
				printf("malformed obj file!\n");
				exit(1);
			}
		}

		listClear(&objLineArguments);
	}

	return mesh;
}

void printMesh(MESH *m) {
	MATRIX *position = getTranslationVector(m->transform);

	printf("Details of mesh at (%lf, %lf, %lf):\n", position->data[0], position->data[1], position->data[2]);
	printf("--| %i vertices\n", m->verts.size);
	printf("--| %i normals\n", m->normals.size);
	printf("--| %i tris\n", m->tris.size);
	freeMatrix(position);

	free(position);
	position = NULL;
}

void freeMesh(MESH *m) {
	for (int i = 0; i < m->verts.size; i++) {
		freeMatrix(listGetElement(&m->verts, i));
	}
	for (int i = 0; i < m->normals.size; i++) {
		freeMatrix(listGetElement(&m->normals, i));
	}
	for (int i = 0; i < m->tris.size; i++) {
		free(listGetElement(&m->tris, i));
	}

	listClear(&m->verts);
	listClear(&m->normals);
	listClear(&m->tris);

	freeTransform(m->transform);
	free(m->transform);
	m->transform = NULL;
}
