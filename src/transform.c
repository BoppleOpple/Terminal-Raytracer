#include "transform.h"
#include "mathUtils.h"
#include "matrix.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

TRANSFORM *createTransform() {
	TRANSFORM *t = malloc(sizeof(TRANSFORM));

	// all matrices will be identities by default
	*t = (TRANSFORM) {
		createIdentityMatrix(4),
		createIdentityMatrix(4),
		createIdentityMatrix(4)
	};
	return t;
}

MATRIX *createTranslationMatrix(MATRIX *posVector) {
	MATRIX *m = createIdentityMatrix(4);

	// translation matrix info goes on the right column
	for (int i = 0; i < posVector->rows; i++)
		setElement(m, i, 3, getElement(posVector, i, 0));
	return m;
}

MATRIX *createScaleMatrix(MATRIX *scaleVector) {
	MATRIX *m = createIdentityMatrix(4);

	// scale matrix info goes on the diagonal
	for (int i = 0; i < scaleVector->rows; i++)
		setElement(m, i, i, getElement(scaleVector, i, 0));
	return m;
}

MATRIX *createRotationMatrixX(double xRot) {
	MATRIX *result = createIdentityMatrix(4);
	double sinAngle = sin(xRot);
	double cosAngle = cos(xRot);

	// rotation in x sets values in y and z columns
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

	// rotation in y sets values in yxand z columns
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

	// rotation in z sets values in x and y columns
	setElement(result, 0, 0,  cosAngle);
	setElement(result, 0, 1, -sinAngle);
	setElement(result, 1, 0,  sinAngle);
	setElement(result, 1, 1,  cosAngle);
	return result;
}

MATRIX *createRotationMatrix(MATRIX *rotVector) {
	// creates a rotation matrix by combining rotation in x, y, and z
	// M_z * M_y * M_x

	MATRIX *result = createRotationMatrixZ(getElement(rotVector, 2, 0));

	MATRIX *next = createRotationMatrixY(getElement(rotVector, 1, 0));
	multMatrix(result, next);

	// free stuff as we go
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
	// free existing translation
	if (t->translation) {
		freeMatrix(t->translation);
		free(t->translation);
	}

	// set a new one
	t->translation = createTranslationMatrix(posVector);
}

void setScale(TRANSFORM *t, MATRIX *scaleVector) {
	// free existing scale
	if (t->scale) {
		freeMatrix(t->scale);
		free(t->scale);
	}

	// set a new one
	t->scale = createScaleMatrix(scaleVector);
}

void setRotation(TRANSFORM *t, MATRIX *rotVector) {
	// free existing rotation
	if (t->rotation) {
		freeMatrix(t->rotation);
		free(t->rotation);
	}

	// set a new one
	t->rotation = createRotationMatrix(rotVector);
}

MATRIX *getTranslationVector(TRANSFORM *t) {
	// just read from specified positions in translation matrix
	return createVector(
		getElement(t->translation, 0, 3),
		getElement(t->translation, 1, 3),
		getElement(t->translation, 2, 3)
	);
}

MATRIX *getScaleVector(TRANSFORM *t) {
	// just read from specified positions in scale
	return createVector(
		getElement(t->translation, 0, 0),
		getElement(t->translation, 1, 1),
		getElement(t->translation, 2, 2)
	);
}

MATRIX *getRotationVector(TRANSFORM *t) {
	// do a bunch of math that i have on paper somewhere
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
	// create and combine translation matrix
	MATRIX *translationMatrix = createTranslationMatrix(posVector);
	multMatrix(t->translation, translationMatrix);

	// free it
	freeMatrix(translationMatrix);
	free(translationMatrix);
	translationMatrix = NULL;
}

void translateXYZ(TRANSFORM *t, double x, double y, double z) {
	// just call the existing one with a new vector
	MATRIX *vector = createVector(x, y, z);
	translate(t, vector);
	freeMatrix(vector);
	free(vector);
}

void scale(TRANSFORM *t, MATRIX *scaleVector) {
	// create and combine scale matrix
	MATRIX *scaleMatrix = createScaleMatrix(scaleVector);
	multMatrix(t->scale, scaleMatrix);

	// free it
	freeMatrix(scaleMatrix);
	free(scaleMatrix);
	scaleMatrix = NULL;
}

void scaleXYZ(TRANSFORM *t, double x, double y, double z) {
	// just call the existing one with a new vector
	MATRIX *vector = createVector(x, y, z);
	scale(t, vector);
	freeMatrix(vector);
	free(vector);
}

void rotate(TRANSFORM *t, MATRIX *rotVector) {
	// create and combine rotation matrix
	MATRIX *rotationMatrix = createRotationMatrix(rotVector);
	multMatrix(t->rotation, rotationMatrix);

	// free it
	freeMatrix(rotationMatrix);
	free(rotationMatrix);
	rotationMatrix = NULL;
}

void rotateXYZ(TRANSFORM *t, double x, double y, double z) {
	// just call the existing one with a new vector
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

TRANSFORM *combine(TRANSFORM *t1, TRANSFORM *t2) {
	TRANSFORM *combination = createTransform();

	// this may be inaccurate but i never actually use it so oh well
	// TODO fix this
	multMatrixTo(t1->translation, t2->translation, &combination->translation);
	multMatrixTo(t1->scale, t2->scale, &combination->scale);
	multMatrixTo(t1->rotation, t2->rotation, &combination->rotation);

	return combination;
}

void applyTransformation(TRANSFORM *t, MATRIX *v) {
	// get the matrix
	MATRIX *transformMatrix = getTransformMatrix(t);

	// transform using it
	multMatrixTo(transformMatrix, v, &v);

	// free it
	freeMatrix(transformMatrix);
	free(transformMatrix);
	transformMatrix = NULL;
}

void printTransform(TRANSFORM *t) {
	// just print all the things
	printf("translation:\n");
	printMatrix(t->translation);
	printf("scale:\n");
	printMatrix(t->scale);
	printf("rotation:\n");
	printMatrix(t->rotation);
}

void freeTransform(TRANSFORM *t) {
	// just free all the things
	freeMatrix(t->translation);
	freeMatrix(t->scale);
	freeMatrix(t->rotation);
	free(t->translation);
	free(t->scale);
	free(t->rotation);
	t->translation = NULL;
	t->scale = NULL;
	t->rotation = NULL;
}