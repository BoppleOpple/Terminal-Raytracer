#include "camera.h"
#include "matrix.h"
#include <math.h>
#include <stdlib.h>

CAMERA *createCamera(double minDistance, double maxDistance, double fov) {
	CAMERA *c = malloc(sizeof(CAMERA));

	// fill out all fields from params, and give it a default transform
	*c = (CAMERA) {
		minDistance,
		maxDistance,
		fov,
		createTransform()
	};

	return c;
}

MATRIX *getScreenRay(CAMERA *c, int px, int py, struct winsize *outDimensions) {
	// scale depends on height, so x = px / width * (width / height) and y = py / height (not for now)
	// double aspectRatio = (double) outDimensions->ws_col / outDimensions->ws_row;
	double screenPositionX = 2.0 * (px - (double) outDimensions->ws_col / 2.0) / outDimensions->ws_col;
	double screenPositionY = 2.0 * (py - (double) outDimensions->ws_row / 2.0) / outDimensions->ws_row;

	// printf("%lf, %lf\n", screenPositionX, screenPositionY);

	// get the ray info in local space by scaling its normalize viewport positions to create a plane defined by x = 1
	MATRIX *imagePlaneSize = createVector(1.0, cos(c->fov / 2.0), sin(c->fov / 2.0));
	MATRIX *result = createVector(1.0, screenPositionX, screenPositionY);
	multMatrixElementwise(result, imagePlaneSize);

	// and normalize the result
	normalizeVector(result);

	freeMatrix(imagePlaneSize);
	free(imagePlaneSize);
	imagePlaneSize = NULL;
	
	return result;
}

void freeCamera(CAMERA *c) {
	// free the transform, as everything else is statically allocated
	freeTransform(c->transform);
	free(c->transform);
	c->transform = NULL;
}