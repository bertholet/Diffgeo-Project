#include "StdAfx.h"
#include "mesh.h"
#include "OBIFileReader.h"
#include "matrixFactory.h"
#include "DirectionalLight.h"
#include "Operator.h"



mesh::mesh(void)
{
	position = matrixFactory::translate(0,0,-5);
	rotation = matrixFactory::id();
	color.set(1.f,1.f,1.f);
}

mesh::mesh( const char* file )
{
	OBIFileReader f;
	f.parse(file);
	vertices = f.getVertices();
	faces = f.getFaces();

	if(f.getNormals().size() != 0){
		normals = f.getNormals();
		face_normals_perVertex = f.getFaceNormals();
	}
	else{
		initNormalsFromVertices();
	}

	initFaceNormals();

	tex = f.getTex();

	position = matrixFactory::translate(0,0,-5);
	rotation = matrixFactory::id();
	color.set(1.f,1.f,1.f);
}

mesh::mesh( const char* file, tuple3f col )
{
	OBIFileReader f;
	f.parse(file);
	vertices = f.getVertices();
	faces = f.getFaces();

	if(f.getNormals().size() != 0){
		normals = f.getNormals();
		face_normals_perVertex = f.getFaceNormals();
	}
	else{
		initNormalsFromVertices();
	}
	initFaceNormals();

	tex = f.getTex();

	position = matrixFactory::translate(0,0,-5);
	rotation = matrixFactory::id();
	color = col;
}

mesh::mesh( const char* file, tuple3f col, float scale )
{
	OBIFileReader f;
	f.parse(file);
	vertices = f.getVertices();
	faces = f.getFaces();

	if(f.getNormals().size() != 0){
		normals = f.getNormals();
		face_normals_perVertex = f.getFaceNormals();
	}
	else{
		initNormalsFromVertices();
	}
	initFaceNormals();

	tex = f.getTex();

	position = matrixFactory::scale(scale) * matrixFactory::translate(0,0,-5);
	rotation = matrixFactory::id();

	color = col;
}

mesh::~mesh(void)
{

}

void mesh::initNormalsFromVertices()
{
	cout << ">> Normals are generated, no normals read!\n";
	//vector<int> nrFaces(vertices.size());
	normals.clear();
	normals.reserve(vertices.size());
	normals.assign(vertices.size(), tuple3f());

	tuple3f temp_normal;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		temp_normal = tuple3f::cross(vertices[faces[i].a] - vertices[faces[i].b], vertices[faces[i].c] - vertices[faces[i].b]);
		temp_normal.normalize();
		normals[faces[i].a] += temp_normal;
		normals[faces[i].b] += temp_normal;
		normals[faces[i].c] += temp_normal;
	}

	for(unsigned int i = 0; i < normals.size(); i++){
		normals[i].normalize();
	}


	face_normals_perVertex = faces;
}

void mesh::initFaceNormals()
{
	face_normals.clear();
	face_normals.reserve(faces.size());

	tuple3f temp_normal;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		temp_normal = tuple3f::cross(vertices[faces[i].a] - vertices[faces[i].b], vertices[faces[i].c] - vertices[faces[i].b]);
		temp_normal.normalize();
		face_normals.push_back(temp_normal);
	}
}



void mesh::glDisplayVertices( void )
{
	matrixf rot = matrixFactory::rotateY(0.00101f);

	position = /*rot2*/rot*position;

	glLoadMatrixf((GLfloat *) &position); 

	glBegin(GL_POINTS);
	glColor3f(1.f,1.f,1.f);
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		glVertex3fv( (GLfloat *) & vertices[i]);

	}
	glEnd();
}

void mesh::glDisplayLines( void )
{
	matrixf rot = matrixFactory::rotateY(0.00101f);

	position = /*rot2*/rot*position;

	glLoadMatrixf((GLfloat *) &position); 

	glBegin(GL_LINES);
	glColor3f(1.f,1.f,1.f);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//glVertex3fv( (GLfloat *) & vertices[i]);
		glVertex3fv( (GLfloat *) & vertices[faces[i].a]);

		glVertex3fv((GLfloat *) & vertices[faces[i].b]);

		glVertex3fv((GLfloat *) & vertices[faces[i].c]);

	}
	glEnd();
}


DirectionalLight l(tuple3f(0,0,-1), tuple3f(1,1,1));
void mesh::glDisplay( void )
{
	matrixf rot = matrixFactory::rotateY(0.00101f);
	matrixf rot2 = matrixFactory::rotateX(0.00253f);
	matrixf rot3 = matrixFactory::rotateZ(0.0017f);
	//matrixf t = matrixFactory::translate(0,0,-0.01);
	tuple3f normal;
	tuple3f c;

	position = /*rot2*/rot*position;
	l.direction = rot.transpose()/*rot2.transpose()*/*l.direction;

	glLoadMatrixf((GLfloat *) &position); 
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++)
		{
			//normal = tuple3f::cross(vertices[faces[i].x] - vertices[faces[i].y], vertices[faces[i].z] - vertices[faces[i].y]);
			//normal.normalize();
			//cout << "faces[i] :" << vertices[faces[i].x].x<< "," << vertices[faces[i].x].y << "," << vertices[faces[i].x].z << "\n";
			//glVertex3f( vertices[faces[i].x].x, vertices[faces[i].x].y, vertices[faces[i].x].z);
			//c= intensities(i, l.direction);
			c= intensitiesFlat(i, l.direction);
			

			glColor3f(c.x*color.x,c.x*color.y,c.x*color.z);
			glVertex3fv( (GLfloat *) & vertices[faces[i].a]);

			//c= normals[faces_normals[i].y].dot(l.direction);
			glColor3f(c.y*color.x,c.y*color.y,c.y*color.z);
			glVertex3fv((GLfloat *) & vertices[faces[i].b]);

			//c= normals[faces_normals[i].z].dot(l.direction);
			glColor3f(c.z*color.x,c.z*color.y,c.z*color.z);
			glVertex3fv((GLfloat *) & vertices[faces[i].c]);

		}
	glEnd();
/*	glColor3f(1.f,1.f,1.f);
	glBegin(GL_LINES);
	for(unsigned int i = 0; i <faces.size(); i++){
		glVertex3fv((GLfloat *)&vertices[faces[i].x]);
		glVertex3fv((GLfloat *) & (vertices[faces[i].x]+ normals[face_normals_perVertex[i].x]));
	}
	glEnd();*/
}

void mesh::glDisplay( colorMap & cMap )
{
	matrixf rot = matrixFactory::rotateY(0.00101f);
	matrixf rot2 = matrixFactory::rotateX(0.00253f);
	matrixf rot3 = matrixFactory::rotateZ(0.0017f);
	//matrixf t = matrixFactory::translate(0,0,-0.01);
	tuple3f normal;
	tuple3f c;

	position = /*rot2*/rot*position;
	l.direction = rot.transpose()/*rot2.transpose()*/*l.direction;

	tuple3f color;
	glLoadMatrixf((GLfloat *) &position); 
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		c= intensitiesFlat(i, l.direction);

		color = cMap.color(faces[i].a);
		glColor3f(c.x*color.x,c.x*color.y,c.x*color.z);
		glVertex3fv( (GLfloat *) & vertices[faces[i].a]);

		color = cMap.color(faces[i].b);
		glColor3f(c.y*color.x,c.y*color.y,c.y*color.z);
		glVertex3fv((GLfloat *) & vertices[faces[i].b]);

		color = cMap.color(faces[i].c);
		glColor3f(c.z*color.x,c.z*color.y,c.z*color.z);
		glVertex3fv((GLfloat *) & vertices[faces[i].c]);

	}
	glEnd();

}

tuple3f mesh::intensities( unsigned int faceNr, tuple3f &direction )
{
	float t1= normals[face_normals_perVertex[faceNr].a].dot(direction), 
		t2=normals[face_normals_perVertex[faceNr].b].dot(direction), 
		t3=normals[face_normals_perVertex[faceNr].c].dot(direction);
	return tuple3f(
		(t1>0? t1:-t1),
		(t2>0? t2:-t2),
		(t3>0? t3:-t3));
}

tuple3f mesh::intensitiesFlat( unsigned int faceNr, tuple3f &direction )
{
	float c = face_normals[faceNr].dot(direction);
	c= ( c>0? c: -c);
	return tuple3f(c,c,c);
}

void mesh::addUniformNoise( float max )
{
	tuple3f randm;
	for(unsigned int i = 0; i < vertices.size(); i++){
		randm = tuple3f(((float)rand())/RAND_MAX *max,((float)rand())/RAND_MAX * max,((float)rand())/RAND_MAX * max);
		vertices[i]+= randm;
	}
}

void mesh::addNormalNoise( float max )
{
	static float const alpha = 0.25;
	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i] += normals[i] *( ((float)rand())/RAND_MAX * max );
	}
}




