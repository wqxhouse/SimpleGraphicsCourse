#include "Orbital.h"


Orbital::Orbital()
{
	radius = 100;
	thickness = 10;
	alpha = 1;
}

Orbital::Orbital(float radius, float thickness, float alpha)
{
	this->radius = radius;
	this->thickness = thickness;
	this->alpha = alpha;
}

Orbital::~Orbital()
{

}

//void Orbital::init()
//{
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glEnable(GL_DEPTH_TEST);
//}


void Orbital::draw()
{
	float factor = thickness * 0.5;
	glutSolidTorus(10, 100, 10, 100);
}

void Orbital::setAlpha(float alpha)
{
	this->alpha = alpha;
}

void Orbital::setRadius(float radius)
{
	this->radius = radius;
}

void Orbital::setThickness(float thickness)
{
	this->thickness = thickness;
}
