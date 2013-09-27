#include "Frustum.h"
#include <cmath>
#define M_PI 3.14159265358979

Frustum::Frustum()
	: fovy(45.0f), fAspect(1), zNear(1), zFar(425)
{

}


Frustum::Frustum(float fovy, float fAspect, float zNear, float zFar)
	: fovy(fovy), fAspect(fAspect), zNear(zNear), zFar(zFar)
{
	const float y = zNear * tanf(fovy * 0.5 * M_PI / 180);
	const float x = fAspect * y;
	
		
}
	


Frustum::~Frustum()
{

}
