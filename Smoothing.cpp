#include "StdAfx.h"
#include "Smoothing.h"
#include "Operator.h"
#include <vector>

Smoothing::Smoothing(void)
{
}

Smoothing::~Smoothing(void)
{
}

void Smoothing::initNeighbors(mesh& m){
	vector<tuple3i> & faces = m.faces;

	for(unsigned int i = 0; i < faces.size(); i++){
		if(contains(neighbors[faces[i].a], faces[i].b) == 0){
			
		}
		if(contains(neighbors[faces[i].a], faces[i].c) == 0){
			
		}
		if(contains(neighbors[faces[i].b], faces[i].b) == 0){
			
		}
		if(contains(neighbors[faces[i].b], faces[i].b) == 0){
			
		}
		if(contains(neighbors[faces[i].c], faces[i].b) == 0){
			
		}
		if(contains(neighbors[faces[i].c], faces[i].b) == 0){
			
		}
	}
}

int Smoothing::contains(vector<int> , int face){
	return 1;
}

void Smoothing::smootheMesh_explicitEuler( mesh& m )
{
	float lambda= 1, dt= 0.0001f;
	float vol_old = Operator::volume(m), factor;
	Operator::calcAllCurvNormals(m, normals);

	vector<tuple3f>& vertices = m.vertices;
	
	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i] = vertices[i] + normals[i] * (lambda * dt);
	}

	factor = pow(vol_old / Operator::volume(m), 0.33333333f);

	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i] *= factor;
	}
}


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


void Smoothing::smootheMesh_implicitEuler( mesh& m )
{
	float lambda= 1, dt= 0.001f;
	vector<tuple3f>& vertices = m.vertices;
}
