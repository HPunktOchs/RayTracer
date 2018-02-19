#ifndef __FLAT_COLOR_H__
#define __FLAT_COLOR_H__

#include "Material.h"

class FlatColor : public Material {
private:
   Color color;

public:
   FlatColor::FlatColor(double r, double g, double b) {
	   this->color.r = r;
	   this->color.g = g;
	   this->color.b = b;
   }

   virtual Color getColor() {
	   return color;
   }
};

#endif
