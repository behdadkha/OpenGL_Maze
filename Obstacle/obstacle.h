
#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle{
public:
	Obstacle();
	//position
	float ox, oy, oz;
	//direction
	float dx, dy, dz;

	float speed;

	bool hit;
};

#endif