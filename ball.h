#pragma once
#include "mesh.h"

class ball :
	public mesh
{
public:
	ball(float r, int verticesPerHalfCycle);
	ball(float r, int nrPhi, int nrPsi);
	ball(float r, int nrPhi, int nrPsi,bool FUN);
	~ball(void);
};
