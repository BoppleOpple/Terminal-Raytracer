#ifndef RAYTRACE_MATRIX
#define RAYTRACE_MATRIX

typedef struct {
	int rows;
	int cols;
	double *data;
} MATRIX;

/**
 * @brief Create a Matrix struct
 *
 * Creates a new matrix and fills it with zeros 
 *
 * @param r Number of rows
 * @param c Number of columns
 * @return MATRIX New zero matrix
 */
MATRIX createMatrix(int r, int c);

/**
 * @brief Create a square identity matrix
 * 
 * @param s dimensions of the square matrix (i.e. 4 -> 4x4 matrix)
 * @return MATRIX 
 */
MATRIX createIdentityMatrix(int s);

MATRIX copyMatrix(MATRIX *m);

/**
 * @brief Create a Vector object
 * 
 * @param x 
 * @param y 
 * @param z 
 * @return MATRIX 
 */
MATRIX createVector(double x, double y, double z);

void addScalar(MATRIX *m, double s);

MATRIX addMatrix(MATRIX *m1, MATRIX *m2);

void multScalar(MATRIX *m, double s);

MATRIX multMatrix(MATRIX *m1, MATRIX *m2);

MATRIX multMatrixElementwise(MATRIX *m1, MATRIX *m2);

void fillMatrix(MATRIX *m, double n);

double getElement(MATRIX *m, int r, int c);

void setElement(MATRIX *m, int r, int c, double val);

double minor(MATRIX *m, int r, int c);

double cofactor(MATRIX *m, int r, int c);

double determinant(MATRIX *m);

MATRIX inverse(MATRIX *m);

double vectorDotProduct(MATRIX *v1, MATRIX *v2);

MATRIX vectorCrossProduct(MATRIX *v1, MATRIX *v2);

double vectorLength(MATRIX *v);

void normalizeVector(MATRIX *v);

int isZeroMatrix(MATRIX *m);

void printMatrix(MATRIX *m);

void printVector3(MATRIX *m);

/**
 * @brief Frees the data of a matrix
 * 
 * @param m The matrix to free
 */
void freeMatrix(MATRIX *m);

#endif