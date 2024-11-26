#include "matrix.h"
#include "list.h"

#ifndef RAYTRACE_MESH
#define RAYTRACE_MESH

typedef struct {
	MATRIX *vertices[3];
	MATRIX normal;
} TRIANGLE;

typedef struct {
	LIST verts;
	LIST tris;
	MATRIX location;
} MESH;

MESH createMesh(TRIANGLE *tris, int triCount);

MESH meshFromOBJ(const char *filepath);

#endif