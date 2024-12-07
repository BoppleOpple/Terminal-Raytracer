#include "matrix.h"

#ifndef RAYTRACE_TRANSFORM
#define RAYTRACE_TRANSFORM

typedef struct {
	MATRIX *translation;
	MATRIX *scale;
	MATRIX *rotation;
} TRANSFORM;

TRANSFORM *createTransform();

MATRIX *createTranslationMatrix(MATRIX *posVector);

MATRIX *createScaleMatrix(MATRIX *scaleVector);

MATRIX *createRotationMatrix(MATRIX *rotVector);

void setTranslation(TRANSFORM *t, MATRIX *posVector);

void setScale(TRANSFORM *t, MATRIX *scaleVector);

void setRotation(TRANSFORM *t, MATRIX *rotVector);

MATRIX *getTranslationVector(TRANSFORM *t);

MATRIX *getScaleVector(TRANSFORM *t);

MATRIX *getRotationVector(TRANSFORM *t);

void translate(TRANSFORM *t, MATRIX *posVector);

void translateXYZ(TRANSFORM *t, double x, double y, double z);

void scale(TRANSFORM *t, MATRIX *scaleVector);

void scaleXYZ(TRANSFORM *t, double x, double y, double z);

void rotate(TRANSFORM *t, MATRIX *rotVector);

void rotateXYZ(TRANSFORM *t, double x, double y, double z);

MATRIX *getTransformMatrix(TRANSFORM *t);

TRANSFORM *combine(TRANSFORM *t1, TRANSFORM *t2);

void applyTransformation(TRANSFORM *t, MATRIX *v);

void printTransform(TRANSFORM *t);

#endif