#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#define NOT_SHINY -1
#define NOT_REFLECTIVE -1
#define NOT_REFRACTIVE -1
#define AIR_REFRACTIVE_INDEX 1

#include <stdlib.h>
#include <iostream>
#include "Color.h"

class Vector;
class Color;
class NormalMap;

class Material {
public:
	double kSpec;
	double kDiff;

   virtual Color getColor() = 0;
   virtual double getShininess() {
	   return NOT_SHINY;
   }

   virtual double getReflectivity() {
	   return NOT_REFLECTIVE;
   }

   virtual double getRefractiveIndex() {
	   return NOT_REFRACTIVE;
   }

};

#endif
