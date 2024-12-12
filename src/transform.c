#include "transform.h"
#include "mathUtils.h"
#include "matrix.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

TRANSFORM *createTransform() {
	TRANSFORM *t = malloc(sizeof(TRANSFORM));
	*t = (TRANSFORM) {
		createIdentityMatrix(4),
		createIdentityMatrix(4),
		createIdentityMatrix(4)
	};
	return t;
}

MATRIX *createTranslationMatrix(MATRIX *posVector) {
	MATRIX *m = createIdentityMatrix(4);
	for (int i = 0; i < posVector->rows; i++)
		setElement(m, i, 3, getElement(posVector, i, 0));
	return m;
}

MATRIX *createScaleMatrix(MATRIX *scaleVector) {
	MATRIX *m = createIdentityMatrix(4);
	for (int i = 0; i < scaleVector->rows; i++)
		setElement(m, i, i, getElement(scaleVector, i, 0));
	return m;
}

MATRIX *createRotationMatrixX(double xRot) {
	MATRIX *result = createIdentityMatrix(4);
	double sinAngle = sin(xRot);
	double cosAngle = cos(xRot);
	setElement(result, 1, 1,  cosAngle);
	setElement(result, 1, 2, -sinAngle);
	setElement(result, 2, 1,  sinAngle);
	setElement(result, 2, 2,  cosAngle);
	return result;
}

MATRIX *createRotationMatrixY(double yRot) {
	MATRIX *result = createIdentityMatrix(4);
	double sinAngle = sin(yRot);
	double cosAngle = cos(yRot);
	setElement(result, 0, 0,  cosAngle);
	setElement(result, 0, 2, -sinAngle);
	setElement(result, 2, 0,  sinAngle);
	setElement(result, 2, 2,  cosAngle);
	return result;
}

MATRIX *createRotationMatrixZ(double zRot) {
	MATRIX *result = createIdentityMatrix(4);
	double sinAngle = sin(zRot);
	double cosAngle = cos(zRot);
	setElement(result, 0, 0,  cosAngle);
	setElement(result, 0, 1, -sinAngle);
	setElement(result, 1, 0,  sinAngle);
	setElement(result, 1, 1,  cosAngle);
	return result;
}

MATRIX *createRotationMatrix(MATRIX *rotVector) {
	MATRIX *result = createRotationMatrixZ(getElement(rotVector, 2, 0));

	MATRIX *next = createRotationMatrixY(getElement(rotVector, 1, 0));
	multMatrix(result, next);
	freeMatrix(next);
	free(next);
	next = NULL;

	next = createRotationMatrixX(getElement(rotVector, 0, 0));
	multMatrix(result, next);
	freeMatrix(next);
	free(next);
	next = NULL;

	return result;
}

void setTranslation(TRANSFORM *t, MATRIX *posVector) {
	if (t->translation) {
		freeMatrix(t->translation);
		free(t->translation);
	}
	t->translation = createTranslationMatrix(posVector);
}

void setScale(TRANSFORM *t, MATRIX *scaleVector) {
	if (t->scale) {
		freeMatrix(t->scale);
		free(t->scale);
	}
	t->scale = createScaleMatrix(scaleVector);
}

void setRotation(TRANSFORM *t, MATRIX *rotVector) {
	t->rotation = createRotationMatrix(rotVector);
}

MATRIX *getTranslationVector(TRANSFORM *t) {
	return createVector(
		getElement(t->translation, 0, 3),
		getElement(t->translation, 1, 3),
		getElement(t->translation, 2, 3)
	);
}

MATRIX *getScaleVector(TRANSFORM *t) {
	return createVector(
		getElement(t->translation, 0, 0),
		getElement(t->translation, 1, 1),
		getElement(t->translation, 2, 2)
	);
}

MATRIX *getRotationVector(TRANSFORM *t) {
	double sinYRot = -getElement(t->rotation, 2, 0);
	printMatrix(t->rotation);
	if (sinYRot == 1.0) {
		return createVector(
			atan2(getElement(t->rotation, 0, 1), getElement(t->rotation, 0, 2)),
			PI/2,
			0.0
		);
	} else if (sinYRot == -1.0) {
		return createVector(
			atan2(-getElement(t->rotation, 0, 1), -getElement(t->rotation, 0, 2)),
			PI/2,
			0.0
		);
	} else {
		double yRot = asin(sinYRot);
		double cosYRot = cos(yRot);
		printf("%lf\n", cosYRot);
		return createVector(
			atan2(getElement(t->rotation, 2, 1)/cosYRot, getElement(t->rotation, 2, 2)/cosYRot),
			yRot,
			atan2(getElement(t->rotation, 1, 0)/cosYRot, getElement(t->rotation, 0, 0)/cosYRot));
	}
}

void translate(TRANSFORM *t, MATRIX *posVector) {
	MATRIX *translationMatrix = createTranslationMatrix(posVector);
	multMatrix(t->translation, translationMatrix);
}

void translateXYZ(TRANSFORM *t, double x, double y, double z) {
	MATRIX *vector = createVector(x, y, z);
	translate(t, vector);
	freeMatrix(vector);
	free(vector);
}

void scale(TRANSFORM *t, MATRIX *scaleVector) {
	MATRIX *scaleMatrix = createScaleMatrix(scaleVector);
	multMatrix(t->scale, scaleMatrix);
}

void scaleXYZ(TRANSFORM *t, double x, double y, double z) {
	MATRIX *vector = createVector(x, y, z);
	scale(t, vector);
	freeMatrix(vector);
	free(vector);
}

void rotate(TRANSFORM *t, MATRIX *rotVector) {
	MATRIX *rotationMatrix = createRotationMatrix(rotVector);
	multMatrix(t->rotation, rotationMatrix);
}

void rotateXYZ(TRANSFORM *t, double x, double y, double z) {
	MATRIX *vector = createVector(x, y, z);
	rotate(t, vector);
	freeMatrix(vector);
	free(vector);
}

MATRIX *getTransformMatrix(TRANSFORM *t) {
	MATRIX *combination = NULL;
	// because translation is leftmost, it will be last when this matrix is multiplied to the left of a vector
	multMatrixTo(t->translation, t->rotation, &combination);
	multMatrix(combination, t->scale);
	return combination;
}

/**
 * @brief this no longer sucks major ass
 *
 * eh aaaaaaaaaaaaaeeeeeeeeeaeae
 * aaaaaaaaaaaaaeeeeeeeeeaeaeëëëë
 *
 * @TODO
 * @param t1 
 * @param t2 
 * @return TRANSFORM 
 */
TRANSFORM *combine(TRANSFORM *t1, TRANSFORM *t2) {
	TRANSFORM *combination = createTransform();

	multMatrixTo(t1->translation, t2->translation, &combination->translation);
	multMatrixTo(t1->scale, t2->scale, &combination->scale);
	multMatrixTo(t1->rotation, t2->rotation, &combination->rotation);

	return combination;
}

void applyTransformation(TRANSFORM *t, MATRIX *v) {
	MATRIX *transformMatrix = getTransformMatrix(t);
	return multMatrixTo(transformMatrix, v, &v);
}

void printTransform(TRANSFORM *t) {
	printf("translation:\n");
	printMatrix(t->translation);
	printf("scale:\n");
	printMatrix(t->scale);
	printf("rotation:\n");
	printMatrix(t->rotation);
}

void freeTransform(TRANSFORM *t) {
	freeMatrix(t->translation);
	freeMatrix(t->scale);
	freeMatrix(t->rotation);
	t->translation = NULL;
	t->scale = NULL;
	t->rotation = NULL;
}