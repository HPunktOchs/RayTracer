#include "Sphere.h"
#include "Triangle.h"
#include "Image.h"

using namespace std;


const int xWidth = 500;
const int yWidth = 500;
const int reflectiveMaxDepth = 5;
Image img(xWidth, yWidth);

Sphere light(new Vector3d(xWidth *0.5, yWidth*0.4, 200.), 1, new Vector3d(255., 255., 255.));
vector<Object*> objects;

Vector3d spekularLight(const Intersection &intersec, const Vector3d &lightDirVec, const Ray& ray, Object* obj);
Vector3d doLighting(Object* obj, const Intersection& intersec, Ray ray, const int& depth);
Vector3d calcAmbient(Object* obj);
Vector3d calcDiffuseAndSpec(Object* obj, const Intersection& intersec, const Ray& ray);
Vector3d traceRay(const Ray& ray, const int& depth);
Vector3d reflectiveRefractive(Object* obj, const Intersection& intersec, Ray ray, const int& depth);
Vector3d reflVector(const Vector3d& vec, const Vector3d& normal);

void clamp255(Vector3d& col) {
	col[0] = (col[0] > 255) ? 255 : (col[0] < 0) ? 0 : col[0];
	col[1] = (col[1] > 255) ? 255 : (col[1] < 0) ? 0 : col[1];
	col[2] = (col[2] > 255) ? 255 : (col[2] < 0) ? 0 : col[2];
}

void rayCasting() {				
		for (int y = 0; y < yWidth; y++) {		
			for (int x = 0; x < xWidth; x++) {
				Ray ray(new Vector3d(x, y, 0), new Vector3d(0, 0, 1));
				Vector3d pix_color(traceRay(ray, 0));
				img.putColor(x, y, pix_color);
			}
		}	
		img.writeImage();
}

Vector3d traceRay(const Ray& ray, const int& depth) {
	Vector3d pix_color(0.,0.,0.);
	for (int o = 0; o < objects.size(); o++) {
		Object* obj = objects[o];
		Intersection intersec;
		bool check = obj->intersect(ray, intersec);
		if (check) {
			if (dynamic_cast<Sphere*>(obj)) {
				int i = 0;
			}
			pix_color = doLighting(obj, intersec, ray, depth);
		}
	}
	return pix_color;
}

Vector3d doLighting(Object* obj, const Intersection& intersec, Ray ray, const int& depth) {
	Vector3d color(0., 0., 0.);
	Vector3d DiffAndSpec(calcDiffuseAndSpec(obj,intersec,ray));
	Vector3d ambient(calcAmbient(obj));
	//Vector3d reflectiveAndRefractive(reflectiveRefractive(obj, intersec, ray, depth));
	color = ambient + DiffAndSpec;// +reflectiveAndRefractive;
	clamp255(color);
	return color;
}

//Vector3d calcDiffuseAndSpec(Object* obj, const Intersection& intersec, const Ray& ray) {
//	Vector3d lightDirVec(light.getCenter()->matrix() - intersec.intersection->matrix());
//	Vector3d betrachterRay(ray.getOrg()->matrix() - intersec.intersection->matrix());
//	betrachterRay.normalize();
//	lightDirVec.normalize();
//	double diffusion = intersec.normal->matrix().dot(lightDirVec);
//	if (diffusion < 0.) {
//		diffusion = 0.;
//	}
//	double spec = pow(spekularLight(intersec, lightDirVec).dot(betrachterRay.matrix()), obj->getGlanzFaktor());
//	return obj->getColor()->matrix()* (obj->getKDiff()*diffusion + obj->getKSpec()*spec);
//}

Vector3d calcDiffuseAndSpec(Object* obj, const Intersection& intersec, const Ray& ray) {
	Vector3d diffColor(0., 0., 0.);
	Vector3d specColor(0., 0., 0.);
	Vector3d lightDirVec(light.getCenter()->matrix() - intersec.intersection->matrix());
	lightDirVec.normalize();
	double diffusion = intersec.normal->matrix().dot(lightDirVec);
	if (diffusion < 0.) {
		diffusion = 0.;
	}
	diffColor = obj->getColor()->matrix()*diffusion;
	specColor = spekularLight(intersec, lightDirVec, ray, obj).matrix();
	return obj->getKDiff()*diffColor + obj->getKSpec()*specColor;
}

Vector3d calcAmbient(Object* obj) {
	//return light.getColor()->matrix()*0.2;		 
	return obj->getColor()->matrix()*0.1;
}

//Vector3d spekularLight(const Intersection &intersec, const Vector3d &lightDirVec) {
//	Vector3d n(intersec.normal->matrix());
//	n.normalize();
//	double test = n.dot(lightDirVec.matrix());
//	if (test < 0.) { 
//		return Vector3d(0., 0., 0.);
//	}
//	Vector3d reflVec(2*n*(test)-lightDirVec.matrix());
//	return reflVec;
//}

Vector3d spekularLight(const Intersection &intersec, const Vector3d &lightDirVec, const Ray& ray, Object* obj) {
	Vector3d specColor(0., 0., 0.);
	if (obj->getGlanzFaktor() == 0) {
		return specColor;
	}
	Vector3d betrachterRay(ray.getOrg()->matrix() - intersec.intersection->matrix());
	betrachterRay.normalize();
	Vector3d n(intersec.normal->matrix());
	n.normalize();
	Vector3d reflected(reflVector(lightDirVec,n));
	reflected.normalize();
	double test = n.dot(lightDirVec.matrix());//betrachterRay.dot(reflected);
	if (test < 0.) {
		return Vector3d(0., 0., 0.);
	}
	double factor = pow(reflected.dot(betrachterRay.matrix()), obj->getGlanzFaktor());
	specColor = factor*light.getColor()->matrix();
	return specColor;
}

Vector3d reflVector(const Vector3d& vec, const Vector3d& normal) {
	return normal*2* vec.dot(normal) - vec;
}

Vector3d refrVector(const Vector3d& normal, const Vector3d& incident, double n1, double n2) {
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

//Vector3d reflectiveRefractive(Object* obj, const Intersection& intersec, Ray ray, const int& depth) {
//	if (obj->getPRefl() < 1e-4 && obj->getPTransp() < 1e-4 || (depth == reflectiveMaxDepth)) {
//		return Vector3d(0., 0., 0.);
//	}
//	Vector3d refl(0.,0.,0.);
//	Vector3d refr(0., 0., 0.);
//	if (obj->getPRefl() > 1e-4) {
//		Vector3d reflected(reflVector(ray.getDir()->matrix(), intersec.intersection->matrix()));
//		Ray reflRey(new Vector3d(intersec.intersection->matrix()), new Vector3d(reflected));
//		refl = traceRay(reflRey, depth+1);
//		if (obj->getPTransp() > 1e-4) {
//			double ior = 1.1;
//			double eta = ior;// (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
//			double cosi = -intersec.normal->dot(ray.getDir()->matrix());
//			double k = sqrt(1 - (eta * eta * (1 - cosi * cosi)));
//			Vector3d tmp1(ray.getDir()->matrix()*eta);
//			Vector3d tmp2(intersec.normal->matrix()*(eta*cosi - k));
//			Vector3d refrdir(tmp1 + tmp2);
//			refrdir.normalize();
//			Ray refrRey(new Vector3d(intersec.intersection->matrix()),new Vector3d(refrdir));
//			refr = traceRay(refrRey, depth + 1);
//		}
//		return refl + refr;
//	}
//}

double getReflectance(const Vector3d& normal, const Vector3d& incident,	double n1, double n2) {
	double n = n1 / n2;
	double cosI = -normal.dot(incident);
	double sinT2 = n * n * (1.0 - cosI * cosI);

	if (sinT2 > 1.0) {
		return 1.0;
	}

	double cosT = sqrt(1.0 - sinT2);
	double r0rth = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
	double rPar = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);
	return (r0rth * r0rth + rPar * rPar) / 2.0;
}

Vector3d reflectiveRefractive(Object* obj, const Intersection& intersec, Ray ray, const int& depth) {
	if (obj->getPRefl() < 1e-4 && obj->getPTransp() < 1e-4 || depth == reflectiveMaxDepth) {
		return Vector3d(0., 0., 0.);
	}
	double reflectivePercentage = obj->getPRefl();
	double refractivePercentage = 0;
	// calc differently for dif materials in future.
	reflectivePercentage = getReflectance(intersec.normal->matrix(), ray.getDir()->matrix(), 1, 1.5);
	refractivePercentage = 1 - reflectivePercentage;
	//
	if (refractivePercentage <= 0 && reflectivePercentage <= 0) {
		return Vector3d(0.,0.,0.);
	}
	Vector3d reflectiveColor(0.,0.,0.);
	Vector3d refractiveColor(0.,0.,0.);
	if (reflectivePercentage > 0) {
		Vector3d reflected = reflVector(ray.getOrg()->matrix(),	intersec.normal->matrix());
		reflected.normalize();
		Ray reflectedRay(new Vector3d(intersec.intersection->matrix()), new Vector3d(reflected));
		reflectiveColor = traceRay(reflectedRay, depth+1) * reflectivePercentage;
	}
	if (refractivePercentage > 0) {
		Vector3d refracted = refrVector(intersec.normal->matrix(),ray.getDir()->matrix(), 1, 1.5);
		Ray refractedRay(new Vector3d(intersec.intersection->matrix()), new Vector3d(refracted));
		refractiveColor = traceRay(refractedRay, depth + 1) * refractivePercentage;
	}
	return reflectiveColor + refractiveColor;
}

void buildScene() {
	// Glanzfaktor seems to have a problem sometimes. sometimes the spec. light is just black, nothing else. 
	//(center, r, color, glanzFaktor, kSpec, kDiff, pTransp, pRefl)
	Sphere* sphere = new Sphere(new Vector3d(xWidth *0.3, yWidth*0.5, -10), 80, new Vector3d(255., 255., 255.), 30, 0.5, 0.5);	//red
	Sphere* sphere2 = new Sphere(new Vector3d(xWidth *0.7, yWidth*0.5, -10), 80, new Vector3d(0, 0., 255.), 30, 0.5, 0.5);
	// its important to go from left to the right with the edges. otherwise the normal will be facing the other-way around and will not be lighted correctly!
	vector<Vector3d*> edges;
	edges.push_back(new Vector3d(250., 0., 0.));
	edges.push_back(new Vector3d(300., 100., 0.));
	edges.push_back(new Vector3d(200., 100., 0.));
	Triangle* tri = new Triangle(edges, new Vector3d(0, 139, 139), 50, 0.5, 0.5);
	objects.push_back(tri);
	objects.push_back(sphere);
	objects.push_back(sphere2);
}

int main(int argc, char **argv) {
   buildScene();
   rayCasting();
   return 0;
}
