#ifndef __RAY_TRACER_H__
#define __RAY_TRACER_H__

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "Vector.h"
#include "Camera.h"
#include "Image.h"

class Ray;
class Color;
class Intersection;
class Object;
class Light;
class Material;

class RayTracer {
public:
   int width;
   int height;
   std::string imgName;
   int maxReflections;
   int superSamples;
   Camera camera;
   double imageScale;
   int depthComplexity;
   double dispersion;
   unsigned long long raysCast;
   Material* startingMaterial;

   std::vector<Object*> objects;
   std::vector<Light*> lights;
   std::map<std::string, Material*> materials;

   RayTracer(int width_, int height_, int maxReflections_, int superSamples_, int depthComplexity_, std::string img);
   ~RayTracer();
   void addObject(Object* object) {
      objects.push_back(object);
   }
   void addLight(Light* light) {
      lights.push_back(light);
   }
   void traceRays();
   void readScene();

private:
   Color castRayForPixel(int, int);
   Color castRay(const Ray&);
   Color performLighting(const Intersection&);
   Color calcAmbient(const Color& color);
   Color getDiffuseAndSpecularLighting(const Intersection&, const Color&);
   Color spekularLight(const Intersection &intersec, const Vector &lightDirVec);
   Color getReflectiveRefractiveLighting(const Intersection&);
   double getReflectance(const Vector&, const Vector&, double, double);
   Vector refractVector(const Vector&, const Vector&, double, double);
   Vector reflectVector(Vector, Vector);
   Intersection getClosestIntersection(const Ray& ray);


   Color calcDiffuseAndSpec(const Intersection& intersec);
};

#endif
