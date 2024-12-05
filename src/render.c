#include "render.h"
#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *CHARACTERS = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

IMPACT createImpact(MATRIX position, TRIANGLE *impactedTri, double distance, int contained) {
	return (IMPACT) {
		position,
		impactedTri,
		distance,
		contained
	};
}

void updateViewportSize(struct winsize *destination) {
	ioctl(STDIN_FILENO, TIOCGWINSZ, destination);
}

char *renderToString(CAMERA *c, struct winsize *outDimensions, MESH *m) {
	char *output = malloc(sizeof(char) * outDimensions->ws_row * (outDimensions->ws_col + 1) + 1);
	MATRIX cameraPosition = getTranslationVector(&c->transform);

	for (int i = 0; i < outDimensions->ws_row; i++) {
		for (int j = 0; j < outDimensions->ws_col; j++) {
			MATRIX ray = getScreenRay(c, i, j, outDimensions);

			IMPACT *result = getRayMeshImpact(&ray, &cameraPosition, m);

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

	MATRIX meshTransformationMatrix = getTransformMatrix(&m->transform);
	MATRIX inverseMeshTransformationMatrix = inverse(&meshTransformationMatrix);

	MATRIX relativeRay = multMatrix(&inverseMeshTransformationMatrix, ray);
	relativeRay = multMatrix(&m->transform.translation, &relativeRay); // don't translate the direction vector
	MATRIX relativeRayOrigin = multMatrix(&inverseMeshTransformationMatrix, ray);

	for (int i = 0; i < m->tris.size; i++) {
		currentImpact = getRayTriImpact(&relativeRay, &relativeRayOrigin, listGetElement(&m->tris, i));
		if (!currentImpact)
			continue;

		if (!closestImpact && currentImpact->insideTri)
			closestImpact = currentImpact;
		else if (currentImpact->impactDistance < closestImpact->impactDistance && currentImpact->impactDistance > 0.0 && currentImpact->insideTri)
			closestImpact = currentImpact;
		else
			free(currentImpact);
	}

	closestImpact->impactPosition = multMatrix(&meshTransformationMatrix, &closestImpact->impactPosition);

	return closestImpact;
}

IMPACT *getRayTriImpact(MATRIX *ray, MATRIX *rayOrigin, TRIANGLE *tri) {
	// vector || tri normal
	if (vectorDotProduct(ray, tri->normal) == 0.0) return NULL;

	IMPACT *result = malloc(sizeof(IMPACT));

	multScalar(rayOrigin, -1.0);
	MATRIX relativeOrigin = addMatrix(*tri->vertices, &relativeOrigin);
	double distance = vectorDotProduct(tri->normal, &relativeOrigin) / vectorDotProduct(tri->normal, ray);

	MATRIX offset = copyMatrix(ray);
	multScalar(&offset, distance);

	MATRIX impactPoint = addMatrix(rayOrigin, &offset);

	*result = createImpact(impactPoint, tri, distance, 0);

	// check if the point is contained by the triangle
	return result;
}

void clearScreen() {
	printf("\033[2J\033[H");
}