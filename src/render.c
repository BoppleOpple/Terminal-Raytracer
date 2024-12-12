#include "render.h"
#include "mathUtils.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define NUM_THREADS 16

const char *CHARACTERS = " .'`^\",:;Il!i><~+_-?[]{}1()tfjrxnuvcXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
// const char *CHARACTERS = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

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
	*i = (IMPACT) {
		position,
		impactedTri,
		distance
	};

	return i;
}

void updateViewportSize(struct winsize *destination) {
	ioctl(STDIN_FILENO, TIOCGWINSZ, destination);
}

char *renderToString(CAMERA *c, struct winsize *outDimensions, MESH *m) {
	char *output = malloc(sizeof(char) * outDimensions->ws_row * (outDimensions->ws_col + 1) + 1);
	MATRIX *cameraPosition = getTranslationVector(c->transform);

	int numCharacters = strlen(CHARACTERS);

	for (int i = 0; i < outDimensions->ws_row; i++) {
		for (int j = 0; j < outDimensions->ws_col; j++) {
			MATRIX *ray = getScreenRay(c, j, i, outDimensions);

			IMPACT *result = getRayMeshImpact(ray, cameraPosition, m);

			*(output + i * (outDimensions->ws_col + 1) + j) = (result) ? *(CHARACTERS + (int) ((1 - (result->impactDistance - c->minDistance) / c->maxDistance) * numCharacters) % numCharacters) : ' ';
			// printVector3(&ray);

			freeMatrix(ray);
			free(ray);
			ray = NULL;
			if (result) {
				freeImpact(result);
				free(result);
				result = NULL;
			}
		}
		*(output + i * (outDimensions->ws_col + 1) + outDimensions->ws_col) = (i == outDimensions->ws_row - 1) ? 0 : '\n';
	}

	freeMatrix(cameraPosition);
	free(cameraPosition);
	cameraPosition = NULL;

	return output;
}

IMPACT *getRayMeshImpact(MATRIX *ray, MATRIX *rayOrigin, MESH *m) { // maybe add clipping distance
	pthread_t threads[NUM_THREADS];
	IMPACT *closestImpact = NULL;

	MATRIX *meshTransformationMatrix = getTransformMatrix(m->transform);
	MATRIX *inverseMeshTransformationMatrix = getInverse(meshTransformationMatrix);

	MATRIX *relativeRay = NULL;
	multMatrixTo(inverseMeshTransformationMatrix, ray, &relativeRay);

	multMatrixTo(m->transform->translation, relativeRay, &relativeRay); // don't translate the direction vector

	MATRIX *relativeRayOrigin = NULL;
	multMatrixTo(inverseMeshTransformationMatrix, rayOrigin, &relativeRayOrigin);


	// https://ramcdougal.com/threads.html
	int trisPerThread = m->tris.size / NUM_THREADS;

	for (int i = 0; i < NUM_THREADS; i++) {
		struct threadData *data = malloc(sizeof(struct threadData));
		data->ray = relativeRay;
		data->rayOrigin = relativeRayOrigin;
		data->bestImpact = &closestImpact;
		data->mesh = m;
		data->triStart = i * trisPerThread;
		data->triEnd = (i + 1) * trisPerThread;
		pthread_create(&threads[i], NULL, &getBestRayTriImpact, (void *) data);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	if (closestImpact)
		multMatrixTo(meshTransformationMatrix, closestImpact->impactPosition, &closestImpact->impactPosition);

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


	return closestImpact;
}

void *getBestRayTriImpact(void *data) {
	struct threadData *threadData = (struct threadData *) data;
	IMPACT *currentImpact = NULL;

	for (int i = threadData->triStart; i < threadData->triEnd; i++) {
		currentImpact = getRayTriImpact(threadData->ray, threadData->rayOrigin, listGetElement(&threadData->mesh->tris, i));

		if (!currentImpact)
			continue;

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
	MATRIX *reparameterizationSystem = createMatrix(3, 4);

	for (int i = 0; i < 3; i++) {
		setElement(reparameterizationSystem, i, 0, getElement(tri->vertices[1], i, 0) - getElement(tri->vertices[0], i, 0));
		setElement(reparameterizationSystem, i, 1, getElement(tri->vertices[2], i, 0) - getElement(tri->vertices[0], i, 0));
		setElement(reparameterizationSystem, i, 2, getElement(ray, i, 0));
		setElement(reparameterizationSystem, i, 3, getElement(rayOrigin, i, 0) - getElement(tri->vertices[0], i, 0));
	}

	rref(reparameterizationSystem);

	MATRIX *intersection = copyMatrix(rayOrigin);
	MATRIX *offset = copyMatrix(ray);
	multScalar(offset, -getElement(reparameterizationSystem, 2, 3));
	addMatrix(intersection, offset);

	freeMatrix(offset);
	free(offset);
	offset = NULL;

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
	
	IMPACT *impact = createImpact(
		intersection,
		tri,
		getElement(reparameterizationSystem, 2, 3)
	);

	freeMatrix(reparameterizationSystem);
	free(reparameterizationSystem);
	reparameterizationSystem = NULL;
	
	return impact;
}

void clearScreen() {
	printf("\033[2J\033[H");
}

void freeImpact(IMPACT *i) {
	freeMatrix(i->impactPosition);
	free(i->impactPosition);
	i->impactPosition = NULL;
	i->impactTri = NULL;
}

void printImpact(IMPACT *i) {
	if (!i) {
		printf("There was no impact\n");
		return;
	}
	printf("An impact occured %lf units away, at\n", i->impactDistance);
	printVector3(i->impactPosition);
	printf("\n");
}