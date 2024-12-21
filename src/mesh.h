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

/**
 * @brief helper to make an empty mesh struct

 * @return MESH the new mesh
 */
MESH *createMesh();

/**
 * @brief helper to make a new mesh struct from an obj file
 *
 * @param filepath char* the file destination
 * @return MESH
 */
MESH *meshFromOBJ(const char *filepath);

/**
 * @brief prints mesh information
 *
 * @param m MESH the target mesh
 */
void printMesh(MESH *m);

/**
 * @brief frees all the fields of a mesh
 *
 * @param m MESH the target mesh
 */
void freeMesh(MESH *m);

#endif
