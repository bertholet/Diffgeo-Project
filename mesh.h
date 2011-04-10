#pragma once
#include "matrixf.h"
#include <vector>
#include "tuple3.h"
#include "colorMap.h"
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

class mesh
{
protected:
	matrixf position;
	matrixf rotation;
		//the normals at the vertice positions
	vector<tuple3f> normals;
	//the normals to the faces.
	vector<tuple3f> face_normals;
	//the indices of the normals at the vertex points of the faces
	vector<tuple3i> face_normals_perVertex; 
	vector<tuple3f> tex;
	tuple3f color;
public:
	vector<tuple3f> vertices;
	//index of the vertices of a face
	vector<tuple3i> faces; 

	mesh(void);
	mesh(const char* file);
	mesh(const char* file, tuple3f col);
	mesh(const char* file, tuple3f col, float scale);
	~mesh(void);

	void initNormalsFromVertices();
	void initFaceNormals();

	void addNormalNoise( float max );
	void scaleVertices(float scale);

	vector<tuple3f>& getVertices(void){return vertices;}
	vector<tuple3i>& getFaces(){return faces;}
	
	void glDisplayVertices(void);
	void glDisplayLines(void);
	void glDisplay(void);
	void glDisplay(colorMap & cMap);
	void addUniformNoise(float sigma);
	tuple3f intensities( unsigned int faceNr, tuple3f &direction );
	tuple3f intensitiesFlat( unsigned int faceNr, tuple3f &direction );

};
