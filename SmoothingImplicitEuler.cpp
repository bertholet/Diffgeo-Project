#include "StdAfx.h"
#include "SmoothingImplicitEuler.h"
#include <algorithm>

/* PARDISO prototype. */
extern "C" __declspec(dllimport) void pardisoinit (void   *, int    *,   int *, int *, double *, int *);
extern "C" __declspec(dllimport) void pardiso     (void   *, int    *,   int *, int *,    int *, int *, 
												   double *, int    *,    int *, int *,   int *, int *,
												   int *, double *, double *, int *, double *);
extern "C" __declspec(dllimport) void pardiso_chkmatrix  (int *, int *, double *, int *, int *, int *);
extern "C" __declspec(dllimport) void pardiso_chkvec     (int *, int *, double *, int *);
extern "C" __declspec(dllimport) void pardiso_printstats (int *, int *, double *, int *, int *, int *,
														  double *, int *);
using namespace std;


ImplicitEulerSmoothing::ImplicitEulerSmoothing( mesh &m, float lambda, float dt)
{
	vector<tuple3f> & vertices = m.vertices;
	vector<tuple3i> & faces = m.faces;
	vector<int>  * neighbors = new vector<int>[vertices.size()];
	vector<int>::iterator low;
	float minus_lambdaDt_div_m;
	//vector<int> & neighbors[] = *neighborsptr;

	//find neighbors
	for(unsigned int i = 0; i < faces.size(); i++){

		//adds the integers such that the vector stays ordered.
		ifNotContainedAdd(neighbors[faces[i].a], faces[i].b);
		ifNotContainedAdd(neighbors[faces[i].b], faces[i].a);

		ifNotContainedAdd(neighbors[faces[i].a], faces[i].c);
		ifNotContainedAdd(neighbors[faces[i].c], faces[i].a);

		ifNotContainedAdd(neighbors[faces[i].b], faces[i].c);
		ifNotContainedAdd(neighbors[faces[i].c], faces[i].b);
	}


	//set up ia
	ia.reserve(vertices.size()+1);
	ia.push_back(0);
	int j = 0;
	int nb;
	for(unsigned int i = 0; i < vertices.size(); i++){
		nb = neighbors[i].size();
		ia.push_back((ia.back()) + nb +1);
	}

	//set up ja
	ja.reserve(vertices.size()*6);
	int sz;
	for(int i = 0; i < vertices.size(); i++){
		j=0;
		nb = neighbors[i][j];
		sz = neighbors[i].size();
		while(nb < i && j < sz){
			nb = neighbors[i][j];
			ja.push_back(nb);
			j++;
		}

		ja.push_back(i);
		
		if(j < sz && neighbors[i][j] == i){
			//should not happen
			throw new std::exception("Assertion failed !!!!! matrix not well constructed, goto debug, my f(r)iend...");
		}

		while(j < sz){
			nb = neighbors[i][j];
			ja.push_back(nb);
			j++;
		}
	}

	//set up values of the sparse matrix
	a.reserve(ja.size());
	for(int i = 0; i < vertices.size(); i++){
		j=0;
		sz = neighbors[i].size();
		minus_lambdaDt_div_m = -lambda * dt /sz;
		while(nb < i && j < sz){
//			nb = neighbors[i][j];
			a.push_back(minus_lambdaDt_div_m);
			j++;
		}

		a.push_back(1 + lambda * dt);

		while(j < sz){
//			nb = neighbors[i][j];
			a.push_back(minus_lambdaDt_div_m);
			j++;
		}
	}

}

ImplicitEulerSmoothing::~ImplicitEulerSmoothing(void)
{

}

void ImplicitEulerSmoothing::smootheMesh( mesh &m )
{
	//refactor this stuff asap
	//put killing this annoying library in the destructor. init stuff in the constructor.
	//(muhahaha destoy it, destroy it...)

	int      mtype = 1;        /* Real STRUCTURALLY symmetric matrix */

	/* Internal solver memory pointer pt,                  */
	/* 32-bit: int pt[64]; 64-bit: long int pt[64]         */
	/* or void *pt[64] should be OK on both  architectures  */ 
	void    *pt[64]; 

	/* Pardiso control parameters. */
	int      int_params[64];
	double   double_params[64];
	int      maxfct, mnum, phase, error, msglvl, solver;

	/* Number of processors. */
	int      num_procs;


	///////////////Put this stuff in the constructor...
	error = 0;
	solver = 1; /* 0: direct solving, 1 iteratvie solving*/
	pardisoinit (pt,  &mtype, &solver, int_params, double_params, &error); 

	if (error != 0) 
	{
		if (error == -10 )
			printf("No license file found \n");
		if (error == -11 )
			printf("License is expired \n");
		if (error == -12 )
			printf("Wrong username or hostname \n");

		int ah;
		cin >> ah;
	}
	else
		printf("[PARDISO]: License check was successful ... \n");
}


void ImplicitEulerSmoothing::ifNotContainedAdd( vector<int> &v, int a )
{
	vector<int>::iterator low;

	low = lower_bound(v.begin(), v.end(),a);
	if(*low < a){
		v.insert(low,1,a);
	}
}
