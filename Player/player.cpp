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


#include "player.h"
#include <cstdlib>
#include <ctime>



Player::Player(){

	this -> px = 0;
	this -> py = 0;
	this -> pz = 10;

	this -> dx = 0;
	this -> dy = 0;
	this -> dz = 0;

	this -> life = 5;
	this -> numStars = 0;
	
}

void Player::drawPlayer(){
	glBegin(GL_QUADS);
		//back side
        glColor3f(1, 1, 0);
        glVertex3f(-0.5,0, -2);
        glVertex3f(-0.5,1, -2);
        glVertex3f(0.5, 1, -2);
        glVertex3f(0.5, 0, -2);

        //front side
        glColor3f(0.1, 0.5, 0);
        glVertex3f(-0.5,0, -1);
        glVertex3f(-0.5,1, -1);
        glVertex3f(0.5, 1, -1);
        glVertex3f(0.5, 0, -1);

        //left side
        glColor3f(1, 1, 0);
        glVertex3f(-0.5,0, -2);
        glVertex3f(-0.5,0, -1);
        glVertex3f(-0.5, 1, -1);
        glVertex3f(-0.5, 1, -2);

        //right side
        glColor3f(0, 0, 1);
        glVertex3f(0.5,0, -1);
        glVertex3f(0.5,0, -2);
        glVertex3f(0.5, 1, -2);
        glVertex3f(0.5, 1, -1);

        //bottom side
        glColor3f(1, 1, 0);
        glVertex3f(-0.5,0, -1);
        glVertex3f(-0.5,0, -2);
        glVertex3f(0.5, 0, -2);
        glVertex3f(0.5, 0, -1);

        //top side
        glColor3f(0.5, 0, 0.3);
        glVertex3f(-0.5,1, -1);
        glVertex3f(0.5,1, -1);
        glVertex3f(0.5, 1, -2);
        glVertex3f(-0.5, 1, -2);

    glEnd();
}
