#include "Object.cpp"
#include <vector>

using namespace std;

class Triangle : public Object {
private:
	std::vector<Vector3d*> edges;


public:
	Triangle(vector<Vector3d*> edges, int depth, Vector3d* col, int glanzFaktor = 1, double kSpec = 0.1, double kDiff = 1.) : Object(col, glanzFaktor, kSpec, kDiff) {
		this->edges = edges;
		//std::cout << "cent: " << center->matrix() << std::endl;
		//std::cout << "color: " << color->matrix() << std::endl;
	}

	~Triangle() {}

	bool intersect(Ray ray, Intersection& inter) {

		return true;
	}
};