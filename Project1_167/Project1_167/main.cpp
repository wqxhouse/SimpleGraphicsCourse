#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include "assert.h"
#include <array>
#include "objreader.h"

#include "Vector3.h"
#include "Matrix4.h"
#include "Camera.h"


/* Constants rounded for 21 decimals. */
#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_1_SQRTPI 0.564189583547756286948
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT_2 0.707106781186547524401

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLint Tesslation = 0;

static GLfloat cameraX = 0.0f;
static GLfloat cameraY = 0.0f;
static GLfloat cameraZ = 0.0f; 

//debug
static GLint loopTimes = 0.0;

static GLint colorControl = 0;

static float viewMatrix[16];


void DrawAllShape();
void RenderScene();
void Reshape(int w, int h);
void Setup();
void Timer(int);
void SpecialKeys(int key, int x, int y);
void KeyBoard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

void processSelection(int xPos, int yPos);
void unProject(int x, int y);

void DrawCube(int n);
void DrawCubeTess(int n, int tessExtent);

void DrawCylinder(GLfloat radius, GLfloat height, int tessExtent);
void DrawCone(GLfloat radius, GLfloat height, int tessExtent);

void divideTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c, 
	std::vector<GLfloat> &drawArray, int tessExtent);
void drawTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c);


void DrawIcosSphere(GLfloat radius, int tessExtent);
void DrawLatSphere(GLfloat radius, int latSegments, int longSegments);

void randColor()
{
	GLfloat randR = ((double) rand() / (RAND_MAX));
	GLfloat randG = ((double) rand() / (RAND_MAX));
	GLfloat randB = ((double) rand() / (RAND_MAX));
	glColor3f(randR, randG, randB);
}


void drawObjModel(char *path)
{
	if(*path == '\0')
	{
		printf("Empty string unsupported!");
		return;
	}

	int nVerts;
	float *vertices;
	float *normals;
	float *texcoords;
	int nIndices;
	int *indices;

	ObjReader::readObj(path, nVerts, &vertices, &normals, &texcoords, nIndices, &indices);

	//find max x, y, z
	float xMax = vertices[0]; int xMaxIndex = 0;
	float yMax = vertices[1]; int yMaxIndex = 1;
	float zMax = vertices[2]; int zMaxIndex = 2;

	float xMin = vertices[0]; int xMinIndex = 0;
	float yMin = vertices[1]; int yMinIndex = 1;
	float zMin = vertices[2]; int zMinIndex = 2;

	for(int i = 0; i < nVerts * 3; i += 3)
	{
		// find x max & min
		if(vertices[i] > xMax)
		{
			xMax = vertices[i];
			xMaxIndex = i;
		}
		else if(vertices[i] < xMin)
		{
			xMin = vertices[i];
			xMinIndex = i;
		}

		//find y max & min
		if(vertices[i + 1] > yMax)
		{
			yMax = vertices[i + 1];
			yMaxIndex = i + 1;
		}
		else if(vertices[i + 1] < xMin)
		{
			yMin = vertices[i + 1];
			yMinIndex = i + 1;
		}

		//find z max & min
		if(vertices[i + 2] > zMax)
		{
			zMax = vertices[i + 2];
			zMaxIndex = i + 2;
		}
		else if(vertices[i + 2] < xMin)
		{
			zMin = vertices[i + 2];
			zMinIndex = i + 2;
		}
	}

	Vector3 vxMax(xMax, vertices[xMaxIndex + 1], vertices[xMaxIndex + 2]);
	Vector3 vxMin(xMin, vertices[xMinIndex + 1], vertices[xMinIndex + 2]);

	Vector3 vyMax(vertices[yMaxIndex - 1], yMax, vertices[yMaxIndex + 1]);
	Vector3 vyMin(vertices[yMinIndex - 1], yMin, vertices[yMinIndex + 1]);

	Vector3 vzMax(vertices[zMaxIndex - 2], vertices[zMaxIndex - 1], zMax);
	Vector3 vzMin(vertices[zMinIndex - 2], vertices[zMinIndex - 1], zMin);

	//DEBUGGING PRINT
	/*printf("vxMax = %.2f, %.2f, %.2f\n", vxMax['x'], vxMax['y'], vxMax['z']);
	printf("vxMin = %.2f, %.2f, %.2f\n", vxMin['x'], vxMin['y'], vxMin['z']);

	printf("vyMax = %.2f, %.2f, %.2f\n", vyMax['x'], vyMax['y'], vyMax['z']);
	printf("vxMin = %.2f, %.2f, %.2f\n", vyMin['x'], vyMin['y'], vyMin['z']);

	printf("vzMax = %.2f, %.2f, %.2f\n", vzMax['x'], vzMax['y'], vzMax['z']);
	printf("vxMin = %.2f, %.2f, %.2f\n", vzMin['x'], vzMin['y'], vzMin['z']);*/

	//TODO: resize model to fit in window
	//refactor later: here assume a frustum size



	//Calculate obj's origin
	const float centerX = xMax - xMin;
	const float centerY = yMax - yMin;
	const float centerZ = zMax - zMin;

	if(centerX != 0 || centerY != 0 || centerX != 0)
	{
		Matrix4 transToOrigin;
		transToOrigin.set(12, -centerX);
		transToOrigin.set(13, -centerY);
		transToOrigin.set(14, -centerZ);
	}

	


	
	
}


void DrawJet();
void DrawGround(GLfloat extent, GLfloat step, GLfloat yPosition);
void DrawSphere(GLfloat extent);

void enable2DProj();
void quit2DProj();
void DrawGUI2D();
void respondGU2D(int signal, int x, int y);


//TEST simple ortho projection without boundaries
void simpleOrthoProj()
{
	Matrix4 mat;
	mat.set(10, 0);
	glMatrixMode(GL_PROJECTION);

	float matArr[16];
	mat.getMatrixArray(matArr);
	glLoadMatrixf(matArr);

	glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Light101");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(KeyBoard);
	glutMouseFunc(Mouse);

	//	glutTimerFunc(33, Timer, 1);

	Setup();
	glutMainLoop();

	//drawObjModel("");

	return 0;
}

void Setup()
{
	GLfloat ambientLight[] = {1, 1, 1, 1};

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_CCW);

	glClearColor(1, 1, 1, 1);

	//lighting parameters
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	//Material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//setup camera
	Camera cam = Camera(Vector3(0, 10, 10), 
						Vector3(0, 0, 0), 
						Vector3(0, 1, 0.5));

	cam.getCameraMatrix(viewMatrix);

}



void Reshape(int w, int h)
{
	printf("reshape");
	GLfloat nRange = 100.0f;
	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	//glMatrixMode(GL_PROJECTION);

	//glLoadIdentity();

	//
	//// Establish clipping volume (left, right, bottom, top, near, far)
	//if (w <= h) 
	//	glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	//else 
	//	glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	//double mat[16];
	//glGetDoublev(GL_PROJECTION_MATRIX, mat);

	//glMatrixMode(GL_MODELVIEW);


}

void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(33, Timer, 1);
}

void SpecialKeys(int key, int x, int y) 
{
	if(key == GLUT_KEY_UP)
		xRot-= 5.0f;

	if(key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if(key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if(key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if(key > 356.0f)
		xRot = 0.0f;

	if(key < -1.0f)
		xRot = 355.0f;

	if(key > 356.0f)
		yRot = 0.0f;

	if(key < -1.0f)
		yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
}

void KeyBoard(unsigned char key, int x, int y)
{
	if(key == 'w')
	{
		cameraY += 2.0f;
	}

	if(key == 's')
	{
		cameraY -= 2.0f;
	}

	if(key == 'a')
	{
		cameraX += 2.0f;
		cameraZ += 2.0f;
	}

	if(key == 'd')
	{
		cameraX -= 2.0f;
		cameraZ -= 2.0f;
	}

	if(key == 'r')
	{
		xRot = 0.0f;
		yRot = 0.0f;
	}

	if(key == 'i')
	{
		Tesslation++;
	}

	if(key == 'o')
	{
		if(Tesslation > 0)
		{
			Tesslation--;
		}
	}


	//control loops, debugging purpose
	if(key == 'l')
	{
		loopTimes++;
	}

	if(key == ';')
	{
		if(loopTimes > 0)
		{
			loopTimes--;
		}
				
	}

	if(key == 'c')
	{
		if(colorControl == 1)
		{
			glDisable(GL_CULL_FACE);
			colorControl = 0;
		}
		else
		{
			glEnable(GL_CULL_FACE);
			colorControl = 1;
		}
	
	}


	//switch between shapes
	if(key == '1')
	{

	}
	if(key == '2')
	{

	}
	if(key == '3')
	{

	}
	if(key == '4')
	{

	}
	if(key == '5')
	{

	}

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		/*GLuint window_width = glutGet(GLUT_WINDOW_WIDTH);
		GLuint window_height = glutGet(GLUT_WINDOW_HEIGHT);

		GLubyte color[4];
		GLfloat depth;
		GLuint index;

		glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
		glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);*/

		//unProject(x, y);
		processSelection(x, y);
	}
}

void unProject(int x, int y)
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint    viewMatrix[4];

	glGetDoublev(GL_MODELVIEW, modelMatrix);
	glGetDoublev(GL_PROJECTION, projMatrix);
	glGetIntegerv(GL_VIEWPORT, viewMatrix);

	GLdouble objX, objY, objZ;

	gluUnProject(x, viewMatrix[3] - y, 0, modelMatrix, projMatrix, viewMatrix, &objX, &objY, &objZ);
	printf("X: %.3f, Y: %.3f, Z: %.3f\n", objX, objY, objZ);
}

#define BUFFER_LENGTH 64
void processSelection(int xPos, int yPos)
{
	// Space for selection buffer
	static GLuint selectBuff[BUFFER_LENGTH];
	// Hit counter and viewport storage
	GLint hits, viewport[4];

	// Set up selection buffer
	glSelectBuffer(BUFFER_LENGTH, selectBuff);

	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);


	float buff[16];
	glGetFloatv(GL_PROJECTION_MATRIX, buff);


	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);
	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending two pixels
	// in the vertical and horizontal direction
	glLoadIdentity();

	gluPickMatrix(xPos, viewport[3] - yPos, 2, 2, viewport);
	// Apply perspective matrix
	//fAspect = (float)viewport[2] / (float)viewport[3];
	
	//glOrtho (-100, 100, -100, 100, -100, 100);

	//gluOrtho2D(0, 512, 0, 512);
	glGetFloatv(GL_PROJECTION_MATRIX, buff);


	glMatrixMode(GL_MODELVIEW);
	// Draw the scene
	DrawAllShape();
	
	GLenum e = glGetError();

	glGetFloatv(GL_PROJECTION_MATRIX, buff);
	std::cerr <<"error: " << e << std::endl;

	// Collect the hits
	hits = glRenderMode(GL_RENDER);
	printf("h: %d\nselectBuff: %d, %d, %d, %d\n", hits,selectBuff[0], selectBuff[1], selectBuff[2], selectBuff[3] );
	// If a single hit occurred, display the info.
	//if(hits == 1)
	{
		//respondGUI(1, xPos, yPos);
	}
	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glGetFloatv(GL_PROJECTION_MATRIX, buff);


	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);

}
#undef BUFFER_SIZE

void DrawGround(GLfloat extent, GLfloat step, GLfloat yPosition)
{
	glColor3f(0, 0, 0);

	glBegin(GL_LINES);
	{
		for(int i = -extent; i <= extent; i += step)
		{
			//Draw vertical lines x-z
			glVertex3f(i, yPosition, -extent);
			glVertex3f(i, yPosition,  extent);

			//Draw Horizontal lines
			glVertex3f(-extent, yPosition, i);
			glVertex3f( extent, yPosition, i);
		}
	}
	glEnd();

}

void DrawJet()
{
	// Save matrix state and do the rotation
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);


	// Nose Cone /////////////////////////////
	// White
	glColor3ub(255, 255, 255);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.0f, 60.0f);
	glVertex3f(-15.0f, 0.0f, 30.0f);
	glVertex3f(15.0f,0.0f,30.0f);

	// Black
	glColor3ub(0,0,0);
	glVertex3f(15.0f,0.0f,30.0f);
	glVertex3f(0.0f, 15.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, 60.0f);

	// Red
	glColor3ub(255,0,0);
	glVertex3f(0.0f, 0.0f, 60.0f);
	glVertex3f(0.0f, 15.0f, 30.0f);
	glVertex3f(-15.0f,0.0f,30.0f);


	// Body of the Plane ////////////////////////
	// Green
	glColor3ub(0,255,0);
	glVertex3f(-15.0f,0.0f,30.0f);
	glVertex3f(0.0f, 15.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, -56.0f);

	glColor3ub(255,255,0);
	glVertex3f(0.0f, 0.0f, -56.0f);
	glVertex3f(0.0f, 15.0f, 30.0f);
	glVertex3f(15.0f,0.0f,30.0f);	

	glColor3ub(0, 255, 255);
	glVertex3f(15.0f,0.0f,30.0f);
	glVertex3f(-15.0f, 0.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, -56.0f);

	///////////////////////////////////////////////
	// Left wing
	// Large triangle for bottom of wing
	glColor3ub(128,128,128);
	glVertex3f(0.0f,2.0f,27.0f);
	glVertex3f(-60.0f, 2.0f, -8.0f);
	glVertex3f(60.0f, 2.0f, -8.0f);

	glColor3ub(64,64,64);
	glVertex3f(60.0f, 2.0f, -8.0f);
	glVertex3f(0.0f, 7.0f, -8.0f);
	glVertex3f(0.0f,2.0f,27.0f);

	glColor3ub(192,192,192);
	glVertex3f(60.0f, 2.0f, -8.0f);
	glVertex3f(-60.0f, 2.0f, -8.0f);
	glVertex3f(0.0f,7.0f,-8.0f);

	// Other wing top section
	glColor3ub(64,64,64);
	glVertex3f(0.0f,2.0f,27.0f);
	glVertex3f(0.0f, 7.0f, -8.0f);
	glVertex3f(-60.0f, 2.0f, -8.0f);

	// Tail section///////////////////////////////
	// Bottom of back fin
	glColor3ub(255,128,255);
	glVertex3f(-30.0f, -0.50f, -57.0f);
	glVertex3f(30.0f, -0.50f, -57.0f);
	glVertex3f(0.0f,-0.50f,-40.0f);

	// top of left side
	glColor3ub(255,128,0);
	glVertex3f(0.0f,-0.5f,-40.0f);
	glVertex3f(30.0f, -0.5f, -57.0f);
	glVertex3f(0.0f, 4.0f, -57.0f);

	// top of right side
	glColor3ub(255,128,0);
	glVertex3f(0.0f, 4.0f, -57.0f);
	glVertex3f(-30.0f, -0.5f, -57.0f);
	glVertex3f(0.0f,-0.5f,-40.0f);

	// back of bottom of tail
	glColor3ub(255,255,255);
	glVertex3f(30.0f,-0.5f,-57.0f);
	glVertex3f(-30.0f, -0.5f, -57.0f);
	glVertex3f(0.0f, 4.0f, -57.0f);

	// Top of Tail section left
	glColor3ub(255,0,0);
	glVertex3f(0.0f,0.5f,-40.0f);
	glVertex3f(3.0f, 0.5f, -57.0f);
	glVertex3f(0.0f, 25.0f, -65.0f);

	glColor3ub(255,0,0);
	glVertex3f(0.0f, 25.0f, -65.0f);
	glVertex3f(-3.0f, 0.5f, -57.0f);
	glVertex3f(0.0f,0.5f,-40.0f);

	// Back of horizontal section
	glColor3ub(128,128,128);
	glVertex3f(3.0f,0.5f,-57.0f);
	glVertex3f(-3.0f, 0.5f, -57.0f);
	glVertex3f(0.0f, 25.0f, -65.0f);

	glEnd(); // Of Jet

	glPopMatrix();
}


#include <vector>

// your framework of choice here

class SolidSphere
{
protected:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texcoords;
	std::vector<GLushort> indices;

public:
	SolidSphere(float radius, unsigned int rings, unsigned int sectors)
	{
		float const R = 1./(float)(rings-1);
		float const S = 1./(float)(sectors-1);
		int r, s;

		vertices.resize(rings * sectors * 3);
		normals.resize(rings * sectors * 3);
		texcoords.resize(rings * sectors * 2);
		std::vector<GLfloat>::iterator v = vertices.begin();
		std::vector<GLfloat>::iterator n = normals.begin();
		std::vector<GLfloat>::iterator t = texcoords.begin();
		for(r = 0; r < rings; r++) 
		{
			for(s = 0; s < sectors; s++) 
			{
				float const y = sin( -M_PI_2 + M_PI * r * R );
				float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
				float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

				*t++ = s*S;
				*t++ = r*R;

				*v++ = x * radius;
				*v++ = y * radius;
				*v++ = z * radius;

				*n++ = x;
				*n++ = y;
				*n++ = z;
			}
		}

		indices.resize(rings * sectors * 4);
		std::vector<GLushort>::iterator i = indices.begin();
		for(r = 0; r < rings-1; r++) 
			for(s = 0; s < sectors-1; s++) {
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s+1);
			*i++ = (r+1) * sectors + (s+1);
			*i++ = (r+1) * sectors + s;
		}
	}

	void draw(GLfloat x, GLfloat y, GLfloat z)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x,y,z);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
		glNormalPointer(GL_FLOAT, 0, &normals[0]);
		glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
		glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
		glPopMatrix();
	}
};

//void naiveSquare()
//{
//	int l = 50;
//
//
//	glRotatef(xRot, 1, 0, 0);
//	glRotatef(yRot, 0, 1, 0);
//
//	glBegin(GL_TRIANGLES);
//	{
//		glVertex3f(-l, l, l);
//		glVertex3f(-l, -l, l);
//		glVertex3f(l, l, l);
//	}
//	glEnd();
//
//	glBegin(GL_TRIANGLES);
//	{
//		glVertex3f(l, l, l);
//		glVertex3f(-l, -l, l);
//		glVertex3f(l, -l, l);
//	}
//	glEnd();
//}

// extract this method so that select buffer may not call extra things such as 
// transformations in Renderscene() repeatedly and cause unexpected result, 
// especially the glutSwapbuffers() call that may make the whole screen black

#define SPHERE1 1
#define SPHERE2 2
void DrawAllShape()
{
	GLfloat mat[16];
	
	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glGetFloatv(GL_PROJECTION_MATRIX, mat);

	glPushMatrix();


	
	//get matrix debug:
	glGetFloatv(GL_PROJECTION_MATRIX, mat);




	//glLoadIdentity(); // suspicious

	glGetFloatv(GL_PROJECTION_MATRIX, mat);






	gluOrtho2D(0, 512, 0, 512);
	 
	glGetFloatv(GL_PROJECTION_MATRIX, mat);

	
	glLoadName(20);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(10, 10, 0);
	DrawGUI2D();
	glPopMatrix();

	glLoadName(30);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(-50, -50, 0);
	DrawGUI2D();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glGetFloatv(GL_PROJECTION_MATRIX, mat);

	// 3D
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glGetFloatv(GL_PROJECTION_MATRIX, mat);


	//glLoadIdentity();
	glOrtho(-100, 100, -100, 100, -100, 100);

	
	glMatrixMode(GL_MODELVIEW);






	glGetFloatv(GL_MODELVIEW_MATRIX, mat);







	glLoadName(SPHERE1);
	glColor3f(0, 0.5f, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	DrawLatSphere(50, 32, 32);

	glGetFloatv(GL_PROJECTION_MATRIX, mat);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); // for projection 
	//glInitNames();
	//glPushName(0);

	////C++ÄÜÖ±½Ócall constructor????
	///*SolidSphere(50, 4, 3);
	//Vector3(1, 2, 3);*/


	////2D drawing 
	////enable2DProj();
	////glLoadIdentity();
	////glLoadName(10);
	////DrawGUI2D();
	////quit2DProj();
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//gluOrtho2D(0, 512, 0, 512);

	//DrawGUI2D();

	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	//int xx;
	////glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &xx);
	//printf("234");

	//// normal 3d space
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();

	//glOrtho(-100, 100, -100, 100, -100, 100);

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glColor3f(0, .5f, 1);

	//glLoadName(SPHERE2);
	//glTranslatef(30, 20, 10);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//DrawIcosSphere(30, 2);
	//glPopMatrix();

	//glPushMatrix();
	//glLoadName(SPHERE1);

	//SolidSphere s(50, 32, 32);
	//s.draw(-10, -20, -10);
	//glPopMatrix();


	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();

	////glMatrixMode(GL_MODELVIEW);
}

#include "House.h"
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glFrustum(-10.0, 10.0, -10.0, 10.0, 10, 1000.0);
	//glTranslatef(0, 0, -20); //????
	//simpleOrthoProj();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	

	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glLoadIdentity();
	//glLoadMatrixf(viewMatrix);
	/*float buf[16];
	gluLookAt(-15, 5, 10,  -5, 0, 0, 0, 1, 0.5);
	glGetFloatv(GL_MODELVIEW_MATRIX, buf);*/

	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);*/
	glColor3f(0, 0.5, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	glTranslatef(-0.3, -0.3, 0);
	glutSolidTeapot(0.3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0.2, 0);
	glutSolidSphere(0.5, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -1.1);
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0.5, 0);

		glVertex3f(-0.5, 0, 0);
	}
	glEnd();
	glPopMatrix();

	//glVertexPointer(3, GL_FLOAT, 0, House::vertices);
	//glColorPointer(3, GL_FLOAT, 0, House::colors);
	//glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, House::indices);

	glutSwapBuffers();

}


void DrawCube(int n)
{
	GLfloat l = (GLfloat)n / 2;

	glColor3f(0, .5f, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLE_STRIP);
	{
		//front and back
		glVertex3f(-l, l, l);
		glVertex3f(-l, -l, l);
		glVertex3f(l, l, l);
		glVertex3f(l, -l, l); 
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	{
		//front and back
		glVertex3f(-l, l, -l);
		glVertex3f(-l, -l, -l);
		glVertex3f(l, l, -l);
		glVertex3f(l, -l, -l);
	}
	glEnd();

	//left and right
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(-l, l, -l);
		glVertex3f(-l, -l, -l);
		glVertex3f(-l, l, -l);
		glVertex3f(-l, l, l);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(l, l, l);
		glVertex3f(l, -l, l);
		glVertex3f(l, l, -l);
		glVertex3f(l, -l, -l);
	}
	glEnd();


	//up and bottom
	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(-l, l, -l);
		glVertex3f(-l, l, l);
		glVertex3f(l, l, -l);
		glVertex3f(l, l, l);
	}
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	{
		glVertex3f(-l, -l, -l);
		glVertex3f(-l, -l, l);
		glVertex3f(l, -l, -l);
		glVertex3f(l, -l, l);
	}
	glEnd();

}

void DrawCubeTess(int n, int tessExtent)
{

	std::vector<GLfloat> drawArray;

	GLfloat l = (GLfloat)n / 2;

	GLshort vt[] = {
		//front
		-l, l, l, 
		-l, -l, l, 
		l, l, l, 

		l, l, l,
		-l, -l, l,
		l, -l, l, 


		//back
		-l, l, -l, 
		l, l, -l, 
		-l, -l, -l, 

		l, l, -l,
		-l, -l, -l,
		l, -l, -l, 

		//left
		-l, l, -l, 
		-l, -l, -l, 
		-l, l, l,

		-l, l, l, 
		-l, -l, -l, 
		-l, -l, l,

		//right
		l, l, l,
		l, -l, l, 
		l, l, -l,

		l, l, -l, 
		l, -l, l, 
		l, -l, -l,

		//top
		-l, l, -l, 
		-l, l, l, 
		l, l, -l, 

		l, l, -l, 
		-l, l, l, 
		l, l, l, 

		//bottom

		-l, -l, -l, 
		-l, -l, l, 
		l, -l, -l, 

		l, -l, -l, 
		-l, -l, l, 
		l, -l, l
	};


	for(int i = 0; i < 108; i += 9)
	{
		Vector3 vA = Vector3( vt[i]    , vt[i + 1], vt[i + 2] );
		Vector3 vB = Vector3( vt[i + 3], vt[i + 4], vt[i + 5] );
		Vector3 vC = Vector3( vt[i + 6], vt[i + 7], vt[i + 8] );

		divideTriangle(vA, vB, vC, drawArray, tessExtent);

		//render
		int drawArrayLength = drawArray.size();
		if(drawArrayLength % 9 != 0)
		{
			assert(false);
		}

		//Draw a single outer triangle
		for(int i = 0; i < drawArrayLength; i += 9)
		{
			vA.set(i    , drawArray);
			vB.set(i + 3, drawArray);
			vC.set(i + 6, drawArray);

			drawTriangle(vA, vB, vC);
		}

		drawArray.clear();
	}
}

void divideTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c, 
	std::vector<GLfloat> &drawArray, int tessExtent)
{
	Vector3 v[3];
	if(tessExtent > 0)
	{
		v[0].set((a['x'] + b['x']) / 2, 'x');
		v[0].set((a['y'] + b['y']) / 2, 'y');
		v[0].set((a['z'] + b['z']) / 2, 'z');

		v[1].set((a['x'] + c['x']) / 2, 'x');
		v[1].set((a['y'] + c['y']) / 2, 'y');
		v[1].set((a['z'] + c['z']) / 2, 'z');

		v[2].set((b['x'] + c['x']) / 2, 'x');
		v[2].set((b['y'] + c['y']) / 2, 'y');
		v[2].set((b['z'] + c['z']) / 2, 'z');

		divideTriangle(a   , v[0], v[1], drawArray, tessExtent - 1);
		divideTriangle(v[0], b   , v[2], drawArray, tessExtent - 1);
		divideTriangle(v[1], v[2], c,    drawArray, tessExtent - 1);
		divideTriangle(v[0], v[1], v[2], drawArray, tessExtent - 1);
	}
	else
	{
		//drawTriangle(a, b, c);
		drawArray.push_back(a['x']);
		drawArray.push_back(a['y']);
		drawArray.push_back(a['z']);

		drawArray.push_back(b['x']);
		drawArray.push_back(b['y']);
		drawArray.push_back(b['z']);

		drawArray.push_back(c['x']);
		drawArray.push_back(c['y']);
		drawArray.push_back(c['z']);
	}
}

void drawTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(a['x'], a['y'], a['z']);
		glVertex3f(b['x'], b['y'], b['z']);
		glVertex3f(c['x'], c['y'], c['z']);
	}
	glEnd();
}

void DrawCylinder(GLfloat radius, GLfloat height, int tessExtent)
{
	//control the tessellation to minimum 3 since a triangle top is the least cylinder
	if(tessExtent < 3)
	{
		tessExtent = 3;
	}
	else if(tessExtent > 360)
	{
		//for performance.. jiudan desu ga
		tessExtent = 360;
	}

	GLdouble angle = 360.0 / tessExtent;

	GLfloat halfHeight = height / 2.0f;

	Vector3 topPoint = Vector3(0, halfHeight, 0);
	Vector3 bottomPoint = Vector3(0, -halfHeight, 0);

	std::vector<Vector3 *> circumPoints;

	//Using float ended up with f = 359.9999 poping an extra loop
	for(GLdouble f = 0.0f; f < 360; f += angle)
	{
		GLfloat fRadian = f * M_PI / 180;
		Vector3 *p1 = new Vector3(radius * (GLfloat)cos(fRadian),  halfHeight, radius * (GLfloat)sin(fRadian));
		Vector3 *p2 = new Vector3(radius * (GLfloat)cos(fRadian), -halfHeight, radius * (GLfloat)sin(fRadian));
		circumPoints.push_back(p1);
		circumPoints.push_back(p2);
	}

	//draw head and bottom
	glBegin(GL_LINE_LOOP);
	{
		for(int i = 0; i < tessExtent; i++)
		{
			Vector3 *pVec1 = circumPoints[2 * i];
			glVertex3f(pVec1->get('x'), pVec1->get('y'), pVec1->get('z'));
			printf("POINTS: %f, %f, %f\n", pVec1->get('x'), pVec1->get('y'), pVec1->get('z'));
		}
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	{
		for(int i = 0; i < tessExtent; i++)
		{
			Vector3 *pVec2 = circumPoints[2 * i + 1];
			glVertex3f(pVec2->get('x'), pVec2->get('y'), pVec2->get('z'));
		}
	}
	glEnd();


	//connect vertical lines
	glBegin(GL_LINES);
	{
		for(int i = 0; i < circumPoints.size(); i += 2)
		{
			Vector3 *pVup = circumPoints[i];
			Vector3 *pVdown = circumPoints[i + 1];

			glVertex3f(pVup->get('x'), pVup->get('y'), pVup->get('z'));		
			printf("LINES: %f, %f, %f\n", pVup->get('x'), pVup->get('y'), pVup->get('z'));


			glVertex3f(pVdown->get('x'), pVdown->get('y'), pVdown->get('z'));			

		}
	}
	glEnd();

	//clear memory
	for(int i = 0; i < circumPoints.size(); i++)
	{
		delete(circumPoints[i]);
	}
}


//buggy version but looks great
//void DrawCubeTess(int n, int tessExtent)
//{
//
//	std::vector<GLfloat> drawArray;
//
//	GLfloat l = (GLfloat)n / 2;
//
//	GLushort vt[] = {
//		//front
//		-l, l, l, 
//		l, l, l, 
//		-l, -l, l, 
//
//		l, l, l,
//		-l, -l, l,
//		l, -l, l, 
//
//
//		//back
//		-l, l, -l, 
//		l, l, -l, 
//		-l, -l, -l, 
//
//		l, l, -l,
//		-l, -l, -l,
//		l, -l, -l, 
//
//		//left
//		-l, l, -l, 
//		-l, -l, -l, 
//		-l, l, l,
//
//		-l, l, l, 
//		-l, -l, -l, 
//		-l, -l, l,
//
//		//right
//		l, l, l,
//		l, -l, l, 
//		l, l, -l,
//
//		l, l, -l, 
//		l, -l, l, 
//		l, -l, -l,
//
//		//top
//		-l, l, -l, 
//		-l, l, l, 
//		l, l, -l, 
//
//		l, l, -l, 
//		-l, l, l, 
//		l, l, l, 
//
//		//bottom
//
//		-l, -l, -l, 
//		-l, -l, l, 
//		l, -l, -l, 
//
//		l, -l, -l, 
//		l, -l, l, 
//		l, -l, l
//	};
//
//
//	for(int i = 0; i < 108; i += 9)
//	{
//		Vector3 vA = Vector3( vt[i]    , vt[i + 1], vt[i + 2] );
//		Vector3 vB = Vector3( vt[i + 3], vt[i + 4], vt[i + 5] );
//		Vector3 vC = Vector3( vt[i + 6], vt[i + 7], vt[i + 8] );
//
//		divideTriangle(vA, vB, vC, drawArray, tessExtent);
//
//		//render
//		glRotatef(xRot, 1, 0, 0);
//		glRotatef(yRot, 0, 1, 0);
//
//
//		//set mirror matrix
//		Matrix4 mat;
//		mat.set(0, -1);
//		mat.set(5, -1);
//
//		int drawArrayLength = drawArray.size();
//		if(drawArrayLength % 9 != 0)
//		{
//			assert(false);
//		}
//
//		for(int i = 0; i < drawArray.size(); i += 9)
//		{
//			vA.set(i    , drawArray);
//			vB.set(i + 3, drawArray);
//			vC.set(i + 6, drawArray);
//
//			//draw first half
//			drawTriangle(vA, vB, vC);
//
//			/*Vector3 &transformedA = mat.multiplyVec3(vA);
//			Vector3 &transformedB = mat.multiplyVec3(vB);
//			Vector3 &transformedC = mat.multiplyVec3(vC);
//
//			drawTriangle(transformedA, transformedB, transformedA);*/
//		}
//	}
//}
//
//void divideTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c, 
//	                std::vector<GLfloat> &drawArray, int tessExtent)
//{
//	Vector3 v[3];
//	if(tessExtent > 0)
//	{
//		v[0].set((a['x'] + b['x']) / 2, 'x');
//		v[0].set((a['y'] + b['y']) / 2, 'y');
//		v[0].set((a['z'] + b['z']) / 2, 'z');
//
//		v[1].set((a['x'] + c['x']) / 2, 'x');
//		v[1].set((a['y'] + c['y']) / 2, 'y');
//		v[1].set((a['z'] + c['z']) / 2, 'z');
//
//		v[2].set((b['x'] + c['x']) / 2, 'x');
//		v[2].set((b['y'] + c['y']) / 2, 'y');
//		v[2].set((b['z'] + c['z']) / 2, 'z');
//
//		divideTriangle(a   , v[0], v[1], drawArray, tessExtent - 1);
//		divideTriangle(v[0], b,    v[2], drawArray, tessExtent - 1);
//		divideTriangle(v[1], v[2], c,    drawArray, tessExtent - 1);
//		divideTriangle(v[0], v[1], v[2], drawArray, tessExtent - 1);
//	}
//	else
//	{
//		//drawTriangle(a, b, c);
//		drawArray.push_back(a['x']);
//		drawArray.push_back(a['y']);
//		drawArray.push_back(a['z']);
//
//		drawArray.push_back(b['x']);
//		drawArray.push_back(b['y']);
//		drawArray.push_back(b['z']);
//
//		drawArray.push_back(c['x']);
//		drawArray.push_back(c['y']);
//		drawArray.push_back(c['z']);
//	}
//}
//
//void drawTriangle(const Vector3 &a, const Vector3 &b, const Vector3 &c)
//{
//	glBegin(GL_TRIANGLES);
//	{
//		glVertex3f(a['x'], a['y'], a['z']);
//		glVertex3f(b['x'], b['y'], b['z']);
//		glVertex3f(c['x'], c['y'], c['z']);
//	}
//	glEnd();
//}


void DrawCone(GLfloat radius, GLfloat height, int tessExtent)
{
	if(tessExtent < 3)
	{
		tessExtent = 3;
	}
	else if(tessExtent > 360)
	{
		tessExtent = 360;
	}

	GLfloat halfHeight = height / 2;
	GLdouble angle = 360.0 / tessExtent;
	std::vector<Vector3 *> circumVec;

	Vector3 topPoint = Vector3(0, height, 0);

	//push Circum points
	for(GLdouble f = 0.0; f < 360.0; f += angle)
	{
		GLdouble fRadian = f * M_PI / 180;
		Vector3 *vec = new Vector3(radius * (GLfloat)cos(fRadian), -halfHeight, radius * (GLfloat)sin(fRadian));
		circumVec.push_back(vec);
	}


	//draw the ircum
	glBegin(GL_LINE_LOOP);
	{
		for(int i = 0; i < circumVec.size(); i++)
		{
			Vector3 *v = circumVec[i];
			glVertex3f(v->get('x'), v->get('y'), v->get('z'));
		}
	}
	glEnd();


	//draw the vertical lines
	glBegin(GL_LINES);
	{
		for(int i = 0; i < circumVec.size(); i++)
		{
			Vector3 *v = circumVec[i];

			glVertex3f(topPoint.get('x'), topPoint.get('y'), topPoint.get('z'));
			glVertex3f(v->get('x'), v->get('y'), v->get('z'));
		}
	}
	glEnd();

	//free up memory
	for(int i = 0; i < circumVec.size(); i++)
	{
		delete(circumVec[i]);
	}

}

void DrawIcosSphere(GLfloat radius, int tessExtent)
{
	glColor3f(0, 0.5, 1);
	if(tessExtent > 10)
	{
		tessExtent = 10;
	}
	
	//first draw a icosahedron
	std::vector<Vector3 *> vecArr;
	
	const GLfloat goldenRatio = (sqrt(5.0) + 1) / 2;
	const GLfloat initialRadius = sqrt(1 + goldenRatio * goldenRatio);
	const GLfloat a = radius * 1 / initialRadius;
	const GLfloat b = radius * goldenRatio / initialRadius;

	//all 12 points for icosahedron
	vecArr.push_back(new Vector3(-a,  b,  0));  // 0 y
	vecArr.push_back(new Vector3(-a, -b,  0));
	vecArr.push_back(new Vector3( a,  b,  0));
	vecArr.push_back(new Vector3( a, -b,  0));  // 3

	vecArr.push_back(new Vector3( 0, -a,  b));  // z
	vecArr.push_back(new Vector3( 0, -a, -b));
	vecArr.push_back(new Vector3( 0,  a,  b));  // 6
	vecArr.push_back(new Vector3( 0,  a, -b));  

	vecArr.push_back(new Vector3(-b,  0,  a));  // x
	vecArr.push_back(new Vector3(-b,  0, -a));  // 9
	vecArr.push_back(new Vector3( b,  0,  a));
	vecArr.push_back(new Vector3( b,  0, -a));
	
	
	std::array<std::array<GLuint, 3>, 20> vecIndices = {
		{
			{2, 0, 6},
			{0, 2, 7}, 
			{0, 8, 6}, 
			{0, 9, 8}, 
			{0, 7, 9},
			{2, 6, 10}, 
			{2, 10, 11}, 
			{2, 11, 7}, 
			{6, 8, 4}, 
			{6, 4, 10},
			{7, 5, 9}, 
			{7, 11, 5},
			{8, 1, 4}, 
			{10, 4, 3}, 
			{4, 1, 3},
			{9, 5, 1}, 
			{5, 11, 3}, 
			{1, 5, 3},
			{8, 9, 1}, 
			{11, 10, 3}

		}
	};

	std::vector<GLfloat> tessTriangleArr;

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < 20; i++)
	{
		Vector3 *v1 = vecArr[ vecIndices[i][0] ];
		Vector3 *v2 = vecArr[ vecIndices[i][1] ];
		Vector3 *v3 = vecArr[ vecIndices[i][2] ];

		divideTriangle(*v1, *v2, *v3, tessTriangleArr, tessExtent);
		assert(tessTriangleArr.size() % 9 == 0);

		Matrix4 scaleMat;
		scaleMat.set(0, radius);
		scaleMat.set(5, radius);
		scaleMat.set(10, radius);

		for(int j = 0; j < tessTriangleArr.size(); j += 9)
		{
			Vector3 tempV1;
			tempV1.set(j, tessTriangleArr);
			Vector3 tempV2;
			tempV2.set(j + 3, tessTriangleArr);
			Vector3 tempV3;
			tempV3.set(j + 6, tessTriangleArr);

			//normalize all to unit 
			tempV1.normalize();
			tempV2.normalize();
			tempV3.normalize();

			//scale
			Vector3 &scaledV1 = scaleMat.multiplyVec3(tempV1);
			Vector3 &scaledV2 = scaleMat.multiplyVec3(tempV2);
			Vector3 &scaledV3 = scaleMat.multiplyVec3(tempV3);

			drawTriangle(scaledV1, scaledV2, scaledV3);
		}
	}
	glEnd();

	

	//free up memory
	for(int i = 0; i < vecArr.size(); i++)
	{
		delete(vecArr[i]);
	}
}

void DrawLatSphere(GLfloat radius, GLint latSegments, GLint longSegments)
{
	//the minimal setting to make things look like a sphere
	if(latSegments < 3)
	{
		latSegments = 3;
	}

	if(longSegments < 3)
	{
		longSegments = 3;
	}

	//for the additional top vertex
	const GLint _longSeg = longSegments + 1;

	GLfloat latPart  = 1.0f / latSegments;
	GLfloat longPart = 1.0f / longSegments;

	std::vector<Vector3> vertices;
	vertices.resize(_longSeg * longSegments);
	std::vector<Vector3>::iterator vertIt = vertices.begin();

	const GLfloat phi   = M_PI * longPart;
	const GLfloat theta = M_PI * 2 * latPart;

	//insert vertices needed to be drawn
	for(int longS = 0; longS < _longSeg; longS++)
	{
		for(int latS = 0; latS < latSegments; latS++)
		{
			const GLfloat x = radius * (GLfloat) sin( longS * phi ) * cos( latS * theta );
			const GLfloat z = radius * (GLfloat) sin( longS * phi ) * sin( latS * theta );
			const GLfloat y = radius * (GLfloat) cos( longS * phi );

			*vertIt++ = Vector3(x, y, z);
		}
	}

	//GLushort indices[] = {
	//	0, 1, 4, 3, 
	//	1, 2, 5, 4, 
	//	3, 4, 7, 6, 
	//	4, 5, 8, 7,
	//	6, 7, 10, 9, 
	//	7, 8, 11, 10
	//};

	//glBegin(GL_QUADS);
	//{
	//	for(int i = 0; i < 24; i++)
	//	{
	//		glVertex3f(vertices[indices[i]].get('x'),
	//			       vertices[indices[i]].get('y'),
	//				   vertices[indices[i]].get('z'));
	//	}
	//}
	//glEnd();

	
	std::vector<GLushort> indices;
	indices.resize(latSegments* (_longSeg - 1) * 4); // 4 vertices forms a quad
	std::vector<GLushort>::iterator indexIt = indices.begin();

	//insert the order to draw vertices
	for(int longS = 0; longS < _longSeg - 1; longS++)
	{
		for(int latS = 0; latS < latSegments; latS++)
		{
			if(latS == latSegments - 1)
			{
				*indexIt++ = longS * latSegments + latS;
				*indexIt++ = longS * latSegments;

				*indexIt++ = (longS + 1) * latSegments;
				*indexIt++ = (longS + 1) * latSegments + latS;
			}
			else  //normal case
			{
				*indexIt++ = longS * latSegments + latS;
				*indexIt++ = longS * latSegments + (latS + 1);

				//next line
				*indexIt++ = (longS + 1) * latSegments + (latS + 1);
				*indexIt++ = (longS + 1) * latSegments + latS;
			} 
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);

}

//void DrawLatSphere(GLfloat radius, int latSegments, int longSegments)
//{
//	if(latSegments < 3)
//	{
//		latSegments = 3;
//	}
//	
//	if(longSegments < 3)
//	{
//		longSegments = 3;
//	}
//
//	const GLfloat theta = 2 * (GLfloat)M_PI / latSegments;
//	const GLfloat phi   = 2 * (GLfloat)M_PI / longSegments;
//
//	std::vector<Vector3> vecArr;
//	std::vector<GLushort> indices;
//
//	vecArr.resize(latSegments * (longSegments / 2));
//	indices.resize((longSegments - 1) * (longSegments - 1) * 4);
//
//	std::vector<Vector3>::iterator vit = vecArr.begin();
//	for(int i = 0; i < longSegments / 2; i++)
//	{
//		for(int j = 0; j < latSegments; j++)
//		{
//			const GLfloat x = radius * (GLfloat)sin(i * phi) * cos(j * theta);
//			const GLfloat y = radius * (GLfloat)cos(i * phi);
//			const GLfloat z = radius * (GLfloat)sin(i * phi) * sin(j * theta);
//
//			//vecArr.push_back(Vector3(x, y, z));
//			*vit++ = Vector3(x, y, z);
//		}
//	}
//
//	assert(vecArr.size() % latSegments == 0);
//
//	//indices
//	std::vector<GLushort>::iterator it = indices.begin();
//	for(int i = 0; i < (longSegments / 2) - 1; i++)
//	{
//		for(int j = 0; j < latSegments - 1; j++)
//		{
//			*it++ = i * longSegments + j;
//			*it++ = i * longSegments + (j + 1);
//			*it++ = (i + 1) * longSegments + (j + 1);
//			*it++ = (i + 1) * longSegments + j;
//		}
//	}
//
//	//draw
//	glEnableClientState(GL_VERTEX_ARRAY);
//
//	glVertexPointer(3, GL_FLOAT, 0, &vecArr[0]);
//
//	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
//	glDisableClientState(GL_VERTEX_ARRAY);
//
//
//}

void enable2DProj()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();


	const GLuint w = glutGet(GLUT_WINDOW_WIDTH);
	const GLuint h = glutGet(GLUT_WINDOW_HEIGHT);

	//following opengl convention
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void quit2DProj()
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

#define GUI 10
void DrawGUI2D()
{
	const GLint uiWidth = 100;
	const GLint uiHeight = 30;
	const GLint offset = 10;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLint width = viewport[2];
	GLint height = viewport[3];

	GLfloat upperLeft[3] =  {width - offset - uiWidth, height - offset, 0};
	GLfloat upperRight[3] = {width - offset, height - offset, 0};
	GLfloat lowerRight[3] = {width - offset, height - offset - uiHeight, 0};
	GLfloat lowerLeft[3] =  {width - offset - uiWidth, height - offset - uiHeight, 0};


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	{
		glColor3f(0, 0.5, 0);
		glVertex3fv(upperLeft);
		glVertex3fv(upperRight);
		glVertex3fv(lowerRight);
		glVertex3fv(lowerLeft);
	}
	glEnd();


	/*glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		{
			glLoadIdentity();
			
			glLoadName(GUI);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_QUADS);
			{
				glColor3f(0, 0.5, 0);
				glVertex3fv(upperLeft);
				glVertex3fv(upperRight);
				glVertex3fv(lowerRight);
				glVertex3fv(lowerLeft);
			}
			glEnd();
			
		}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);*/
	
}
#undef GUI

void respondGUI2D(GLint signal, int x, int y)
{
	if(signal == 1)
	{
		//extremely highly coupled with resolution 512
		GLint offsetX = x - (512 - 10 - 100);
		GLfloat factor = (GLfloat)x / 100.0f;
		GLfloat rotAngle = 360 * factor;

		glRotatef(rotAngle, 0, 0, 1);
	}
}


