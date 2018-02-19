#ifndef __OBJECT_H__
#define __OBJECT_H__

class Intersection;
class Ray;

class Object {
public:
   virtual Intersection intersect(Ray) = 0;
   //virtual Boundaries getBounds() = 0;
};

#endif
