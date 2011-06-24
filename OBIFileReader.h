#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "tuple3.h"

using namespace std;

class OBIFileReader
{
	vector<tuple3f> vertices;
	vector<tuple3f> normals;
	vector<tuple3i> faces;
	vector<tuple3i> faces_normals;
	vector<tuple3i> faces_textures;
	vector<tuple3f> tex;

public:
	OBIFileReader();
	~OBIFileReader(void);

	void parse(const char * file);
	vector<tuple3f> getVertices(){return vertices;}
	vector<tuple3f> getNormals(){return normals;}
	vector<tuple3f> getTex(){return tex;}
	vector<tuple3i> getFaces(){return faces;}
	vector<tuple3i> getFaceNormals(){
		return faces_normals;}
	vector<tuple3i> getFaceTextures(){
		return faces_textures;}

	void checkAllVerticesUsed(void);
	//int hasVertex(void);
	//int hasFace(void);
	
	//void operator >>(Vertex * other);
	//void operator >>(Face * other);

};
