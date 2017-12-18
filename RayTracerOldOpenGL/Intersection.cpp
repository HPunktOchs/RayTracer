#include "Eigen/Geometry"

class Ray;

using namespace Eigen;

class Intersection {
	public:
		Vector3d* normal;
		Vector3d* intersection;
		Ray* ray;
		bool hit;

		Intersection() {}
		~Intersection() {}
};