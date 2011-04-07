#pragma once
#include"mesh.h"
#include <vector>

class Smoothing
{
public:
	vector<tuple3f> normals;
	vector<vector<int>> neighbors;
	Smoothing(void);
	~Smoothing(void);

	void initNeighbors(mesh& m);
	int contains(vector<int> , int face);
	void smootheMesh_explicitEuler(mesh& m);
	void smootheMesh_implicitEuler(mesh& m);
};
