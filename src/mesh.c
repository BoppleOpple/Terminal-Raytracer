#include "mesh.h"
#include "matrix.h"
#include "stringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MESH *createMesh() {
	MESH *m = malloc(sizeof(MESH));
	// set everything to empty or default

	*m = (MESH) {
		listCreate(),
		listCreate(),
		listCreate(),
		createTransform()
	};
	return m;
}

MESH *meshFromOBJ(const char *filepath) {
	// initialize all the things
	char buffer[256];
	MESH *mesh = createMesh();
	LIST objLineArguments;
	FILE *file = fopen(filepath, "r");

	printf("file: %s\n\n", filepath);

	// read each line of the file
	while (fgets(buffer, sizeof(buffer), file)) {
		objLineArguments = splitSpaces(buffer);
		
		// continue if its empty
		if (objLineArguments.size <= 0)
			continue;
		
		// cast forst element from void* to string
		char *arg = (char *) listGetElement(&objLineArguments, 0);

		// perform the correct operation depending on the first argument
		if (strcmp(arg, "v") == 0) { // "v": new vertex
			// ensure there are enough args
			if (objLineArguments.size == 4) {
				// if there are add a new one
				MATRIX *newVertex = createVector(
					atof((char *) listGetElement(&objLineArguments, 1)),
					atof((char *) listGetElement(&objLineArguments, 2)),
					atof((char *) listGetElement(&objLineArguments, 3))
				);

				listAppendItem(&mesh->verts, newVertex);
			} else {
				// otherwise yell
				printf("malformed obj file!\n");
				exit(1);
			}
		} else if (strcmp(arg, "vn") == 0) { // "vn": vertex normal
			// ensure there are enough args
			if (objLineArguments.size == 4) {
				// if there are add a new one
				MATRIX *newVertex = createVector(
					atof((char *) listGetElement(&objLineArguments, 1)),
					atof((char *) listGetElement(&objLineArguments, 2)),
					atof((char *) listGetElement(&objLineArguments, 3))
				);

				listAppendItem(&mesh->normals, newVertex);
			} else {
				// otherwise yell
				printf("malformed obj file!\n");
				exit(1);
			}
		} else if (strcmp(arg, "f") == 0) { // "f": new face
			// ensure there are enough args
			if (objLineArguments.size >= 4) {
				// if there are add a new one
				int polygonVertices[objLineArguments.size - 1];
				// int polygonTextureVertices[objLineArguments.size - 1];
				int polygonNormals[objLineArguments.size - 1];
				for (int i = 1; i < objLineArguments.size; i++) {
					LIST vertexInfo = split(listGetElement(&objLineArguments, i), '/');

					// add the vertex and normal indices
					polygonVertices[i-1] = atoi(listGetElement(&vertexInfo, 0));
					// polygonTextureVertices[i-1] = atoi(listGetElement(&vertexInfo, 1));
					polygonNormals[i-1] = atoi(listGetElement(&vertexInfo, 2));

					listClear(&vertexInfo);
				}

				// since they arent always triangles, make polygons into sever triangls around the first vertex notated
				for (int i = 2; i < objLineArguments.size - 1; i++) {
					TRIANGLE *newTri = malloc(sizeof(TRIANGLE));
					// set the tri info

					newTri->vertices[0] = listGetElement(&mesh->verts, polygonVertices[0] - 1);
					newTri->vertices[1] = listGetElement(&mesh->verts, polygonVertices[i-1] - 1);
					newTri->vertices[2] = listGetElement(&mesh->verts, polygonVertices[i] - 1);

					newTri->normal = listGetElement(&mesh->normals, polygonNormals[i] - 1);

					// add the tri
					listAppendItem(&mesh->tris, newTri);
				}
			} else {
				// otherwise yell
				printf("malformed obj file!\n");
				exit(1);
			}
		}

		// free the args
		listClear(&objLineArguments);
	}

	return mesh;
}

void printMesh(MESH *m) {
	// store the position for easy access
	MATRIX *position = getTranslationVector(m->transform);

	printf("Details of mesh at (%lf, %lf, %lf):\n", position->data[0], position->data[1], position->data[2]);
	printf("--| %i vertices\n", m->verts.size);
	printf("--| %i normals\n", m->normals.size);
	printf("--| %i tris\n", m->tris.size);

	// free the temp position
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
	// for (int i = 0; i < m->tris.size; i++) {
	// 	free(listGetElement(&m->tris, i));
	// }

	listClear(&m->verts);
	listClear(&m->normals);
	listClear(&m->tris);

	freeTransform(m->transform);
	free(m->transform);
	m->transform = NULL;
}
