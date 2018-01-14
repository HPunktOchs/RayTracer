#pragma once

#include <iostream>
#include "Ray.h"
#include "Intersection.h"


class Object {
public:
	Object(Vector3d* color, int glanzFaktor = 1, double kSpec = 0.1, double kDiff = 1.) {
		this->color = color;
		this->glanzFaktor = glanzFaktor;
		this->kSpec = kSpec;
		this->kDiff = kDiff;
	}
	virtual ~Object() {}
	virtual bool intersect(Ray ray, Intersection& inter) = 0;
	// virtual Vector3d* getNormal(Vector3d* pi) = 0;

	Vector3d* getColor() {
		return this->color;
	}

	int getGlanzFaktor() {
		return this->glanzFaktor;
	}

	double getKSpec() {
		return this->kSpec;
	}

	double getKDiff() {
		return this->kDiff;
	}

private:
	Vector3d* color;
	int glanzFaktor;
	double kSpec;
	double kDiff;
};