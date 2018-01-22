#pragma once

#include "Object.h"


class Sphere : public Object {
private:
	Vector3d* center;
	double r;
public:
	Sphere(Vector3d* center, double r, Vector3d* col, int glanzFaktor = 1, double kSpec = 0.1, double kDiff = 1., double pTransp = 0., double pRefl = 0.) : Object(col, glanzFaktor, kSpec, kDiff, pTransp, pRefl) {
		this->center = center;
		this->r = r;
	}

	~Sphere() {}

	//bool intersect(Ray ray, Intersection& inter) {
	//	// Simple sphere intersection. To be used with direct rays.
	//	Vector3d sphere_origin = this->center->matrix();
	//	double sphere_radius2 = this->r;

	//	float alpha = -ray.getDir()->matrix().dot((ray.getOrg()->matrix() - this->center->matrix()));
	//	Vector3d q = ray.getOrg()->matrix() + alpha * ray.getDir()->matrix() - sphere_origin;

	//	float distToCenter2 = q.dot(q);
	//	if (distToCenter2 > sphere_radius2)
	//		return false;

	//	float a = sqrt(sphere_radius2 - distToCenter2);
	//	if (alpha >= a)
	//		alpha -= a;
	//	else if (alpha + a > 0.0)
	//		alpha += a;
	//	else
	//		return false;

	//	inter.hit = true;
	//	inter.intersection = new Vector3d(ray.getOrg()->matrix() + alpha * ray.getDir()->matrix());
	//	Vector3d tmp(inter.intersection->matrix() - sphere_origin);
	//	tmp.normalize();
	//	inter.normal = new Vector3d(tmp);
	//	return true;
	//}

	bool intersect(Ray ray, Intersection& inter) {
		// Simple sphere intersection. To be used with direct rays.
		Vector3d deltap(ray.getOrg()->matrix() - this->center->matrix());
		double a = ray.getDir()->dot(ray.getDir()->matrix());
		double b = deltap.dot(ray.getDir()->matrix()) * 2;
		double c = deltap.dot(deltap) - (this->r * this->r);
		double disc = b * b - 4 * a * c;
		if (disc < 0) {
			return false; // No intersection.
		}
		disc = sqrt(disc);
		double q;
		if (b < 0) {
			q = (-b - disc) * 0.5;
		}
		else {
			q = (-b + disc) * 0.5;
		}
		double r1 = q / a;
		double r2 = c / q;
		if (r1 > r2) {
			double tmp = r1;
			r1 = r2;
			r2 = tmp;
		}
		double distance = r1;
		if (distance < 0) {
			distance = r2;
		}
		if (distance < 0 || isnan(distance)) {
			return false; // No intersection.
		}
		inter.hit = true;
		inter.intersection = new Vector3d(ray.getOrg()->matrix() + distance * ray.getDir()->matrix());
		Vector3d tmp(inter.intersection->matrix() - this->center->matrix());
		tmp.normalize();
		inter.normal = new Vector3d(tmp);
		return true;
	}


	Vector3d* getCenter() {
		return this->center;
	}

	Vector3d* getNormal(Vector3d* pi) {
		return new Vector3d((pi->matrix() - this->center->matrix()) / this->r);
	}

	
};