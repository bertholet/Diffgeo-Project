#pragma once
#include "tuple3.h"
#include "mesh.h"
#include <vector>

class Operator
{

public:
	Operator(void);
	~Operator(void);
	static void calcAllCurvNormals(mesh &m, vector<tuple3f> &target);
	static void calcAllGaussCurvs(mesh &m, vector<float> &target);
	static void calcAllAMixed(mesh &m, vector<float> &target);
	static float volume(mesh &m);
};
