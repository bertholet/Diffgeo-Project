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

