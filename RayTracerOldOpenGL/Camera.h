#pragma once

class Camera {
public:
	Vector3d lookAt;
	Vector3d position;
	Vector3d up;
	Vector3d w, u, v;
	double screenWidth;

	Camera() {
		this->position = Vector3d(0.0, 0.0, 100.0);
		this->up = Vector3d(0.0, 1.0, 0.0);
		this->lookAt = Vector3d(0.0, 0.0, 0.0);
		this->screenWidth = 500;

		calculateWUV();
	}

	Camera(const Vector3d& position_, const Vector3d& up_, const Vector3d& lookAt_, double screenWidth_) :
		position(position_), up(up_), lookAt(lookAt_), screenWidth(screenWidth_) {
		calculateWUV();
	}

private:
	void calculateWUV() {
		w = (lookAt - position);
		w.normalize();
		u = up.cross(w);
		u.normalize();
		v = w.cross(u);
		v.normalize();
	}
};