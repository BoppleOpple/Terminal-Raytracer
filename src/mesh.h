#include "list.h"
#include "matrix.h"
#include "transform.h"

#ifndef RAYTRACE_MESH
#define RAYTRACE_MESH

typedef struct {
	MATRIX *vertices[3];
	MATRIX *normal;
} TRIANGLE;

typedef struct {
	LIST verts;
	LIST normals;
	LIST tris;
	TRANSFORM *transform;
} MESH;

MESH *createMesh();

MESH *meshFromOBJ(const char *filepath);

void printMesh(MESH *m);

void freeMesh(MESH *m);

#endif
