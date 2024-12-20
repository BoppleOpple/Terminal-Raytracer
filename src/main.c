#include "camera.h"
#include "debug.h"
#include "mathUtils.h"
#include "matrix.h"
#include "mesh.h"
#include "render.h"
#include "stringUtils.h"
#include "transform.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h> // for sleep on linux

#define MAX_FRAME_RATE 5

const unsigned int targetMicroSeconds = 1000000 / MAX_FRAME_RATE;

/**
 * @brief test a bunch of my functions
 */
void test() {
	MATRIX *testVector1 = createVector(1.0, 1.0, 0.0);
	MATRIX *testVector2 = createVector(2.0, 2.0, 0.0);

	printf("vector a:\n");
	printVector3(testVector1);
	printf("vector b:\n");
	printVector3(testVector2);

	// test cross product
	printf("vector a x b:\n");
	MATRIX *result = vectorCrossProduct(testVector1, testVector2);
	printVector3(result);

	// test dot product
	printf("vector a * b:\n");
	double dotResult = vectorDotProduct(testVector1, testVector2);
	printf("%lf\n", dotResult);
	printf("\n");

	// free tests
	freeMatrix(testVector1);
	freeMatrix(testVector2);
	freeMatrix(result);
	free(testVector1);
	free(testVector2);
	free(result);
	testVector1 = NULL;
	testVector2 = NULL;
	result = NULL;


	// test impact math (old now)
	MATRIX *ray = createVector(0.0, 0.0, 1.0);
	MATRIX *rayOrigin = createVector(0.0, 0.0, -3.0);
	MATRIX *normal = createVector(0.0, 0.0, -1.0);
	MATRIX *normalOrigin = createVector(0.0, 0.0, 0.0);

	// IMPACT *impact = getRayPlaneImpact(ray, rayOrigin, normal, normalOrigin);
	// printImpact(impact);

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

	// freeImpact(impact);
	// free(impact);
	// impact = NULL;

	printf("\n");

	// test matrix reduction with a non-singular matrix
	MATRIX *reducableMatrix = createMatrix(2, 3);
	setElement(reducableMatrix, 0, 0, 0.0);
	setElement(reducableMatrix, 0, 1, 4.0);
	setElement(reducableMatrix, 0, 2, 6.0);
	setElement(reducableMatrix, 1, 0, 6.0);
	setElement(reducableMatrix, 1, 1, 1.0);
	setElement(reducableMatrix, 1, 2, 2.0);

	printf("reducable matrix:\n");
	printMatrix(reducableMatrix);

	rref(reducableMatrix);

	printf("reduced matrix:\n");
	printMatrix(reducableMatrix);

	freeMatrix(reducableMatrix);
	free(reducableMatrix);
	reducableMatrix = NULL;


	// test string splitting
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

	listClear(&wordList);
	listClear(&splitAtT);
	free(first5);
	first5 = NULL;

}

/**
 * @brief Main function
 *
 * Handles i/o and rendering loop, takes a path to a .obj file
 */
int main(int argc, char *argv[]) {
	// initialize all the stuff, could be defines for a bunch of these but oh well
	// maybe i'll do that later actually (TODO)

	CAMERA *sceneCamera = createCamera(1.0, 6.0, PI / 2.0);
	FILE *frameTimeFile;
	FILE *frameFile;
	LIST frameTimes = listCreate();
	struct winsize windowDims;
	struct timespec frameStart, frameEnd;
	unsigned int *deltaMicroSeconds;
	char *filepath = NULL;
	char *frameTimeCSVPath = "./output/frameTimes.csv";
	char *frameOutputFormat = "./output/frame_%i.txt";
	int frame = 0;
	int numFrames = 10;
	int loop = 1;
	int debug = 0;
	int saveTimes = 0;
	int saveFrames = 1;

	// initialize the camera at not the origin

	translateXYZ(sceneCamera->transform, -3.0, 0.0, 0.0);

	// run "tests" (these are barely tests :P)
	test();

	// check for the mesh file
	if (argc < 2) {
		printf("please pass a mesh (.obj) file.\n");
		return 1;
	}
	filepath = argv[1];


	// initialize a mesh rotated somehow, depending on the mesh
	MESH *renderedMesh = meshFromOBJ(filepath);
	rotateXYZ(renderedMesh->transform, -PI/2.0, 0.0, PI);
	// printMesh(testMesh);
	

	// rendering loop
	while (loop) {
		// get frameTime for masx framerate
		clock_gettime(CLOCK_MONOTONIC_RAW, &frameStart);

		// do things!
		updateViewportSize(&windowDims);


		// do the actual rendering
		clearScreen();

		// spin a bit each frame
		rotateXYZ(renderedMesh->transform, 0.0, 0.1, 0.0);

		// print to screen
		char *screenString = renderToString(sceneCamera, &windowDims, renderedMesh);
		printf("%s", screenString);

		// save to file
		if (saveFrames) {
			char frameOutputPath[1024];
			sprintf(frameOutputPath, frameOutputFormat, frame);
			frameFile = fopen(frameOutputPath, "w");
			fprintf(frameFile, "%s", screenString);
		}

		// clean up a bit
		free(screenString);
		screenString = NULL;

		if (debug) {
			printf("frame %i\n", frame);
			printf("display: %i x %i\n", windowDims.ws_col, windowDims.ws_row);
		}
		// stop doing things!

		clock_gettime(CLOCK_MONOTONIC_RAW, &frameEnd);

		// calculate deltaTime
		deltaMicroSeconds = malloc(sizeof(unsigned int));

		*deltaMicroSeconds = delta_us(frameStart, frameEnd);
		if (debug) {
			printf("target: %u\n", targetMicroSeconds);
			printf("delta: %u\n", *deltaMicroSeconds);
		}

		listAppendItem(&frameTimes, deltaMicroSeconds);

		if (numFrames != -1 && frame >= numFrames)
			loop = 0;

		if (*deltaMicroSeconds < targetMicroSeconds) 
			usleep(targetMicroSeconds - *deltaMicroSeconds);

		free(deltaMicroSeconds);
		frame++;
	}

	// save frame times for debugging
	if (saveTimes) {
		frameTimeFile = fopen(frameTimeCSVPath, "a");
		for (int i = 0; i < frameTimes.size; i++)
			fprintf(frameTimeFile, (i < frameTimes.size - 1) ? "%u, " : "%u\n", *((unsigned int*) listGetElement(&frameTimes, i)));
	}

	// clear memory
	listClear(&frameTimes);

	freeMesh(renderedMesh);
	free(renderedMesh);
	renderedMesh = NULL;

	freeCamera(sceneCamera);
	free(sceneCamera);
	sceneCamera = NULL;

	// leave
	return 0;
}
