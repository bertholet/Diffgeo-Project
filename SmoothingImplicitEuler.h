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

	//Pardiso parameter
	/* Internal solver memory pointer pt,                  */
	/* 32-bit: int pt[64]; 64-bit: long int pt[64]         */
	/* or void *pt[64] should be OK on both  architectures  */ 
	void    *pt[64]; 

	int int_params[64];
	double double_params[64];

	//diverse other parameters
	int      maxfct, mnum, phase, error, print_stats, solver, num_procs, nr_eqs, matrix_type;

private:
	void ifNotContainedAdd(vector<int> &v, int a);
	void findNeighbors( vector<tuple3i> &faces, vector<int>  * result );
	void setUpSparseMatrix( vector<tuple3f> &vertices, vector<int> * neighbors, float lambda, float dt ) ;
	void pardisoInitFactorize(void);
public:
	ImplicitEulerSmoothing(mesh &m, float lambda, float dt);
	~ImplicitEulerSmoothing(void);
	void smootheMesh(mesh &m);

	double calcBoxNormError(void);

	void updateVerticesAndB( mesh & m );
};
