#include "camera.h"
#include "mathUtils.h"
#include "matrix.h"
#include <math.h>

CAMERA createCamera(double focalLength) {
	return (CAMERA) {
		0.0,
		10.0,
		PI / 2.0,
		createTransform()
	};
}

MATRIX getScreenRay(CAMERA *c, int px, int py, struct winsize *outDimensions) {
	// scale depends on height, so x = px / width * (width / height) and y = py / height
	double aspectRatio = (double) outDimensions->ws_col / outDimensions->ws_row;
	double screenPositionX = 2.0 * (px - (double) outDimensions->ws_col / 2.0) / outDimensions->ws_col * aspectRatio;
	double screenPositionY = 2.0 * (py - (double) outDimensions->ws_row / 2.0) / outDimensions->ws_row;

	MATRIX imagePlaneSize = createVector(cos(c->fov / 2.0), sin(c->fov / 2.0), 1.0);
	MATRIX result = createVector(screenPositionX, screenPositionY, 1.0);
	result = multMatrixElementwise(&imagePlaneSize, &result);
	normalizeVector(&result);
	
	return result;
}

// MATRIX getCameraProjectionMatrix(CAMERA *c, struct winsize *viewport) {
// 	MATRIX centeringMatrix = createIdentityMatrix(4);
// 	setElement(&centeringMatrix, 0, 3, viewport->ws_col/2.0);
// 	setElement(&centeringMatrix, 1, 3, viewport->ws_row/2.0);
	
// 	MATRIX depthNormalizationMatrix = createIdentityMatrix(4);
// 	setElement(&centeringMatrix, 2, 2, 2.0*c->maxDistance*c->minDistance / (c->minDistance - c->maxDistance));
// 	setElement(&centeringMatrix, 2, 3, (c->maxDistance + c->minDistance) / (c->maxDistance - c->minDistance));
// 	setElement(&centeringMatrix, 3, 2, -1.0);
// 	setElement(&centeringMatrix, 3, 3, 0.0);

// 	MATRIX projectionMatrix = createIdentityMatrix(4);
// 	setElement(&projectionMatrix, 0, 0, c->minDistance);
// 	setElement(&projectionMatrix, 1, 1, c->minDistance);

// 	MATRIX viewportScalingMatrix = createIdentityMatrix(4);
// 	setElement(&viewportScalingMatrix, 0, 0, 2.0/viewport->ws_col);
// 	setElement(&viewportScalingMatrix, 1, 1, 2.0/viewport->ws_row);

// 	MATRIX result = createIdentityMatrix(4);
// 	result = multMatrix(&centeringMatrix, &result);
// 	result = multMatrix(&depthNormalizationMatrix, &result);
// 	result = multMatrix(&projectionMatrix, &result);
// 	result = multMatrix(&viewportScalingMatrix, &result);

// 	return result;
// }