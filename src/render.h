#include <sys/ioctl.h>

#ifndef RAYTRACE_RENDER
#define RAYTRACE_RENDER

void updateViewportSize(struct winsize *destination);

void clearScreen();

#endif