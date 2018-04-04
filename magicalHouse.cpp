/******************************************************************************
*
* Official Name:  Farhana Mustafa
*
* E-mail:  fmustafa@syr.edu
*
* Assignment:  Homework 4
*
* Environment/Compiler:  Visual Studio 2015
*
* Date:  April 3rd, 2018
*
* References:  none
*
* Interactions:
Moving around:
	Up arrow to move forward.
	Down arrow to move backwards.
	Left arrow to rotate to the left.
	Right arrow to rotate to the right.
	Press the shift key with the up arrow to look up.
	Press the shift key with the down arrow to look down.
Door:
	Left click on door to open/close it.
	OR Press space bar to open/close door.
Key:
	Left click on key to animate it.
	OR Press 'k' to animate the key.
	Press 'r' to turn the key to the right.
	Press 'l' (lowercase  L) to turn the key to the left.
Light Switch:
	Left click on light switch to turn it on/off.
	OR Press 't' to turn the light switch on/off.
Misc:
	Press '0' (zero) to get an overhead view.
	Press 's' to enable the headlight.
	Press the escape key to exit the program.
Cheat Sheet:
	Pressing 'r' is the correct way to open treasure chest. (Gold appears in chest)
	Pressing 'l' is the incorrect way to open treasure chest. (Nothing happens)
******************************************************************************/

#include <iostream>
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159
using namespace std;

static float meX = 0, meY = 0, meZ = 0;
static float angle = 0;
static float stepsize = 5.0;  //step size
static float turnsize = 10.0; //degrees to turn
static float pointOfView = 0.0;
static float spotExponent = 2.0;
static float xMove = 0.0, zMove = 5.0;
static float spotAngle = 10.0;
static int isSelecting = 0; // In selection mode?
static int hits; // Number of entries in hit buffer.
static unsigned int buffer[1024]; // Hit buffer.
static unsigned int closestName = 0; // Name of closest hit.
static int isAnimate = 0; // Animated?
static int animationPeriod = 50; // Time interval between frames.
static int t = 0; // Time parameter.

static GLUquadricObj *qobj; // Create a pointer to a quadric object.
static bool overhead = false;
static int doorOpen = 0;
static int lightSwitchClicked = 0;
static int lampClicked = 1;
static int rButtonWasPressed = 0;
static int lButtonWasPressed = 0;
static int inout = 1;
static int enableSpotLight = 0;
static int xTrans = -36.0;
static int yTrans = -9.0;
static int zTrans = 0;
static int keyAngle = 0.0;
static int keyPartAngle = 90.0;
static int lidAngle = 0.0;
static int moveLidBack = 104.0;
static int moveLidUp = -15.0;

static long font = (long)GLUT_BITMAP_8_BY_13; //Fonts

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Initialization routine.
void setup(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	glEnable(GL_LIGHTING);

	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec0[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightDifAndSpec1[] = { 0.0, 1.0, 0.0, 1.0 };
	float lightDifAndSpec2[] = { 0.0, 1.0, 0.0, 1.0 };
	float globAmb[] = { 0.3, 0.3, 0.3, 1.0 };

	//Light0 properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);

	// Light1 properties.
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec1);

	// Light2 properties.
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDifAndSpec2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightDifAndSpec2);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	glEnable(GL_LIGHT0); // Enable particular light source.
	glEnable(GL_LIGHT1); // Enable particular light source.
	glEnable(GL_LIGHT2); // Enable particular light source.

	// Create the new quadric object.
	qobj = gluNewQuadric();
}

// Process hit buffer to find record with smallest min-z value.
void findClosestHit(int hits, unsigned int buffer[])
{
	unsigned int *ptr, minZ;

	minZ = 0xffffffff; // 2^32 - 1
	ptr = buffer;
	closestName = 0;
	for (int i = 0; i < hits; i++)
	{
		ptr++;
		if (*ptr < minZ)
		{
			minZ = *ptr;
			ptr += 2;
			closestName = *ptr;
			ptr++;
		}
		else ptr += 3;
	}
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void drawTreeHouse() {
	float matAmbAndDif1[] = { 0.20, 0.50, 1.00, 1.0 }; // blue
	float matAmbAndDif4[] = { 1.00, 0.30, 0.70, 1.0 }; // pink
	float matAmbAndDif5[] = { 1.00, 0.39, 0.39, 1.0 }; // orange
	float matAmbAndDif6[] = { 0.87, 0.58, 0.98, 1.0 }; // purple


	//wall; left of door
	glPushMatrix();
	glTranslatef(26.0, -3.0, 50.0);
	glScalef(21.0, 20.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif4);
	glutSolidCube(2.0);
	glPopMatrix();

	//wall; right of door
	glPushMatrix();
	glTranslatef(-26.0, -3.0, 50.0);
	glScalef(21.0, 20.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif4);
	glutSolidCube(2.0);
	glPopMatrix();

	//wall; above door
	glPushMatrix();
	glTranslatef(0.0, 13.0, 50.0);
	glScalef(5.0, 4.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif4);
	glutSolidCube(2.0);
	glPopMatrix();

	//back wall
	glPushMatrix();
	glTranslatef(0.0, -3.0, 110.0);
	glScalef(47.0, 20.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidCube(2.0);
	glPopMatrix();

	//left wall; left of window
	glPushMatrix();
	glTranslatef(46.0, -3.0, 61.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(12.0, 20.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);
	glutSolidCube(2.0);
	glPopMatrix();

	//window
	glPushMatrix();
	glTranslatef(46.0, 2.5, 73.0);
	//glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(0.3, 0.3, 0.8);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(46.0, 12.9, 80.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.3, 0.3, 0.8);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();

	glPushMatrix(); //wall above window
	glTranslatef(46.0, 15, 79.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(8.0, 2.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);
	glutSolidCube(2.0);
	glPopMatrix();

	glPushMatrix(); //wall below window
	glTranslatef(46.0, -14, 79.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(8.0, 9.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);
	glutSolidCube(2.0);
	glPopMatrix();

	//end window

	//left wall; right of window
	glPushMatrix();
	glTranslatef(46.0, -3.0, 99.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(12.0, 20.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif5);
	//glColor3f(0.65, 0.50, 0.39);
	glutSolidCube(2.0);
	glPopMatrix();


	//right wall
	glPushMatrix();
	glTranslatef(-46.0, -3.0, 80.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(30.0, 20.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif6);
	//glColor3f(0.65, 0.50, 0.39);
	glutSolidCube(2.0);
	glPopMatrix();
}

void drawLight() {
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos2[] = { 0.0, 10.0, 20.0, 1.0 };

	glPushMatrix();
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	glTranslatef(0.0, 13.0, 78.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidDodecahedron();
	glPopMatrix();
}

void drawLightSwitch() {
	float matAmbAndDif7[] = { 0.90, 0.90, 0.90, 1.0 };
	float matAmbAndDif8[] = { 1.00, 1.00, 1.00, 1.0 };

	//lightswitch
	glPushMatrix();
	glTranslatef(45.5, -13.0, 70.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(2.0, 3.0, 2.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif8);
	glutSolidCube(1.0);
	glPopMatrix();

	//lightswitch button
	glPushMatrix();
	glTranslatef(44.5, -12.8, 70.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glScalef(1.0, 1.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif7);
	glutSolidCube(1.0);
	glPopMatrix();

	if (lightSwitchClicked) {
		glDisable(GL_LIGHT0); // Disable particular light source.
	}
	if (!lightSwitchClicked) {
		glEnable(GL_LIGHT0); // Enable particular light source.
	}
}

void drawDoor() {
	glMatrixMode(GL_MODELVIEW);

	float matAmbAndDif2[] = { 0.36, 0.20, 0.09, 1.0 };
	float matAmbAndDif3[] = { 0.81, 0.71, 0.23, 1.0 };

	//door
	glPushMatrix();

	if (doorOpen) {
		glTranslatef(5.0, -6.0, 45.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
	}
	else
		glTranslatef(0.0, -6.0, 50.0); //closed door
	glScalef(5.0, 17.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
	//glColor3f(0.86, 0.58, 0.44);
	glutSolidCube(2.0);
	glPopMatrix();

	//doorknob
	glPushMatrix();
	if (doorOpen)
	{
		glTranslatef(5.0, -10.0, 45.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
	}
	else
		glTranslatef(-3.0, -10.0, 50.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif3);
	//glColor3f(0.81, 0.71, 0.23);
	glutSolidSphere(2.0, 20.0, 20.0);
	glPopMatrix();
}

void drawFloor() {
	float matAmbAndDif1[] = { 0.91, 0.76, 0.65, 1.0 };

	glPushMatrix();
	glTranslatef(0.0, -23.5, 80.0);
	//glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(45.0, 0.5, 29.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidCube(2.0);
	glPopMatrix();
}

void drawRoof(){
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };

	glPushMatrix();
	glTranslatef(0.0, 20.0, 80.0);
	glScalef(45.0, 0.5, 29.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidCube(2.0);
	glPushMatrix();
}

void drawTable() {
	float matAmbAndDif1[] = { 0.137255, 0.556863, .419608, 1.0 };

	//Table Top
	glPushMatrix();
	glTranslatef(-35.0, -10.0, 95.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(5.0, 8.0, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	//glColor3f(0.137255, 0.556863, .419608); //Sea Green
	glutSolidSphere(1.0, 20.0, 20.0);
	glPopMatrix();

	//table leg
	glPushMatrix();
	glTranslatef(-35.0, -10.0, 95.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.5, 1.0, 0.5);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	//glColor3f(0.137255, 0.556863, .419608); //Sea Green
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();
}

void drawKey() {
	float matAmbAndDif1[] = { 0.81, 0.71, 0.23, 1.0 };

	//stick part
	glPushMatrix();
	glTranslatef(xTrans, yTrans, 90.0);
	glRotatef(keyAngle, 0.0, 1.0, 0.0);
	glScalef(0.4, 0.4, 0.4);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	//glColor3f(0.81, 0.71, 0.23); 
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();

	//loop thing
	glPushMatrix();
	glTranslatef(xTrans, yTrans, 89.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	//glColor3f(0.81, 0.71, 0.23);
	glutSolidTorus(0.5, 1.0, 10.0, 10.0);
	glPopMatrix();

	//key parts
	glPushMatrix();
	glTranslatef(xTrans, yTrans, 100.0);
	glRotatef(keyPartAngle, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.15);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	//glColor3f(0.81, 0.71, 0.23);
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xTrans, yTrans, 98.0);
	glRotatef(keyPartAngle, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.1);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	//glColor3f(0.81, 0.71, 0.23);
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();

}

void drawTreasureChest() {
	float matAmbAndDif1[] = { 0.137255, 0.556863, .419608, 1.0 };
	float matAmbAndDif2[] = { 0.137255, 0.9, .419608, 1.0 };
	float matAmbAndDif3[] = { 0.0, 0.0, 0.0, 1.0 };

	//trunk
	glPushMatrix();
	glTranslatef(35.0, -19.0, 104.0);
	glScalef(4.0, 1.5, 2.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidCube(4.0);
	glPopMatrix();

	//lid
	glPushMatrix();
	glTranslatef(35.0, moveLidUp, moveLidBack);
	glRotatef(lidAngle, 1.0, 0.0, 0.0);
	glScalef(4.0, 0.5, 2.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
	glutSolidCube(4.0);
	glPopMatrix();

	//key hole
	glPushMatrix();
	glTranslatef(35.0, -18.0, 100.0);
	glScalef(0.5, 1.0, 0.25);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif3);
	glutSolidSphere(1.0, 30.0, 30.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35.0, -19.3, 100.0);
	glScalef(0.5, 1.5, 0.25);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif3);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawLamp() {
	float lightPos1[] = { 10.0, 10.0, 20.0, 1.0 };
	float matAmbAndDif1[] = { 0.90, 0.91, 0.98, 1.0 };
	float matAmbAndDif2[] = { 0.85, 0.85, 0.10, 1.0 };
	
	//lamp shade
	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glTranslatef(-35.0, 4.0, 58.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	//glScalef(0.3, 0.3, 0.8);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
	glutSolidTeapot(4.0);
	glPopMatrix();

	//stand
	glPushMatrix();
	glTranslatef(-35.0, 2.0, 58.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.3, 0.3, 0.8);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	gluCylinder(qobj, 1, 1, 25, 15, 5);
	glPopMatrix();

	if (lampClicked) {
		glDisable(GL_LIGHT1); // Disable particular light source.
	}
	if (!lampClicked) {
		glEnable(GL_LIGHT1); // Enable particular light source.
	}

}

void drawMoon() {
	float lightPos0[] = { 0.0, 10.0, 20.0, 1.0 };
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glTranslatef(70.0, 30.0, 80.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(4.0, 20.0, 20.0);
	glPopMatrix();
}

void drawStars()
{
	float matAmbAndDif1[] = { 1.0, 1.0, 1.0, 1.0 };

	glPushMatrix();
	glTranslatef(0.0, 30.0, 10.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(70.0, 28.0, 70.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();
	glTranslatef(60.0, 30.0, 60.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.0, 30.0, 18.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30.0, 30.0, 5.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 30.0, 30.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, 30.0, 20.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0, 30.0, 8.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 30.0, 101.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-20.0, 30.0, 118.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30.0, 30.0, 51.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 30.0, 130.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, 30.0, 120.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0, 30.0, 81.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();
}

void drawTree()
{
	float matAmbAndDif1[] = { 0.0, 1.0, 0.0, 1.0 };
	float matAmbAndDif2[] = { 0.65, 0.16, 0.16, 1.0 };

	glPushMatrix();
	glTranslatef(60.0, 7.0, 73.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(3.0, 10.0, 30.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(60.0, -10.0, 73.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);
	gluCylinder(qobj, 1, 1, 18, 15, 5);
	glPopMatrix();
}

void drawSpotlight() {

	float lightPos[] = { meX, meY, meZ, 1.0 }; // Spotlight position.
	float spotDirection[] = { static_cast <float> (sin(angle * PI / 180)), meY, static_cast <float>(cos(angle * PI / 180)) }; // Spotlight direction. 

	glPushMatrix();
		glTranslatef(xMove, 0.0, zMove); // Move the spotlight.

		// Draw the spotlight cone in wireframe after disabling lighting
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glRotatef(-180.0, 1.0, 0.0, 0.0);
			glColor3f(1.0, 1.0, 1.0);
			glutWireCone(3.0 * tan(spotAngle / 180.0 * PI), 3.0, 20, 20);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		// Spotlight properties including position.	
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

	glPopMatrix();
}

void drawGold() {
	float matAmbAndDif1[] = { 0.81, 0.71, 0.23, 1.0 };

	glPushMatrix();
	glTranslatef(31.0, -15.0, 104.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(33.0, -15.0, 104.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35.0, -15.0, 104.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37.0, -15.0, 104.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(39.0, -15.0, 104.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(39.0, -15.0, 102.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37.0, -15.0, 102.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(35.0, -15.0, 102.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(33.0, -15.0, 102.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(31.0, -15.0, 102.0);
	glScalef(1.0, 0.3, 1.0);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glutSolidSphere(0.5, 20.0, 20.0);
	glPopMatrix();
}

void addAllText() {
	glColor3f(1.0, 0.3, 0.7);
	glRasterPos3f(-4.0, 5.0, 30.0);
	writeBitmapString((void*)font, "Welcome - click on the door to enter.");
}

void drawAll() {

	float matShine[] = { 50.0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (inout) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
		
	else {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}
	glDisable(GL_LIGHTING);

	if (overhead == true)
		gluLookAt(0.0, 60.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);

	gluLookAt(meX, meY, meZ,
		sin(angle * PI / 180) + meX, pointOfView, cos(angle * PI / 180) + meZ,
		0.0, 1.0, 0.0);

	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	if (isSelecting) glLoadName(1);
	drawDoor();

	if (isSelecting) glLoadName(2);
	drawLightSwitch();

	if (isSelecting) glLoadName(3);
	drawKey();

	if (isSelecting) glLoadName(4);
	drawLamp();


	if (isSelecting) glLoadName(0);
	drawTreeHouse();
	//drawRoof(); // roof makes many items disappear
	drawFloor();
	drawLight();
	if (enableSpotLight)
		drawSpotlight();
	drawTable();
	drawTreasureChest();
	if (rButtonWasPressed)
		drawGold();
	drawMoon();
	drawStars();
	drawTree();
	addAllText();
}

// The mouse callback routine.
void mousePickFunction(int button, int state, int x, int y)
{
	int viewport[4]; // Viewport data.

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //mouse clicked
	{
		glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data.

		glSelectBuffer(1024, buffer); // Specify buffer to write hit records in selection mode
		(void)glRenderMode(GL_SELECT); // Enter selection mode.

		// Save the viewing volume defined in the resize routine.
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		// Define a viewing volume corresponding to selecting in 3 x 3 region around the cursor.
		glLoadIdentity();
		gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);
		gluPerspective(120, 1, 1, 100);

		glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
		glLoadIdentity();

		glInitNames(); // Initializes the name stack to empty.
		glPushName(0); // Puts name 0 on top of stack.

		// Determine hits by calling drawBallAndTorus() so that names are assigned.
		isSelecting = 1;
		drawAll();

		hits = glRenderMode(GL_RENDER); // Return to rendering mode, returning number of hits.

		// Determine closest of the hit objects (if any).
		findClosestHit(hits, buffer);
		if (closestName == 1) { //door
			doorOpen = !doorOpen;
		}

		if (closestName == 2) { //light switch
			lightSwitchClicked = !lightSwitchClicked;		
		}
			
		if (closestName == 3) { //key
			isAnimate = !isAnimate;
			glutPostRedisplay();
		}

		if (closestName == 4) { //lamp
			lampClicked = !lampClicked;
		}

		cout << "closest hit = " << closestName << endl;

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glutPostRedisplay();
	}
}


void drawScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	isSelecting = 0;

	drawAll();
	glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
	if (isAnimate)
	{
		if (xTrans != 35.0) {
			xTrans += 1;
			if (yTrans != -19.0)
				yTrans -= 1;
		}
			
		/*if (keyPartAngle != 180.0) {
			keyPartAngle += 5;
		}*/

		//if (keyAngle != 90.0) keyAngle += 2.0;

		if (rButtonWasPressed) {
			if (lidAngle != 90.0)
				lidAngle += 3.0;
			if (moveLidBack != 107)
				moveLidBack += 1;
			if (moveLidUp != -12.0)
				moveLidUp += 1;
		}

		if (lButtonWasPressed) {
			if (lidAngle != 90.0)
				lidAngle += 3.0;
			if (moveLidBack != 107)
				moveLidBack += 1;
			if (moveLidUp != -12.0)
				moveLidUp += 1;
		}
	}
	glutTimerFunc(animationPeriod, animate, 1);
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:\n" << endl;
	cout << "Moving around:" << endl;
	cout << "\tUp arrow to move forward." << endl;
	cout << "\tDown arrow to move backwards." << endl;
	cout << "\tLeft arrow to rotate to the left." << endl;
	cout << "\tRight arrow to rotate to the right." << endl;
	cout << "\tPress the shift key with the up arrow to look up." << endl;
	cout << "\tPress the shift key with the down arrow to look down." << endl;
	cout << "Door:" << endl;
	cout << "\tLeft click on door to open/close it." << endl;
	cout << "\tOR Press space bar to open/close door." << endl;
	cout << "Key:" << endl;
	cout << "\tLeft click on key to animate it." << endl;
	cout << "\tOR Press 'k' to animate the key." << endl;
	cout << "\tPress 'r' to turn the key to the right." << endl;
	cout << "\tPress 'l' (lowercase  L) to turn the key to the left." << endl;

	cout << "Light Switch:" << endl;
	cout << "\tLeft click on light switch to turn it on/off." << endl;
	cout << "\tOR Press 't' to turn the light switch on/off." << endl;

	cout << "Misc:" << endl;
	cout << "\tPress '0' (zero) to get an overhead view." << endl;
	cout << "\tPress 's' to enable the headlight." << endl;
	cout << "\tPress the escape key to exit the program." << endl;

	cout << "Cheat Sheet:" << endl;
	cout << "\tPressing 'r' is the correct way to open treasure chest. (Gold appears in chest) " << endl;
	cout << "\tPressing 'l' is the incorrect way to open treasure chest. (Nothing happens)" << endl;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:
			exit(0);
			break;
		case ' ':
			//inout = 1;
			doorOpen = !doorOpen;
			glutPostRedisplay();
			break;
			//inout = 0;
		case '0':
			overhead = !overhead;
			glutPostRedisplay();
			break;
		case 's':
			enableSpotLight = !enableSpotLight;
			glutPostRedisplay();
			break;
		case 't':
			lightSwitchClicked = !lightSwitchClicked;
			glutPostRedisplay();
			break;
		case 'k':
			isAnimate = !isAnimate;
			glutPostRedisplay();
			break;
		case 'r': //right, correct way to open chest
			rButtonWasPressed = !rButtonWasPressed;
			break;
		case 'l': //left, incorrect way to open chest
			lButtonWasPressed = !lButtonWasPressed;
			break;
		default:
			break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP: //forward
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
			pointOfView += stepsize;
		else
		{
			meZ += stepsize*cos(angle*PI / 180);
			meX += stepsize*sin(angle*PI / 180);
		}
		if (zMove < 80.0) zMove += 1;
		//cout << zMove << endl;
		break;
	case GLUT_KEY_DOWN: //back
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
			pointOfView -= stepsize;
		else
		{
			meZ -= stepsize*cos(angle*PI / 180);
			meX -= stepsize*sin(angle*PI / 180);
		}
		break;
	case GLUT_KEY_RIGHT: //turn right
		angle -= turnsize;
		break;
	case GLUT_KEY_LEFT: //turn left
		angle += turnsize;
		break;
	}
	glutPostRedisplay();
}


// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(850, 600);
	glutInitWindowPosition(400, 50);
	glutCreateWindow("A Magical House");
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutMouseFunc(mousePickFunction); // Mouse control.
	glutTimerFunc(5, animate, 1);
	glutMainLoop();

	return 0;
}
