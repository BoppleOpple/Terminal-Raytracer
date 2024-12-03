#include "camera.h"
#include "matrix.h"

CAMERA createCamera(double focalLength) {
	return (CAMERA) {
		focalLength,
		createTransform()
	};
}

MATRIX getCameraProjectionMatrix(CAMERA *c) {
	// matrix from 2d homogeneous to 3d homogeneous
	MATRIX projectionMatrix = createMatrix(3, 4);
	setElement(&projectionMatrix, 0, 0, c->focalLength);
	setElement(&projectionMatrix, 1, 1, c->focalLength);
	setElement(&projectionMatrix, 2, 2, 1.0);
	return projectionMatrix;
}