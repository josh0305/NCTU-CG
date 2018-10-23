#define _USE_MATH_DEFINES 
#include "glut.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <Windows.h>

using namespace std;

void display();
void reshape(int _width, int _height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void passiveMouseMotion(int x, int y);
void idle();
void lighting();
void drawSphere(double r, int stack, int slice);

int width = 400, height = 400;
int degree = 0, radius = 1;
static bool paused = false;
static bool not_sphere = false;

int main(int argc, char** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
glutInitWindowSize(width, height);
glutInitWindowPosition(0, 0);
glutCreateWindow("WindowName");

lighting();

glutDisplayFunc(display);
glutReshapeFunc(reshape);
glutKeyboardFunc(keyboard);
glutMouseFunc(mouse);
glutMotionFunc(mouseMotion);
glutPassiveMotionFunc(passiveMouseMotion);
glutIdleFunc(idle);

glutMainLoop();

return 0;
}

void display()
{
//ModelView Matrix
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(0.0f, 50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
//Projection Matrix
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45, width / (GLfloat)height, 0.1, 1000);
//Viewport Matrix
glViewport(0, 0, width, height);
//
glMatrixMode(GL_MODELVIEW);
glEnable(GL_CULL_FACE);		//remove back face
glEnable(GL_DEPTH_TEST);
glEnable(GL_NORMALIZE);
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
glClearDepth(1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//color
GLfloat red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat yellow[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

//sun
glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
drawSphere(6 * radius, 60, 240);


//earth
glRotatef(23, 0.0f, 0.0f, 1.0f);
glRotatef(degree /365, 0.0f, 1.0f, 0.0f);
glTranslatef(8 * radius + 20, 0.0, 0.0);
glRotatef(degree ,0.0f, 1.0f, 0.0f);
glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
glShadeModel(GL_SMOOTH);
int slice = 360, stack = 180;
if (not_sphere)
{
	slice = 4;
	stack = 2;
}
drawSphere(2 * radius, stack, slice);

//Revolving axis
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
glBegin(GL_LINES);
glVertex3f(0, 4.0*radius, 0.0);
glVertex3f(0, -4.0*radius, 0.0);
glEnd();

//moon
glRotatef(degree / 28, 0.0f, 1.0f, 0.0f);
glTranslatef(3 * radius + 3, 0.0, 0.0);
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
glShadeModel(GL_SMOOTH);
drawSphere(radius, 60, 240);

glPopMatrix();

glutSwapBuffers();
}

void reshape(int _width, int _height) {
width = _width;
height = _height;
}


void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'p':
		paused = !paused;
		break;
	case 'o':
		not_sphere = !not_sphere;
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y) {
}

void mouseMotion(int x, int y) {
}

void passiveMouseMotion(int x, int y) {
}

void idle() {
Sleep(20);	//about 50 fps
if(!paused)	
	degree = (degree + 1) % 360;
glutPostRedisplay();
}

void drawSphere(double r, int stack, int slice)
{
	for (int i = 0; i <= stack; i++)
	{
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / stack);
		double z0 = sin(lat0)*r;
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / stack);
		double z1 = sin(lat1)*r;
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= slice; j++)
		{
			double lng = 2 * M_PI * (double)(j - 1) / slice;
			double x = cos(lng)*r;
			double y = sin(lng)*r;

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}

void lighting()
{
	// enable lighting
	glEnable(GL_LIGHTING);
	//Add directed light
	GLfloat diffuse_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambient_color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glEnable(GL_LIGHT0);								//open light0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);	//set diffuse color of light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);	//set ambient color of light0
	glLightfv(GL_LIGHT0, GL_POSITION, position);		//set position of light 0
}