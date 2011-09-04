#pragma once
#include "mesh.h"
#include "tuple3.h"
#include "Operator.h"
#include "colorMap.h"
#include <vector>
#include <string>
#include <algorithm>

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
	int *border;
	int components;
	tuple3f col1,  col2;
	int sz;
public:
	borderColorMap(vector<int> & border_,vector<int> & borderStarts, tuple3f color1, tuple3f color2){
		sz = max_(border_)+1;
		//vector<int>::iterator it = border_.begin();
		components = borderStarts.size();
		printf("%d border components found \n", components);

		border = new int[sz];
		vector<int>::iterator end; 
		for(int i = 0; i < sz; i++){
			border[i] = -1; //border[i] = false;
			for(int j = 0; j < borderStarts.size(); j++){
				end = (j == components -1 ? border_.end(): border_.begin() + borderStarts[j+1]);
				if(find(border_.begin() + borderStarts[j], end, i) != end){
					border[i] = j;
				} 
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
	
	int max_( vector<int> & border_ ){
		int max = -1;
		for(unsigned int i = 0 ; i < border_.size(); i++){
			if(border_[i] > max){
				max = border_[i];
			}
		}
		return max;
	}
};