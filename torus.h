#pragma once
#include "mesh.h"

class torus :
	public mesh
{
public:
	torus(float r1, float r2, int nrVertices_smallRing);
	torus(float r1, float r2,int nrVertices_smallRing,int nrVertices_BigRing);
	~torus(void);
};
