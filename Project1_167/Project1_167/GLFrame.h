#pragma once

#include "Vector3.h"
#include "Matrix4.h"

class GLFrame
{
public:
	GLFrame(void);
	~GLFrame(void);

	void GetMatrix(Matrix4 &mat, bool bRotateOnly = false);
	void ApplyCameraTransform(bool bRotateOnly);


protected:
	Vector3 m_vPosition;//translation column
	Vector3 m_vUp;		//y column
	Vector3 m_vForward; //z column

};

