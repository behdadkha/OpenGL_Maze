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
#include "maze.h"
#include <stdio.h>
#define walls 35
using namespace std;

int numWalls = walls;
//for each wall
GLfloat vertices[20][3] = {
	//left side
	{0, 0, 1},
	{0, 1, 1},
	{0,1,-1},
	{0,0,-1},

	//right side
	{0.5, 0, 1},
	{0.5, 0, -1},
	{0.5, 1, -1},
	{0.5, 1, 1},

	//front side
	{0,0,1},
	{0.5, 0 ,1},
	{0.5 , 1, 1},
	{0,1,1},

	//back side
	{0,0,-1},
	{0,1,-1},
	{0.5,1,-1},
	{0.5,0,-1},

	//top side
	{0,1,1},
	{0.5,1,1},
	{0.5,1,-1},
	{0,1,-1}
};
int distance = 3;
float xmultiplier = -9;
float zmultiplier = -15;
float translate[walls][3] = {
	//walls around the maze
		//left side
		{-16,0,-24},
		//right side
		{15.5,0,-24},
		//bottom
		{-16,0,16},
		//top side
		{-16,0,-63.5},
	//walls like |
	{4,0,0},
	{1,0,0},
	{1,0,-6.4},
	{-2,0,0},
	{11,0,-6.45},

	//walls like --
	{-1.47,0,-5.3},
	{-4.5,0,0},
	{1,0,-7.9},
	{4,0,-5},
	{4.5,0,0},

	//second coppy ///////
		{4+xmultiplier,0,0+zmultiplier},
		{1+xmultiplier,0,0+zmultiplier},
		{1+xmultiplier,0,-6.4f+zmultiplier},
		{-2+xmultiplier,0,0+zmultiplier},
		{11+xmultiplier,0,-6.45f+zmultiplier},

		//walls like --
		{-2+xmultiplier,0,-5.3f+zmultiplier},
		{-6.5f+xmultiplier,0,0+zmultiplier},
		{1+xmultiplier,0,-7.9f+zmultiplier},
		{4+xmultiplier,0,-5+zmultiplier},

	//third set
		//walls like |
		{-5,0,0},
		{-8,0,0},
		{-2,0,-10},
		//walls like ---
		{-2,0,-15},

	//forth set{left}
		//walls like |
		{-11,0,1.4},
		{-11,0,-7.5},
		//{-11,0,-1.5},
		//walls like --
		{-15.5, 0, -2},
		{-11, 0, -5},
	//fifth {top right}
		//walls like |
		{5,0,-10},
		{2,0,-10},
		{5,0,-19},
		{11,0,-15.5}

};

float scale[walls][3] = {
	//walls around the maze
		//left side
		{1,3,40},
		//right side
		{1,3,40},
		//bottom side
		{64,3,0.3},
		//top side
		{64,3,0.3},
	//walls like |
	{1,3,5},
	{1,3,5},
	{1,3,1.3},
	{1,3,5},
	{1,3,1.65},

	//walls like --
	{5,3,0.2},//end
	{5,3,0.3},
	{20,3,0.2},
	{14,3,0.2},
	{23,3,0.4},

	///second copy
		//walls like |
		{1,3,5},
		{1,3,5},
		{1,3,1.3},
		{1,3,5},
		{1,3,1.65},

		//walls like --
		{5.9,3,0.2},//end
		{10,3,0.3},
		{20,3,0.2},
		{14,3,0.2},
	//third set
		{1,3,5},
		{1,3,5},
		{1,3,5},
		//walls like --
		{15,3,0.2},
	//forth set
		//walls like |
		{1,3,3.7},
		{1,3,2.5},
		//{1,8,0.4},
		//walls like --
		{9,3,0.3},
		{7,3,0.3},
	//fifth
		// |
		{1,3,2},
		{1,3,2},
		{1,3,4},
		{1,3,7.5}


};

void Maze::makeWall(){
	glBegin(GL_QUADS);
	glColor3f(0,0.5,0.5);
	for (int i = 0; i < 20; i++){
		glVertex3fv(vertices[i]);
	}
	glEnd();
}

Maze::Maze(){

	for(int i = 0; i < numWalls; i++){
		vector<float> vecs;
		vecs.push_back(translate[i][0]);
		vecs.push_back(translate[i][1]);
		vecs.push_back(translate[i][2]);
		this -> wallPositions.push_back(vecs);
	}

	for(int i = 0; i < numWalls; i++){
		vector<float> v;
		v.push_back(scale[i][0]);
		v.push_back(scale[i][1]);
		v.push_back(scale[i][2]);
		this -> wallScale.push_back(v);
	}
	

}
void Maze::drawMaze(){

	for(int i = 0; i < numWalls; i++){
		glPushMatrix();
			glTranslatef(translate[i][0],translate[i][1],translate[i][2]);
			glScalef(scale[i][0],scale[i][1],scale[i][2]);
			makeWall();
		glPopMatrix();
	}
}

int Maze::numberWalls(){
	return numWalls;
}




	
