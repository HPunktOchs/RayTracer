#ifndef __PLANE_H__
#define __PLANE_H__

#include "Intersection.h"

class Plane : public Object {
public:
	Vector p0;
	Vector n;
	Material* m;

	Plane(Vector p0_, Vector n_, Material* m_) : p0(p0_), n(n_.normalize()), m(m_) {}

	~Plane() {
		delete this->m;
	}

	virtual Intersection intersect(Ray ray) {
		double denom = this->n.dot(ray.direction);
		if (denom > 1e-6) {
			Vector p0l0 = this->p0 - ray.origin;
			double distance = p0l0.dot(n) / denom;
			if (distance >= 0) {
				Vector p = ray.origin + ray.direction*distance;
				return Intersection(ray,p,distance,this->n, ray.material, this->m, this);
			}
		}
		return Intersection();
	}


};
#endif