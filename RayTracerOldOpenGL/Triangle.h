#pragma once

#include "Object.h"
#include <vector>

using namespace std;

class Triangle : public Object {
private:
	vector<Vector3d*> edges;

public:
	Triangle(vector<Vector3d*> edges, Vector3d* col, int glanzFaktor = 1, double kSpec = 0.1, double kDiff = 1.) : Object(col, glanzFaktor, kSpec, kDiff) {
		this->edges = edges;
	}

	~Triangle() {}

	bool intersect(Ray ray, Intersection& inter) {
		Vector3d orig = *ray.getOrg();
		Vector3d dir = *ray.getDir();
		Vector3d v0 = *this->edges[0];
		Vector3d v1 = *this->edges[1];
		Vector3d v2 = *this->edges[2];

		Vector3d v0v1 = v1 - v0;
		Vector3d v0v2 = v2 - v0;
		Vector3d N = v0v1.cross(v0v2);		  // Normal

		double NdotRayDirection = N.dot(dir);
		if (fabs(NdotRayDirection) < 1e-10) { // Check if parallel.
			return false;
		}
		double d = N.dot(v0);
		double t = (N.dot(orig) + d) / NdotRayDirection;
		if (t < 0) {						 // Check if Triangle is behind the Ray.
			return false;
		}
		Vector3d P = orig + t * dir;
		//test if Intersect Point is Inside the Triangle.		
		Vector3d edge0 = v1 - v0;
		Vector3d vp0 = P - v0;
		Vector3d C = edge0.cross(vp0);
		if (N.dot(C) < 0) {
			return false;
		}
		Vector3d edge1 = v2 - v1;
		Vector3d vp1 = P - v1;
		C = edge1.cross(vp1);
		if (N.dot(C) < 0) {
			return false;
		}
		Vector3d edge2 = v0 - v2;
		Vector3d vp2 = P - v2;
		C = edge2.cross(vp2);
		if (N.dot(C) < 0) {
			return false;
		}
		N.normalize();
		inter.normal = new Vector3d(N);
		inter.hit = true;
		inter.intersection = new Vector3d(P.matrix());
		return true;
	}
};