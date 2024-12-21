#include "render.h"
#include "debug.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// default number of threads for multithreading
#define NUM_THREADS 1

const char *CHARACTERS = " .'`^\",:;Il!i><~+_-?[]{}1()tfjrxnuvcXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
// const char *CHARACTERS = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";


// a container for the threading params
struct threadData {
	MATRIX *ray;
	MATRIX *rayOrigin;
	IMPACT **bestImpact;
	MESH *mesh;
	int triStart;
	int triEnd;
};

IMPACT *createImpact(MATRIX *position, TRIANGLE *impactedTri, double distance) {
	IMPACT *i = malloc(sizeof(IMPACT));
	// just set all the fields
	*i = (IMPACT) {
		position,
		impactedTri,
		distance
	};

	return i;
}

void updateViewportSize(struct winsize *destination) {
	// use the standard function with awful syntax
	ioctl(STDIN_FILENO, TIOCGWINSZ, destination);
}

char *renderToString(CAMERA *c, struct winsize *outDimensions, MESH *m) {
	// create the string to hold results
	char *output = malloc(sizeof(char) * outDimensions->ws_row * (outDimensions->ws_col + 1) + 1);

	// also make this more easily accessible
	MATRIX *cameraPosition = getTranslationVector(c->transform);

	// for debugging and profiling
	struct timespec start;
	struct timespec end;

	int numCharacters = strlen(CHARACTERS);

	double rayTime = 0;

	for (int i = 0; i < outDimensions->ws_row; i++) {
		for (int j = 0; j < outDimensions->ws_col; j++) {
			// set timers around the code to profile
			clock_gettime(CLOCK_MONOTONIC_RAW, &start);
			MATRIX *ray = getScreenRay(c, j, i, outDimensions);
			clock_gettime(CLOCK_MONOTONIC_RAW, &end);

			// accumulate whatever we're profiling for
			rayTime += delta_s(start, end);

			// get the impact
			IMPACT *result = getRayMeshImpact(ray, cameraPosition, m);

			// render according to that impact depth
			*(output + i * (outDimensions->ws_col + 1) + j) = (result) ? *(CHARACTERS + (int) ((1 - (result->impactDistance - c->minDistance) / c->maxDistance) * numCharacters) % numCharacters) : ' ';
			// printVector3(&ray);

			freeMatrix(ray);
			free(ray);
			ray = NULL;

			// free it now that we're done with it
			if (result) {
				freeImpact(result);
				free(result);
				result = NULL;
			}
			
		}
		// pick the right line terminator
		*(output + i * (outDimensions->ws_col + 1) + outDimensions->ws_col) = (i == outDimensions->ws_row - 1) ? 0 : '\n';
	}

	// print profiling info
	printf("ray creation time: %lf s\n", rayTime);
	freeMatrix(cameraPosition);
	free(cameraPosition);
	cameraPosition = NULL;

	return output;
}

IMPACT *getRayMeshImpact(MATRIX *ray, MATRIX *rayOrigin, MESH *m) { // maybe add clipping distance
	// init thread stuff
	pthread_t threads[NUM_THREADS];
	struct threadData threadData[NUM_THREADS];
	IMPACT *closestImpact = NULL;
	
	// get the necessary matrices
	MATRIX *meshTransformationMatrix = getTransformMatrix(m->transform);
	MATRIX *inverseMeshTransformationMatrix = getInverse(meshTransformationMatrix);

	// transform the ray but don't rtranslate it (direction only)
	MATRIX *relativeRay = NULL;
	multMatrixTo(inverseMeshTransformationMatrix, ray, &relativeRay);

	multMatrixTo(m->transform->translation, relativeRay, &relativeRay); // don't translate the direction vector

	// transform the origin
	MATRIX *relativeRayOrigin = NULL;
	multMatrixTo(inverseMeshTransformationMatrix, rayOrigin, &relativeRayOrigin);


	// https://ramcdougal.com/threads.html
	int trisPerThread = m->tris.size / NUM_THREADS;

	// build a struct for each thread
	// could decrease the number of fields in the struct, since ray, rayOrigin, bestImpact, and mesh are the same for all threads
	for (int i = 0; i < NUM_THREADS; i++) {
		threadData[i].ray = relativeRay;
		threadData[i].rayOrigin = relativeRayOrigin;
		threadData[i].bestImpact = &closestImpact;
		threadData[i].mesh = m;
		threadData[i].triStart = i * trisPerThread;
		threadData[i].triEnd = (i + 1) * trisPerThread;
		pthread_create(&threads[i], NULL, &getBestRayTriImpact, (void *) &threadData[i]);
	}

	// wait for the threads to finish
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	// untranslate the impact if it happened
	if (closestImpact)
		multMatrixTo(meshTransformationMatrix, closestImpact->impactPosition, &closestImpact->impactPosition);

	// free all the things
	freeMatrix(meshTransformationMatrix);
	freeMatrix(inverseMeshTransformationMatrix);
	freeMatrix(relativeRay);
	freeMatrix(relativeRayOrigin);
	free(meshTransformationMatrix);
	free(inverseMeshTransformationMatrix);
	free(relativeRay);
	free(relativeRayOrigin);
	meshTransformationMatrix = NULL;
	inverseMeshTransformationMatrix = NULL;
	relativeRay = NULL;
	relativeRayOrigin = NULL;

	// return the stuff
	return closestImpact;
}

void *getBestRayTriImpact(void *data) {
	// cast the data to the struct, bc it has to be void for threading i think
	struct threadData *threadData = (struct threadData *) data;
	IMPACT *currentImpact = NULL;

	// only check the specified tris
	for (int i = threadData->triStart; i < threadData->triEnd; i++) {
		currentImpact = getRayTriImpact(threadData->ray, threadData->rayOrigin, listGetElement(&threadData->mesh->tris, i));

		// if it didnt happen ignore the freeing
		if (!currentImpact)
			continue;
		
		// if its closer than the current impact, set the closest impact (which is referenced across all threads)
		if (!*threadData->bestImpact)
			*threadData->bestImpact = currentImpact;
		else if (currentImpact->impactDistance < (*threadData->bestImpact)->impactDistance && currentImpact->impactDistance > 0.0) {
			freeImpact(*threadData->bestImpact);
			free(*threadData->bestImpact);
			*threadData->bestImpact = currentImpact;
		} else {
			freeImpact(currentImpact);
			free(currentImpact);
			currentImpact = NULL;
		}
	}

	return NULL;
}

IMPACT *getRayTriImpact(MATRIX *ray, MATRIX *rayOrigin, TRIANGLE *tri) {
	// the matrix for the system of equations
	MATRIX *reparameterizationSystem = createMatrix(3, 4);
	
	// store all the vectors in a matrix for the linear combination
	for (int i = 0; i < 3; i++) {
		setElement(reparameterizationSystem, i, 0, getElement(tri->vertices[1], i, 0) - getElement(tri->vertices[0], i, 0));
		setElement(reparameterizationSystem, i, 1, getElement(tri->vertices[2], i, 0) - getElement(tri->vertices[0], i, 0));
		setElement(reparameterizationSystem, i, 2, getElement(ray, i, 0));
		setElement(reparameterizationSystem, i, 3, getElement(rayOrigin, i, 0) - getElement(tri->vertices[0], i, 0));
	}

	// solve it using the worst rref function known to man
	rref(reparameterizationSystem);

	// do the math with the system's results
	MATRIX *intersection = copyMatrix(rayOrigin);
	MATRIX *offset = copyMatrix(ray);
	multScalar(offset, -getElement(reparameterizationSystem, 2, 3));
	addMatrix(intersection, offset);

	// free the temp variable
	freeMatrix(offset);
	free(offset);
	offset = NULL;

	// if its oob of the tri, free everything and return null
	if (getElement(reparameterizationSystem, 0, 3) < 0
			|| getElement(reparameterizationSystem, 1, 3) < 0
			|| getElement(reparameterizationSystem, 0, 3) + getElement(reparameterizationSystem, 1, 3) > 1) {

		freeMatrix(reparameterizationSystem);
		freeMatrix(intersection);
		free(reparameterizationSystem);
		free(intersection);
		reparameterizationSystem = NULL;
		intersection = NULL;
		return NULL;
	}
	// otherwise make the impact struct
	IMPACT *impact = createImpact(
		intersection,
		tri,
		getElement(reparameterizationSystem, 2, 3)
	);

	// free temp vars
	freeMatrix(reparameterizationSystem);
	free(reparameterizationSystem);
	reparameterizationSystem = NULL;
	
	// and return
	return impact;
}

void clearScreen() {
	// just print the ansi
	printf("\033[2J\033[H");
}

void freeImpact(IMPACT *i) {
	freeMatrix(i->impactPosition);
	free(i->impactPosition);
	i->impactPosition = NULL;

	// dont free the tri
	i->impactTri = NULL;
}

void printImpact(IMPACT *i) {
	if (!i) {
		// tell us if its null
		printf("There was no impact\n");
		return;
	}
	// otherwise give distance and position
	printf("An impact occured %lf units away, at\n", i->impactDistance);
	printVector3(i->impactPosition);
	printf("\n");
}