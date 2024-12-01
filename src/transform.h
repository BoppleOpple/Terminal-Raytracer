#include "matrix.h"

#ifndef RAYTRACE_TRANSFORM
#define RAYTRACE_TRANSFORM

typedef struct {
	MATRIX translation;
	MATRIX scale;
	MATRIX rotation;
} TRANSFORM;

TRANSFORM createTransform();

MATRIX createTranslationMatrix(MATRIX posVector);

MATRIX createScaleMatrix(MATRIX scaleVector);

MATRIX createRotationMatrix(MATRIX rotVector);

void setTranslation(TRANSFORM *t, MATRIX posVector);

void setScale(TRANSFORM *t, MATRIX scaleVector);

void setRotation(TRANSFORM *t, MATRIX rotVector);

MATRIX getTranslationVector(TRANSFORM *t);

MATRIX getScaleVector(TRANSFORM *t);

MATRIX getRotationVector(TRANSFORM *t);

void translate(TRANSFORM *t, MATRIX posVector);

void scale(TRANSFORM *t, MATRIX scaleVector);

void rotate(TRANSFORM *t, MATRIX rotVector);

TRANSFORM combine(TRANSFORM *t1, TRANSFORM *t2);

void printTransform(TRANSFORM *t);

#endif