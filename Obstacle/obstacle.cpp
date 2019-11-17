
#include <vector>
#include "obstacle.h"
#include <time.h>
#include <stdlib.h>
using namespace std;

Obstacle::Obstacle(){
	this -> ox = -5 - rand()% 30;
	this -> oy = 0.5;
	this -> oz = -30 - rand()% 30;

	this -> dx = 50;
	this -> dy = 0;
	this -> dz = 0;

	this -> speed = 0.001;
	this -> hit = false;
}
