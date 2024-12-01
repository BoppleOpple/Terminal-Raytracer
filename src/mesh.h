#include "list.h"
#include "matrix.h"
#include "transform.h"

#ifndef RAYTRACE_MESH
#define RAYTRACE_MESH

typedef struct {
	MATRIX *vertices[3];
	MATRIX *normals[3];
} TRIANGLE;

typedef struct {
	LIST verts;
	LIST normals;
	LIST tris;
	TRANSFORM transform;
} MESH;

MESH createMesh(LIST *verts, LIST *normals, LIST *tris);

MESH meshFromOBJ(const char *filepath);

void printMesh(MESH *m);

#endif