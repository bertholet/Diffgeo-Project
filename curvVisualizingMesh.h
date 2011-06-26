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


class borderColorMap:colorMap
{
	string info;
	bool *border;
	tuple3f col1,  col2;
	int sz;
public:
	borderColorMap(vector<int> & border_, tuple3f color1, tuple3f color2){
		sz = border_.back();
		vector<int>::iterator it = border_.begin();

		border = new bool[sz];
		for(int i = 0; i < sz; i++){
			if(*it == i){
				it++;
				border[i] = true;
			} 
			else{
				border[i] = false;
			}
		}
		col1 = color1;
		col2 = color2;
	}
	~borderColorMap(){
		delete border;
	}

	tuple3f color(int vertexNr);
	string additionalInfo(void);
};