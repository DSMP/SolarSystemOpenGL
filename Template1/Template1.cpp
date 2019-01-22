// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include "Template1.h"


GLfloat M_PI = 3.14f;
GLint mass = 10;
GLfloat k_coeff = 1000;
GLint previous_x = 0;
GLint previous_y = 0;
GLfloat period = (float)(sqrt(mass / k_coeff));
bool growing = true;
int maxDistance = 1, zoom = -50;
double b = 0.6, eyex = 0.0, eyey = 0.0, centerx = 0.0, centery = 0.0;
double z_position = 0;
GLuint _textureId, _textureId2;

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
										 //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void ObjectWithTexture(float tx, float ty, float tz, GLuint texture, GLUquadric *quadric, float radius, int rows, int columns)
{
	glTranslatef(tx, ty, tz);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, radius, rows, columns);
	glTranslatef(-tx, -ty, -tz);
}


void Ball(int a, double radius) {
	glPushMatrix();
	glutSolidSphere(radius, 20, 20);
	glPopMatrix();
}
void Galactic(int a) {
	glPushMatrix();
	double sunRadius = 13.92;
	glColor3f(0.9, 0.7, 0.05);
	Ball(a, sunRadius); // sun
	glColor3f(1, 1, 1);
	glTranslatef(sunRadius + 5.8, 0, 0);
	Ball(a, 0.049); // Mercury
	glTranslatef(sunRadius + 10.8, 0, 0);
	Ball(a, 0.12); // Wenus
	glTranslatef(sunRadius + 14.9, 0, 0);
	Ball(a, 0.13); // Ziemia
	glTranslatef(sunRadius + 228, 0, 0);
	Ball(a, 0.07); // Mars
	glTranslatef(sunRadius + 778, 0, 0);
	Ball(a, 1.42); // Jowisz
	glTranslatef(sunRadius + 142.6, 0, 0);
	Ball(a, 1.20); // Saturn
	glTranslatef(sunRadius + 287.1, 0, 0);
	Ball(a, 0.51); // Uran
	glTranslatef(sunRadius + 449.8, 0, 0);
	Ball(a, 0.49); // 	Neptun
	glPopMatrix();
}
void MyDisplay(void) {
	// The new scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Galactic(1);
	glLoadIdentity();
	gluLookAt(eyex, eyey, zoom, 0, 0, 0, 0, 1, 0);
	// The end of scene
	glFlush();//start processing buffered OpenGL routines
}
void MyInit(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);//select clearing (background) color
									 /* initialize viewing values */
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//=1
	
	gluPerspective(70.0, 1.777777777777778, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
	zoom = -50;
}
void OnMotion(int x, int y)
{
	printf(" OnMotion prev: %d %d now: %d %d ", previous_x, previous_y, x, y);
	if (previous_x < x)
	{
		printf("previous_x < x");
		eyex += 0.1*10;
		//centerx -= -0.05;
	}
	if (previous_x > x)
	{
		eyex += -0.1*10;
		//centerx -= -0.05;
		printf("previous_x > x");
	}
	if (previous_y < y)
	{
		eyey -= 0.1*10;
		//centerx -= -0.05;
		printf("previous_y < y");
	}
	if (previous_y > y)
	{
		eyey += +0.1*10;
		//centerx -= -0.05;
		printf("previous_y > y");
	}
	previous_x = x;
	previous_y = y;
}
void glutKeyboardFunc(int key, int x, int y)
{
	switch (key)
	{
	case 27: break;
	case 101:
		zoom += 20; 
		break;
	case 103:
		zoom -= 20;
		break;
	case 114: // r
		zoom = -50;
		eyex = 0.0;
		eyey = 0.0;
		centerx = 0.0;
		break;
	}

}
void render(int a)
{
	Galactic(a);
	glutPostRedisplay();
	Growing();
	glutTimerFunc(25, render, a);
}
void Growing()
{
	if (growing)
	{
		b+=0.01;
	}
	else
	{
		b -= 0.01;
	}
	if (b <= 0.4)
	{
		growing = true;
	}
	if (b >= maxDistance)
	{
		growing = false;
	}
}

int main(int argc, char** argv) { //<- for normal API
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//single buffer and RGBA
	glutInitWindowSize(1100, 600);//initial window size
	glutInitWindowPosition(50, 50);
	glutCreateWindow("My window");//create widnow, hello title bar
	MyInit();
	glutDisplayFunc(MyDisplay);//
	glutMotionFunc(OnMotion);
	glutSpecialFunc(glutKeyboardFunc);
	glutTimerFunc(100, render, 0.6);
	glutMainLoop();//enter main loop and process events
	return 0;
}