#ifndef RAYTRACE_MATRIX
#define RAYTRACE_MATRIX

typedef struct {
	int rows;
	int cols;
	double *data;
} MATRIX;

/**
 * @brief create a new MATRIX struct
 *
 * creates a new matrix and fills it with zeros 
 *
 * @param r int number of rows
 * @param c int number of columns
 * @return MATRIX New zero matrix
 */
MATRIX *createMatrix(int r, int c);

/**
 * @brief create a square identity matrix
 * 
 * @param s int dimensions of the square matrix (i.e. 4 -> 4x4 matrix)
 * @return MATRIX the new identity matrix
 */
MATRIX *createIdentityMatrix(int s);

/**
 * @brief copies the data of a MATRIX to a new MATRIX and returns it
 *
 * @param m MATRIX the source matrix
 * @return MATRIX the returned copy
 */
MATRIX *copyMatrix(MATRIX *m);

/**
 * @brief create a column vactor MATRIX struct
 * 
 * @param x double the vector's x component
 * @param y double the vector's y component
 * @param z double the vector's z component
 * @return MATRIX 
 */
MATRIX *createVector(double x, double y, double z);

/**
 * @brief adds a scalar to each element of a MATRIX
 *
 * @param m MATRIX the matrix to increment
 * @param s double the scalar to add
 */
void addScalar(MATRIX *m, double s);

/**
 * @brief adds a MATRIX to another MATRIX element-wise
 *
 * @param m1 MATRIX the matrix to increment
 * @param m2 MATRIX the matrix to increment by
 */
void addMatrix(MATRIX *m1, MATRIX *m2);

/**
 * @brief multiplies each element of a MATRIX by a scalar
 *
 * @param m MATRIX the matrix to rescale
 * @param s double the scalar to multiply by
 */
void multScalar(MATRIX *m, double s);

/**
 * @brief performs matrix multiplication on two MATRIX structs and stores the result in another MATRIX
 *
 * @param m1 MATRIX the left matrix
 * @param m2 MATRIX the right matrix
 * @param m2 MATRIX the destination matrix
 */
void multMatrixTo(MATRIX *m1, MATRIX *m2, MATRIX **dst);

/**
 * @brief performs matrix multiplication on two MATRIX structs and stores the result in the first MATRIX
 *
 * @param m1 MATRIX the left matrix (and the destination)
 * @param m2 MATRIX the right matrix
 */
void multMatrix(MATRIX *m1, MATRIX *m2);

/**
 * @brief multiplies a MATRIX by another MATRIX element-wise
 *
 * @param m1 MATRIX the matrix to multiply
 * @param m2 MATRIX the matrix to multiply by
 */
void multMatrixElementwise(MATRIX *m1, MATRIX *m2);

/**
 * @brief sets each element of a MATRIX to a value
 *
 * @param m MATRIX the matrix to modify
 * @param n double the value to set
 */
void fillMatrix(MATRIX *m, double n);

/**
 * @brief gets the value of a specific element of a MATRIX
 *
 * @param m MATRIX the matrix to read from
 * @param r int the row of the element
 * @param c int the column of the element
 * @return double the value of the element in row r, column c
 */
double getElement(MATRIX *m, int r, int c);

/**
 * @brief sets the value of a specific element of a MATRIX to a specified value
 *
 * @param m MATRIX the matrix to read from
 * @param r int the row of the element
 * @param c int the column of the element
 * @param val double the value to store in row r, column c
 */
void setElement(MATRIX *m, int r, int c, double val);

/**
 * @brief computes and returns the minor of an element of a matrix
 *
 * @param m MATRIX the matrix to read from
 * @param r int the row of the element
 * @param c int the column of the element
 * @return double the value of the minor
 */
double minor(MATRIX *m, int r, int c);

/**
 * @brief computes and returns the cofactor of an element of a matrix
 *
 * @param m MATRIX the matrix to read from
 * @param r int the row of the element
 * @param c int the column of the element
 * @return double the value of the cofactor
 */
double cofactor(MATRIX *m, int r, int c);

/**
 * @brief computes and returns the determinant of a matrix
 *
 * @param m MATRIX the matrix to read from
 * @return double the value of the determinant
 */
double determinant(MATRIX *m);

/**
 * @brief computes and returns the inverse of a matrix
 *
 * @param m MATRIX the matrix to read from
 * @return MATRIX the matrix's inverse
 */
MATRIX *getInverse(MATRIX *m);

/**
 * @brief row reduces a matrix in-place
 *
 * @param m MATRIX the matrix to reduce
 */
void rref(MATRIX *m);

/**
 * @brief scales a row of a MATRIX by a specified factor in-place
 *
 * @param m MATRIX the matrix to operate on
 * @param r MATRIX the row to scale
 * @param scale double the value to scale by
 */
void scaleRow(MATRIX *m, int r, double scale);

/**
 * @brief increments one row of a matrix by a multiple of another row of a matrix in-place
 *
 * @param m MATRIX the matrix to operate on
 * @param sourceRow int the row to increment by
 * @param destinationRow int the row to add to
 * @param scale double the value to scale by
 */
void addRowMultiple(MATRIX *m, int sourceRow, int destinationRow, double scale);

/**
 * @brief swaps two rows of a matrix in-place
 *
 * @param m MATRIX the matrix to operate on
 * @param r1 int the first row to swap
 * @param r2 int the second row to swap
 */
void swapRows(MATRIX *m, int r1, int r2);

/**
 * @brief computes and returns the dot product of two vectors
 *
 * @param v1 MATRIX the first operand vector
 * @param v2 MATRIX the second operand vector
 * @return double the value of the dot product
 */
double vectorDotProduct(MATRIX *v1, MATRIX *v2);

/**
 * @brief computes and returns the cross product of two vectors
 *
 * @param v1 MATRIX the first operand vector
 * @param v2 MATRIX the second operand vector
 * @return MATRIX the value of the cross product
 */
MATRIX *vectorCrossProduct(MATRIX *v1, MATRIX *v2);

/**
 * @brief computes and returns the norm of a vector
 *
 * @param v MATRIX the vector to read from
 * @return double the vector's length
 */
double vectorLength(MATRIX *v);

/**
 * @brief rescales a vector so that it has length 1 in-place
 *
 * @param v MATRIX the vector to normalize
 */
void normalizeVector(MATRIX *v);

/**
 * @brief returns truthy if every element of a matrix is 0
 *
 * @param m MATRIX the matrix to read from
 * @return int the boolean result
 */
int isZeroMatrix(MATRIX *m);

/**
 * @brief pretty prints a MATRIX struct
 *
 * @param m MATRIX the matrix to read from
 */
void printMatrix(MATRIX *m);

/**
 * @brief pretty prints a MATRIX struct that is a column vector
 *
 * @param m MATRIX the matrix to read from
 */
void printVector3(MATRIX *m);

/**
 * @brief Frees the data of a matrix
 * 
 * @param m The matrix to free
 */
void freeMatrix(MATRIX *m);

#endif