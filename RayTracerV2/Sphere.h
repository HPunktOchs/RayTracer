#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <math.h>
#include "Vector.h"
#include "Ray.h"
#include "Intersection.h"
#include "Object.h"

class Material;

class Sphere : public Object {
public:
   Vector center;
   double radius;
   Material* material;

   Sphere(Vector center_, double radius_, Material* material_) : center(center_),
    radius(radius_), material(material_) {}

   virtual Intersection intersect(Ray);
};

#endif
