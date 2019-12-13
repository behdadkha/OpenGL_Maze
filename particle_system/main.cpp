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
#include <math.h>
#include "particle.h"
using namespace std;


bool friction = true;
bool pause = false;
bool light = false;
bool particleCAM = false;
bool cannon = false;
bool hole = false;
bool showLight = false;

//rotate scene
int yangle = 0;
int xangle = 0;

vector<Particle> particles;

//camera
GLdouble eye[] = { 10, 5, 20};
GLdouble lookAt[] = { 0, 3, 0 };
GLdouble up[] = { 0, 1, 0 };

//cannon
int cannon_angle[] = {90,-9,1};

//light
float light_pos[2][4] = {
    {2,2,0,1},
    {-4,2,5,1}
};
float amb[2][4] = {
    {1,0,0,1},
    {0,0.5,0,1}
};
float diff[2][4] = {
    {1,0,1,1},
    {0,1,0,1}
};
float spec[2][4] = {
    {1,1,1,1},
    {0,1,0,1}
};

//material
// 1: gold, 2:copper , 3:silver, 4:brass
GLfloat materialAmbient[4][4] = {
    { 0.24725, 0.1995, 0.0745, 1.0},
    { 0.19125, 0.0735, 0.0225, 1.0},
    { 0.19225, 0.19225, 0.19225, 1.0},
    {0.329412, 0.223529, 0.027451, 1.0}
};
GLfloat materialDiffuse[4][4] = {
    { 0.75164, 0.60648, 0.22648, 1.0},
    { 0.7038, 0.27048, 0.0828, 1.0},
    { 0.50754, 0.50754, 0.50754, 1.0},
    {0.780392, 0.568627, 0.113725, 1.0}
};
GLfloat materialSpecular[4][4] = {
    { 0.628281, 0.555802, 0.366065, 1.0},
    { 0.256777, 0.137622, 0.086014, 1.0},
    { 0.508273,0.508273,0.508273, 1.0},
    {0.992157, 0.941176, 0.807843, 1.0}
};
GLfloat materialShiny[4] = {
    0.4,
    0.1,
    0.4,
    0.21794872
};


void drawFloor(void){
    

    if(!hole){
        glBegin(GL_QUADS);
            glNormal3f(0,1,0);
            glColor3f(1, 0, 0);
            glVertex3f(-8,-0.1,8);
            glVertex3f(8,-0.1,8);
            glVertex3f(8,-0.1,-8);
            glVertex3f(-8,-0.1,-8);
        glEnd(); 
    }else{
        //2 holes
        glBegin(GL_QUADS);
            glNormal3f(0,1,0);
            glColor3f(1, 0, 0);
            glVertex3f(-8,-0.1,8);
            glVertex3f(8,-0.1,8);
            glVertex3f(8,-0.1,4);
            glVertex3f(-8,-0.1,4);

            glNormal3f(0,1,0);
            glVertex3f(-8,-0.1,2);
            glVertex3f(8,-0.1,2);
            glVertex3f(8,-0.1,-8);
            glVertex3f(-8,-0.1,-8);

            glNormal3f(0,1,0);
            glVertex3f(4,-0.1,4);
            glVertex3f(-3,-0.1,4);
            glVertex3f(-3,-0.1,2);
            glVertex3f(4,-0.1,2);

            glNormal3f(0,1,0);
            glVertex3f(-7,-0.1,4);
            glVertex3f(-8,-0.1,4);
            glVertex3f(-8,-0.1,2);
            glVertex3f(-7,-0.1,2);

            glNormal3f(0,1,0);
            glVertex3f(6,-0.1,4);
            glVertex3f(8,-0.1,4);
            glVertex3f(8,-0.1,2);
            glVertex3f(6,-0.1,2);
        glEnd(); 
        

    }


    
}

void setLight(){
    for (unsigned int i = 0; i < 2; i++) {
        glLightfv(GL_LIGHT0 + i, GL_POSITION, light_pos[i]);
        glLightfv(GL_LIGHT0 + i, GL_AMBIENT, amb[i]);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, diff[i]);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, spec[i]);
    }
}

void setMaterial(int mat){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient[mat]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse[mat]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular[mat]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShiny[mat]);
}


int findP(){
    for(int i = 0; i < particles.size(); i++){
        if(particles[i].id == 1){
            return i;
        }
        
    }
    return 0;
}

void drawSphere(Particle p){
    glColor3f(p.r,p.g,p.b);
    glTranslatef(p.px,p.py,p.pz);
    glRotatef(p.rx, 1,0,0);
    glRotatef(p.ry, 0,1,0);
    glRotatef(p.rz, 0,0,1);
    setMaterial(p.material);
    glutSolidSphere(0.1,15,15);
}
void drawCannon(){
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glTranslatef(0,1,0);
    glRotatef(cannon_angle[0], 1,0,0);
    glRotatef(cannon_angle[1], 0,1,0);
    glRotatef(cannon_angle[2], 0,0,1);
    gluCylinder(quadratic,0.4f,0.5f,1.0f,32,32);

    glColor3f(0,0,1);
    gluCylinder(quadratic,0.4f,0.5f,0.1f,32,32);
}
void makeScene(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //camera
    if(!particleCAM){
        gluLookAt(eye[0],eye[1],eye[2],lookAt[0], lookAt[1], lookAt[2], up[0],up[1], up[2]);
    }else{
        int particle_index = findP();
        gluLookAt(particles[particle_index].px, particles[particle_index].py+0.5,particles[particle_index].pz, 0, 1, 0, up[0],up[1], up[2]);
    }

    //floor
    glPushMatrix();
        glRotatef(yangle,0,1,0);
        glRotatef(xangle,1,0,0);
        setMaterial(4);
        drawFloor();
    glPopMatrix();

    //draw lights as a solid sphere key == b
    if(showLight){

        //light0
        glPushMatrix();
            glColor3f(0,1,0);
            glTranslatef(light_pos[0][0], light_pos[0][1], light_pos[0][2]);
            glutSolidSphere(0.5,15,15);
        glPopMatrix();

        //light1
        glPushMatrix();
            glColor3f(0,1,0);
            glTranslatef(light_pos[1][0], light_pos[1][1], light_pos[1][2]);
            glutSolidSphere(0.5,15,15);
        glPopMatrix();   
        
    }
    //cannon
    if(cannon){
        glPushMatrix();
            glColor3f(0,1,0);
            glRotatef(yangle,0,1,0);
            glRotatef(xangle,1,0,0);
            setMaterial(0);
            drawCannon();
        glPopMatrix();
    }

    //sphere
    for(int i = 0; i < particles.size(); i++){
        glPushMatrix();
            if(cannon){
                glTranslatef(0,1,0);
            }
            glRotatef(yangle,0,1,0);
            glRotatef(xangle,1,0,0);
            drawSphere(particles[i]);
        glPopMatrix();
    }

}
void display(void) {

    if(light)
        setLight();

    makeScene();

    glutSwapBuffers();
    glutPostRedisplay();
}

void handleBounce(int i){
    //bounce only its in the borders
    if(particles[i].px <= 8 && particles[i].px >= -8 && particles[i].pz <= 8 && particles[i].pz >= -8)
        {
            if(hole == true && ((particles[i].px >= 4 && particles[i].px <= 6) || (particles[i].px >= -7 && particles[i].px <= -3)) && particles[i].pz <= 4 && particles[i].pz >= 2){
                return;
            }else {
                if(particles[i].py <= 0){
                    if(friction){
                        //decrese the particles height by 20%
                        if(cannon && particles[i].cy < -4){
                            particles[i].cy -= particles[i].cy * 0.2;
                            particles[i].speed -= particles[i].speed * 0.4;
                        }
                        particles[i].hy -= particles[i].hy * 0.2;
                        particles[i].speed -= particles[i].speed * 0.01;
                        //if there is no bouce then reduce the speed
                        if(particles[i].hy <= 2 && particles[i].hy != 0){
                            particles[i].speed -= particles[i].speed * 0.1;
                        }
                }
                particles[i].dy = particles[i].hy;
            }
        }
    }
}
void checkBallAge(int i){
    if(particles[i].age > 500){

        if(particleCAM){
            
            if(i == findP()){
                particleCAM = false;
            }
        }
        particles.erase (particles.begin() + i);
        
    }

}

void checkBellowBoard(int i){
    if(particles[i].py < -5){
            
        if(particleCAM){
            if(i == findP())
                particleCAM = false;
        }
        particles.erase (particles.begin() + i);
    }

}
void BallMovement(int val){

    glutPostRedisplay();
    for(int i = 0; i < particles.size(); i++){ 

        //check if particle has fallen bellow the board
        checkBellowBoard(i);

        
        if(cannon && particles[i].cy > 1){
            particles[i].dz = 4 * (cannon_angle[0]-90);
            particles[i].dx = 4 * (-cannon_angle[1]);

            particles[i].dy = particles[i].cy;
            particles[i].cy = particles[i].cy - (9.8);

            if(particles[i].dy > 0 && -cannon_angle[1] > 0)
                particles[i].dy -= 2 * (-cannon_angle[1]);
            else if(particles[i].dy > 0 && -cannon_angle[1] < 0){
                particles[i].dy -= 2 * (cannon_angle[1]);
            } 
        }

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

void handleReshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50,1,1,100);

    glMatrixMode(GL_MODELVIEW);
}

void handleKeyboard(unsigned char key, int _x, int _y) {
    if (key == 'q') {
        exit(0);
    }
    if(key == 'f'){
        if(friction)
            friction = false;
        else
            friction = true;
    }else if(key == ' '){
        if(pause){
            pause = false;
        }else{
            pause = true;
        }
    }else if(key == 'r'){
        particles.clear();
    }else if(key == 'l'){
        if(!light){
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            light = true;
        }else{
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            light = false;
        }
    }
    else if(key == 'i'){ 
        if(!particleCAM){
            particles[particles.size() - 1].id = 1;
            particleCAM = true;
        }else{
            particles[findP()].id = 0;
            particleCAM = false;
        }
    }
    else if(key == 'c'){
        particles.clear();
        if(cannon)
            cannon = false;
        else
            cannon = true;
    }else if(key == 'p'){
        if(hole){
            hole = false;
        }else{
            hole = true;
        }
    }else if(key == 'b'){
        if(showLight)
            showLight = false;
        else
            showLight = true;
    }

    //light0 movement u:up, j:down, h:left k:right x:out(z) z:in
    if(key == 'u'){
        light_pos[0][1]++;
    }
    else if(key == 'j'){
        light_pos[0][1]--;
    }
    else if(key == 'h'){
        light_pos[0][0]--;
    }
    else if(key == 'k'){
        light_pos[0][0]++;
    }
    else if(key == 'x'){
        light_pos[0][2]++;
    }
    else if(key == 'z'){
        light_pos[0][2]--;
    }

    //light1 movement 2:up 4:down 1:left 3:right 5:out(z) 6:in(z)
    if(key == '2'){
        light_pos[1][1]++;
    }
    else if(key == '4'){
        light_pos[1][1]--;
    }
    else if(key == '1'){
        light_pos[1][0]--;
    }
    else if(key == '3'){
        light_pos[1][0]++;
    }
    else if(key == '5'){
        light_pos[1][2]++;
    }
    else if(key == '6'){
        light_pos[1][2]--;
    }
    
    //cannon movement
    if(key == 'w'){
        if(cannon_angle[0] > 25)
            cannon_angle[0]--;

    }else if(key == 's'){
        if(cannon_angle[0] < 170)
            cannon_angle[0]++;
    }
    else if(key == 'a'){
        if(cannon_angle[1] < 80)
            cannon_angle[1]++;
    }
    else if(key == 'd'){
        if(cannon_angle[1] > -80)
            cannon_angle[1]--;
    }
    

}
void special(int key, int _x, int _y){
    if(key == GLUT_KEY_UP){
        xangle++;
    }else if(key == GLUT_KEY_DOWN){
        xangle--;
        
    }else if (key == GLUT_KEY_LEFT){
        yangle++;
    }else if(key == GLUT_KEY_RIGHT){
        yangle--;
    }
}
void showControl(){

    printf("\n%s\n", "1.   'i' --> Particle camera");
    printf("%s\n", "2.   'p' --> Floor holes");
    printf("%s\n", "3.   'c' --> cannon");
    printf("%s\n", "        'w', 's' --> rotate cannon around x direction");
    printf("%s\n\n", "      'a','d' --> rotate cannon around y direction");
    printf("%s\n", "  'f' --> Toggle friction");
    printf("%s\n", "  'up arrow' --> rotate the scene around x axis");
    printf("%s\n", "  'down arrow' --> rotate the scene around x axis");
    printf("%s\n", "  'left arrow' --> rotate the scene around y axis");
    printf("%s\n", "  'right arrow' --> rotate the scene around y axis");
    printf("%s\n", "  'space bar' --> start or pause the animation");
    printf("%s\n", "  'r' --> Reset the flow of the particles");
    printf("%s\n\n", "  'L' --> Enable/Disable lighting");


    printf("%s\n", "   light0");
    printf("%s\n", "  'u','j' --> move light0 in y direction");
    printf("%s\n", "  'h','k' --> move light0 in x direction");
    printf("%s\n", "  'z','x' --> move light0 in z direction");

    printf("%s\n", "   light1");
    printf("%s\n", "  '2','4' --> move light1 in y direction");
    printf("%s\n", "  '1','3' --> move light1 in x direction");
    printf("%s\n", "  '5','6' --> move light1 in z direction");
    
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(650,800);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);
    glutCreateWindow("Fountain");

    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(handleReshape);

    showControl();

    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.3, 0.5, 1, 0);

    glutTimerFunc(0,BallMovement,0);
    glutTimerFunc(0,addParticle,0);
    glutMainLoop();

    return 0;
}
