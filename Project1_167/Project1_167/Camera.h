#pragma once

#include "Vector3.h"
#include "Matrix4.h"

class Camera
{
public:
	Camera(Vector3 &e, Vector3 &d, Vector3 &up);

	void getCameraMatrix(float cameraMat[]);

	~Camera();

private:
	Vector3 m_e;
	Vector3 m_d;
	Vector3 m_up;

	Matrix4 m_cameraMat;

};

