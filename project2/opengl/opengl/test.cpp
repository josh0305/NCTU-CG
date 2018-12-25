/*
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <cmath>
#include <Windows.h>
#include "glew.h"
#include "glut.h"
#include "FreeImage.h"


using namespace std;

void display();
void reshape(int _width, int _height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void passiveMouseMotion(int x, int y);
void idle();

void init();
void init_texture();
void lighting();
void drawSphere(double r, int stack, int slice);

unsigned char* readBMP(char* filename);

int width = 400, height = 400;
float degree_e = 0.0, degree_m = 0.0, degree_rev = 0.0, radius = 1.0, speed = 100;
static bool paused = false;
static bool not_sphere = false;

int image_width, image_height;
unsigned int texture_id;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("WindowName");

	glewInit();

	init();
	init_texture();

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
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
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

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glNormal3f(0, 0, 1);
	glTexCoord2f(5.0f, 5.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glNormal3f(0, 0, 1);
	glTexCoord2f(5.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();
	glPopMatrix();
	

	//moon
	//glPushMatrix();
	//glTranslatef(dis*cos(degree_e*M_PI / 180), 0.0, dis*sin(degree_e*M_PI / 180));
	//glRotatef(23.5, 0.0f, 0.0f, 1.0f);
	//glRotatef(degree_m, 0.0f, 1.0f, 0.0f);
	//glTranslatef(3 * radius + 3, 0.0, 0.0);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
	//glShadeModel(GL_SMOOTH);
	//drawSphere(radius, 60, 240);
	//glPopMatrix();

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
		degree_e = degree_e + speed / 365;
		if (degree_e >= 360.0f)
			degree_e = degree_e - 360.0f;

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

void drawSphere(double r, int stack, int slice)
{

	double s = image_width / slice;
	double t = image_height / stack;
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
			glTexCoord2f((double)i * s, (double)j * t);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glTexCoord2f((double)(i * s), (double)(j * t));
			glVertex3f(x * zr1, y * zr1, z1);
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


void init_texture()
{

	//enable 2D texture
	glEnable(GL_TEXTURE_2D);

	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType("./earth.jpg", 0), "./earth.jpg");
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);
	
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);

	glBindTexture(GL_TEXTURE_2D, 0);
}
*/