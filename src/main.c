#include "camera.h"
#include "matrix.h"
#include "mesh.h"
#include "render.h"
#include "stringUtils.h"
#include "transform.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h> // for sleep on linux

#define FRAME_RATE 20

const unsigned int targetMicroSeconds = 1000000 / FRAME_RATE;

int main(int argc, char *argv[]) {
	CAMERA *sceneCamera = createCamera(1.0);
	struct winsize windowDims;
	struct timespec frameStart, frameEnd;
	char *filepath = NULL;
	int frame = 0;
	int loop = 1;
	int debug = 0;

	translate(sceneCamera->transform, createVector(-2.0, 0.0, 0.0));

	MATRIX *testVector1 = createVector(1.0, 1.0, 0.0);
	MATRIX *testVector2 = createVector(2.0, 2.0, 0.0);

	printf("vector a:\n");
	printVector3(testVector1);
	printf("vector b:\n");
	printVector3(testVector2);
	printf("vector a x b:\n");
	MATRIX *result = vectorCrossProduct(testVector1, testVector2);
	printVector3(result);
	printf("vector a * b:\n");
	double dotResult = vectorDotProduct(testVector1, testVector2);
	printf("%lf\n", dotResult);
	printf("\n");

	freeMatrix(testVector1);
	freeMatrix(testVector2);
	freeMatrix(result);
	free(testVector1);
	free(testVector2);
	free(result);
	testVector1 = NULL;
	testVector2 = NULL;
	result = NULL;

	MATRIX *ray = createVector(0.0, 0.0, 1.0);
	MATRIX *rayOrigin = createVector(0.0, 0.0, -3.0);
	MATRIX *normal = createVector(0.0, 0.0, -1.0);
	MATRIX *normalOrigin = createVector(0.0, 0.0, 0.0);

	IMPACT *impact = getRayPlaneImpact(ray, rayOrigin, normal, normalOrigin);
	printImpact(impact);

	freeMatrix(ray);
	freeMatrix(rayOrigin);
	freeMatrix(normal);
	freeMatrix(normalOrigin);
	free(ray);
	free(rayOrigin);
	free(normal);
	free(normalOrigin);
	ray = NULL;
	rayOrigin = NULL;
	normal = NULL;
	normalOrigin = NULL;

	freeImpact(impact);
	free(impact);
	impact = NULL;

	if (argc < 2) {
		printf("please pass a mesh (.obj) file.\n");
		return 1;
	}
	filepath = argv[1];

	char example[] = "this is a test string.";

	char *first5 = slice(example, 0, 5);

	printf("Test string:\n");
	printf("%s\n", example);
	printf("Truncated:\n");
	printf("%s\n", first5);
	printf("\n");

	LIST wordList = splitSpaces(example);
	listPrint(&wordList, "\"%s\"");

	LIST splitAtT = split(example, 't');
	listPrint(&splitAtT, "\"%s\"");



	MESH *testMesh = meshFromOBJ(filepath);
	// rotate(&testMesh.transform, createVector(0, 0, PI));
	// printMesh(&testMesh);

	while (loop) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &frameStart);

		// do things!
		updateViewportSize(&windowDims);

		rotate(testMesh->transform, createVector(0.0, 0.0, 0.1));

		clearScreen();
		printf("%s", renderToString(sceneCamera, &windowDims, testMesh));


		if (debug) {
			printf("frame %i\n", frame++);
			printf("display: %i x %i\n", windowDims.ws_col, windowDims.ws_row);
		}
		// stop doing things!

		clock_gettime(CLOCK_MONOTONIC_RAW, &frameEnd);

		unsigned int deltaMicroSeconds = (frameEnd.tv_sec - frameStart.tv_sec) * 1000000 + (frameEnd.tv_nsec - frameStart.tv_nsec) / 1000;
		if (debug) {
			printf("target: %u\n", targetMicroSeconds);
			printf("delta: %u\n", deltaMicroSeconds);
		}

		if (deltaMicroSeconds < targetMicroSeconds) 
			usleep(targetMicroSeconds - deltaMicroSeconds);
	}
}