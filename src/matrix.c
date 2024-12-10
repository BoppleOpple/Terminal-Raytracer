#include "matrix.h"
#include "mathUtils.h"
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_STRING_LENGTH 3 + DBL_MANT_DIG - DBL_MIN_EXP

MATRIX createTempMatrix(int r, int c) {
	return (MATRIX) {r, c, malloc( sizeof(double) * r * c )};
}

MATRIX *createMatrix(int r, int c) {
	MATRIX *m = malloc(sizeof(MATRIX));
	*m = createTempMatrix(r, c);
	fillMatrix(m, 0.0);
	return m;
}

MATRIX *createIdentityMatrix(int s) {
	MATRIX *m = createMatrix(s, s);
	for (int i = 0; i < s; i++)
		setElement(m, i, i, 1.0);
	
	return m;
}

MATRIX *copyMatrix(MATRIX *m) {
	MATRIX *copy = createMatrix(m->rows, m->cols);

	for (int i = 0; i < m->rows * m->cols; i++)
		*(copy->data + i) = *(m->data + i);

	return copy;
}

MATRIX *createVector(double x, double y, double z) {
	MATRIX *m = malloc(sizeof(MATRIX));
	
	*m = (MATRIX) {4, 1, malloc( sizeof(double) * 4 )};
	*(m->data + 0) = x;
	*(m->data + 1) = y;
	*(m->data + 2) = z;
	*(m->data + 3) = 1.0;
	return m;
}

void addScalar(MATRIX *m, double s) {
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) += s;
}

void addMatrix(MATRIX *m1, MATRIX *m2) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		printf("matrices must be the same size; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	MATRIX result = createTempMatrix(m1->rows, m1->cols);
	for (int i = 0; i < result.rows * result.cols; i++)
		*(result.data + i) = *(m1->data + i) + *(m2->data + i);
	
	freeMatrix(m1);

	*m1 = result;
}

void multScalar(MATRIX *m, double s) {
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) *= s;
}

void multMatrixTo(MATRIX *m1, MATRIX *m2, MATRIX **dst) {
	if (m1->cols != m2->rows) {
		printf("matrices must be size m x n and n x p; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	MATRIX m = createTempMatrix(m1->rows, m2->cols);

	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
			for (int k = 0; k < m1->cols; k++)
				setElement(&m, i, j, getElement(&m, i, j) + getElement(m1, i, k)*getElement(m2, k, j) );
	
	if (*dst) {
		// printf("dst exists ig: %p\n", dst);
		freeMatrix(*dst);
	} else {
		*dst = malloc(sizeof(MATRIX));
		// printf("allocating (%i, %i) matrix\n", m.rows, m.cols);
	}
	**dst = m;
}

void multMatrix(MATRIX *m1, MATRIX *m2) {
	multMatrixTo(m1, m2, &m1);
}

void multMatrixElementwise(MATRIX *m1, MATRIX *m2) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		printf("matrices must be the same size; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	MATRIX m = createTempMatrix(m1->rows, m1->cols);
	for (int i = 0; i < m.rows * m.cols; i++)
		*(m.data + i) = *(m1->data + i) * *(m2->data + i);

	freeMatrix(m1);
	
	*m1 = m;
}

void fillMatrix(MATRIX *m, double n) {
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) = n;
}

double getElement(MATRIX *m, int r, int c) {
	return *(m->data + m->cols * r + c);
}

void setElement(MATRIX *m, int r, int c, double val) {
	*(m->data + m->cols * r + c) = val;
}

double minor(MATRIX *m, int r, int c) {
	MATRIX *submatrix = createMatrix(m->rows-1, m->cols-1);
	for (int i = 0; i < submatrix->rows; i++)
		for (int j = 0; j < submatrix->cols; j++)
			setElement(submatrix, i, j, getElement(m, (i<r) ? i : i+1, (j<c) ? j : j+1));
	
	double result = determinant(submatrix);

	freeMatrix(submatrix);
	free(submatrix);
	
	return result;
}

double cofactor(MATRIX *m, int r, int c) {
	return (((r + c) % 2) ? -1 : 1) * minor(m, r, c);
}

double determinant(MATRIX *m) {
	if (m->rows != m->cols) {
		printf("cannot find the determinant of non-square matrix (%ix%i)", m->rows, m->cols);
		exit(1);
	}
	if (m->rows == 1)
		return getElement(m, 0, 0);

	double result = 0.0;

	for (int i = 0; i < m->cols; i++)
		result += getElement(m, 0, i) * cofactor(m, 0, i);
	
	return result;
}

MATRIX *getInverse(MATRIX *m) {
	double det = determinant(m);
	MATRIX *result = createMatrix(m->rows, m->cols);

	if (det == 0) {
		printf("matrix is not invertible!\n");
		exit(1);
	}

	for (int i = 0; i < result->rows; i++)
		for (int j = 0; j < result->cols; j++)
			setElement(result, i, j, cofactor(m, j, i));

	multScalar(result, 1.0/det);

	return result;
}

void rref(MATRIX *m) {
	// if matrix is singular, throw error (may change later)
	if (m->cols == m->rows && determinant(m) == 0) {
		printf("cannot solve linear system with a singular matrix\n");
		exit(1);
	}

	if (m->cols < m->rows) {
		printf("matrix is underconstrained\n");
		exit(1);
	}

	for (int i = 0; i < m->rows; i++) {
		scaleRow(m, i, 1.0/getElement(m, i, i));
		for (int j = i + 1; j < m->cols; j++)
			addRowMultiple(m, i, j, -getElement(m, j, i));
	}

	for (int i = m->rows - 1; i >= 0; i--)
		for (int j = i - 1; j >= 0; j--)
			addRowMultiple(m, i, j, -getElement(m, j, i));
}

void scaleRow(MATRIX *m, int r, double scale) {
	for (int j = 0; j < m->cols; j++)
		setElement(m, r, j, scale * getElement(m, r, j));
}

void addRowMultiple(MATRIX *m, int sourceRow, int destinationRow, double scale) {
	for (int j = 0; j < m->cols; j++)
		setElement(m, destinationRow, j, getElement(m, destinationRow, j) + scale * getElement(m, sourceRow, j));
}

void swapRows(MATRIX *m, int r1, int r2) {
	double temp = 0.0;
	for (int j = 0; j < m->cols; j++) {
		temp = getElement(m, r1, j);
		setElement(m, r2, j, getElement(m, r1, j));
		setElement(m, r1, j, temp);
	}
}

double vectorDotProduct(MATRIX *v1, MATRIX *v2) {
	double result = 0.0;
	for (int i = 0; i < v1->rows - 1; i++)
		result += getElement(v1, i, 0)*getElement(v2, i, 0);
	
	return result;
}

MATRIX *vectorCrossProduct(MATRIX *v1, MATRIX *v2) {
	return createVector(
		getElement(v1, 1, 0)*getElement(v2, 2, 0) - getElement(v1, 2, 0)*getElement(v2, 1, 0),
		getElement(v1, 2, 0)*getElement(v2, 0, 0) - getElement(v1, 0, 0)*getElement(v2, 2, 0),
		getElement(v1, 0, 0)*getElement(v2, 1, 0) - getElement(v1, 1, 0)*getElement(v2, 0, 0)
	);
}

double vectorLength(MATRIX *v) {
	return sqrt(
		pow(getElement(v, 0, 0), 2.0) +
		pow(getElement(v, 1, 0), 2.0) +
		pow(getElement(v, 2, 0), 2.0)
	);
}

void normalizeVector(MATRIX *v) {
	multScalar(v, 1.0/vectorLength(v));
}

int isZeroMatrix(MATRIX *m) {
	for (int i = 0; i < m->rows; i++)
		for (int j = 0; j < m->cols; j++)
			if (getElement(m, i, j) != 0.0) return 1;
	
	return 1;
}

void printMatrix(MATRIX *m) {
	int max_digits = 0;
	char digitString[DOUBLE_STRING_LENGTH] = "";

	for (int i = 0; i < m->rows * m->cols; i++) {
		sprintf(digitString, "%lf", *(m->data + i));
		max_digits = MAX(max_digits, strlen(digitString));
	}

	for (int i = 0; i < m->rows; i++) {
		printf((i == 0) ? "[ " : "| ");
		for (int j = 0; j < m->cols; j++) {
			printf("%*lf ", max_digits, getElement(m, i, j));
		}
		printf("%c\n", (i == m->rows - 1) ? ']' : '|');
	}
}

void printVector3(MATRIX *m) {
	printf("<%lf, %lf, %lf>\n", m->data[0], m->data[1], m->data[2]);
}

void freeMatrix(MATRIX *m) {
	free(m->data);
	m->rows = 0;
	m->cols = 0;
	m->data = NULL;
}