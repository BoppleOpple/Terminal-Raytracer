#include "debug.h"

unsigned int delta_us(struct timespec start, struct timespec end) {
	// just do math
	return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
}

float delta_s(struct timespec start, struct timespec end) {
	// and rescale the previous math
	return (float) delta_us(start, end) / 1000000.0;
}