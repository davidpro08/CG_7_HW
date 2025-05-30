#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>
#include <random>
#include <limits>
#define _USE_MATH_DEFINES
#include <cmath>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//// EX1
//void reshape(int w, int h)
//{
//	glutPostRedisplay();
//}
//void display()
//{
//	glutSwapBuffers();
//}

//// EX2
//void reshape(int w, int h)
//{
//	glutPostRedisplay();
//}
//void display()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glutSwapBuffers();
//}

//// EX3
//void reshape(int w, int h)
//{
//	glutPostRedisplay();
//}
//void display()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glBegin(GL_TRIANGLES);
//	glColor3f(1, 0, 0);
//	glVertex2f(0, 0);
//	glColor3f(0, 1, 0);
//	glVertex2f(1, 0);
//	glColor3f(0, 0, 1);
//	glVertex2f(0, 1);
//	glEnd();
//
//	glutSwapBuffers();
//}

//// EX4
//void reshape(int w, int h)
//{
//	glutPostRedisplay();
//}
//void display()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glBegin(GL_TRIANGLE_STRIP);
//	glVertex2f(-5, -5);
//	glVertex2f(0, 5);
//	glVertex2f(5, -5);
//	glVertex2f(10, 5);
//	glVertex2f(15, -5);
//	glVertex2f(20, 5);
//	glEnd();
//
//	glutSwapBuffers();
//}

//// EX5
//void reshape(int w, int h)
//{
//	glutPostRedisplay();
//}
//void display()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
//	glTranslatef(0, 0, -7);
//	glScalef(2, 2, 2);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000);
//	glViewport(0, 0, 512, 512);
//
//	glutSolidSphere(1, 32, 16);
//
//	glutSwapBuffers();
//}

//// EX6
//void reshape(int w, int h)
//{
//	glutPostRedisplay();
//}
//void display()
//{
//	glClearColor(0, 0, 0, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
//	glTranslatef(0, 0, -7);
//	glScalef(2, 2, 2);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000);
//	glViewport(0, 0, 512, 512);
//
//	glEnable(GL_DEPTH_TEST);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef(0, 0, -7);
//	glColor3f(0, 0, 1);
//	glutSolidSphere(2, 32, 16);
//	glLoadIdentity();
//	glTranslatef(2, 0, -10);
//	glColor3f(1, 0, 0);
//	glutSolidSphere(2, 32, 16);
//
//	glutSwapBuffers();
//}

// EX7
void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000);
	glViewport(0, 0, 512, 512);

	glutPostRedisplay();
}
void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	glTranslatef(0, 0, -7);
	glScalef(2, 2, 2);

	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float ka[] = { 0, 1, 0, 0 };
	float kd[] = { 0, 0.5, 0, 0 };
	float ks[] = { 0.5, 0.5, 0.5, 0 };
	float p = 32;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT, GL_SHININESS, p);

	float Ia[] = { 0.2, 0.2, 0.2, 0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ia);

	float l[] = { -4, 4, 4, 1 };
	float la[] = { 0, 0, 0, 0 };
	float ld[] = { 1, 1, 1, 0 };
	float ls[] = { 1, 1, 1, 0 };

	glLightfv(GL_LIGHT0, GL_POSITION, l);
	glLightfv(GL_LIGHT0, GL_AMBIENT, la);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ls);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -7);
	glColor3f(0, 0, 1);
	glutSolidSphere(2, 32, 16);

	//
	glShadeModel(GL_FLAT);

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	// Initialize GLUT.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutCreateWindow("OpenGL");
	// Set up GLUT callbacks.
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	// Start rendering. 
	glutMainLoop();
	return 0;
}