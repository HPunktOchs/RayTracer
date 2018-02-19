#include <algorithm>
#include <math.h>
#include "Triangle.h"
#include "Material.h"

//Intersection Triangle::intersect(Ray ray) {
//  return Intersection();
//  Vector e1, e2, h, s, q, normal;
//  double a, f, u, v, distance;
//  e1 = Vector(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
//  e2 = Vector(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
//  normal = ((v1 - v0).cross(v2 - v0)).normalize();
//  h = ray.direction.cross(e2);
//  a = e1.dot(h);
//  if (a > -0.00001 && a < 0.00001) {
//	  return Intersection();
//  }
//  f = 1 / a;
//  s = Vector(ray.origin.x - v0.x, ray.origin.y - v0.y, ray.origin.z - v0.z);
//  u = f * s.dot(h);
//  if (u < 0.0 || u > 1.0) {
//	  return Intersection();
//  }
//  q = s.cross(e1);
//  v = f * ray.direction.dot(q);
//  if (v < 0.0 || u + v > 1.0) {
//	  return Intersection();
//  }
//  distance = f * e2.dot(q);
//  // Ray Intersection
//  if (distance > 0.00001) {
//     Vector point = ray.origin + Vector(distance) * ray.direction;
//     return Intersection(ray, point, distance, normal, ray.material, material, this);
//  }
//  return Intersection();
//}

Intersection Triangle::intersect(Ray ray) {
	double u, v, t;
	Vector dir = ray.direction;
	Vector v0v1(this->v1 - this->v0);
	Vector v0v2(this->v2 - this->v0);
	Vector pvec(dir.cross(v0v2));
	double det = v0v1.dot(pvec);

	// if the determinant is negative the triangle is backfacing
	// if the determinant is close to 0, the ray misses the triangle
	//if (det < 1e-10) return false;
	if (fabs(det) < 1e-10) {
		return Intersection();
	}
	double invDet = 1 / det;

	Vector tvec = ray.origin - v0;
	u = tvec.dot(pvec) * invDet;
	if (u < 0 || u > 1) return Intersection();

	Vector qvec = tvec.cross(v0v1);
	v = dir.dot(qvec) * invDet;
	if (v < 0 || u + v > 1) return Intersection();

	Intersection success;
	t = v0v2.dot(qvec) * invDet;
	Vector p = this->v0 + v0v1*u + v0v2*v; // A + u(B−A) + v(C−A)
	Vector normal = v0v1.cross(v0v2);
	normal.normalize();
	double distance = (p - ray.origin).length();
	return Intersection(ray, p, distance, normal, ray.material, material, this);
}
