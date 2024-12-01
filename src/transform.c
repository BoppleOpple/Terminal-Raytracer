#include "transform.h"
#include "matrix.h"
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

// MATRIX createRotationMatrix(MATRIX rotVector);

void setTranslation(TRANSFORM *t, MATRIX posVector) {
	t->translation = createTranslationMatrix(posVector);
}

void setScale(TRANSFORM *t, MATRIX scaleVector) {
		t->scale = createScaleMatrix(scaleVector);
}

// void setRotation(TRANSFORM *t, MATRIX rotVector);

MATRIX getTranslationVector(TRANSFORM *t) {
	return createVector(
		getElement(&t->translation, 0, 3),
		getElement(&t->translation, 1, 3),
		getElement(&t->translation, 2, 3)
	);
}
// ⊂(-ω -⊂)

MATRIX getScaleVector(TRANSFORM *t) {
	return createVector(
		getElement(&t->translation, 0, 0),
		getElement(&t->translation, 1, 1),
		getElement(&t->translation, 2, 2)
	);
}

// MATRIX getRotationVector(TRANSFORM *t);

void translate(TRANSFORM *t, MATRIX posVector) {
	MATRIX translationMatrix = createTranslationMatrix(posVector);
	t->translation = multMatrix(&t->translation, &translationMatrix);
}

void scale(TRANSFORM *t, MATRIX scaleVector) {
	MATRIX scaleMatrix = createScaleMatrix(scaleVector);
	t->scale = multMatrix(&t->scale, &scaleMatrix);
}

// void rotate(TRANSFORM *t, MATRIX rotVector);

// TRANSFORM combine(TRANSFORM *t1, TRANSFORM *t2);

void printTransform(TRANSFORM *t) {
	printf("translation:\n");
	printMatrix(&t->translation);
	printf("scale:\n");
	printMatrix(&t->scale);
	printf("rotation:\n");
	printMatrix(&t->rotation);
}