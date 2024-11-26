#include "matrix.h"
#include "mesh.h"

int main(int argc, char *argv[]) {
	MATRIX m = createMatrix(4, 2);

	fillMatrix(&m, 1.5);
	setElement(&m, 2, 1, 5.0);

	meshFromOBJ("./res/suzanne.obj");
	printMatrix(&m);
}