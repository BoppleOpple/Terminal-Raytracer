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

/**
 * @brief helper to make a new impact struct
 *
 * @param position MATRIX impact position
 * @param impactedTri TRIANGLE the tri that the impact was on
 * @param distance double distance to the impact
 * @return IMPACT the new impact
 */
IMPACT *createImpact(MATRIX *position, TRIANGLE *impactedTri, double distance);

/**
 * @brief helper to store the window size in a more readable function
 *
 * @param destination struct winsize target struct
 */
void updateViewportSize(struct winsize *destination);

/**
 * @brief renders a mesh from a camera to a string
 *
 * @param c CAMERA the camera whose viewport is being used
 * @param outDimensions struct winsize the viewport dimensions
 * @param m MESH the mesh to render
 * @return char* the rendered string
 */
char *renderToString(CAMERA *c, struct winsize *outDimensions, MESH *m);

/**
 * @brief helper to clear terminal for rendering
 */
void clearScreen();

/**
 * @brief gets the impact struct between a ray and a mesh
 *
 * @param ray MATRIX the direction vector of the ray
 * @param rayOrigin MATRIX the origin vector of the ray
 * @param m MESH the mesh to render
 * @return IMPACT the resulting impact struct
 */
IMPACT *getRayMeshImpact(MATRIX *ray, MATRIX *rayOrigin, MESH *m);

/**
 * @brief for threading, a wrapper for getRayTriImpact
 *
 * @param data void* the thread data
 */
void *getBestRayTriImpact(void *data);

/**
 * @brief gets the impact struct between a ray and a tri
 *
 * @param ray MATRIX the direction vector of the ray
 * @param rayOrigin MATRIX the origin vector of the ray
 * @param tri TRIANGLE the tri to render
 * @return IMPACT the resulting impact struct, NULL if it was oob of the tri
 */
IMPACT *getRayTriImpact(MATRIX *ray, MATRIX *rayOrigin, TRIANGLE *tri);

/**
 * @brief frees the fields of an IMPACT struct
 *
 * @param i IMPACT impact to free
 */
void freeImpact(IMPACT* i);

/**
 * @brief prints an IMPACT struct
 *
 * @param i IMPACT the impact to print
 */
void printImpact(IMPACT *i);

#endif