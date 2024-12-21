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

/**
 * @brief helper to make a new camera struct
 *
 * @param minDistance double the distance to the min clipping plane
 * @param maxDistance double the distance to the max clipping plane
 * @param fov double the camera's fov
 * @return CAMERA the new camera struct
 */
CAMERA *createCamera(double minDistance, double maxDistance, double fov);

/**
 * @brief returns the direction of a ray emitted from a given pixel in a viewport
 *
 * @param c CAMERA the camera the ray is emitted from
 * @param px int the x pixel coordinate of the ray
 * @param px int the x pixel coordinate of the ray
 * @param outDimensions struct winsize the dimensions of the viewport
 * @return MATRIX the ray direction vector
 */
MATRIX *getScreenRay(CAMERA *c, int px, int py, struct winsize *outDimensions);

/**
 * @brief frees all the fields of a CAMERA struct
 *
 * @param c CAMERA the camera to free
 */
void freeCamera(CAMERA *c);

#endif