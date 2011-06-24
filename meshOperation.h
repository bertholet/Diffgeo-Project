#pragma once
#include "mesh.h"
#include "tuple3.h"
#include <vector>
#include <algorithm>

class meshOperation
{
public:
	meshOperation(void);
	~meshOperation(void);

	static void getHalf(mesh & m, mesh & target, tuple3f direction, float dist){
		vector<int> usedVertices;
		vector<tuple3i> faces;
		vector<tuple3f> vertices;

		for(int i = 0; i < m.faces.size(); i++){

			if(((tuple3f) m.vertices[m.faces[i].a]).dot(direction)>dist ||
				((tuple3f) m.vertices[m.faces[i].b]).dot(direction)>dist||
				((tuple3f) m.vertices[m.faces[i].c]).dot(direction)>dist){
				usedVertices.push_back(m.faces[i].a);
				usedVertices.push_back(m.faces[i].b);
				usedVertices.push_back(m.faces[i].c);
				
				faces.push_back(m.faces[i]);
			}
		}

		vector<int>::iterator it;

		// using default comparison (operator <):
		std::sort (usedVertices.begin(), usedVertices.end());
		usedVertices.erase(std::unique(usedVertices.begin(), usedVertices.end()), usedVertices.end());

		int k;
		for(unsigned int i = 0; i < faces.size(); i++){
			k= (std::find(usedVertices.begin(), usedVertices.end(), faces[i].a)-usedVertices.begin());
			faces[i].a = k;
			k= (std::find(usedVertices.begin(), usedVertices.end(), faces[i].b)-usedVertices.begin());
			faces[i].b = k;
			k= (std::find(usedVertices.begin(), usedVertices.end(), faces[i].c)-usedVertices.begin());
			faces[i].c = k;

		}

		for(int i = 0; i < usedVertices.size(); i++){
			vertices.push_back(m.vertices[usedVertices[i]]);
		}

		target.faces = faces;
		target.vertices = vertices;
		target.initNormalsFromVertices();

	}
};
