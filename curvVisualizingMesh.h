#pragma once
#include "mesh.h"
#include "tuple3.h"
#include "Operator.h"
#include "colorMap.h"
#include <vector>
#include <string>

class curvColormap : colorMap
{
	string info;
public:
	float min, max;
	vector<tuple3f> * theNormals;
	vector<tuple3f> curvNormals;
	curvColormap(mesh &myMesh_);
	~curvColormap(void);

	tuple3f color(float val);
	tuple3f color(int vertexNr);
	void setNormals(vector<tuple3f> & normals);
	string additionalInfo(void);
};

class gaussColormap:colorMap
{
	string info;
	float min, max;
public:
	vector<float> gaussCurvs;
	gaussColormap(mesh & myMesh_);
	~gaussColormap();
	tuple3f color(float val);
	tuple3f color(int vertexNr);
	string additionalInfo(void);
};
