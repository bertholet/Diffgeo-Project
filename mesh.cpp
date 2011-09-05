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
	lighTransform = matrixFactory::id();
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
	lighTransform = matrixFactory::id();
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
	face_tex = f.getFaceTextures();

	position = matrixFactory::translate(0,0,-5);
	rotation = matrixFactory::id();
	lighTransform = matrixFactory::id();
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
	face_tex = f.getFaceTextures();

	position = matrixFactory::scale(scale) * matrixFactory::translate(0,0,-5);
	rotation = matrixFactory::id();
	lighTransform = matrixFactory::id();
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

void mesh::addNormalNoise( float max )
{
	static float const alpha = 0.25;
	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i] += normals[i] *( ((float)rand())/RAND_MAX * max );
	}
}

void mesh::scaleVertices( float scale )
{
	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i] *= scale;
	}
}

void mesh::rotY( float f )
{
	matrixf rot = matrixFactory::rotateX(f);
	rotation = rotation*rot;
	lighTransform = rot.transpose()*lighTransform;
}

void mesh::rotX(float f)
{
	matrixf rot = matrixFactory::rotateY(f);
	rotation = rotation*rot;
	lighTransform = rot.transpose()*lighTransform;
}

void mesh::scaleXYZ(float f)
{
	matrixf rot = matrixFactory::scale(f);
	rotation = rotation*rot;
	//lighTransform = rot.transpose()*lighTransform;
}


void mesh::glDisplayVertices( void )
{
	//matrixf rot = matrixFactory::rotateY(0.00101f);

	//position = /*rot2*/rot*position;

	glLoadMatrixf((GLfloat *) &(rotation*position)); 

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
	//matrixf rot = matrixFactory::rotateY(0.00101f);

	//position = /*rot2*/rot*position;

	glLoadMatrixf((GLfloat *) &(rotation*position)); 

	glColor3f(1.f,1.f,1.f);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		//glVertex3fv( (GLfloat *) & vertices[i]);
		glVertex3fv( (GLfloat *) & vertices[faces[i].a]);

		glVertex3fv((GLfloat *) & vertices[faces[i].b]);

		glVertex3fv((GLfloat *) & vertices[faces[i].c]);
		glEnd();
	}
}


DirectionalLight l(tuple3f(0,0,-1), tuple3f(1,1,1));
void mesh::glDisplay( void )
{
//	matrixf rot = matrixFactory::rotateY(0.00101f);
//	matrixf rot2 = matrixFactory::rotateX(0.00253f);
//	matrixf rot3 = matrixFactory::rotateZ(0.0017f);
	//matrixf t = matrixFactory::translate(0,0,-0.01);
	tuple3f normal;
	tuple3f localLightDir;
	tuple3f c;

//	position = rot2*rot*position;
	//l.direction = rot.transpose()*rot2.transpose()*l.direction;
	localLightDir = lighTransform * l.direction;

	glLoadMatrixf((GLfloat *) &(rotation*position)); 
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++)
		{
			//normal = tuple3f::cross(vertices[faces[i].x] - vertices[faces[i].y], vertices[faces[i].z] - vertices[faces[i].y]);
			//normal.normalize();
			//cout << "faces[i] :" << vertices[faces[i].x].x<< "," << vertices[faces[i].x].y << "," << vertices[faces[i].x].z << "\n";
			//glVertex3f( vertices[faces[i].x].x, vertices[faces[i].x].y, vertices[faces[i].x].z);
			//c= intensities(i, l.direction);
			//c= intensitiesFlat(i, l.direction);
			c= intensitiesFlat(i, localLightDir);
			

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
//	matrixf rot = matrixFactory::rotateY(0.00101f);
//	matrixf rot2 = matrixFactory::rotateX(0.00253f);
//	matrixf rot3 = matrixFactory::rotateZ(0.0017f);
	//matrixf t = matrixFactory::translate(0,0,-0.01);
	tuple3f normal;
	tuple3f c;
	tuple3f localLightDir;

	//position = /*rot2*rot*/position;
//	l.direction = rot.transpose()/*rot2.transpose()*/*l.direction;

	localLightDir = lighTransform * l.direction;

	tuple3f color;
	glLoadMatrixf((GLfloat *) &(rotation*position)); 
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
//		c= intensitiesFlat(i, l.direction);
		c= intensitiesFlat(i, localLightDir);
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

void mesh::glTexDisplay(void){
	tuple3f normal;
	tuple3f c;
	tuple3f localLightDir;

	//position = /*rot2*rot*/position;
	//	l.direction = rot.transpose()/*rot2.transpose()*/*l.direction;

	localLightDir = lighTransform * l.direction;

	tuple3f color(0.8f,0.8f,0.8f);
//	tuple3i temp;
	glLoadMatrixf((GLfloat *) &(rotation*position)); 
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
//		temp = face_tex[i];

		c= intensitiesFlat(i, localLightDir);
		glColor3fv((GLfloat *) &c);
		glTexCoord2fv((GLfloat *) &tex[face_tex[i].a]);
		glVertex3fv( (GLfloat *) & vertices[faces[i].a]);

		glColor3fv((GLfloat *) &c);
		glTexCoord2fv((GLfloat *) &tex[face_tex[i].b]);
		glVertex3fv((GLfloat *) & vertices[faces[i].b]);

		glColor3fv((GLfloat *) &c);
		glTexCoord2fv((GLfloat *) &tex[face_tex[i].c]);
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
	tuple3f temp_normal = tuple3f::cross(vertices[faces[faceNr].a] - vertices[faces[faceNr].b], vertices[faces[faceNr].c] - vertices[faces[faceNr].b]);
	temp_normal.normalize();
	//float c = face_normals[faceNr].dot(direction);
	float c = temp_normal.dot(direction);
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

void mesh::normalize( void )
{
	float maxx= vertices[0].x, minx = vertices[0].x,
		 maxy= vertices[0].y, miny = vertices[0].y,
		  maxz= vertices[0].z, minz = vertices[0].z;

	for(vector<tuple3f>::iterator it = vertices.begin(); it != vertices.end(); it++){
		if(it->x > maxx){
			maxx = it->x;
		}
		if(it->y > maxy){
			maxy = it->y;
		}
		if(it->z > maxz){
			maxz = it->z;
		}
		if(it->x < minx){
			minx = it->x;
		}
		if(it->y < miny){
			miny = it->y;
		}
		if(it->z < minz){
			minz = it->z;
		}
	}

	float max = (maxx>maxy? maxx:maxy);
	max = (max>maxz?max:maxz);
	float min = (minx < miny?minx:miny);
	min = (min < minz?min:minz);

	float scale = 2.f/(max - min);
	/*tuple3f translation((-maxx + minx)/2,(-maxy + miny)/2,(-maxz + minz)/2);
	for(vector<tuple3f>::iterator it = vertices.begin(); it != vertices.end(); it++){
		(*it)+=translation;
	}*/
	scaleVertices(scale);


}

void mesh::setTextures_perVertex( double * x, double * y )
{
	tex.clear();
	tex.reserve(vertices.size());

	for(int i = 0; i < vertices.size(); i++){
		tex.push_back(tuple3f((float)x[i], (float)y[i],0.f));
	}

	this->face_tex = faces;
}







