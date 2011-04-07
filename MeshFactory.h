#pragma once
#define _USE_MATH_DEFINES
#include "mesh.h"
#include <math.h>

struct MeshFactory
{
	static mesh ball(int nrVertices){
		mesh ball;

		float x,y, z;
		for(float i = 0; i <1.01; i+= 0.1){
			z = cos(i* M_PI);
			for(float j = 0; j < 1.999; j+=0.1)
			{
				x = sin(i*M_PI)*cos(j * M_PI);
				y = sin(i*M_PI)*cos(j * M_PI);
			}
		}
		return mesh();
	}
};
