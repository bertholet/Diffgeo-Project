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
	matrixf lighTransform;
		//the normals at the vertice positions
	vector<tuple3f> normals;
	//the normals to the faces.
	vector<tuple3f> face_normals;
	//the indices of the normals at the vertex points of the faces
	vector<tuple3i> face_normals_perVertex, face_tex; 
	vector<tuple3f> tex;

	// nbrs[i] is a vector with the indices of the neighbors
	vector<vector<int>> nbrs;
	vector<vector<int>> nbr_fcs;
	tuple3f color;
	bool showOrientation;
public:
	vector<tuple3f> vertices;
	//index of the vertices of a face
	vector<tuple3i> faces; 

	mesh(void);
	mesh(const char* file);
	mesh(const char* file, tuple3f col);
	mesh(const char* file, tuple3f col, float scale);
	~mesh(void);

	void reset(vector<tuple3f> & _vertices, vector<tuple3i> & _faces);
	void initNormalsFromVertices();
	void initFaceNormals();

	void addNormalNoise( float max );
	void scaleVertices(float scale);

	void rotX(float phi);
	void rotY( float phi );
	void scaleXYZ(float scale );
	void normalize(void);

	vector<tuple3f>& getVertices(void){return vertices;}
	vector<tuple3i>& getFaces(){return faces;}
	vector<vector<int>> & getNeighbors() { return nbrs;}
	vector<vector<int>> & getNeighborFaces(){return nbr_fcs;}
	vector<tuple3f> & getTexCoords(){return tex;}
	
	void glDisplayVertices(void);
	void glDisplayLines(void);
	void glDisplay(void);
	void glDisplay(colorMap & cMap);
	void glTexDisplay(void);
	void addUniformNoise(float sigma);
	tuple3f intensities( unsigned int faceNr, tuple3f &direction );
	tuple3f intensitiesFlat( unsigned int faceNr, tuple3f &direction );
	void setTextures_perVertex( double * x, double * y );
	void setTextures_perVertex( double * xy);
	void setTextures_perVertex( vector<tuple3f> & textures );

	void setShowOrientation(bool b){
		showOrientation = b;
	}
	void setPosition( tuple3f &pos );

private:
	void init( const char* file, tuple3f & col, float scale );
};
