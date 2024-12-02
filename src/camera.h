#include "transform.h"

#ifndef RAYTRACE_CAMERA
#define RAYTRACE_CAMERA

typedef struct {
	double focalLength;
	TRANSFORM transform;
} CAMERA;

CAMERA createCamera(double focalLength);

#endif