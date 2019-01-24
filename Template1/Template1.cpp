// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include "Template1.h"


GLfloat M_PI = 3.14f;
GLint previous_x = 0;
GLint previous_y = 0;
bool growing = true;
int maxDistance = 1, zoom = -50;
double b = 0.6, eyex = 0.0, eyey = 0.0, centerx = 0.0, centery = 0.0;
double z_position = 0;
GLuint _textureId, sun, Mercury, Wenus, Earth, Mars, Jupiter, Saturn, Uranus, Neptun;
GLuint* textures = new GLuint[9]{ 0,0,0,0,0,0,0,0,0 };
double spaceRotation = 0, planetScaling = 10, PlanetRotationScale = 0.001, PlanetMovementScale = 0.01;
double* planetsAroundSun = new double[9] {87.969, 224.701, 365.256, 686.960, 4333.287,10756.2,30707.490,60223.353,2.5};
double* planetsRotate = new double[9]{ 58, 243, 23.9333, 24.34, 9.93, 10.39, 17.10, 16, 25.9 };
double* MovementSpeed = new double[9]{0,0,0,0,0,0,0,0,0};
double* RotationSpeed = new double[9]{ 0,0,0,0,0,0,0,0,0 };
GLUquadric* quadric;

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


void Ball(int a, double radius, int planetId) {
	MovementSpeed[planetId] += 1/planetsAroundSun[planetId] * PlanetMovementScale;
	glRotatef(MovementSpeed[planetId], 0, 0, 0);
	glPushMatrix();
	RotationSpeed[planetId] += planetsRotate[planetId] * PlanetRotationScale;
	glRotatef(RotationSpeed[planetId], 0, 1, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, textures[planetId]);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, radius, 20, 20);
	glPopMatrix();
}
void Galactic(int a) {
	glPushMatrix();
	double sunRadius = 13.92;
	glColor3f(0.9, 0.7, 0.05);
	//glRotatef(spaceRotation, 0, 1, 0);
	Ball(a, sunRadius,8); // sun
	glColor3f(1, 1, 1);
	glTranslatef(sunRadius + 5.8, 0, 0);
	Ball(a, 0.049 * planetScaling, 0); // Mercury
	glTranslatef(sunRadius + 10.8, 0, 0);
	Ball(a, 0.12 * planetScaling, 1); // Wenus
	glTranslatef(sunRadius + 14.9, 0, 0);
	Ball(a, 0.13 * planetScaling, 2); // Ziemia
	glTranslatef(sunRadius + 228, 0, 0);
	Ball(a, 0.07 * planetScaling, 3); // Mars
	glTranslatef(sunRadius + 778, 0, 0);
	Ball(a, 1.42 * planetScaling, 4); // Jowisz
	glTranslatef(sunRadius + 142.6, 0, 0);
	Ball(a, 1.20 * planetScaling, 5); // Saturn
	glTranslatef(sunRadius + 287.1, 0, 0);
	Ball(a, 0.51 * planetScaling, 6); // Uran
	glTranslatef(sunRadius + 449.8, 0, 0);
	Ball(a, 0.49 * planetScaling, 7); // 	Neptun
	glPopMatrix();
}
void MyDisplay(void) {
	// The new scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyex, eyey, zoom, 0, 0, 0, 0, 1, 0);
	render(1);
	// The end of scene soil
	glFlush();//start processing buffered OpenGL routines
	glutTimerFunc(25, render, 1);
}
void MyInit(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);//select clearing (background) color
	
	glEnable(GL_TEXTURE_2D); /* initialize viewing values */
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//=1
	
	gluPerspective(70.0, 1.777777777777778, 1, 700);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
	zoom = -50;
	Image* image;
	image = loadBMP("steel24.bmp");
	_textureId = loadTexture(image);
	delete image;
	image = loadBMP("sun24.bmp");
	sun = loadTexture(image);
	textures[8] = sun;
	delete image;
	image = loadBMP("Mercury24.bmp");
	Mercury = loadTexture(image);
	textures[0] = Mercury;
	delete image;
	image = loadBMP("Venus24.bmp");
	Wenus = loadTexture(image);
	textures[1] = Wenus;
	delete image;
	image = loadBMP("Earth24.bmp");
	Earth = loadTexture(image);
	textures[2] = Earth;
	delete image;
	image = loadBMP("Mars24.bmp");
	Mars = loadTexture(image);
	textures[3] = Mars;
	delete image;
	image = loadBMP("Jupiter24.bmp");
	Jupiter = loadTexture(image);
	textures[4] = Jupiter;
	delete image;
	image = loadBMP("Saturn24.bmp");
	Saturn = loadTexture(image);
	textures[5] = Saturn;
	delete image;
	image = loadBMP("Uranus24.bmp");
	Uranus = loadTexture(image);
	textures[6] = Uranus;
	delete image;
	image = loadBMP("Neptune24.bmp");
	Neptun = loadTexture(image);
	delete image;
	textures[7] = Neptun = loadTexture(image);
	;

	quadric = gluNewQuadric();
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
	spaceRotation += 0.01;
	Galactic(spaceRotation);
	glutPostRedisplay();
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
	glutMainLoop();//enter main loop and process events
	return 0;
}