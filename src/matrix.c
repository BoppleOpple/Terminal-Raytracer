#include "matrix.h"
#include "mathUtils.h"
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_STRING_LENGTH 3 + DBL_MANT_DIG - DBL_MIN_EXP

MATRIX createTempMatrix(int r, int c) {
	// create a new matrix
	MATRIX m = {r, c, malloc( sizeof(double) * r * c )};
	// fill its data with zeros
	fillMatrix(&m, 0.0);
	return m;
}

MATRIX *createMatrix(int r, int c) {
	// create a new matrix of zeros
	MATRIX *m = malloc(sizeof(MATRIX));
	*m = createTempMatrix(r, c);

	// and return a pointer to it
	return m;
}

MATRIX *createIdentityMatrix(int s) {
	// create a zero matrix
	MATRIX *m = createMatrix(s, s);

	// and set the diagonal to ones
	for (int i = 0; i < s; i++)
		setElement(m, i, i, 1.0);
	
	return m;
}

MATRIX *copyMatrix(MATRIX *m) {
	// create a new matrix
	MATRIX *copy = createMatrix(m->rows, m->cols);

	// set each element to the same element in m
	for (int i = 0; i < m->rows * m->cols; i++)
		*(copy->data + i) = *(m->data + i);

	return copy;
}

MATRIX *createVector(double x, double y, double z) {
	// create a new matrix
	MATRIX *m = malloc(sizeof(MATRIX));
	
	// set the coordinates (plus a homogenous coordinate)
	*m = (MATRIX) {4, 1, malloc( sizeof(double) * 4 )};
	*(m->data + 0) = x;
	*(m->data + 1) = y;
	*(m->data + 2) = z;
	*(m->data + 3) = 1.0;

	return m;
}

void addScalar(MATRIX *m, double s) {
	// iterate through each element and add s
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) += s;
}

// TODO do this in-place
void addMatrix(MATRIX *m1, MATRIX *m2) {
	// check the operation is valid
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		printf("matrices must be the same size; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}
	// increment each value accordingly
	for (int i = 0; i < m1->rows * m1->cols; i++)
		*(m1->data + i) += *(m2->data + i);
}

void multScalar(MATRIX *m, double s) {
	// iterate through each element and multiply by s
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) *= s;
}

void multMatrixTo(MATRIX *m1, MATRIX *m2, MATRIX **dst) {
	// check the operation is valid
	if (m1->cols != m2->rows) {
		printf("matrices must be size m x n and n x p; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	// create a temp matrix
	MATRIX m = createTempMatrix(m1->rows, m2->cols);

	// perform the matrix multiplication
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
			for (int k = 0; k < m1->cols; k++)
				setElement(&m, i, j, getElement(&m, i, j) + getElement(m1, i, k)*getElement(m2, k, j) );
	
	// setup the memory at *dst
	if (*dst) {
		freeMatrix(*dst);
	} else {
		*dst = malloc(sizeof(MATRIX));
	}

	// store the result
	**dst = m;
}

void multMatrix(MATRIX *m1, MATRIX *m2) {
	// just multiply with destination m1
	multMatrixTo(m1, m2, &m1);
}

void multMatrixElementwise(MATRIX *m1, MATRIX *m2) {
	// check the operation is valid
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		printf("matrices must be the same size; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	// multiply each element
	for (int i = 0; i < m1->rows * m1->cols; i++)
		*(m1->data + i) *= *(m2->data + i);
}

void fillMatrix(MATRIX *m, double n) {
	// set each element in m to n
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) = n;
}

double getElement(MATRIX *m, int r, int c) {
	// just a wrapper for pointer math
	return *(m->data + m->cols * r + c);
}

void setElement(MATRIX *m, int r, int c, double val) {
	// still just a wrapper for pointer math
	*(m->data + m->cols * r + c) = val;
}

double minor(MATRIX *m, int r, int c) {
	MATRIX *submatrix = createMatrix(m->rows-1, m->cols-1);

	// create a submatrix by removing row r and column c
	for (int i = 0; i < submatrix->rows; i++)
		for (int j = 0; j < submatrix->cols; j++)
			setElement(submatrix, i, j, getElement(m, (i<r) ? i : i+1, (j<c) ? j : j+1));
	
	// the minor is the determinant of the matrix
	double result = determinant(submatrix);

	freeMatrix(submatrix);
	free(submatrix);
	
	return result;
}

double cofactor(MATRIX *m, int r, int c) {
	// positive or negative monir of m, depending on r and c
	return (((r + c) % 2) ? -1 : 1) * minor(m, r, c);
}

double determinant(MATRIX *m) {
	// check the operation is valid
	if (m->rows != m->cols) {
		printf("cannot find the determinant of non-square matrix (%ix%i)", m->rows, m->cols);
		exit(1);
	}

	// base case: determinant of 1x1 matrix is its only value
	if (m->rows == 1)
		return getElement(m, 0, 0);

	// for everything else, det is the sum of all cofactors scaled by their elements
	double result = 0.0;
	for (int i = 0; i < m->cols; i++)
		result += getElement(m, 0, i) * cofactor(m, 0, i);
	
	return result;
}

MATRIX *getInverse(MATRIX *m) {
	// setup temp variables
	double det = determinant(m);
	MATRIX *result = createMatrix(m->rows, m->cols);

	// check the operation is valid
	if (det == 0) {
		printf("matrix is not invertible!\n");
		exit(1);
	}

	// each element of the inverse is the cofactor of that element in m / |m|
	for (int i = 0; i < result->rows; i++)
		for (int j = 0; j < result->cols; j++)
			setElement(result, i, j, cofactor(m, j, i) / det);

	return result;
}

void rref(MATRIX *m) {
	// check the operation is valid
	if (m->cols < m->rows) {
		printf("matrix is underconstrained\n");
		exit(1);
	}

	// if matrix is singular, throw error (may change later)
	if (m->cols == m->rows && determinant(m) == 0) {
		printf("cannot solve linear system with a singular matrix\n");
		exit(1);
	}

	// first, just row reduce by making each value under the diagonal 0
	for (int i = 0; i < m->rows; i++) {
		if (getElement(m, i, i) == 0) {
			for (int j = i + 1; j < m->rows; j++) {
				if (getElement(m, j, i) != 0.0) {
					swapRows(m, i, j);
					break;
				}
			}
		}
		scaleRow(m, i, 1.0/getElement(m, i, i));
		for (int j = i + 1; j < m->rows; j++)
			addRowMultiple(m, i, j, -getElement(m, j, i));
	}

	// then, do rescaling and such so that every value above the diagonal is alzo 0 (except the rightmost column)
	for (int i = m->rows - 1; i >= 0; i--)
		for (int j = i - 1; j >= 0; j--)
			addRowMultiple(m, i, j, -getElement(m, j, i));

}

void scaleRow(MATRIX *m, int r, double scale) {
	// just rescale each element in a row
	for (int j = 0; j < m->cols; j++)
		setElement(m, r, j, scale * getElement(m, r, j));
}

void addRowMultiple(MATRIX *m, int sourceRow, int destinationRow, double scale) {
	// just increment each element in a row by some multiple of another row
	for (int j = 0; j < m->cols; j++)
		setElement(m, destinationRow, j, getElement(m, destinationRow, j) + scale * getElement(m, sourceRow, j));
}

void swapRows(MATRIX *m, int r1, int r2) {
	double temp = 0.0;

	// for each element in both rows
	for (int j = 0; j < m->cols; j++) {
		// swap the corresponding elements
		temp = getElement(m, r1, j);
		setElement(m, r1, j, getElement(m, r2, j));
		setElement(m, r2, j, temp);
	}
}

double vectorDotProduct(MATRIX *v1, MATRIX *v2) {
	double result = 0.0;

	// make result the sum of element-wise multiplication
	for (int i = 0; i < v1->rows - 1; i++)
		result += getElement(v1, i, 0)*getElement(v2, i, 0);
	
	return result;
}

MATRIX *vectorCrossProduct(MATRIX *v1, MATRIX *v2) {
	// this is just the definition of a cross product
	return createVector(
		getElement(v1, 1, 0)*getElement(v2, 2, 0) - getElement(v1, 2, 0)*getElement(v2, 1, 0),
		getElement(v1, 2, 0)*getElement(v2, 0, 0) - getElement(v1, 0, 0)*getElement(v2, 2, 0),
		getElement(v1, 0, 0)*getElement(v2, 1, 0) - getElement(v1, 1, 0)*getElement(v2, 0, 0)
	);
}

double vectorLength(MATRIX *v) {
	// return L2 norm of a vector
	return sqrt(
		pow(getElement(v, 0, 0), 2.0) +
		pow(getElement(v, 1, 0), 2.0) +
		pow(getElement(v, 2, 0), 2.0)
	);
}

void normalizeVector(MATRIX *v) {
	// rescale a vector by the inverse of its length
	multScalar(v, 1.0/vectorLength(v));
}

int isZeroMatrix(MATRIX *m) {
	// for each element
	for (int i = 0; i < m->rows; i++)
		for (int j = 0; j < m->cols; j++)
			// return falsey if it isnt 0
			if (getElement(m, i, j) != 0.0) return 0;
	
	return 1;
}

void printMatrix(MATRIX *m) {
	int max_digits = 0;
	char digitString[DOUBLE_STRING_LENGTH] = "";

	// find the largest number printed and set the length of all elements to that
	for (int i = 0; i < m->rows * m->cols; i++) {
		sprintf(digitString, "%lf", *(m->data + i));
		max_digits = MAX(max_digits, strlen(digitString));
	}

	// now print each row
	for (int i = 0; i < m->rows; i++) {
		printf((i == 0) ? "[ " : "| ");
		for (int j = 0; j < m->cols; j++) {
			// and for each element insure it is the length of the longest element
			printf("%*lf ", max_digits, getElement(m, i, j));
		}
		printf("%c\n", (i == m->rows - 1) ? ']' : '|');
	}
}

void printVector3(MATRIX *m) {
	// just print the values of a vector in angle brackets
	printf("<%lf, %lf, %lf>\n", m->data[0], m->data[1], m->data[2]);
}

void freeMatrix(MATRIX *m) {
	// free the data and reset other information
	free(m->data);
	m->rows = 0;
	m->cols = 0;
	m->data = NULL;
}