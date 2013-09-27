#pragma once
#include <GL/glut.h>

class Orbital
{
public:
	 Orbital();
	 Orbital(float radius, float thickness, float alpha);
	 ~Orbital();

	// compose with opengl torus
	//void init();
	void setRadius(float radius);
	void setThickness(float thickness);
	void setAlpha(float alpha);
	void draw();

private:
	float radius;
	float thickness;
	float alpha;
};

