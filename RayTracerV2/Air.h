#ifndef __AIR_H__
#define __AIR_H__

#include "Material.h"

class Air : public Material {
public:
   Air() {}

   Color getColor() {
	   return Color(0.0, 0.0, 0.0);
   }

   double getRefractiveIndex() {
	   return AIR_REFRACTIVE_INDEX;
   }
};

#endif
