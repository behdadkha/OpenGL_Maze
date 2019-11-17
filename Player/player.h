#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
	Player();
	void drawPlayer();
	//position
	float px, py, pz;
	
	//direction
	float dx, dy, dz;

	//life
	int life;
	
	//caught stars
	int numStars;
	
};
#endif
