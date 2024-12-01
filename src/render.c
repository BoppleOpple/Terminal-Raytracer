#include "render.h"
#include <stdio.h>
#include <unistd.h>

void updateViewportSize(struct winsize *destination) {
	ioctl(STDIN_FILENO, TIOCGWINSZ, destination);
}

void clearScreen() {
	printf("\033[2J\033[H");
}