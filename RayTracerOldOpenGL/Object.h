#pragma once

#include <iostream>
#include "Ray.h"
#include "Intersection.h"


class Object {
public:
	Object(Vector3d* color, int glanzFaktor, double kSpec, double kDiff, double pTransp, double pRefl) {
		this->color = color;
		this->glanzFaktor = glanzFaktor;
		this->kSpec = kSpec;
		this->kDiff = kDiff;
		this->pTransp = pTransp;
		this->pRefl = pRefl;
	}
	virtual ~Object() {}
	virtual bool intersect(const Ray& ray, Intersection& inter) = 0;
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

	double getPTransp() {
		return this->pTransp;
	}

	double getPRefl() {
		return this->pRefl;
	}

private:
	Vector3d* color;
	int glanzFaktor;
	double kSpec;
	double kDiff;
	double pTransp;
	double pRefl;
};