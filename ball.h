#pragma once
#include "mesh.h"

class ball :
	public mesh
{
public:
	ball(float r, int verticesPerHalfCycle);
	~ball(void);
};
