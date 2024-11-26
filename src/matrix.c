#include "matrix.h"
#include "mathUtils.h"
#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>

#define DOUBLE_STRING_LENGTH 3 + DBL_MANT_DIG - DBL_MIN_EXP

MATRIX createMatrix(int r, int c) {
	MATRIX m = {r, c, malloc( sizeof(double) * r * c )};
	fillMatrix(&m, 0.0);
	return m;
}

MATRIX createVector(double x, double y, double z) {
	MATRIX m = {3, 1, malloc( sizeof(double) * 3 )};
	*(m.data + 0) = x;
	*(m.data + 1) = y;
	*(m.data + 2) = z;
	return m;
}

void addScalar(MATRIX *m, double s) {
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) += s;
}

MATRIX addMatrix(MATRIX *m1, MATRIX *m2) {
	if (m1->rows != m2->rows || m1->cols != m2->cols) {
		printf("matrices must be the same size; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	MATRIX m = createMatrix(m1->rows, m1->cols);
	for (int i = 0; i < m.rows * m.cols; i++)
		*(m.data + i) = *(m1->data + i) + *(m2->data + i);

	return m;
}

void multScalar(MATRIX *m, double s) {
	for (int i = 0; i < m->rows * m->cols; i++)
		*(m->data + i) *= s;
}

MATRIX multMatrix(MATRIX *m1, MATRIX *m2) {
	if (m1->cols != m2->rows) {
		printf("matrices must be size m x n and n x p; given (%i, %i) and (%i, %i)\n", m1->rows, m1->cols, m2->rows, m2->cols);
		exit(1);
	}

	MATRIX m = createMatrix(m1->rows, m2->cols);

	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
			for (int k = 0; k < m1->cols; k++)
				setElement(&m, i, j, getElement(&m, i, j) + getElement(m1, i, k)*getElement(m2, k, j) );
	
	return m;
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

void freeMatrix(MATRIX *m) {
	free(m->data);
	m->rows = 0;
	m->cols = 0;
	m->data = NULL;
}