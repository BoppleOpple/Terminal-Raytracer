#include <time.h>

#ifndef RAYTRACE_DEBUG
#define RAYTRACE_DEBUG

/**
 * @brief returns the number of microseconds between two timespec structs
 *
 * @param start struct timespec the start timespec struct
 * @param end struct timespec the end timespec struct
 * @return uint the delta in microseconds
 */
unsigned int delta_us(struct timespec start, struct timespec end);

/**
 * @brief returns the time in seconds between two timespec structs
 *
 * @param start struct timespec the start timespec struct
 * @param end struct timespec the end timespec struct
 * @return float the delta in seconds
 */
float delta_s(struct timespec start, struct timespec end);

#endif