#ifndef PARTICLE_H
#define PARTICLE_H
#include <vector>

class Particle{
public:
	Particle();

	int id;

	//position
	float px, py, pz;
	
	//direction
	float dx, dy, dz;

	float cy;//cannon

	float hy;

	float speed;
	
	//rotation
	int rx, ry, rz;

	//color
	float r, g, b;

	int material;
	int age;
	
	
};
#endif
