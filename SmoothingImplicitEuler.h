#pragma once
#include "mesh.h"
#include <vector>

class ImplicitEulerSmoothing
{
	//size of matrix
	int n;
	double (*b)[1];
	double (*x)[1];
	//indices of the values in the sparse matrix
	vector<int> ia, ja;
	//values of the elements of the sparse matrix
	vector<double> a;
private:
	void ifNotContainedAdd(vector<int> &v, int a);
	void findNeighbors( vector<tuple3i> &faces, vector<int>  * result );
	void setUpSparseMatrix( vector<tuple3f> &vertices, vector<int> * neighbors, float lambda, float dt ) ;
public:
	ImplicitEulerSmoothing(mesh &m, float lambda, float dt);
	~ImplicitEulerSmoothing(void);
	void smootheMesh(mesh &m);
};
