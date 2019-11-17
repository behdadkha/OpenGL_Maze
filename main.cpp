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
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <math.h>
#include "Player/player.h"
#include "Star/star.h"
#include "World/world.h"
#include "World/maze.h"
#include "Obstacle/obstacle.h"
using namespace std;

Player p;
World w;
Star s(0);
//Obstacle o(0);

//star rotation angle(increment later)
int starAngle = 0;

int sceneAngle = 0;

int playerAngleX = 0;

bool pause = false;

float scenePos = 50;

//stars
vector<Star> stars;

//camera
GLdouble eye[] = {0, 50, 3};
GLdouble lookAt[] = { 0, 1, 0 };
GLdouble up[] = { 0, 1, 0 }; 


void showStatus(){
	printf("Life: %d  stars: %d\n", p.life, p.numStars);
}

void drawDart(){
	glutSolidSphere(0.5,15,15);
}

void drawScene(){
	
	glPushMatrix();
		w.drawFloor();
	glPopMatrix();

	//stars
	for(int i = 0; i < stars.size(); i++){
		glPushMatrix();
			glTranslatef(stars[i].sx,stars[i].sy,stars[i].sz);
			glRotatef(starAngle, 0,1,0);
			glScalef(0.5,0.5,0.5);
			s.drawStar();
		glPopMatrix();
	}
	//player
	glPushMatrix();
    	glTranslatef(p.px,p.py,p.pz);
		glRotatef(playerAngleX, 1,0,0);
		glTranslatef(-p.px,-p.py-0.5,-p.pz+1.55);
		glTranslatef(p.px,p.py,p.pz);
		p.drawPlayer();
	glPopMatrix();

	//maze
	glPushMatrix();
    w.maze->drawMaze();
    glPopMatrix();

	//darts
	for(int i = 0; i < w.darts.size(); i++){
		glPushMatrix();
			glColor3f(0,0,0);
			glTranslatef(w.darts[i].ox, w.darts[i].oy,w.darts[i].oz);
			drawDart();
		glPopMatrix();
	}

	
}
//the caught star's position
float caughtStarX = 0;
float caughtStarZ = 0;
int count = 0;

//animation for the caught star-->timer func
void caughtStar(int i){
	//check if it is the same star(deletion might change the order)
	if(count == 0 || (caughtStarX == stars[i].sx && caughtStarZ == stars[i].sz)){
		stars[i].sy += 0.02;
		stars[i].sz += 0.003;
		count++;
		caughtStarX = stars[i].sx;
		caughtStarZ = stars[i].sz;
	}

	if(stars[i].sy < 5){
		glutTimerFunc(170,caughtStar,i);
	}
	
}
void catchStar(){

	for(int i = 0; i < stars.size(); i++){
		if(p.px >= stars[i].sx-1 && p.px <= stars[i].sx+1 
			&& p.pz >= stars[i].sz-1 && p.pz <= stars[i].sz+1){
			count = 0;
			glutTimerFunc(0,caughtStar,i);
			break;
		}
		if(stars[i].sy > 5){
			stars.erase(stars.begin()+i);
			p.numStars++;
			showStatus();
		}
	}
}

void dartAnimation(){

	for(int i = 0; i < w.darts.size(); i++){
		//if dart has hit the player
		if(w.darts[i].hit){
			continue;
		}
		w.darts[i].ox += w.darts[i].dx * w.darts[i].speed;
		if(w.darts[i].ox <= -15){
			w.darts[i].dx = 50;
		}
		if(w.darts[i].ox >= 15){
			w.darts[i].dx = -50;
		}
	}
	
}
void gameOver(){
	printf("Game over\n");
	showStatus();
	eye[1] = 50;
	pause = true;
}
void deductHealth(int i){
	p.life--;
	w.darts[i].hit = true;
	showStatus();
	if(p.life == 0){
		gameOver();
	}
}
void dartHitsPlayer(){
	for(int i = 0; i < w.darts.size(); i++){
		if(w.darts[i].hit){
			continue;
		}
		if(!(w.darts[i].ox > p.px+1 || w.darts[i].ox < p.px-1
			|| w.darts[i].oz > p.pz+1 || w.darts[i].oz < p.pz-1)){
			deductHealth(i);
		}
	
	}
	
}
int cnt = 0;
bool demo = false;

void display(void) {
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0] + p.px,eye[1],eye[2] + p.pz, p.px, p.py+2, p.pz, up[0],up[1], up[2]);

    //drawscene
    glTranslatef(p.px,p.py,p.pz);
	glRotatef(sceneAngle, 0,1,0);
	glTranslatef(-p.px,-p.py-0.5,-p.pz);
    glTranslatef(0,0,scenePos);
    drawScene();

    glutSwapBuffers();
    glutPostRedisplay();
	
}

int closestWall(){
	float check = 10;
	int index = 0;
	int sindex = 0;

	//w.maze->numberWalls()
	for(int i = 0; i < w.maze->numberWalls(); i++){
		float s[5][2];
		if(w.wallScale[i][0] > 3){//if the wall is like ----
			float s[5][2]= {
				{w.wallPositions[i][0], w.wallPositions[i][2] + w.wallScale[i][2]/4},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/2, w.wallPositions[i][2]+ w.wallScale[i][2]/4},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/3, w.wallPositions[i][2]+ w.wallScale[i][2]/4},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/4, w.wallPositions[i][2]+ w.wallScale[i][2]/4},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/10, w.wallPositions[i][2]+ w.wallScale[i][2]/4}
			};
		}else{//if the wall is like |
			float s[5][2] = {
				{w.wallPositions[i][0]+ w.wallScale[i][0]/4, w.wallPositions[i][2]},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/4, w.wallPositions[i][2] + w.wallScale[i][2]},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/4, w.wallPositions[i][2] + w.wallScale[i][2]/2},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/4, w.wallPositions[i][2] - w.wallScale[i][2]},
				{w.wallPositions[i][0]+ w.wallScale[i][0]/4, w.wallPositions[i][2] - w.wallScale[i][2]/2}
			};
		}


		float diss1 = sqrt(pow(s[0][0]-(p.px),2) + pow(s[0][1]-(p.pz),2));
		float diss2 = sqrt(pow(s[1][0]-(p.px),2) + pow(s[1][1]-(p.pz),2));
		float diss3 = sqrt(pow(s[2][0]-(p.px),2) + pow(s[2][1]-(p.pz),2));

		float diss4 = sqrt(pow(s[3][0]-(p.px),2) + pow(s[3][1]-(p.pz),2));
		float diss5 = sqrt(pow(s[4][0]-(p.px),2) + pow(s[4][1]-(p.pz),2));


		if(diss1 < diss2 && diss1 < diss3 && diss1 < diss4 && diss1 < diss5){
			sindex = 0;
		}else if(diss2 < diss1 && diss2 < diss3 && diss2 < diss4 && diss2 < diss5){
			sindex = 1;
		}else if(diss3 < diss1 && diss3 < diss2 && diss3 < diss4 && diss3 < diss5){
			sindex = 2;
		}else if(diss4 < diss1 && diss4 < diss2 && diss4 < diss3 && diss4 < diss5){
			sindex = 3;
		}else {
			sindex = 4;
		}
		float dis = sqrt(pow(s[sindex][0]-(p.px),2) + pow(s[sindex][1]-(p.pz),2));
		if(dis < check){
			check = dis;
			index = i;
		}
		
	}
	return index;
}
bool checkWall(int dir){

	int index = closestWall();
	switch(dir){
		case 0:
			if(p.pz > w.wallPositions[index][2]+w.wallScale[index][2]+1 || p.pz < w.wallPositions[index][2]-w.wallScale[index][2]
		    	|| (p.px <= w.wallPositions[index][0]-0.5 || p.px >= w.wallPositions[index][0]+(w.wallScale[index][0])/2 + 0.6)){
		    	return true;
		    }
		    break;
		case 1:
			if(p.pz > w.wallPositions[index][2]+w.wallScale[index][2]+0.4 || p.pz < w.wallPositions[index][2]-w.wallScale[index][2]-1
		    	|| (p.px <= w.wallPositions[index][0]-0.5 || p.px >= w.wallPositions[index][0]+(w.wallScale[index][0])/2 +0.6)){
		    	return true;
		    }
		    break;
		case 2:
			if(p.pz > w.wallPositions[index][2]+w.wallScale[index][2]+0.4 || p.pz < w.wallPositions[index][2]-w.wallScale[index][2]-1
		    	|| (p.px <= w.wallPositions[index][0] || p.px >= w.wallPositions[index][0]+(w.wallScale[index][0])/2 + 1)){
		    	return true;
		    }
		    break;
		case 3:
			if(p.pz > w.wallPositions[index][2]+w.wallScale[index][2]+0.4 || p.pz < w.wallPositions[index][2]-w.wallScale[index][2]-1
		    	|| (p.px <= w.wallPositions[index][0]-0.9 || p.px >= w.wallPositions[index][0]+(w.wallScale[index][0])/2)){
		    	return true;
		    }
		    break;
	}
	
	
    return false;

	
}
bool doneCamDart = false;
void special(int key, int _x, int _y){
	if(!pause){
		if(key == GLUT_KEY_UP){  
	        if(checkWall(0) && p.pz-0.4 > -64){
	        	p.pz -= 0.4;
	        	playerAngleX -= 20;//roll
	        }

	    }else if(key == GLUT_KEY_DOWN){
	    	if(checkWall(1)&& p.pz+0.4 < 16){
	    		p.pz += 0.4;
	    		playerAngleX += 20;
	    	}

	    }else if (key == GLUT_KEY_LEFT){
	    	if(checkWall(2)&& p.px > -14.5){
	    		p.px -= 0.4;
	    	}
	        	
	    }else if(key == GLUT_KEY_RIGHT){
	    	if(checkWall(3)&& p.px+0.4 < 15){
	        	p.px += 0.4;
	    	}
	    }
	}
	
}

void handleReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50,1,1,100);

    glMatrixMode(GL_MODELVIEW);
}

void StarRotation(int val){
	starAngle++;
	glutTimerFunc(17,StarRotation,0);
}

void handleKeyboard(unsigned char key, int _x, int _y){
	if(key == 'w'){
		if(eye[1] < 8){
			eye[1]++;
			eye[2] -= 0.4;
		}
		
	}else if (key == 's'){
		if(eye[1] >= 4){
			eye[1]--;
			eye[2] += 0.4;
		}

	}else if(key == 'a' && p.pz < -22){
		sceneAngle--;
	}else if(key == 'd' && p.pz < -22){
		sceneAngle++;
	}else if (key == ' '){
		demo = true;
		eye[1] = 50;
	}else if(key == 'q'){
		exit(0);
	}
}

void idle(){
	if(!pause){
		//camera when player gets to darts
		//zooms in when player gets to darts or gets to the maze
	    if(p.pz < -22 && p.pz > -23){
	    	if(eye[1] >= 4){
	    		eye[1]--;
	    		eye[2] += 0.4;
	    	}
	    }else if(p.pz > -22 && p.pz < -21){
	    	if(eye[1] < 8){
	    		eye[1]++;
	    		eye[2] -= 0.4;
	    	}

	    }//

	   	//dart
    	dartAnimation();
    	dartHitsPlayer();

		//catch stars
		catchStar();
	}
	if(demo){
		cnt++;
		if(cnt % 15 == 0){
			if(eye[1] != 8)
				eye[1]--;
			if(scenePos != 0)
				scenePos--;
		}
		if(eye[1] == 8 && scenePos == 0){
			demo = false;
		}
	}
}

void showControls(){
	printf("\n%s\n", "up arrow --> move forward");
	printf("%s\n", "down arrow--> move backward");
	printf("%s\n", "right arrow --> move right");
	printf("%s\n\n", "left arrow --> move left");

	printf("%s\n", "space --> take a quick peek from above");
	printf("%s\n", "'w' --> move the camera up(can't get too far)");
	printf("%s\n", "'s' --> move the camera down");
	printf("%s\n", "'a','d' --> look to the left and right(only in the dart section)");
	printf("%s\n", "'q' --> to quit");
	printf("%s\n", "---------------------------------------");

	showStatus();
}
int main(int argc, char** argv) {
	demo = true;
	srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitWindowSize(650,800);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);
    glutCreateWindow("MAZE");

    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(handleReshape);
    glutIdleFunc(idle);

    showControls();
    
   	for(int i = 0; i < w.numStars; i++){
    	stars.push_back(w.stars[i]);
    }

    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glutTimerFunc(0,StarRotation,0);

    glClearColor(0, 0, 0, 0);

    glutMainLoop();

    return 0;
}