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

#include "particle.h"
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <vector>
using namespace std;


///////////


////////

Particle::Particle(){

	this -> id = 0;
	
	this -> px = 0;
	this -> py = 0;
	this -> pz = -60;

	this -> dx = -20 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(20+20)));
	this -> dy = 350;
	this -> dz = -20 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(20+20)));

	this -> cy = 250;
	this -> hy = 350;
	//rotation
	this -> rx = rand() % 361;
	this -> ry = rand() % 361;
	this -> rz = rand() % 361;

	this -> speed = 0.0015;
	//color
	this -> r = rand() % 2;
	this -> g = rand() % 2;
	this -> b = rand() % 2;

	//material
	this -> material = rand() % 3;

	this -> age = 0;
}


