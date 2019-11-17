#ifdef __APPLE__
#  define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "star.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

float starPositions[7][3]{
	{-13,1,-12},
	{-6,1,-21},
	{-13,1,0.6},
	{8.8,1,-6.5},
	{-3.2,1,1.7},
	{-9,1,-17},
	{0, 1, -7}
};

void Star::drawStar(){
	glPointSize(5);
	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0);

        glVertex3f(0,-0.3,0);
        glVertex3f(-1.2,-1,0);
        glVertex3f(-0.8,0,0);

        glVertex3f(-1.6,0.4,0);//left
        glVertex3f(-0.7,0.6,0);

        glVertex3f(0,1.2,0);//top

        glVertex3f(0.7,0.6,0);//right
        glVertex3f(1.6,0.4,0);

        glVertex3f(0.8,0,0);
        glVertex3f(1.2,-1,0);
 
	glEnd();
}

Star::Star(int pos){
	this -> sx = starPositions[pos][0];
	this -> sy = starPositions[pos][1];
	this -> sz = starPositions[pos][2];

	this -> evil = false;

	Materials m;
	this -> material = m.gold();
}

