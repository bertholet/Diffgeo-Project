#pragma once
#include "mesh.h"

class simplestCube :
	public mesh
{
public:
	simplestCube(void){
		vertices.push_back(tuple3f(-0.5f,0-0.5f,0-0.5f));
		vertices.push_back(tuple3f(-0.5f,-0.5f,0.5f));
		vertices.push_back(tuple3f(-0.5f,0.5f,0.5f));
		vertices.push_back(tuple3f(-0.5f,0.5f,-0.5f));
		vertices.push_back(tuple3f(0.5f,-0.5f,-0.5f));
		vertices.push_back(tuple3f(0.5f,-0.5f,0.5f));
		vertices.push_back(tuple3f(0.5f,0.5f,0.5f));
		vertices.push_back(tuple3f(0.5f,0.5f,-0.5f));

		faces.push_back(tuple3i(0,2,1));
		faces.push_back(tuple3i(0,3,2));

		faces.push_back(tuple3i(0,1,5));
		faces.push_back(tuple3i(0,5,4));

		faces.push_back(tuple3i(0,4,7));
		faces.push_back(tuple3i(0,7,3));

		faces.push_back(tuple3i(4,5,6));
		faces.push_back(tuple3i(4,6,7));

		faces.push_back(tuple3i(1,2,6));
		faces.push_back(tuple3i(1,6,5));

		faces.push_back(tuple3i(3,6,2));
		faces.push_back(tuple3i(3,7,6));

		normals = vertices;
		for(int i = 0; i < 8; i++){
			normals[i].normalize();
		}

		face_normals_perVertex = faces;
		initFaceNormals();
	}
	~simplestCube(void){;}
};
