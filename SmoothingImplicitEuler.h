#pragma once
#include "mesh.h"
#include <vector>

class ImplicitEulerSmoothing
{
	//indices of the values in the sparse matrix
	vector<int> ia, ja;
	//values of the elements of the sparse matrix
	vector<double> a;
public:
	ImplicitEulerSmoothing(mesh &m);
	~ImplicitEulerSmoothing(void);
	void smootheMesh(mesh &m);
};
