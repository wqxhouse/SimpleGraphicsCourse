#include "Matrix4.h"
#include <memory.h>
#include <cmath>
#include <iostream>
#include <algorithm>

// Constructors //////////////////////////////
Matrix4::Matrix4()
{
	loadIdentity();
}

//Row Major Matrix - conforms to DirectX standard
Matrix4::Matrix4(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	m_entries[0] = m11;
	m_entries[1] = m12;
	m_entries[2] = m13;
	m_entries[3] = m14;

	m_entries[4] = m21;
	m_entries[5] = m22;
	m_entries[6] = m23;
	m_entries[7] = m24;

	m_entries[8] = m31;
	m_entries[9] = m32;
	m_entries[10] = m33;
	m_entries[11] = m34;
	
	m_entries[12] = m41;
	m_entries[13] = m42;
	m_entries[14] = m43;
	m_entries[15] = m44;
}


Matrix4::~Matrix4()
{

}


// Public Methods ////////////////////////////
//Multiply (matrix-times-matrix)
Matrix4 Matrix4::multiplyMat(const Matrix4 &mat)
{
	//explicit for the speed
	//TODO: potential corner case to increase speed

	return Matrix4(	
		m_entries[0]*mat.m_entries[0]+m_entries[4]*mat.m_entries[1]+m_entries[8]*mat.m_entries[2],
		m_entries[1]*mat.m_entries[0]+m_entries[5]*mat.m_entries[1]+m_entries[9]*mat.m_entries[2],
		m_entries[2]*mat.m_entries[0]+m_entries[6]*mat.m_entries[1]+m_entries[10]*mat.m_entries[2],
		0.0f,
		m_entries[0]*mat.m_entries[4]+m_entries[4]*mat.m_entries[5]+m_entries[8]*mat.m_entries[6],
		m_entries[1]*mat.m_entries[4]+m_entries[5]*mat.m_entries[5]+m_entries[9]*mat.m_entries[6],
		m_entries[2]*mat.m_entries[4]+m_entries[6]*mat.m_entries[5]+m_entries[10]*mat.m_entries[6],
		0.0f,
		m_entries[0]*mat.m_entries[8]+m_entries[4]*mat.m_entries[9]+m_entries[8]*mat.m_entries[10],
		m_entries[1]*mat.m_entries[8]+m_entries[5]*mat.m_entries[9]+m_entries[9]*mat.m_entries[10],
		m_entries[2]*mat.m_entries[8]+m_entries[6]*mat.m_entries[9]+m_entries[10]*mat.m_entries[10],
		0.0f,
		m_entries[0]*mat.m_entries[12]+m_entries[4]*mat.m_entries[13]+m_entries[8]*mat.m_entries[14]+m_entries[12],
		m_entries[1]*mat.m_entries[12]+m_entries[5]*mat.m_entries[13]+m_entries[9]*mat.m_entries[14]+m_entries[13],
		m_entries[2]*mat.m_entries[12]+m_entries[6]*mat.m_entries[13]+m_entries[10]*mat.m_entries[14]+m_entries[14],
		1.0f);
}

//Multiply (matrix-times-vector): if multiplying with a point, 
//dehomogenize the result after the multiplication
Vector4 Matrix4::multiplyVec4(const Vector4 &v4)
{
	const float &x = v4['x'];
	const float &y = v4['y'];
	const float &z = v4['z'];
	const float &w = v4['w'];

	Vector4 vec(	
		m_entries[0]*x
		+ m_entries[4]*y
		+ m_entries[8]*z
		+ m_entries[12]*w,

		m_entries[1]*x
		+ m_entries[5]*y
		+ m_entries[9]*z
		+ m_entries[13]*w,

		m_entries[2]*x
		+ m_entries[6]*y
		+ m_entries[10]*z
		+ m_entries[14]*w,

		m_entries[3]*x
		+ m_entries[7]*y
		+ m_entries[11]*z
		+ m_entries[15]*w
		);

	
	const float &vec_w = vec['w'];
	if(vec_w > 1.0f)
	{
		float factor = 1.0f / vec_w;
		vec.set(vec['x'] * factor, 
			    vec['y'] * factor,
				vec['z'] * factor, 
				1.0f);
		return vec;
	}

	return vec;


}

Vector3 Matrix4::multiplyVec3(const Vector3 &v3)
{
	//assume extension w = 1

	const float &x = v3['x'];
	const float &y = v3['y'];
	const float &z = v3['z'];


	float w = m_entries[3]*x
		+ m_entries[7]*y
		+ m_entries[11]*z
		+ m_entries[15];

	Vector3 vec(	
		m_entries[0]*x
		+ m_entries[4]*y
		+ m_entries[8]*z
		+ m_entries[12],

		m_entries[1]*x
		+ m_entries[5]*y
		+ m_entries[9]*z
		+ m_entries[13],

		m_entries[2]*x
		+ m_entries[6]*y
		+ m_entries[10]*z
		+ m_entries[14]
		);

	if(w > 1.0f)
	{
		float factor = 1.0f / w;
		vec.set(
			vec['x'] * factor, 
			vec['y'] * factor,
			vec['z'] * factor);

		return vec;
	}

	return vec;

}

//Make rotation matrix about x axis
void Matrix4::setRotationX(const float angle)
{
	loadIdentity();

	m_entries[5] = (float)cos(angle * DEG_TO_RAD);
	m_entries[6] = (float)sin(angle * DEG_TO_RAD);

	m_entries[9] = -m_entries[6];
	m_entries[10] = m_entries[5];
}

//Make rotation matrix about y axis
void Matrix4::setRotationY(const float angle)
{
	loadIdentity();

	m_entries[0] =  (float)cos(angle * DEG_TO_RAD);
	m_entries[2] = -(float)sin(angle * DEG_TO_RAD);

	m_entries[8] = -m_entries[2];
	m_entries[10] = m_entries[0];
}

//Make rotation matrix about z axis
void Matrix4::setRotationZ(const float angle)
{
	loadIdentity();

	m_entries[0] = (float)cos(angle * DEG_TO_RAD);
	m_entries[1] = (float)sin(angle * DEG_TO_RAD);

	m_entries[4] = -m_entries[1];
	m_entries[5] =  m_entries[0];
}

//Make rotation matrix about arbitrary (unit) axis
//Formula from: http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/
void Matrix4::setRotationVec(const Vector3 &axis, const float angle)
{
	loadIdentity();

	Vector3 &normAxis = axis.getNormalizedVec();
	const float &x = normAxis['x'];
	const float &y = normAxis['y'];
	const float &z = normAxis['z'];

	float sinValue = (float)sin(angle * DEG_TO_RAD);
	float cosValue = (float)cos(angle * DEG_TO_RAD);
	float oneMinusCos = 1.0f - cosValue;

	m_entries[0] = x*x + cosValue * (1-x*x);
	m_entries[4] = x*y*(oneMinusCos) - sinValue*z;
	m_entries[8] = x*z*(oneMinusCos) + sinValue*y;

	m_entries[1] = x*y*(oneMinusCos) + sinValue*z;
	m_entries[5] = y*y + cosValue * (1-y*y);
	m_entries[9] = y*z*(oneMinusCos) - sinValue*x;

	m_entries[2] = x*z*(oneMinusCos) - sinValue*y;
	m_entries[6] = y*z*(oneMinusCos) + sinValue*x;
	m_entries[10] = z*z + cosValue * (1-z*z);

}

//Make non-uniform scaling matrix
void Matrix4::setScale(Vector3 &factor)
{
	loadIdentity();

	m_entries[0] = factor['x'];
	m_entries[5] = factor['y'];
	m_entries[10] = factor['z'];
}

//Make translation matrix
void Matrix4::setTranslate(Vector3 &factor)
{
	m_entries[12] = factor['x'];
	m_entries[13] = factor['y'];
	m_entries[14] = factor['z'];
}

//Print (display all 16 matrix components numerically on the screen in a 4x4 array)
void Matrix4::print() const
{
	for(int i = 0; i < 16; i++)
	{	
		if(i % 4 == 0 && i != 0)
		{
			std::cout<< std::endl;
		}

		std::cout<< m_entries[i] << " ";
	}

	std::cout<<std::endl;
}

float Matrix4::operator [] (int index) const
{
	return m_entries[index];
}

void Matrix4::loadIdentity()
{
	memset(m_entries, 0, 16 * sizeof(float));
	m_entries[0] = 1.0f;
	m_entries[5] = 1.0f;
	m_entries[10] = 1.0f;
	m_entries[15] = 1.0f;
}

void Matrix4::getMatrixArray(float m[])
{
	std::copy(m_entries, m_entries + 16, m);
}

void Matrix4::set(int index, float value)
{
	m_entries[index] = value;
}