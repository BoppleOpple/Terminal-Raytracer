#include "matrix.h"

#ifndef RAYTRACE_TRANSFORM
#define RAYTRACE_TRANSFORM

typedef struct {
	MATRIX *translation;
	MATRIX *scale;
	MATRIX *rotation;
} TRANSFORM;

/**
 * @brief create a transform with default scale, translation, and rotation
 *
 * @return TRANSFORM* pointer to the new transform
 */
TRANSFORM *createTransform();

/**
 * @brief create a matrix that handles translation
 *
 * @param posVector MATRIX pointer to the translation vector
 * @return MATRIX pointer to the new matrix
 */
MATRIX *createTranslationMatrix(MATRIX *posVector);

/**
 * @brief create a matrix that handles scaling
 *
 * @param scaleVector MATRIX pointer to the scale vector
 * @return MATRIX pointer to the new matrix
 */
MATRIX *createScaleMatrix(MATRIX *scaleVector);

/**
 * @brief create a matrix that handles rotation
 *
 * @param rotVector MATRIX pointer to the rotation vector
 * @return MATRIX pointer to the new matrix
 */
MATRIX *createRotationMatrix(MATRIX *rotVector);

/**
 * @brief sets the translation of a transform from a vector
 *
 * @param t TRANSFORM pointer to the target transform
 * @param posVector MATRIX pointer to the translation vector
 */
void setTranslation(TRANSFORM *t, MATRIX *posVector);

/**
 * @brief sets the scale of a transform from a vector
 *
 * @param t TRANSFORM pointer to the target transform
 * @param scaleVector MATRIX pointer to the scale vector
 */
void setScale(TRANSFORM *t, MATRIX *scaleVector);

/**
 * @brief sets the rotation of a transform from a vector
 *
 * @param t TRANSFORM pointer to the target transform
 * @param rotVector MATRIX pointer to the rotation vector
 */
void setRotation(TRANSFORM *t, MATRIX *rotVector);

/**
 * @brief generates a vector representing the translation of a tramsform
 *
 * @param t TRANSFORM pointer to the target transform
 * @return MATRIX pointer to the new translation vector
 */
MATRIX *getTranslationVector(TRANSFORM *t);

/**
 * @brief generates a vector representing the scale of a tramsform
 *
 * @param t TRANSFORM pointer to the target transform
 * @return MATRIX pointer to the new scale vector
 */
MATRIX *getScaleVector(TRANSFORM *t);

/**
 * @brief generates a vector representing the rotation of a tramsform
 *
 * @param t TRANSFORM pointer to the target transform
 * @return MATRIX pointer to the new rotation vector
 */
MATRIX *getRotationVector(TRANSFORM *t);

/**
 * @brief translates a tramsform by a vector
 *
 * @param t TRANSFORM pointer to the target transform
 * @param posVector MATRIX pointer to the translation vector
 */
void translate(TRANSFORM *t, MATRIX *posVector);

/**
 * @brief translates a tramsform by x, y, and z values
 *
 * @param t TRANSFORM pointer to the target transform
 * @param x double translation in x
 * @param y double translation in y
 * @param z double translation in z
 */
void translateXYZ(TRANSFORM *t, double x, double y, double z);

/**
 * @brief scales a tramsform by a vector
 *
 * @param t TRANSFORM pointer to the target transform
 * @param scaleVector MATRIX pointer to the scale vector
 */
void scale(TRANSFORM *t, MATRIX *scaleVector);

/**
 * @brief scales a tramsform by x, y, and z values
 *
 * @param t TRANSFORM pointer to the target transform
 * @param x double scale in x
 * @param y double scale in y
 * @param z double scale in z
 */
void scaleXYZ(TRANSFORM *t, double x, double y, double z);

/**
 * @brief rotates a tramsform by a vector
 *
 * @param t TRANSFORM pointer to the target transform
 * @param scaleVector MATRIX pointer to the rotation vector
 */
void rotate(TRANSFORM *t, MATRIX *rotVector);

/**
 * @brief rotates a tramsform by x, y, and z values
 *
 * @param t TRANSFORM pointer to the target transform
 * @param x double rotation in x
 * @param y double rotation in y
 * @param z double rotation in z
 */
void rotateXYZ(TRANSFORM *t, double x, double y, double z);

/**
 * @brief generates a matrix to perform the transormation of a transform
 *
 * @param t TRANSFORM pointer to the target transform
 * @return MATRIX pointer to the new transformation matrix
 */
MATRIX *getTransformMatrix(TRANSFORM *t);

/**
 * @brief generates a new transform as a combination of two other transforms
 *
 * @param t1 TRANSFORM pointer to the first transform
 * @param t1 TRANSFORM pointer to the second transform
 * @return TRANSFORM pointer to the new transform
 */
TRANSFORM *combine(TRANSFORM *t1, TRANSFORM *t2);

/**
 * @brief transforms a vector in-place according to a specified transform
 *
 * @param t TRANSFORM pointer to the transform
 * @param v TRANSFORM pointer to the vector
 */
void applyTransformation(TRANSFORM *t, MATRIX *v);

/**
 * @brief prints info about a transform
 *
 * @param t TRANSFORM pointer to the transform
 */
void printTransform(TRANSFORM *t);

/**
 * @brief frees all fields of a transform
 *
 * @param t TRANSFORM pointer to the transform
 */
void freeTransform(TRANSFORM *t);

#endif