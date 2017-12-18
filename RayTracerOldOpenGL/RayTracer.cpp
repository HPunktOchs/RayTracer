#include "Sphere.cpp"
#include <vector>
#include <fstream>

using namespace std;

class RayTracer {
private:
	const int xWidth = 500;
	const int yWidth = 500;
	vector<Object*> objects;
	Sphere* lightSrc;


public:
	RayTracer() {
		this->createObjects();
		this->lightSrc = new Sphere(new Vector3d(xWidth *0., yWidth*0., 200.), 1, new Vector3d(255., 255., 255.));
	}

	~RayTracer() {}

	void createObjects() {
		Sphere* sphere2 = new Sphere(new Vector3d(xWidth *0.8, yWidth*0.25, 0), 1000, new Vector3d(0, 0., 255.), 50, 0.5, 0.5);
		Sphere* sphere = new Sphere(new Vector3d(xWidth *0.5, yWidth*0.5, 0), 1000, new Vector3d(255., 0., 0.), 50, 0.5, 0.5);	//red
		this->objects.push_back(sphere2);
		this->objects.push_back(sphere);

	}

	void castRays() {
		std::ofstream out; ("out.ppm");
		out << "P3\n" << xWidth << '\n' << yWidth << '\n' << "255\n";
			for (int y = 0; y < yWidth; y++) {
				for (int x = 0; x < xWidth; x++) {
					Ray ray(new Vector3d(x, y, 0), new Vector3d(0, 0, 1));
					Vector3d pix_color(this->traceRay(ray));
					out << (int)pix_color[0] << std::endl;
					out << (int)pix_color[1] << std::endl;
					out << (int)pix_color[2] << std::endl;
				}
			}
	}

	Vector3d traceRay(const Ray& ray) {
		Vector3d black(0., 0., 0.);
		Vector3d white(255., 255., 255.);
		for (int o = 0; o < objects.size(); o++) {
			Object* obj = objects[o];
			Intersection intersec;
			bool check = obj->intersect(ray, intersec);
			if (check) {	
				Vector3d pix(doLighting(obj, intersec, ray));
				clamp255(pix);
				return pix;
			}
			return black;
		}
	}

	Vector3d doLighting(Object* obj, const Intersection& intersec, const Ray& ray) {
		Vector3d totalCol(0., 0., 0.);
		Vector3d diffuAndSpec(this->calcDiffusalAndSpecularLight(obj, intersec, ray));
		/*Vector3d ambient(this->calcAmbientLight());*/
		Vector3d ambient(obj->getColor()->matrix()*0.2);
		totalCol = ambient + diffuAndSpec;
		clamp255(totalCol);
		return totalCol;
	}

	//TODO: Anpassen und auf 90° eingenzen
	Vector3d calcDiffusalAndSpecularLight(Object* obj, const Intersection& intersec, Ray ray) {
		Vector3d black(0.,0.,0.);
		Vector3d lightDirVec(this->lightSrc->getCenter()->matrix() - intersec.intersection->matrix());
		Vector3d betrachterRay(ray.getOrg()->matrix() - intersec.intersection->matrix());

		betrachterRay.normalize();
		lightDirVec.normalize();
		double diffusion = intersec.normal->matrix().dot(lightDirVec);
		if (diffusion < 0.) {
			diffusion = 0.;
		}
		Vector3d objColor(obj->getColor()->matrix());

		double cosSpec = calcSpectatorVector(intersec, lightDirVec).dot(betrachterRay.matrix());
		if (cosSpec < 0.) {
			cosSpec = 0.;
		}
		double spec = pow(cosSpec, obj->getGlanzFaktor());
		return obj->getColor()->matrix()*(obj->getKDiff()*diffusion + obj->getKSpec()*spec);
		/*return this->lightSrc->getColor()->matrix()*(obj->getKDiff()*diffusion + obj->getKSpec()*spec);*/
	}

	Vector3d calcAmbientLight() {
		return *(this->lightSrc->getColor())*0.2;
	}

	Vector3d calcSpectatorVector(const Intersection& intersec, const Vector3d &lightDirVec) {
		Vector3d n(intersec.normal->matrix());
		n.normalize();
		Vector3d reflVec(2 * n*(n.dot(lightDirVec.matrix())) - lightDirVec.matrix());
		return reflVec;
	}

	void clamp255(Vector3d& col) {
		col[0] = (col[0] > 255) ? 255 : (col[0] < 0) ? 0 : col[0];
		col[1] = (col[1] > 255) ? 255 : (col[1] < 0) ? 0 : col[1];
		col[2] = (col[2] > 255) ? 255 : (col[2] < 0) ? 0 : col[2];
	}

};