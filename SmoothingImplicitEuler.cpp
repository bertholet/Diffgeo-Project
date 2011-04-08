#include "StdAfx.h"
#include "SmoothingImplicitEuler.h"

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


ImplicitEulerSmoothing::ImplicitEulerSmoothing( mesh &m )
{
	vector<tuple3f> & vertices = m.vertices;

	vector<int>  * neighbors = new vector<int>[vertices.size()];

	for(unsigned int i = 0; i < vertices.size(); i++){

		//find neighbors

		//neighbors
	}
}

ImplicitEulerSmoothing::~ImplicitEulerSmoothing(void)
{

}

void ImplicitEulerSmoothing::smootheMesh( mesh &m )
{
	
}
