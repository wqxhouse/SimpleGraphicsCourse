//#include <stdio.h>
//#include <stdlib.h>
//#include <GL/glut.h>
//#include <vector>
//#include <algorithm>
//#include <iostream>
//#include <math.h>
//#include "assert.h"
//#include <array>
//
//#include "Vector3.h"
//#include "Matrix4.h"
//
//void drawshape()
//{
//	glutWireSphere(50, 32, 32);
//}
//
//
//void render()
//{
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	glColor3f(0, 0, 0);
//	drawshape();
//
//	glutSwapBuffers();
//}
//
//
//void processClick(int x, int y)
//{
//	//glRenderMode(GL_SELECT);
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//
//	glViewport(0, 0, 100, 100);
//	gluPerspective(45, 1, 1, 200);
//
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//}
//
//void mouse(int button, int state, int x, int y)
//{
//	if(state == GLUT_DOWN)
//	{
//		printf("hello");
//		processClick(x, y);
//	}
//}
//
//void reshape(int width, int height)
//{
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	glOrtho(-100, 100, -100, 100, -100, 100);
//
//	glMatrixMode(GL_MODELVIEW);
//}
//
//
//
//
//int main(int argc, char **argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(512, 512);
//	glutCreateWindow("test");
//
//	glutDisplayFunc(render);
//	glutReshapeFunc(reshape);
//	
//	glutMouseFunc(mouse);
//
//	glEnable(GL_DEPTH_TEST);
//	glClearColor(1, 1, 1, 1);
//
//	glutMainLoop();
//	return 0;
//}