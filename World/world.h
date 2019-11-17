
#include "maze.h"
#include "../Star/star.h"
#include "../Obstacle/obstacle.h"
#ifndef WORlD_H
#define WORlD_H

class World{
public:
	World();
	void drawFloor();
	float time;

	int numStars;

	int numEnemies;

	int numDarts;

	std::vector<Star> stars;//stars
	std::vector<Obstacle> darts;//obstacles
	
	Maze *maze;

	std::vector<std::vector<float>> wallPositions;
	std::vector<std::vector<float>> wallScale;
	
};
#endif
