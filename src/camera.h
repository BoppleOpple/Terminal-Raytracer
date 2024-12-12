#include "mesh.h"
#include "transform.h"
#include <sys/ioctl.h>

#ifndef RAYTRACE_CAMERA
#define RAYTRACE_CAMERA

// forward direction is +Z
typedef struct {
	double minDistance;
	double maxDistance;
	double fov;
	TRANSFORM *transform;
} CAMERA;

CAMERA *createCamera(double focalLength);

MATRIX *getScreenRay(CAMERA *c, int px, int py, struct winsize *outDimensions);

void freeCamera(CAMERA *c);

#endif