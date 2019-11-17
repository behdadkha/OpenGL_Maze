
#include "materials.h"
#include <stdlib.h>
using namespace std;

//gold
vector<float> Materials::gold(){
	float goldM[10] = {
		0.24725, 0.1995, 0.0745, 
		0.75164, 0.60648, 0.22648, 
		0.628281, 0.555802, 0.366065, 
		0.4
	};
	vector<float> v;
	v.assign(goldM, goldM+10);

	return v;
}
//silver
vector<float> Materials::silver(){
	float silverM[10] = {
		0.19225, 0.19225, 0.19225,
		0.50754, 0.50754, 0.50754,
		0.508273, 0.508273,	0.508273,
		0.4
	};
	vector<float> v;
	v.assign(silverM, silverM+10);

	return v;
}
//ruby
vector<float> Materials::ruby(){
	float rubyM[10] = {
		0.1745, 0.01175, 0.01175,
		0.61424, 0.04136, 0.04136,
		0.727811, 0.626959, 0.626959,
		0.6
	};
	vector<float> v;
	v.assign(rubyM, rubyM+10);

	return v;
}
