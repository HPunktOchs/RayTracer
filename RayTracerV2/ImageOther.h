#pragma once
#include <fstream>
#include "Color.h"
#include "Vector.h"

class ImageOther {
public:
	ImageOther(int w, int h, std::string img): out(img, std::ofstream::trunc) {
		this->width = w;
		this->height = h;
		this->pixmap = new Color*[h];
		for (int y = 0; y < this->height; y++) {
			pixmap[y] = new Color[w];
		}
	}

	void putColor(int y, int x, Color col) {
		this->pixmap[y][x] = col;
	}

	void writeImage() {
		this->out << "P3\n" << this->width << '\n' << this->height << '\n' << "255\n";
		for (int y = 0; y < this->height; y++) {
			for (int x = 0; x < this->width; x++) {
				Color tmp(pixmap[y][x]);
				tmp = tmp * 255;
				out << (int)tmp.r << std::endl;
				out << (int)tmp.g << std::endl;
				out << (int)tmp.b << std::endl;
			}
		}
	}
private:
	int width;
	int height;
	Color** pixmap;
	std::ofstream out;
};

