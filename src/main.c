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
	struct winsize windowDims;
	struct timespec frameStart, frameEnd;
	char *filepath = NULL;
	int frame = 0;
	int loop = 0;

	TRANSFORM testTransform = createTransform();

	printTransform(&testTransform);
	translate(&testTransform, createVector(3.0, 2.0, 1.0));
	scale(&testTransform, createVector(3.0, 2.0, 1.0));
	printTransform(&testTransform);

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