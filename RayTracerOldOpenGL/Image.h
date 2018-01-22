#pragma once
#include <fstream>
#include <Eigen/Geometry>

using namespace Eigen;

class Image {
public:
	Image(int w, int h): out("wTri.ppm", ofstream::trunc) {
		this->width = w;
		this->height = h;
		this->pixmap = new Vector3d*[h];
		for (int x = 0; x < this->width; x++) {
			pixmap[x] = new Vector3d[w];
		}
	}

	void putColor(int x, int y, Vector3d col) {
		this->pixmap[y][x] = col;
	}

	void writeImage() {
		this->out << "P3\n" << this->height << '\n' << this->width << '\n' << "255\n";
		for (int y = 0; y < this->height; y++) {
			for (int x = 0; x < this->width; x++) {
				Vector3d tmp(pixmap[y][x]);
				out << (int)tmp[0] << endl;
				out << (int)tmp[1] << endl;
				out << (int)tmp[2] << endl;
			}
		}
	}
private:
	int width;
	int height;
	Vector3d** pixmap;
	std::ofstream out;
};

