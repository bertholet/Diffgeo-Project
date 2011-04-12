#pragma once
#include "mesh.h"

class cube :
	public mesh
{
public:
	cube(float side, int n);
	~cube(void);
};
