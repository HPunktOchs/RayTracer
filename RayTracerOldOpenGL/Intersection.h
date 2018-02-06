#pragma once

#include <Eigen/Geometry>

using namespace Eigen;

class Intersection {
public:
	Vector3d* normal;
	Vector3d* intersection;
	// Ray* ray;
	bool hit;

	Intersection() {}
	~Intersection() {
		delete this->normal;
		delete this->intersection;
	}
};