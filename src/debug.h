#include <time.h>

#ifndef RAYTRACE_DEBUG
#define RAYTRACE_DEBUG

unsigned int delta_us(struct timespec start, struct timespec end);

float delta_s(struct timespec start, struct timespec end);

#endif