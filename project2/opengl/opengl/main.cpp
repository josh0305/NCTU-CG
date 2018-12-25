
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define PI 3.14159265358

#include <iostream>
#include <math.h>
#include <cmath>
#include <Windows.h>
#include "glew.h"
#include "glut.h"
#include "FreeImage.h"


using namespace std;

struct VertexAttribute {
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texcoord[2];
};

void display();
void reshape(int _width, int _height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void passiveMouseMotion(int x, int y);
void idle();

void init();
GLuint init_texture(const char*);
void lighting();
void drawSphere(int stack, int slice);

int width = 400, height = 400;
float degree_e = 0.0, degree_m = 0.0, degree_rev = 0.0, radius = 1.0, speed = 50;
static bool paused = false;
static bool not_sphere = false;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("WindowName");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

void display()
{
	//ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 5.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, width / (GLfloat)height, 0.1, 1000);
	//Viewport Matrix
	glViewport(0, 0, width, height);
	
	//clear
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	lighting();

	//color
	GLfloat red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };


	//earth
	glBindTexture(GL_TEXTURE_2D, init_texture("./earth.jpg"));
	glPushMatrix();
	glRotatef(-23.5, 0.0f, 0.0f, 1.0f);
	glRotatef(degree_rev, 0.0f, 1.0f, 0.0f);
	glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	glScalef(radius, radius, radius);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	int slice = 360, stack = 180;
	if (not_sphere)
	{
		slice = 4;
		stack = 2;
	}
	drawSphere( stack, slice);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Revolving axis
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
	glBegin(GL_LINES);
	glVertex3f(0, 2.0*radius, 0.0);
	glVertex3f(0, -2.0*radius, 0.0);
	glEnd();
	glPopMatrix();

	//moon
	glBindTexture(GL_TEXTURE_2D, init_texture("./moon.jpg"));
	glPushMatrix();
	//glTranslatef(dis*cos(degree_e*M_PI / 180), 0.0, dis*sin(degree_e*M_PI / 180));
	//glRotatef(23.5, 0.0f, 0.0f, 1.0f);
	glRotatef(degree_m, 0.0f, 1.0f, 0.0f);
	glScalef(0.5*radius, 0.5*radius, 0.5*radius);
	glTranslatef(3 * radius + 1.5*radius, 0.0, 0.0);
	drawSphere(60, 240);
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
	if (!paused)
	{
		//degree_e = degree_e + speed / 365;
		//if (degree_e >= 360.0f)
		//	degree_e = degree_e - 360.0f;

		degree_rev = degree_rev + speed;
		if (degree_rev >= 360.0f)
			degree_rev = degree_rev - 360.0f;

		degree_m = degree_m + speed / 28;
		if (degree_m >= 360.0f)
			degree_m = degree_m - 360.0f;
	}
	glutPostRedisplay();
}

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);

	//enable lighting
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
}

/*void drawSphere(double r, int stack, int slice)
{
	double s = 1 / (double)slice;
	double t = 1 / (double)stack;
	double x, y, z;
	double slice_step = 2 * PI / slice, stack_step = PI / stack;
	for (int i = 0; i < slice; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < stack + 1; j++) {
			x = sin(j * stack_step) * cos(i*slice_step);
			y = cos(j * stack_step);
			z = sin(j * stack_step) * sin(i*slice_step);
			glNormal3d(x, y, z);
			glTexCoord2f((double)i * s, (double)j * t);
			glVertex3d(x, y, z);

			x = sin(j * stack_step) * cos((i + 1)*slice_step);
			y = cos(j * stack_step);
			z = sin(j * stack_step) * sin((i + 1)*slice_step);
			glNormal3d(x, y, z);
			glTexCoord2f((double)(i + 1) * s, (double)j * t);
			glVertex3d(x, y, z);
		}
		glEnd();
	}
}*/

void drawSphere(int slice, int stack) 
{
	VertexAttribute vertices[4];
	int direction[4][2] = { { 0, 0 },{ 1, 0 },{ 0, 1 },{ 1, 1 } };
	int index[6] = { 2, 1, 0, 2, 3, 1 };
	float slice_step = 2 * PI / slice, stack_step = PI / stack;
	for (int i = 0; i < slice; i++) {
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < stack; j++) {
			for (int k = 0; k < 4; k++) {
				vertices[k].position[0] = vertices[k].normal[0] = sin((j + direction[k][1]) * stack_step) * cos((i + direction[k][0])*slice_step);
				vertices[k].position[1] = vertices[k].normal[1] = cos((j + direction[k][1]) * stack_step);
				vertices[k].position[2] = vertices[k].normal[2] = sin((j + direction[k][1]) * stack_step) * sin((i + direction[k][0])*slice_step);
				vertices[k].texcoord[0] = float(i + direction[k][0]) / slice;
				vertices[k].texcoord[1] = 1 - float(j + direction[k][1]) / stack;
			}
			for (int k = 0; k < 6; k++) {
				glNormal3fv(vertices[index[k]].normal);
				glTexCoord2fv(vertices[index[k]].texcoord);
				glVertex3fv(vertices[index[k]].position);
			}
		}
		glEnd();
	}
}

void lighting()
{
	//Add directed light
	float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float position[] = { 0.0f, 10.0f, 0.0f, 1.0f };

	glEnable(GL_LIGHT0);		
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);	
}


GLuint init_texture(const char* fn)
{
	
	//enable 2D texture
	glEnable(GL_TEXTURE_2D);

	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(fn, 0), fn);
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	GLuint texture_id;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture_id;
}
