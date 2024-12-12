#include "camera.h"
#include "mesh.h"
#include <sys/ioctl.h>

#ifndef RAYTRACE_RENDER
#define RAYTRACE_RENDER

typedef struct {
	MATRIX *impactPosition;
	TRIANGLE *impactTri;
	double impactDistance;
} IMPACT;

IMPACT *createImpact(MATRIX *position, TRIANGLE *impactedTri, double distance);

void updateViewportSize(struct winsize *destination);

char *renderToString(CAMERA *c, struct winsize *outDimensions, MESH *m);

void clearScreen();

IMPACT *getRayMeshImpact(MATRIX *ray, MATRIX *rayOrigin, MESH *m);

void *getBestRayTriImpact(void *data);

IMPACT *getRayTriImpact(MATRIX *ray, MATRIX *rayOrigin, TRIANGLE *tri);

void freeImpact(IMPACT* i);

void printImpact(IMPACT *i);

#endif