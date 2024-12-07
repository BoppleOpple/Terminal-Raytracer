#include "render.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *CHARACTERS = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

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

	for (int i = 0; i < outDimensions->ws_row; i++) {
		for (int j = 0; j < outDimensions->ws_col; j++) {
			MATRIX *ray = getScreenRay(c, j, i, outDimensions);

			IMPACT *result = getRayMeshImpact(ray, cameraPosition, m);

			*(output + i * (outDimensions->ws_col + 1) + j) = (result) ? '0' : ' ';
			// printVector3(&ray);
		}
		*(output + i * (outDimensions->ws_col + 1) + outDimensions->ws_col) = (i == outDimensions->ws_row - 1) ? 0 : '\n';
	}

	return output;
}

IMPACT *getRayMeshImpact(MATRIX *ray, MATRIX *rayOrigin, MESH *m) { // maybe add clipping distance
	IMPACT *closestImpact = NULL;
	IMPACT *currentImpact = NULL;

	MATRIX *meshTransformationMatrix = getTransformMatrix(m->transform);
	MATRIX *inverseMeshTransformationMatrix = getInverse(meshTransformationMatrix);

	MATRIX *relativeRay = NULL;
	multMatrixTo(inverseMeshTransformationMatrix, ray, &relativeRay);

	multMatrixTo(m->transform->translation, relativeRay, &relativeRay); // don't translate the direction vector

	MATRIX *relativeRayOrigin = NULL;
	multMatrixTo(inverseMeshTransformationMatrix, rayOrigin, &relativeRayOrigin);

	for (int i = 0; i < m->tris.size; i++) {
		currentImpact = getRayTriImpact(relativeRay, relativeRayOrigin, listGetElement(&m->tris, i));

		if (!currentImpact)
			continue;

		if (!closestImpact) {
			closestImpact = currentImpact;
		} else if (currentImpact->impactDistance < closestImpact->impactDistance && currentImpact->impactDistance > 0.0) {
			freeImpact(closestImpact);
			free(closestImpact);
			closestImpact = currentImpact;
		} else {
			freeImpact(currentImpact);
			free(currentImpact);
		}
	}

	if (closestImpact)
		multMatrixTo(meshTransformationMatrix, closestImpact->impactPosition,&closestImpact->impactPosition);

	return closestImpact;
}

IMPACT *getRayTriImpact(MATRIX *ray, MATRIX *rayOrigin, TRIANGLE *tri) {

	IMPACT *result = getRayPlaneImpact(ray, rayOrigin, tri->normal, tri->vertices[0]);

	result->impactTri = tri;

	MATRIX *negativeVertex1 = copyMatrix(tri->vertices[0]);
	MATRIX *negativeVertex2 = copyMatrix(tri->vertices[1]);
	MATRIX *negativeVertex3 = copyMatrix(tri->vertices[2]);
	multScalar(negativeVertex1, -1.0);
	multScalar(negativeVertex2, -1.0);
	multScalar(negativeVertex3, -1.0);
	
	MATRIX *impactOffset1 = copyMatrix(negativeVertex1);
	addMatrix(impactOffset1, result->impactPosition);

	MATRIX *impactOffset2 = copyMatrix(negativeVertex2);
	addMatrix(impactOffset2, result->impactPosition);

	MATRIX *impactOffset3 = copyMatrix(negativeVertex3);
	addMatrix(impactOffset3, result->impactPosition);

	MATRIX *vertexOffset2 = copyMatrix(negativeVertex1);
	addMatrix(vertexOffset2, tri->vertices[1]);

	MATRIX *vertexOffset3 = copyMatrix(negativeVertex1);
	addMatrix(vertexOffset3, tri->vertices[2]);

	if (vectorDotProduct(impactOffset1, vertexOffset2) < 0.0) return NULL;
	if (vectorDotProduct(impactOffset1, vertexOffset3) < 0.0) return NULL;

	multScalar(vertexOffset2, -1.0);
	multScalar(vertexOffset3, -1.0);

	if (vectorDotProduct(impactOffset2, vertexOffset2) < 0.0) return NULL;
	if (vectorDotProduct(impactOffset3, vertexOffset3) < 0.0) return NULL;

	return result;
}

IMPACT *getRayPlaneImpact(MATRIX *ray, MATRIX *rayOrigin, MATRIX *normal, MATRIX *normalOrigin) {
	// vector || tri normal
	double rayDotNormal = vectorDotProduct(ray, normal);
	if (rayDotNormal == 0.0) return NULL;

	MATRIX *relativeOrigin = copyMatrix(rayOrigin);
	multScalar(relativeOrigin, -1.0);
	addMatrix(relativeOrigin, normalOrigin);

	double distance = vectorDotProduct(normal, relativeOrigin) / rayDotNormal;

	MATRIX *impactPoint = copyMatrix(ray);
	multScalar(impactPoint, distance);

	addMatrix(impactPoint, rayOrigin);


	return createImpact(impactPoint, NULL, distance);
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