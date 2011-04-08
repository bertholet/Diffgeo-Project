#pragma once
#include "mesh.h"
#include <vector>

class ImplicitEulerSmoothing
{
	//indices of the values in the sparse matrix
	vector<int> ia, ja;
	//values of the elements of the sparse matrix
	vector<double> a;
private:
	void ifNotContainedAdd(vector<int> &v, int a);
public:
	ImplicitEulerSmoothing(mesh &m, float lambda, float dt);
	~ImplicitEulerSmoothing(void);
	void smootheMesh(mesh &m);
};
