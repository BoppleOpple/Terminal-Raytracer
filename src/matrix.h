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
 * @brief Create a Vector object
 * 
 * @param x 
 * @param y 
 * @param z 
 * @return MATRIX 
 */
MATRIX createVector(double x, double y, double z);

void addScalar(MATRIX *m, double s);

void multScalar(MATRIX *m, double s);

MATRIX multMatrix(MATRIX *m1, MATRIX *m2);

void fillMatrix(MATRIX *m, double n);

double getElement(MATRIX *m, int r, int c);

void setElement(MATRIX *m, int r, int c, double val);

void printMatrix(MATRIX *m);

/**
 * @brief Frees the data of a matrix
 * 
 * @param m The matrix to free
 */
void freeMatrix(MATRIX *m);

#endif