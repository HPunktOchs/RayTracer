#include <Eigen/Geometry>
#include <iostream>

using namespace Eigen;

class Ray {
private:
	Vector3d* org;
	Vector3d* dir;

public:
	Ray(Vector3d* org, Vector3d* dir) {
		this->org = org;
		this->dir = dir;
		//std::cout << "dir: " <<dir->matrix() << std::endl;
		//std::cout << "org: " << org->matrix() << std::endl;
	}

	Vector3d* getOrg() const {
		return this->org;
	}

	Vector3d* getDir() const {
		return this->dir;
	}
};

