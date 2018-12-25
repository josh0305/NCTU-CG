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
#include "shader.h"
#include <iostream>

using namespace std;

struct VertexAttribute
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texcoord[2];
};

void display();
void reshape(int _width, int _height);
void keyboard(unsigned char key, int x, int y);
void idle();
void init();
void init_texture(const char*, GLuint *);
void lighting();
VertexAttribute *drawSphere(int stack, int slice);

int width = 400, height = 400;
float degree_e = 0.0, degree_m = 0.0, degree_rev = 0.0, radius = 1.0, speed = 1;
int slice = 360, stack = 180;
static bool paused = false;
static bool not_sphere = false;
static float tex_on = 1.0, nor_on = 1.0, spec_on = 1.0;

GLuint program;
GLuint vboName;

GLuint tex_id, nor_id, spec_id;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("WindowName");
	glewInit();

	init();

	init_texture("./earth_texture_map.jpg", &tex_id);
	init_texture("./earth_normal_map.tif", &nor_id);
	init_texture("./earth_specular_map.tif", &spec_id);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

void init() {
	GLuint vert = createShader("shader/multimap.vert", "vertex");
	GLuint geom = createShader("shader/multimap.geom", "geometry");
	GLuint frag = createShader("shader/multimap.frag", "fragment");
	program = createProgram(vert, geom, frag);

	glGenBuffers(1, &vboName);
	glBindBuffer(GL_ARRAY_BUFFER, vboName);

	VertexAttribute *vertices;
	vertices = drawSphere(stack, slice);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * slice*stack * 6, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)(offsetof(VertexAttribute, position)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)(offsetof(VertexAttribute, normal)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)(offsetof(VertexAttribute, texcoord)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void display()
{
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, width / (GLfloat)height, 0.1, 1000);
	//Viewport Matrix
	glViewport(0, 0, width, height);

	lighting();

	GLfloat RevM[16] = { (float)cos(degree_rev*PI / 180),	0,		(float)sin(degree_rev*PI / 180),	0,
		0,	1.0f,								0,	0,
		(float)-sin(degree_rev*PI / 180),	0,		(float)cos(degree_rev*PI / 180),	0,
		0,	0,									0,	1.0f };

	GLfloat TiltM[16] = { (float)cos(23.5*PI / 180),	(float)-sin(23.5*PI / 180),	0,		0,
		(float)sin(23.5*PI / 180),	(float)cos(23.5*PI / 180),	0,		0,
		0,							0,		1.0f,	0,
		0,							0,		0,		1.0f };
	//Assign Value
	GLfloat pmtx[16];
	GLfloat mmtx[16];
	glGetFloatv(GL_PROJECTION_MATRIX, pmtx);
	glGetFloatv(GL_MODELVIEW_MATRIX, mmtx);

	GLfloat lightpos[] = { 3.0f * (float)cos(degree_m*PI / 180), 0.0f, 3.0f * (float)sin(degree_m*PI / 180) };
	GLfloat controls[] = { tex_on, nor_on, spec_on };
	GLfloat camera[] = { 0, 0, 3 };

	//Get Location
	GLint pmatLoc = glGetUniformLocation(program, "Projection");
	GLint mmatLoc = glGetUniformLocation(program, "ModelView");
	GLint RmatLoc = glGetUniformLocation(program, "Revolve");
	GLint TmatLoc = glGetUniformLocation(program, "Tilt");
	GLint mvmLoc = glGetUniformLocation(program, "MVM");

	GLint texloc = glGetUniformLocation(program, "Earth_tex");
	GLint norloc = glGetUniformLocation(program, "Earth_nor");
	GLint specloc = glGetUniformLocation(program, "Earth_spec");

	GLint lightloc = glGetUniformLocation(program, "Light");
	GLint conloc = glGetUniformLocation(program, "Control");
	GLint camloc = glGetUniformLocation(program, "Camera");

	glUseProgram(program);

	//Send to Shader
	glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, pmtx);
	glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, mmtx);
	glUniformMatrix4fv(RmatLoc, 1, GL_FALSE, RevM);
	glUniformMatrix4fv(TmatLoc, 1, GL_FALSE, TiltM);
	glUniformMatrix4fv(mvmLoc, 1, GL_FALSE, mmtx);

	glUniform3fv(lightloc, 1, lightpos);
	glUniform3fv(conloc, 1, controls);
	glUniform3fv(camloc, 1, camera);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glUniform1i(texloc, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, nor_id);
	glUniform1i(norloc, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, spec_id);
	glUniform1i(specloc, 2);


	glDrawArrays(GL_TRIANGLES, 0, slice*stack * 6);
	glBindTexture(GL_TEXTURE_2D, NULL);
	glUseProgram(0);

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
	case '1':
		tex_on = -tex_on;
		break;
	case '2':
		nor_on = -nor_on;
		break;
	case '3':
		spec_on = -spec_on;
	default:
		break;
	}
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

		degree_m = degree_m + speed / 10.0f;
		if (degree_m >= 360.0f)
			degree_m = degree_m - 360.0f;
	}
	glutPostRedisplay();
}

VertexAttribute* drawSphere(int slice, int stack) {
	VertexAttribute vertices[4];
	VertexAttribute *vert = new VertexAttribute[slice * stack * 6];
	int direction[4][2] = { { 0, 0 },{ 1, 0 },{ 0, 1 },{ 1, 1 } };
	int index[6] = { 2, 1, 0, 2, 3, 1 };
	float slice_step = 2 * PI / slice, stack_step = PI / stack;
	int count = 0;
	for (int i = 0; i < slice; i++) {
		for (int j = 0; j < stack; j++) {
			for (int k = 0; k < 4; k++) {
				vertices[k].position[0] = vertices[k].normal[0] = sin((j + direction[k][1]) * stack_step) * cos((i + direction[k][0])*slice_step);
				vertices[k].position[1] = vertices[k].normal[1] = cos((j + direction[k][1]) * stack_step);
				vertices[k].position[2] = vertices[k].normal[2] = sin((j + direction[k][1]) * stack_step) * sin((i + direction[k][0])*slice_step);
				vertices[k].texcoord[0] = float(i + direction[k][0]) / slice;
				vertices[k].texcoord[1] = 1 - float(j + direction[k][1]) / stack;
			}
			for (int k = 0; k < 6; k++) {
				vert[count++] = vertices[index[k]];
			}
		}
	}
	return vert;
}

void lighting()
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	float diffuse[] = { 0.35f, 0.3f, 0.15f, 1.0f };
	float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float position[] = { 3 * cos(degree_m*PI / 180), 0.0, 3 * sin(degree_m*PI / 180), 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}


void init_texture(const char* fn, GLuint *id)
{

	//enable 2D texture
	glEnable(GL_TEXTURE_2D);

	FIBITMAP* pImage = FreeImage_Load(FreeImage_GetFileType(fn, 0), fn);
	FIBITMAP* p32BitsImage = FreeImage_ConvertTo32Bits(pImage);
	int iWidth = FreeImage_GetWidth(p32BitsImage);
	int iHeight = FreeImage_GetHeight(p32BitsImage);

	//GLuint texture_id;

	glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, *id);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return;
}
