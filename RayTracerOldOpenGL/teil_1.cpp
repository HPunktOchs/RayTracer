#include "Sphere.h"
#include "Triangle.h"

#include <fstream>

using namespace std;

const int xWidth = 500;
const int yWidth = 500;


Sphere light(new Vector3d(xWidth *0., yWidth*0., 1000.), 1, new Vector3d(255., 255., 255.));
vector<Object*> objects;

Vector3d spekularLight(const Intersection &intersec, const Vector3d &lightDirVec);
Vector3d doLighting(Object* obj, const Intersection& intersec, Ray ray);
Vector3d calcAmbient(Object* obj);
Vector3d calcDiffuseAndSpec(Object* obj, const Intersection& intersec, const Ray& ray);

void clamp255(Vector3d& col) {
	col[0] = (col[0] > 255) ? 255 : (col[0] < 0) ? 0 : col[0];
	col[1] = (col[1] > 255) ? 255 : (col[1] < 0) ? 0 : col[1];
	col[2] = (col[2] > 255) ? 255 : (col[2] < 0) ? 0 : col[2];
}

void rayCasting() {
		Sphere* sphere = new Sphere(new Vector3d(xWidth *0.5, yWidth*0.5, 0), 500,new Vector3d(255.,0.,0.), 50, 0.5, 0.5);	//red
		Sphere* sphere2 = new Sphere(new Vector3d(xWidth *0.8, yWidth*0.25, 0), 500, new Vector3d(0, 0., 255.), 50, 0.5, 0.5);
		vector<Vector3d*> edges;
		edges.push_back(new Vector3d(150.,0.,0.));
		edges.push_back(new Vector3d(300., 0., 50.));
		edges.push_back(new Vector3d(300., 150., 50.));
		Triangle* tri = new Triangle(edges, new Vector3d(0, 139, 139), 50, 0.5, 0.5);
		objects.push_back(tri);
		objects.push_back(sphere);
		objects.push_back(sphere2);

		std::ofstream out("wTri.ppm");
		out << "P3\n" << xWidth << '\n' << yWidth << '\n' << "255\n";
		double t = 20000;
		Vector3d black(0., 0., 0.);
		Vector3d white(255., 255., 255.);
		Vector3d pix_color;
		for (int y = 0; y < yWidth; y++) {		
			for (int x = 0; x < xWidth; x++) {
				Ray ray(new Vector3d(x, y, 0), new Vector3d(0, 0, 1));
				pix_color = black;
				for (int o = 0; o < objects.size(); o++) {
					Object* obj = objects[o];
					Intersection intersec;
					bool check = obj->intersect(ray, intersec);
					if (check) {
						pix_color = doLighting(obj, intersec, ray);
					}
				}
				out << (int)pix_color[0] << std::endl;
				out << (int)pix_color[1] << std::endl;
				out << (int)pix_color[2] << std::endl;
			}
		}	
}

Vector3d doLighting(Object* obj, const Intersection& intersec, Ray ray) {
	Vector3d color(0., 0., 0.);
	Vector3d DiffAndSpec(calcDiffuseAndSpec(obj,intersec,ray));
	Vector3d ambient(calcAmbient(obj));
	color = ambient + DiffAndSpec;
	clamp255(color);
	return color;
}

Vector3d calcDiffuseAndSpec(Object* obj, const Intersection& intersec, const Ray& ray) {
	Vector3d lightDirVec(light.getCenter()->matrix() - intersec.intersection->matrix());
	Vector3d betrachterRay(ray.getOrg()->matrix() - intersec.intersection->matrix());
	betrachterRay.normalize();
	lightDirVec.normalize();
	double diffusion = intersec.normal->matrix().dot(lightDirVec);
	if (diffusion < 0.) {
		diffusion = 0.;
	}
	double spec = pow(spekularLight(intersec, lightDirVec).dot(betrachterRay.matrix()), obj->getGlanzFaktor());
	return obj->getColor()->matrix()* (obj->getKDiff()*diffusion + obj->getKSpec()*spec);	
}

Vector3d calcAmbient(Object* obj) {
	 return obj->getColor()->matrix()*0.2;
	//return obj->getColor()->matrix()*0.;

}

Vector3d spekularLight(const Intersection &intersec, const Vector3d &lightDirVec) {
	Vector3d n(intersec.normal->matrix());
	n.normalize();
	double test = n.dot(lightDirVec.matrix());
	if (test < 0.) { 
		return Vector3d(0., 0., 0.);
	}
	Vector3d reflVec(2*n*(test)-lightDirVec.matrix());
	return reflVec;
}

int main(int argc, char **argv) {
   rayCasting();
   return 0;
}
