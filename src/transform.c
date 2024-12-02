#include "transform.h"
#include "mathUtils.h"
#include "matrix.h"
#include <complex.h>
#include <math.h>
#include <stdio.h>

TRANSFORM createTransform() {
	return (TRANSFORM) {
		createIdentityMatrix(4),
		createIdentityMatrix(4),
		createIdentityMatrix(4)
	};
}

MATRIX createTranslationMatrix(MATRIX posVector) {
	MATRIX m = createIdentityMatrix(4);
	for (int i = 0; i < posVector.rows; i++)
		setElement(&m, i, 3, getElement(&posVector, i, 0));
	return m;
}

MATRIX createScaleMatrix(MATRIX scaleVector) {
	MATRIX m = createIdentityMatrix(4);
	for (int i = 0; i < scaleVector.rows; i++)
		setElement(&m, i, i, getElement(&scaleVector, i, 0));
	return m;
}

MATRIX createRotationMatrixX(double xRot) {
	MATRIX result = createIdentityMatrix(4);
	double sinAngle = sin(xRot);
	double cosAngle = cos(xRot);
	setElement(&result, 1, 1,  cosAngle);
	setElement(&result, 1, 2, -sinAngle);
	setElement(&result, 2, 1,  sinAngle);
	setElement(&result, 2, 2,  cosAngle);
	return result;
}

MATRIX createRotationMatrixY(double yRot) {
	MATRIX result = createIdentityMatrix(4);
	double sinAngle = sin(yRot);
	double cosAngle = cos(yRot);
	setElement(&result, 0, 0,  cosAngle);
	setElement(&result, 0, 2, -sinAngle);
	setElement(&result, 2, 0,  sinAngle);
	setElement(&result, 2, 2,  cosAngle);
	return result;
}

MATRIX createRotationMatrixZ(double zRot) {
	MATRIX result = createIdentityMatrix(4);
	double sinAngle = sin(zRot);
	double cosAngle = cos(zRot);
	setElement(&result, 0, 0,  cosAngle);
	setElement(&result, 0, 1, -sinAngle);
	setElement(&result, 1, 0,  sinAngle);
	setElement(&result, 1, 1,  cosAngle);
	return result;
}

MATRIX createRotationMatrix(MATRIX rotVector) {
	MATRIX result = createRotationMatrixZ(getElement(&rotVector, 0, 2));
	MATRIX next = createRotationMatrixY(getElement(&rotVector, 0, 1));
	result = multMatrix(&result, &next);

	next = createRotationMatrixX(getElement(&rotVector, 0, 0));
	result = multMatrix(&result, &next);

	return result;
}

void setTranslation(TRANSFORM *t, MATRIX posVector) {
	t->translation = createTranslationMatrix(posVector);
}

void setScale(TRANSFORM *t, MATRIX scaleVector) {
	t->scale = createScaleMatrix(scaleVector);
}

void setRotation(TRANSFORM *t, MATRIX rotVector) {
	t->rotation = createRotationMatrix(rotVector);
}

MATRIX getTranslationVector(TRANSFORM *t) {
	return createVector(
		getElement(&t->translation, 0, 3),
		getElement(&t->translation, 1, 3),
		getElement(&t->translation, 2, 3)
	);
}

MATRIX getScaleVector(TRANSFORM *t) {
	return createVector(
		getElement(&t->translation, 0, 0),
		getElement(&t->translation, 1, 1),
		getElement(&t->translation, 2, 2)
	);
}

MATRIX getRotationVector(TRANSFORM *t) {
	double sinYRot = -getElement(&t->rotation, 2, 0);
	if (sinYRot == 1.0) {
		return createVector(
			atan2(getElement(&t->rotation, 0, 1), getElement(&t->rotation, 0, 2)),
			PI/2,
			0.0
		);
	} else if (sinYRot == -1.0) {
		return createVector(
			atan2(-getElement(&t->rotation, 0, 1), -getElement(&t->rotation, 0, 2)),
			PI/2,
			0.0
		);
	} else {
		double yRot = asin(sinYRot);
		double cosYRot = cos(yRot);
		return createVector(
			atan2(getElement(&t->rotation, 2, 1)/cosYRot, getElement(&t->rotation, 2, 2)/cosYRot),
			yRot,
			atan2(getElement(&t->rotation, 1, 0)/cosYRot, getElement(&t->rotation, 0, 0)/cosYRot));
	}
}

void translate(TRANSFORM *t, MATRIX posVector) {
	MATRIX translationMatrix = createTranslationMatrix(posVector);
	t->translation = multMatrix(&t->translation, &translationMatrix);
}

void scale(TRANSFORM *t, MATRIX scaleVector) {
	MATRIX scaleMatrix = createScaleMatrix(scaleVector);
	t->scale = multMatrix(&t->scale, &scaleMatrix);
}

void rotate(TRANSFORM *t, MATRIX rotVector) {
	MATRIX rotationMatrix = createRotationMatrix(rotVector);
	t->rotation = multMatrix(&t->rotation, &rotationMatrix);
}

MATRIX getTransformMatrix(TRANSFORM *t) {
	MATRIX combination = multMatrix(&t->rotation, &t->scale);
	combination = multMatrix(&t->translation, &combination);
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
TRANSFORM combine(TRANSFORM *t1, TRANSFORM *t2) {
	TRANSFORM combination = createTransform();

	combination.translation = multMatrix(&t1->translation, &t2->translation);
	combination.scale = multMatrix(&t1->scale, &t2->scale);
	combination.rotation = multMatrix(&t1->rotation, &t2->rotation);

	return combination;
}

MATRIX applyTransformation(TRANSFORM *t, MATRIX *v) {
	MATRIX transformMatrix = getTransformMatrix(t);
	return multMatrix(&transformMatrix, v);
}

void printTransform(TRANSFORM *t) {
	printf("translation:\n");
	printMatrix(&t->translation);
	printf("scale:\n");
	printMatrix(&t->scale);
	printf("rotation:\n");
	printMatrix(&t->rotation);
}