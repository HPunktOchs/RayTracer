#ifndef __GLASS_H__
#define __GLASS_H__

#include "Material.h"

class Glass : public Material {
private:
   double refractiveIndex;
   double shininess;

public:
	Glass::Glass(double refrIndex, double shininess) {
		this->refractiveIndex = refrIndex;
		this->shininess = shininess;
	}

	Color Glass::getColor() {
		return Color(0.0, 0.0, 0.0);
	}

	double Glass::getRefractiveIndex() {
		return refractiveIndex;
	}

	double Glass::getShininess() {
		return shininess;
	}
};

#endif
