#include <limits>
#include "RayTracer.h"
#include "Image.h"
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"
#include "Intersection.h"
#include "Light.h"
#include "Air.h"
#include "ShinyColor.h"
#include "FlatColor.h"
#include "Glass.h"
#include "ImageOther.h"

using namespace std;

void clamp(Color& col, double min, double max) {
	col.r = (col.r > max) ? max : (col.r < min) ? min : col.r;
	col.g = (col.g > max) ? max : (col.g < min) ? min : col.g;
	col.b = (col.b > max) ? max : (col.b < min) ? min : col.b;
}

Intersection RayTracer::getClosestIntersection(const Ray& ray) {
	Intersection closestIntersection;
	for (vector<Object*>::iterator itr = objects.begin(); itr < objects.end(); itr++) {
		Intersection intersection = (*itr)->intersect(ray);

		if (intersection.didIntersect && intersection.distance <
			closestIntersection.distance) {
			closestIntersection = intersection;
		}
	}
	return closestIntersection;
}

Color RayTracer::spekularLight(const Intersection &intersec, const Vector &lightDirVec) {
	Color specColor(0., 0., 0.);
	if (intersec.endMaterial->getShininess() == -1) {
		return specColor;
	}
	Vector betrachterRay(intersec.ray.origin - intersec.intersection);
	betrachterRay.normalize();
	Vector n(intersec.normal);
	n.normalize();

	Vector reflected(n * 2 * lightDirVec.dot(n) - lightDirVec);
	reflected.normalize();
	double test = n.dot(lightDirVec);//betrachterRay.dot(reflected);
	if (test < 0.) {
		return Color(0., 0., 0.);
	}
	double factor = pow(reflected.dot(betrachterRay), intersec.endMaterial->getShininess());
	specColor = Color(factor, factor, factor);
	return specColor;
}

Color RayTracer::calcDiffuseAndSpec(const Intersection& intersec) {
	Light* light = this->lights[0];
	Color diffColor(0., 0., 0.);
	Color specColor(0., 0., 0.);
	Vector lightDirVec(light->position - intersec.intersection);
	lightDirVec.normalize();
	double diffusion = intersec.normal.dot(lightDirVec);
	if (diffusion < 0.) {
		diffusion = 0.;
	}
	diffColor = intersec.getColor()*diffusion*light->intensity;
	specColor = spekularLight(intersec, lightDirVec) * light->intensity;
	return /*kDiff*/ diffColor * 1 + /*kSpec*/specColor * 1.;
}

Color RayTracer::calcAmbient(const Color& color) {
	return color*0.1;
}

Color RayTracer::performLighting(const Intersection& intersection) {
	Color color(0., 0., 0.);
	Color DiffAndSpec(calcDiffuseAndSpec(intersection));
	Color ambient(calcAmbient(intersection.getColor()));
	Color reflectiveAndRefractive(getReflectiveRefractiveLighting(intersection));
	color = ambient + DiffAndSpec + reflectiveAndRefractive;
	clamp(color, 0., 1.);
	return color;
}

RayTracer::RayTracer(int width_, int height_, int maxReflections_, int superSamples_, int depthComplexity_, std::string img)
	: width(width_), height(height_), maxReflections(maxReflections_), superSamples(superSamples_), camera(Camera()),
	imageScale(1), depthComplexity(depthComplexity_), dispersion(5.0f), raysCast(0), startingMaterial(new Air()) {
	this->imgName = img;
	this->readScene();
	std::cout << this->objects.size() << " Objects added." << std::endl;
	this->traceRays();
}

RayTracer::~RayTracer() {
   for (vector<Object*>::iterator itr = objects.begin(); itr < objects.end(); itr++) {
      delete *itr;
   }

   for (vector<Light*>::iterator itr = lights.begin(); itr < lights.end(); itr++) {
      delete *itr;
   }

   delete startingMaterial;
}

void RayTracer::traceRays() {
	Image image(this->width, this->height);
	ImageOther otherImage(this->width, this->height, "outOther.ppm");
   int columnsCompleted = 0;
   camera.calculateWUV();
   this->imageScale = camera.screenWidth / (double)width;

   #pragma omp parallel for
   for (int x = 0; x < width; x++) {
      columnsCompleted++;
      float percentage = columnsCompleted/(double)width * 100;
      cout << '\r' << (int)percentage << '%';
      fflush(stdout);
      for (int y = 0; y < height; y++) {
		  Color col = castRayForPixel(x, y);
		  otherImage.putColor(y, x, col);
      }
   }
   cout << "\rDone!" << endl;
   cout << "Rays cast: " << raysCast << endl;
   otherImage.writeImage();
}

Color RayTracer::castRayForPixel(int x, int y) {
   double rayX = (x - width / 2)/2.0;
   double rayY = (y - height / 2)/2.0;
   double pixelWidth = rayX - (x + 1 - width / 2)/2.0;
   double sampleWidth = pixelWidth / superSamples;
   double sampleStartX = rayX - pixelWidth/2.0;
   double sampleStartY = rayY - pixelWidth/2.0;
   double sampleWeight = 1.0 / (superSamples * superSamples);
   Color color(0.,0.,0.);

   for (int x = 0; x < superSamples; x++) {
      for (int y = 0; y < superSamples; y++) {
         Vector imagePlanePoint = camera.lookAt -
          (camera.u * (sampleStartX + (x * sampleWidth)) * imageScale) +
          (camera.v * (sampleStartY + (y * sampleWidth)) * imageScale);
		 Ray lanaDelRay(camera.position, imagePlanePoint - camera.position, maxReflections,startingMaterial);	
         color = color + (castRay(lanaDelRay) * sampleWeight);
      }
   }
   return color;
}

Color RayTracer::castRay(const Ray& ray) {
   raysCast++;
   for (int o = 0; o < objects.size(); o++) {
	   Color color(0., 0., 0.);
	   Object* obj = objects[o];
	   Intersection inter = this->getClosestIntersection(ray);	//Slows down the whole thing. TODO: Binary Search Tree or sth.
	   if (inter.didIntersect) {
		   if (dynamic_cast<Triangle*>(obj)) {
			   int a = 0;
		   }
		   return performLighting(inter);
	   }
   }
   return Color(0.,0.,0.);
}

Color RayTracer::getReflectiveRefractiveLighting(const Intersection& intersection) {
   double reflectivity = intersection.endMaterial->getReflectivity();
   double startRefractiveIndex = intersection.startMaterial->getRefractiveIndex();
   double endRefractiveIndex = intersection.endMaterial->getRefractiveIndex();
   int reflectionsRemaining = intersection.ray.reflectionsRemaining;

   if (reflectivity == NOT_REFLECTIVE && endRefractiveIndex == NOT_REFRACTIVE ||
    reflectionsRemaining <= 0) {
      return Color();
   }

   double reflectivePercentage = reflectivity;
   double refractivePercentage = 0;

   if (endRefractiveIndex != NOT_REFRACTIVE) {
      reflectivePercentage = getReflectance(intersection.normal, intersection.ray.direction, startRefractiveIndex, endRefractiveIndex);
      refractivePercentage = 1 - reflectivePercentage;
   }

   if (refractivePercentage <= 0 && reflectivePercentage <= 0) {
      return Color();
   }
   Color reflectiveColor;
   Color refractiveColor;
   if (reflectivePercentage > 0) {
      Vector reflected = reflectVector(intersection.ray.origin,
       intersection.normal);
      Ray reflectedRay(intersection.intersection, reflected, reflectionsRemaining - 1,
       intersection.ray.material);
      reflectiveColor = castRay(reflectedRay) * reflectivePercentage;
   }
   if (refractivePercentage > 0) {
      Vector refracted = refractVector(intersection.normal,
       intersection.ray.direction, startRefractiveIndex, endRefractiveIndex);
      Ray refractedRay = Ray(intersection.intersection, refracted, 1,
       intersection.endMaterial);

      refractiveColor = castRay(refractedRay) * refractivePercentage;
   }

   return reflectiveColor + refractiveColor;
}

double RayTracer::getReflectance(const Vector& normal, const Vector& incident, double n1, double n2) {
   double n = n1 / n2;
   double cosI = -normal.dot(incident);
   double sinT2 = n * n * (1.0 - cosI * cosI);
   if (sinT2 > 1.0) {
      // Total Internal Reflection.
      return 1.0;
   }
   double cosT = sqrt(1.0 - sinT2);
   double r0rth = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
   double rPar = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);
   return (r0rth * r0rth + rPar * rPar) / 2.0;
}

Vector RayTracer::refractVector(const Vector& normal, const Vector& incident, double n1, double n2) {
   double n = n1 / n2;
   double cosI = -normal.dot(incident);
   double sinT2 = n * n * (1.0 - cosI * cosI);

   if (sinT2 > 1.0) {
      cerr << "Bad refraction vector!" << endl;
      exit(EXIT_FAILURE);
   }

   double cosT = sqrt(1.0 - sinT2);
   return incident * n + normal * (n * cosI - cosT);
}

Vector RayTracer::reflectVector(Vector vector, Vector normal) {
   return normal * 2 * vector.dot(normal) - vector;
}

void RayTracer::readScene() {
	this->camera.position = Vector(1500.,0.,0.);
	this->camera.lookAt = Vector(110., 0., 0.);
	this->camera.up = Vector(0., 1., 0.);
	this->camera.screenWidth = 2000;
	
	Light* light = new Light(Vector(700, 0, 250), 0.9);

	Sphere* sphere = new Sphere(Vector(-100., 0., 0.), 80, new FlatColor(1., 0., 0.));
	Sphere* sphere1 = new Sphere(Vector(200., 100., 0.), 80, new ShinyColor(0.592, 1., 1., 50, 0.9));
	Sphere* sphere2 = new Sphere(Vector(250., -100., 150.), 50, new Glass(1.5, 50));
	Sphere* sphere3 = new Sphere(Vector(200., -100., 0.), 80, new FlatColor(0., 0., 1.));
	Sphere* glas2 = new Sphere(Vector(200., 0., -100.), 50, new Glass(1.5, 50));
	Triangle* xy = new Triangle(Vector(-1000, 300, -1000.), Vector(1000., 300., 0.), Vector(-1000., 300., 1000.), new FlatColor(0.7, 1., 0.));// , 50, 1.));
	Triangle* tri = new Triangle(Vector(-300, 100, 0.), Vector(-250., -100., 0.), Vector(-200., 100., 0.), new FlatColor(0.7, 0.2, 1.));// , 50, 1.));
	Triangle* test = new Triangle(Vector(0, 100, 200.), Vector(0., -100., 0.), Vector(0., 100., -200.), new FlatColor(0.7, 0.2, 1.));// , 50, 1.));

	this->addLight(light);	
	this->addObject(sphere3);
	this->addObject(sphere1);
	this->addObject(sphere2);
	this->addObject(sphere);	
	//this->addObject(tri);
	//this->addObject(test);
	this->addObject(glas2);
}
