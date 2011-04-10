#pragma once
#include "mesh.h"
#include <vector>

#define NRHS 3

class ImplicitEulerSmoothing
{
	//size of matrix
	int n;
	//double (*b)[NRHS];
	//double (*x)[NRHS];
	double *b;
	double *x;
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
	void updateVerticesAndB( mesh & m );
};
