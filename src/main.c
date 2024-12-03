#include "camera.h"
#include "mathUtils.h"
#include "matrix.h"
#include "mesh.h"
#include "render.h"
#include "transform.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h> // for sleep on linux

#define FRAME_RATE 60

const unsigned int targetMicroSeconds = 1000000 / FRAME_RATE;

int main(int argc, char *argv[]) {
	CAMERA sceneCamera = createCamera(1.0);
	struct winsize windowDims;
	struct timespec frameStart, frameEnd;
	char *filepath = NULL;
	int frame = 0;
	int loop = 0;

	TRANSFORM testTransform = createTransform();
	TRANSFORM testTransform2 = createTransform();
	MATRIX testMatrix = createMatrix(3, 3);
	MATRIX transformMatrix = getTransformMatrix(&testTransform);
	MATRIX testVector = createVector(1.0, 1.0, 1.0);

	setElement(&testMatrix, 0, 0, 1);
	setElement(&testMatrix, 0, 1, 2);
	setElement(&testMatrix, 0, 2, 3);
	setElement(&testMatrix, 1, 0, 1);
	setElement(&testMatrix, 1, 1, 3);
	setElement(&testMatrix, 1, 2, 5);
	setElement(&testMatrix, 2, 0, 1);
	setElement(&testMatrix, 2, 1, 4);
	setElement(&testMatrix, 2, 2, 8);

	printf("test matrix:\n");
	printMatrix(&testMatrix);
	printf("determinant:\n");
	printf("%lf\n", determinant(&testMatrix));
	printf("inverse:\n");
	testMatrix = inverse(&testMatrix);
	printMatrix(&testMatrix);
	printf("\n");

	translate(&testTransform, createVector(3.0, 2.0, 1.0));
	scale(&testTransform, createVector(3.0, 2.0, 1.0));
	transformMatrix = getTransformMatrix(&testTransform);
	printf("matrix 1:\n");
	printMatrix(&transformMatrix);
	printf("\n");

	translate(&testTransform2, createVector(-5.0, 1.0, -2.0));
	scale(&testTransform2, createVector(2.0, 2.0, 2.0));
	rotate(&testTransform2, createVector(0.0, 0.0, PI/2));
	transformMatrix = getTransformMatrix(&testTransform2);
	printf("matrix 2:\n");
	printMatrix(&transformMatrix);
	printf("\n");

	TRANSFORM combination = combine(&testTransform, &testTransform2);
	transformMatrix = getTransformMatrix(&combination);
	printf("combination:\n");
	printMatrix(&transformMatrix);
	printf("\n");

	printf("transforms this vector:\n");
	printVector3(&testVector);
	testVector = applyTransformation(&combination, &testVector);
	printf("into this vector:\n");
	printVector3(&testVector);
	printf("\n");

	printf("has a rotation of:\n");
	testVector = getRotationVector(&combination);
	printVector3(&testVector);
	printf("\n");

	if (argc < 2) {
		printf("please pass a mesh (.obj) file.\n");
		return 1;
	}
	filepath = argv[1];

	MESH testMesh = meshFromOBJ(filepath);
	printMesh(&testMesh);

	while (loop) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &frameStart);

		// do things!
		updateViewportSize(&windowDims);
		clearScreen();

		printf("frame %i\n", frame++);
		printf("display: %i x %i\n", windowDims.ws_col, windowDims.ws_row);
		// stop doing things!

		clock_gettime(CLOCK_MONOTONIC_RAW, &frameEnd);

		unsigned int deltaMicroSeconds = (frameEnd.tv_sec - frameStart.tv_sec) * 1000000 + (frameEnd.tv_nsec - frameStart.tv_nsec) / 1000;

		printf("target: %u\n", targetMicroSeconds);
		printf("delta: %u\n", deltaMicroSeconds);

		if (deltaMicroSeconds < targetMicroSeconds) 
			usleep(targetMicroSeconds - deltaMicroSeconds);
	}
}