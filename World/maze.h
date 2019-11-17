#include <vector>
using namespace std;
#ifndef MAZE_H
#define MAZE_H

class Maze{
public:
	Maze();
	vector<vector<float>> wallPositions;
	vector<vector<float>> wallScale;
	void drawMaze();
	int numberWalls();
private:
	void makeWall();
};

#endif