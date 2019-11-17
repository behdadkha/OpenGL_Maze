#include "../Material/materials.h"
#include <vector>
#ifndef STAR_H
#define STAR_H

class Star {
public:
	//pos --> random index to position array in star.cpp
	Star(int pos);

	//position
	float sx, sy, sz;

	//star type
	bool evil;

	void drawStar();
	
	//material
	std::vector<float> material;
};
#endif
