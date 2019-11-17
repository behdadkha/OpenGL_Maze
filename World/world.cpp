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

#include "world.h"
#include "../Star/star.h"
#include "../Obstacle/obstacle.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;



vector<int> randomGen;
bool containsRand(int random){
	for(int i = 0; i < randomGen.size(); i++){
		if(randomGen[i] == random){
			return true;
		}
	}
	return false;
}
int getRandom(){
	srand((unsigned)time(NULL));
	//randomGen.clear();
	int random = rand() % 7;
	if(randomGen.size() == 0){
		randomGen.push_back(random);
	}else{
		while(containsRand(random)){
			random = rand() % 7;
		}
		randomGen.push_back(random);
	}
	
	return random;
}


World::World(){

	this -> time = 500;	// time = 0 => gameover

	this -> numStars = 5;

	this -> numEnemies = 5;
	this -> numDarts = 10;

	//generate stars
	for(int i = 0; i < numStars; i++){
		int random = getRandom();
		Star s(random);
		this -> stars.push_back(s);
	}

	//generate darts
	for(int i = 0; i < numDarts; i++){
		Obstacle o;
		this -> darts.push_back(o);
	}
	//pointer to maze
	Maze m;
	this -> maze = &m;

	//get the wall positions from maze
	//(using the maze's wallPositions led to errors)
	for(int i = 0; i < m.wallPositions.size(); i++){
		this -> wallPositions.push_back(m.wallPositions[i]);
	}

	for(int i = 0; i < m.wallScale.size(); i++){
		this -> wallScale.push_back(m.wallScale[i]);
	}

}
void World::drawFloor(){
	glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex3f(-16,-0.1,16);
        glVertex3f(16,-0.1,16);
        glVertex3f(16,-0.1,-64);
        glVertex3f(-16,-0.1,-64);
    glEnd();
}
