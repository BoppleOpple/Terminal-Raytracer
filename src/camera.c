#include "camera.h"

CAMERA createCamera(double focalLength) {
	return (CAMERA) {
		focalLength,
		createTransform()
	};
}