#pragma once

#include "Matrix4.h"

class Frustum
{
public:
	Frustum();
	Frustum(float fov, float fAspect, float zNear, float zFar);
	~Frustum();

private:
	float fovy;
	float fAspect;
	float zNear;
	float zFar;

	Matrix4 projectionMat;

};

