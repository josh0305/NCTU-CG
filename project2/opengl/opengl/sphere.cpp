#include <cmath>
#include "stdafx.h"

#define PI 3.14159265358

void mySphere(int slice, int stack) {
	double s = 1 / slice;
	double t = 1 / stack;
	double x, y, z;
	double slice_step = 2 * PI / slice, stack_step = PI / stack;
	for (int i = 0; i < slice; i++) {
		glBegin(GL_TRIANGLE_STRIP);
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
			glTexCoord2f((double)j * t, (double)i * s);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glTexCoord2f((double)(j * t), (double)(i * s));
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
	
}