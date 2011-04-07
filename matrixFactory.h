#pragma once
#include "matrixf.h"
#include <math.h>

class matrixFactory
{
public:
	matrixFactory(void);
	~matrixFactory(void);

	static matrixf id(void){
		return matrixf(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	}

	static matrixf translate(float dx, float dy, float dz){
		return matrixf(1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			dx,dy,dz,1);
	}

	static matrixf rotateX(float thetax){
		return matrixf(1,0,0,0,
			0,cos(thetax), -sin(thetax), 0,
			0, sin(thetax), cos(thetax), 0,
			0,0,0,1);
	}

	static matrixf rotateY(float thetay){
		return matrixf(cos(thetay),0,-sin(thetay),0,
			0,1, 0, 0,
			sin(thetay), 0, cos(thetay), 0,
			0,0,0,1);
	}

	static matrixf rotateZ(float thetaz){
		return matrixf(cos(thetaz),sin(thetaz),0,0,
			-sin(thetaz),cos(thetaz), 0, 0,
			0, 0, 1, 0,
			0,0,0,1);
	}
	static matrixf scale( float scal ){
		return matrixf(scal,0,0,0,0,scal,0,0,0,0,scal,0,0,0,0,1);
	}
};
