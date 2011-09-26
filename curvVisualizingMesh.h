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
	borderColorMap(vector<vector<int>> & border_, tuple3f color1, tuple3f color2){
		sz = max_(border_)+1;
		//vector<int>::iterator it = border_.begin();
		components = border_.size();
		printf("%d border components found \n", components);

		border = new int[sz];
		
		for(int i = 0; i < sz; i++){
			border[i] = -1; //border[i] = false;
			for(int j = 0; j < (int) border_.size(); j++){
				if( find(border_[j].begin() , border_[j].end(), i) !=border_[j].end()){
					border[i] = j;
					break;
				} 
			}
		}
		col1 = color1;
		col2 = color2;
	}
	
	borderColorMap(vector<vector<int>> & border_, tuple3f color1, tuple3f color2, vector<vector<float>> & angles){
		sz = max_(border_)+1;
		//vector<int>::iterator it = border_.begin();
		components = border_.size();
		printf("%d border components found \n", components);

		border = new int[sz];

		vector<int>::iterator it;
		for(int i = 0; i < sz; i++){
			border[i] = -1; //border[i] = false;
			for(int j = 0; j < (int) border_.size(); j++){
				if( (it = find(border_[j].begin() , border_[j].end(), i)) !=border_[j].end()){
					if(angles[j][it-border_[j].begin()] >0){
						border[i] = 2*j +1;
					}
					else{
						border[i] = 2*j;
					}
					break;
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
	
	int max_( vector<vector<int>> & border_ ){
		int max = -1;
		for(unsigned int i = 0 ; i < border_.size(); i++){
			for(unsigned int j = 0 ; j < border_[i].size(); j++){
				if(border_[i][j] > max){
					max = border_[i][j];
				}
			}
		}
		return max;
	}
};