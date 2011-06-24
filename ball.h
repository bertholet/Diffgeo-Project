#pragma once
#include "mesh.h"

class ball :
	public mesh
{
public:
	ball(float r, int verticesPerHalfCycle);
	ball(float r, int nrPhi, int nrPsi);
	ball(float r, int nrPhi, int nrPsi,bool FUN);
	ball( float r, int nrPhi, int nrPhi2,int nrPsi );
	~ball(void);
};
