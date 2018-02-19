#ifndef __SHINY_COLOR_H__
#define __SHINY_COLOR_H__

#include "Material.h"

class ShinyColor : public Material {
public:
   Color color;
   double shininess;
   double reflectivity;

   ShinyColor::ShinyColor(double r, double g, double b, double shininess, double reflectivity) {
	   this->shininess = shininess;
	   this->reflectivity = reflectivity;
	   this->color.r = r;
	   this->color.g = g;
	   this->color.b = b;
   }

   Color ShinyColor::getColor() {
	   return color;
   }

   double getShininess() {
	   return shininess;
   }

   double getReflectivity() {
	   return reflectivity;
   }
};

#endif
