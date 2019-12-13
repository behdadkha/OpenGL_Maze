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
#include <string>
#include <time.h>
#include <math.h>
#include "Player/player.h"
#include "Star/star.h"
#include "World/world.h"
#include "World/maze.h"
#include "Obstacle/obstacle.h"
#include "Texture/PPM/PPM.h"
#include "particle_system/particle.h"
using namespace std;

void setMaterial(int mat);
void reset();
void timer(int val);

Player p;
World w;
Maze m;
Star s(0);
vector<Particle> particles;

//star rotation angle(increment later)
int starAngle = 0;

int sceneAngle = 0;

int playerAngleX = 0;
int playerAngleZ = 0;

bool pause = false;
float scenePos = 50;

int gameMode; // 1: normal 2:hard
bool gameModeSelection = true;
bool hover_normal = false;//for game mode selection
bool hover_hard = false;

bool gameover = false;
int cnt = 0; // for demo speed
bool demo = false;
//stars
vector<Star> stars;

//camera
GLdouble eye[] = {0, 50, 3};
GLdouble lookAt[] = { 0, 0, 0 };
GLdouble up[] = { 0, 1, 0 }; 


GLfloat ambient[4] = { 0.5, 0.5, 0.5, 1 };
GLfloat diffuse[4] = { 1, 1, 1, 1 };
GLfloat specular[4] = { 0.5, 0.5, 0.5, 1 };
GLfloat lightPos[4] = { 0, 10, 3, 1 };

// 0: gold, 1:copper , 2:silver, 3:brass 4:yello plastic
			
GLfloat materialAmbient[5][4] = {
    { 0.24725, 0.1995, 0.0745, 1.0},
    { 0.19125, 0.0735, 0.0225, 1.0},
    { 0.19225, 0.19225, 0.19225, 1.0},
    {0.329412, 0.223529, 0.027451, 1.0},
    {0.0, 0.0, 0.0, 1.0}
};
GLfloat materialDiffuse[5][4] = {
    { 0.75164, 0.60648, 0.22648, 1.0},
    { 0.7038, 0.27048, 0.0828, 1.0},
    { 0.50754, 0.50754, 0.50754, 1.0},
    {0.780392, 0.568627, 0.113725, 1.0},
    {0.5 ,0.5, 0.0, 1.0}
};
GLfloat materialSpecular[5][4] = {
    { 0.628281, 0.555802, 0.366065, 1.0},
    { 0.256777, 0.137622, 0.086014, 1.0},
    { 0.508273,0.508273,0.508273, 1.0},
    {0.992157, 0.941176, 0.807843, 1.0},
    {0.60, 0.60, 0.50, 1.0}
};
GLfloat materialShiny[5] = {
    0.4,
    0.1,
    0.4,
    0.21794872,
    0.25
};


struct Image {
    int mWidth;
    int mHeight;
    GLubyte * mImage;

    void load(char * filename) {
        mImage = LoadPPM(filename, &mWidth, &mHeight);
    }

    void draw(unsigned int x, unsigned int y) {
        glRasterPos2i(x + mWidth, y);
 
        glPixelZoom(-1, 1);
        glDrawPixels(mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mImage);
    }

    void texture() {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mImage);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
};
Image wallTexture;
Image floorTexture;

Image black_box;


void drawSphere(Particle p){
    glColor3f(p.r,p.g,p.b);
    glTranslatef(p.px,p.py,p.pz);
    glRotatef(p.rx, 1,0,0);
    glRotatef(p.ry, 0,1,0);
    glRotatef(p.rz, 0,0,1);
    setMaterial(p.material);
    glutSolidSphere(0.1,15,15);
}


void drawDart(){
	glutSolidSphere(0.5,15,15);
}

void disable_lighting(){
	glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}
void enable_lighting(){
	glEnable(GL_LIGHTING);
	if(gameMode == 1){
    	glEnable(GL_LIGHT0);
	}else if(gameMode == 2){
		glEnable(GL_LIGHT0);

		for(int i = 0; i < 3; i++){
			ambient[i] = 0.01;
			diffuse[i] = 0.01;
			specular[i] = 0.01;
		}
	}


    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void setOrthographicProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void setPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void drawText(const char *text, int length, int x, int y){
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0,600,0,600,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i =0; i< length; i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}
void setMaterial(int mat){
	GLfloat emission[4] = {1,0,0,1}; 
	GLfloat emission2[4] = {0,0,0,0}; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient[mat]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse[mat]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular[mat]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShiny[mat]);
    if(mat == 0){
    	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    }else{
    	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission2);
    }

}

void drawScene(){
	
	//maze
	glEnable(GL_TEXTURE_2D);
	enable_lighting();
	glPushMatrix();
		setMaterial(2);
		wallTexture.texture();
	    m.drawMaze();
    glPopMatrix();

	glPushMatrix();
		setMaterial(2);
		floorTexture.texture();
		w.drawFloor();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	//stars
	setMaterial(0);
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
		setMaterial(4);
    	glTranslatef(p.px,p.py+0.5,p.pz);
		glRotatef(playerAngleX, 1,0,0);
		glRotatef(playerAngleZ, 0,1,0);
		glTranslatef(-p.px,-p.py-1,-p.pz+1.55);
		glTranslatef(p.px,p.py+0.5,p.pz);
		p.drawPlayer();
	glPopMatrix();

	

	//darts
	if(gameMode == 1){
		setMaterial(1);
	}else{
		setMaterial(0);
	}
	
	for(int i = 0; i < w.darts.size(); i++){
		
		glPushMatrix();
			glColor3f(1,0,1);
			glTranslatef(w.darts[i].ox, w.darts[i].oy,w.darts[i].oz);
			drawDart();
		glPopMatrix();
	}

	//the final door
	setMaterial(0);
	glPushMatrix();
		glColor3f(0,1,0);
		glTranslatef(0,1,-62);
		glutSolidTorus(0.09f, 1.00f, 32, 32);
	glPopMatrix();

	//sphere
    for(int i = 0; i < particles.size(); i++){
        glPushMatrix();
            drawSphere(particles[i]);
        glPopMatrix();
    }


	//write life and stars
	disable_lighting();
	setOrthographicProjection();
    glPushMatrix();
        glColor3f(1,0,0);
        string lifeTxt = "Life: ";
        string lifeCount = to_string(p.life);
        drawText(lifeTxt.data(),lifeTxt.size(), 10,550);
        drawText(lifeCount.data(),lifeCount.size(), 55,550);

        glColor3f(0,1,0);
        string starTxt = "Stars: ";
        string starCount = to_string(p.numStars);
        drawText(starTxt.data(),starTxt.size(), 80,550);
        drawText(starCount.data(),starCount.size(), 135,550);

        string timerTxt = "Time: ";
        string timerCount = to_string(w.time);
        drawText(timerTxt.data(),timerTxt.size(), 160,550);
        drawText(timerCount.data(),timerCount.size(), 220,550);
    glPopMatrix();

    glPushMatrix();
    	black_box.draw(-30,535);
    glPopMatrix();

	
}
//for particles
//////////////////////////////////////////////
void handleBounce(int i){
    //bounce only its in the borders
    if(particles[i].px <= 8 && particles[i].px >= -8 && particles[i].pz <= -60 && particles[i].pz >= -74)
        {

            if(particles[i].py <= 0){
                
                //decrese the particles height by 20%
                particles[i].hy -= particles[i].hy * 0.2;
                particles[i].speed -= particles[i].speed * 0.01;
                //if there is no bouce then reduce the speed
                if(particles[i].hy <= 2 && particles[i].hy != 0){
                    particles[i].speed -= particles[i].speed * 0.1;
                }
            
            particles[i].dy = particles[i].hy;
            
        }
    }
}

void checkBallAge(int i){
    if(particles[i].age > 500){
        particles.erase (particles.begin() + i);
        
    }

}

void checkBellowBoard(int i){
    if(particles[i].py < -5){

        particles.erase (particles.begin() + i);
    }

}
void BallMovement(int val){

    glutPostRedisplay();
    for(int i = 0; i < particles.size(); i++){ 

        //check if particle has fallen bellow the board
        checkBellowBoard(i);

        //up y directoin
        particles[i].px += particles[i].dx * particles[i].speed;
        particles[i].py += particles[i].dy * particles[i].speed;
        particles[i].pz += particles[i].dz * particles[i].speed;

        //gravity on y direction
        particles[i].dy = particles[i].dy - (9.8);

        handleBounce(i);
        
        //age
        checkBallAge(i);
        
        particles[i].age++;
        //rotation
        particles[i].rx++;
        particles[i].ry++;
        particles[i].rz++;

    }
    
    glutTimerFunc(17,BallMovement,0);
}
void addParticle(int val){
    glutPostRedisplay();
    if(!pause){
        Particle p;
        particles.push_back(p);
    }
    glutTimerFunc(130,addParticle,0);
    
}


////////////////////////////////////////////////////////
//the caught star's position
float caughtStarX = 0;
float caughtStarZ = 0;
int count = 0;

//animation for the caught star-->timer func
void caughtStar(int i){
	//check if it is the same star(deletion might change the order)
	if(count == 0 ||(caughtStarX == stars[i].sx && caughtStarZ == stars[i].sz)){
		stars[i].sy += 0.1;
		stars[i].sz += 0.003;
		count++;
		caughtStarX = stars[i].sx;
		caughtStarZ = stars[i].sz;
	}

	if(stars[i].sy <= 5){

		glutTimerFunc(17,caughtStar,i);
	}
	
}
void catchStar(){

	for(int i = 0; i < stars.size(); i++){
		if(p.px >= stars[i].sx-1 && p.px <= stars[i].sx+1 
			&& p.pz >= stars[i].sz-1 && p.pz <= stars[i].sz+1){
			//count = 0;
			if(count == 0){

				glutTimerFunc(0,caughtStar,i);
			}
			
		}
		if(stars[i].sy > 5){
			stars.erase(stars.begin()+i);
			p.numStars++;
			count = 0;
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
	eye[1] = 50;
	gameover = true;
	reset();
}
void deductHealth(int i){
	p.life--;
	w.darts[i].hit = true;
	
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
void drawArrow(){
	glBegin(GL_QUADS);
		glVertex2f(70, 350);
    	glVertex2f(180, 350);

    	glVertex2f(180, 360);
		glVertex2f(70, 360);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex2f(180, 340);
		glVertex2f(180, 370);
		glVertex2f(200, 355);
	glEnd();
}

void drawSelectGameMode(){
	glColor3f(0,1,0);
    string welcomeTxt = "Welcome to the world!!";
    string gameModeTxt = "Select game mode:";
    string normal = "1. Normal";
    string hard = "2. Difficult";
    drawText(welcomeTxt.data(),welcomeTxt.size(), 200,450);
    drawText(gameModeTxt.data(),gameModeTxt.size(), 210,400);
    drawText(normal.data(),normal.size(), 210,350);
    drawText(hard.data(),hard.size(), 210,300);

    if(gameover){
    	glColor3f(1,0,0);
    	string gameOverTxt = "gameOver";
    	drawText(gameOverTxt.data(),gameOverTxt.size(), 210,550);
    }
}
void display(void) {
	setPerspectiveProjection();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0] + p.px,eye[1],eye[2] + p.pz, lookAt[0]+ p.px, lookAt[1]+ p.py+2,lookAt[2]+ p.pz, up[0],up[1], up[2]);

    if(gameModeSelection){
	    disable_lighting();
		setOrthographicProjection();
		glPushMatrix();
			drawSelectGameMode();
		glPopMatrix();

		if(hover_normal){
			glPushMatrix();
				drawArrow();
			glPopMatrix();
		}
		if(hover_hard){
			glColor3f(1,0,0);
			glTranslatef(0,-50,0);
			glPushMatrix();
				drawArrow();
			glPopMatrix();
		}
	}
	else{
		enable_lighting();
		//setPerspectiveProjection();
	    //drawscene
	    glTranslatef(p.px,p.py,p.pz);
		glRotatef(sceneAngle, 0,1,0);
		glTranslatef(-p.px,-p.py-0.5,-p.pz);
	    glTranslatef(0,0,scenePos);
	    drawScene();

	    
	}
	
	glutSwapBuffers();
	glutPostRedisplay();	
}

int closestWall(){
	float check = 10;
	int index = 0;
	int sindex = 0;

	//w.maze->numberWalls()
	for(int i = 0; i < m.numberWalls(); i++){
		float s[5][2];
		if(m.wallScale[i][0] > 3){//if the wall is like ----
			float s[5][2]= {
				{m.wallPositions[i][0], m.wallPositions[i][2] + m.wallScale[i][2]/4},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/2, m.wallPositions[i][2]+ m.wallScale[i][2]/4},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/3, m.wallPositions[i][2]+ m.wallScale[i][2]/4},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/4, m.wallPositions[i][2]+ m.wallScale[i][2]/4},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/10, m.wallPositions[i][2]+ m.wallScale[i][2]/4}
			};
		}else{//if the wall is like |
			float s[5][2] = {
				{m.wallPositions[i][0]+ m.wallScale[i][0]/4, m.wallPositions[i][2]},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/4, m.wallPositions[i][2] + m.wallScale[i][2]},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/4, m.wallPositions[i][2] + m.wallScale[i][2]/2},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/4, m.wallPositions[i][2] - m.wallScale[i][2]},
				{m.wallPositions[i][0]+ m.wallScale[i][0]/4, m.wallPositions[i][2] - m.wallScale[i][2]/2}
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
			if(p.pz > m.wallPositions[index][2]+m.wallScale[index][2]+1 || p.pz < m.wallPositions[index][2]-m.wallScale[index][2]
		    	|| (p.px <= m.wallPositions[index][0]-0.5 || p.px >= m.wallPositions[index][0]+(m.wallScale[index][0])/2 + 0.6)){
		    	return true;
		    }
		    break;
		case 1:
			if(p.pz > m.wallPositions[index][2]+m.wallScale[index][2]+0.4 || p.pz < m.wallPositions[index][2]-m.wallScale[index][2]-1
		    	|| (p.px <= m.wallPositions[index][0]-0.5 || p.px >= m.wallPositions[index][0]+(m.wallScale[index][0])/2 +0.6)){
		    	return true;
		    }
		    break;
		case 2:
			if(p.pz > m.wallPositions[index][2]+m.wallScale[index][2]+0.4 || p.pz < m.wallPositions[index][2]-m.wallScale[index][2]-1
		    	|| (p.px <= m.wallPositions[index][0] || p.px >= m.wallPositions[index][0]+(m.wallScale[index][0])/2 + 1)){
		    	return true;
		    }
		    break;
		case 3:
			if(p.pz > m.wallPositions[index][2]+m.wallScale[index][2]+0.4 || p.pz < m.wallPositions[index][2]-m.wallScale[index][2]-1
		    	|| (p.px <= m.wallPositions[index][0]-0.9 || p.px >= m.wallPositions[index][0]+(m.wallScale[index][0])/2)){
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
	        	p.pz -= 0.6;
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
	    		playerAngleZ += 20;
	    	}
	        	
	    }else if(key == GLUT_KEY_RIGHT){
	    	if(checkWall(3)&& p.px+0.4 < 15){
	        	p.px += 0.4;
	        	playerAngleZ -= 20;
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

	}else if(key == 'a'){
		sceneAngle--;
	}else if(key == 'd'){
		sceneAngle++;
	}else if (key == ' '){
		demo = true;
		eye[1] = 50;
	}else if(key == 'q'){
		exit(0);
	}
}
void reset(){
	gameModeSelection = true;
	demo = false;
	gameMode = 0;
	p.px = 0;
	p.pz = 10;
	p.py = 0;
	p.life = 5;
	p.numStars = 0;
	playerAngleX = 0;
	playerAngleZ = 0;

	eye[0] = 0;
	eye[1] = 50;
	eye[2] = 3;
	lookAt[0] = 0;
	lookAt[1] = 0;
	lookAt[2] = 0;
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;

	w.newWorld();//generate the world
	//get the stars
	stars.clear();
	for(int i = 0; i < w.numStars; i++){
    	stars.push_back(w.stars[i]);
    }
    glutTimerFunc(0,timer,0);
}
void finalDoor(){
	if(p.pz >= -63 && p.pz <= -61 && p.px >= -0.5 && p.px <= 0.5){
		p.py += 0.1;
		eye[1] += 0.1;
		eye[2] = -2;
		lookAt[1] = -2;
		if(p.py > 20){
			reset();
		}
	}
}

void idle(){
	if(!pause){

	   	//dart
    	dartAnimation();
    	dartHitsPlayer();

		//catch stars
		catchStar();

		//got to the final door
		finalDoor();
		
	}
	if(demo){
		cnt++;
		if(cnt % 6 == 0){
			if(eye[1] != 6)
				eye[1]--;
			if(scenePos != 0)
				scenePos--;
		}
		if(eye[1] == 6 && scenePos == 0){
			eye[1] -= 2;
			if(eye[2] <= 3.0){
				eye[2] += 2.6;
			}
			
			demo = false;

		}
	}
}
float prevMouseX;
float prevMouseY;
void LeftClick(int x, int y){

    float xCursor = (float)(x - 325)/30;
    float yCursor = (float)((800-325) - y)/30;

    if(prevMouseX != 0){
    	lookAt[0] += (prevMouseX - x) * abs(xCursor/200); 
    	lookAt[1] -= (prevMouseY - y) * abs(yCursor/600);  
    }else{
    	lookAt[0] += (xCursor/200); 
    	lookAt[1] -= (yCursor/600);  
    }
     
    

    prevMouseX = x;
    prevMouseY = y;
    
}

void mouse_func(int btn, int state, int x, int y){
 
    if (!pause && btn == GLUT_LEFT_BUTTON){
    	prevMouseX = 0; 
    	prevMouseY = 0;  
        glutMotionFunc(LeftClick);    
    }
    if(gameModeSelection && btn == GLUT_LEFT_BUTTON){
    	if(x < 340 &&  x > 200 && y < 335 && y > 315){
			gameModeSelection = false;
			demo = true;
			gameMode = 1;
		}
		if(x < 340 &&  x > 200 && y < 410 && y > 375){
			gameModeSelection = false;
			demo = true;
			gameMode = 2;
		}
    }
    
}
void passive(int x, int y){
	if(gameModeSelection){
		if(x < 340 &&  x > 200 && y < 335 && y > 315){
			hover_normal = true;
		}else{
			hover_normal = false;
		}
		if(x < 340 &&  x > 200 && y < 410 && y > 375){
			hover_hard = true;
		
		}else{
			hover_hard = false;
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
	printf("%s\n", "'a','d' --> look to the left and right");

	printf("%s\n", "mouse drag --> drag to move the camera");

	printf("%s\n", "'q' --> to quit");
	printf("%s\n", "---------------------------------------");

}
void timer(int val){
	if(gameMode != 0){
		w.time--;
	}
	glutPostRedisplay();
	if(w.time == 0){
		gameOver();
	}else if(p.life != 0){
		glutTimerFunc(200,timer, 0);
	}
	
}
int main(int argc, char** argv) {
	wallTexture.load("./Texture/wall.ppm");
	floorTexture.load("./Texture/carpet_new.ppm");

	black_box.load("./Texture/black_box.ppm");

	demo = false;
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

    ////////////////////////
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    

    /////////////

    glutMouseFunc(mouse_func);
    glutPassiveMotionFunc(passive);

    glutDisplayFunc(display);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glutTimerFunc(0,StarRotation,0);

    glClearColor(0.1, 0.2, 0.4, 1);
    //
	glutTimerFunc(0,BallMovement,0);
    glutTimerFunc(0,addParticle,0);
    //

    glutTimerFunc(0, timer,0);
    glutMainLoop();

    return 0;
}