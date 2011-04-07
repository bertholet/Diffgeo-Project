#include "StdAfx.h"
#include "Operator.h"
#include <iostream>
#include <math.h>

Operator::Operator(void)
{
}

Operator::~Operator(void)
{
}

void Operator::calcAllCurvNormals( mesh &m, vector<tuple3f> &K )
{
	//K is the mean curv normal per vertex
	K.clear();
	K.reserve(m.getVertices().size());
	K.assign(m.getVertices().size(), tuple3f(0,0,0));
	vector<float> AMixed(m.getVertices().size());
	AMixed.assign(m.getVertices().size(), 0.f);
	vector<tuple3i>& faces = m.getFaces();
	vector<tuple3f>& vertices = m.getVertices();

	//for each face
	float cot1, cot2, cot3, norm1sqr, norm2sqr, norm3sqr, temp, areaT;
	int nrFaces = faces.size();
	for(int i = 0; i < nrFaces; i++){
		cot1 = tuple3f::cotPoints(vertices[faces[i].c],vertices[faces[i].a],vertices[faces[i].b]);//cot(c,a,b)
		cot2 = tuple3f::cotPoints(vertices[faces[i].a],vertices[faces[i].b],vertices[faces[i].c]);//cot(a,b,c)
		cot3 = tuple3f::cotPoints(vertices[faces[i].b],vertices[faces[i].c],vertices[faces[i].a]);//cot(b,c,a)

		//norm1sqr = (vertices[faces[i].z] -vertices[faces[i].y]).normSqr();
		K[faces[i].c] += (vertices[faces[i].b] - vertices[faces[i].c]) * cot1;
		K[faces[i].b] += (vertices[faces[i].c] - vertices[faces[i].b]) * cot1;
		
		K[faces[i].c] += (vertices[faces[i].a] - vertices[faces[i].c]) * cot2;
		K[faces[i].a] += (vertices[faces[i].c] - vertices[faces[i].a]) * cot2;
		
		K[faces[i].b] += (vertices[faces[i].a] - vertices[faces[i].b]) * cot3;
		K[faces[i].a] += (vertices[faces[i].b] - vertices[faces[i].a]) * cot3;

		areaT = tuple3f::cross((vertices[faces[i].a] - vertices[faces[i].b]),(vertices[faces[i].c] - vertices[faces[i].b])).norm()/2;
		if(tuple3f::a_bDotc_b(vertices[faces[i].c],vertices[faces[i].a],vertices[faces[i].b])< 0) //triangle obtuse at a
		{
			AMixed[faces[i].a]+=areaT/2;  
			AMixed[faces[i].b]+=areaT/4; 
			AMixed[faces[i].c] +=areaT/4;
		}
		else if(tuple3f::a_bDotc_b(vertices[faces[i].a],vertices[faces[i].b],vertices[faces[i].c])< 0)
		{
			AMixed[faces[i].a]+=areaT/4;  
			AMixed[faces[i].b]+=areaT/2; 
			AMixed[faces[i].c] +=areaT/4;
		}
		else if(tuple3f::a_bDotc_b(vertices[faces[i].b],vertices[faces[i].c],vertices[faces[i].a]) < 0)
		{
			AMixed[faces[i].a]+=areaT/4;  
			AMixed[faces[i].b]+=areaT/4; 
			AMixed[faces[i].c] +=areaT/2;
		}
		else //triangle is non obtuse: vornoi area*/
		{
			norm1sqr = (vertices[faces[i].c] - vertices[faces[i].b]).normSqr();
			norm2sqr = (vertices[faces[i].c] - vertices[faces[i].a]).normSqr();
			norm3sqr = (vertices[faces[i].a] - vertices[faces[i].b]).normSqr();
			AMixed[faces[i].c] += 0.125f * cot1 *norm1sqr;
			AMixed[faces[i].c] += 0.125f *cot2 * norm2sqr;
			AMixed[faces[i].a] += 0.125f *cot2 * norm2sqr;
			AMixed[faces[i].a] += 0.125f *cot3 * norm3sqr;
			AMixed[faces[i].b] += 0.125f *cot3 * norm3sqr;
			AMixed[faces[i].b] += 0.125f *cot1 * norm1sqr;
		}
		//target[faces[i].y] += (cot1)
		//target[]
	}
	for(unsigned int i = 0 ; i < vertices.size(); i++){
		temp = 0.5f/AMixed[i];
		K[i] = K[i] *temp;
	}

	float t = 0;
	for(unsigned int i = 0; i < AMixed.size(); i++){
		t+= AMixed[i];
	}
	//cout << "Overall area is : " << t <<"\n";
}

void Operator::calcAllGaussCurvs( mesh &m, vector<float> &target )
{
	vector<tuple3i>& faces = m.getFaces();
	vector<tuple3f>& vertices = m.getVertices();
	vector<float> aMixed;

	target.clear();
	target.reserve(m.getVertices().size());
	target.assign(m.getVertices().size(), 2* PI);
	int nrFaces = m.getFaces().size();

//	float temp;
	for(int i = 0; i < nrFaces; i++){
//		temp = acos(tuple3f::cosPoints(vertices[faces[i].c],vertices[faces[i].a],vertices[faces[i].b]));
		target[faces[i].a] -= acos(tuple3f::cosPoints(vertices[faces[i].c],vertices[faces[i].a],vertices[faces[i].b]));//angle at a
		//temp = acos(tuple3f::cosPoints(vertices[faces[i].c],vertices[faces[i].b],vertices[faces[i].a]));
		target[faces[i].b] -= acos(tuple3f::cosPoints(vertices[faces[i].c],vertices[faces[i].b],vertices[faces[i].a]));
		//temp = acos(tuple3f::cosPoints(vertices[faces[i].a],vertices[faces[i].c],vertices[faces[i].b]));
		target[faces[i].c] -= acos(tuple3f::cosPoints(vertices[faces[i].a],vertices[faces[i].c],vertices[faces[i].b]));
	}

	calcAllAMixed(m,aMixed);

	for(unsigned int i = 0; i < aMixed.size(); i++){
		target[i] /= aMixed[i];
	}

	float temp=0;
	for(unsigned int i = 0 ; i < target.size(); i++){
		temp+= target[i]*aMixed[i];
	}
	cout << " characteristic of surface is: " << temp / 2 / PI << "\n";
	temp=0;
	for(unsigned int i = 0 ; i < target.size(); i++){
		temp+= aMixed[i];
	}
	cout << " the Area is " << temp << "\n";
}

void Operator::calcAllAMixed( mesh &m, vector<float> &AMixed )
{
	vector<tuple3i>& faces = m.getFaces();
	vector<tuple3f>& vertices = m.getVertices();
	float areaT,cot1, cot2, cot3, norm1sqr, norm2sqr, norm3sqr;
	AMixed.reserve(m.getVertices().size());
	AMixed.assign(m.getVertices().size(), 0);
	int nrFaces = m.getFaces().size();
	
	for(int i = 0; i < nrFaces; i++){
		cot1 = tuple3f::cotPoints(vertices[faces[i].c],vertices[faces[i].a],vertices[faces[i].b]);//cot(c,a,b)
		cot2 = tuple3f::cotPoints(vertices[faces[i].a],vertices[faces[i].b],vertices[faces[i].c]);//cot(a,b,c)
		cot3 = tuple3f::cotPoints(vertices[faces[i].b],vertices[faces[i].c],vertices[faces[i].a]);//cot(b,c,a)
		areaT = tuple3f::cross((vertices[faces[i].a] - vertices[faces[i].b]),(vertices[faces[i].c] - vertices[faces[i].b])).norm()/2;
		if(tuple3f::a_bDotc_b(vertices[faces[i].c],vertices[faces[i].a],vertices[faces[i].b])< 0) //triangle obtuse at a
		{
			AMixed[faces[i].a]+=areaT/2;  
			AMixed[faces[i].b]+=areaT/4; 
			AMixed[faces[i].c] +=areaT/4;
		}
		else if(tuple3f::a_bDotc_b(vertices[faces[i].a],vertices[faces[i].b],vertices[faces[i].c])< 0)
		{
			AMixed[faces[i].a]+=areaT/4;  
			AMixed[faces[i].b]+=areaT/2; 
			AMixed[faces[i].c] +=areaT/4;
		}
		else if(tuple3f::a_bDotc_b(vertices[faces[i].b],vertices[faces[i].c],vertices[faces[i].a]) < 0)
		{
			AMixed[faces[i].a]+=areaT/4;  
			AMixed[faces[i].b]+=areaT/4; 
			AMixed[faces[i].c] +=areaT/2;
		}
		else //triangle is non obtuse: vornoi area*/
		{
			norm1sqr = (vertices[faces[i].c] - vertices[faces[i].b]).normSqr();
			norm2sqr = (vertices[faces[i].c] - vertices[faces[i].a]).normSqr();
			norm3sqr = (vertices[faces[i].a] - vertices[faces[i].b]).normSqr();
			AMixed[faces[i].c] += 0.125f * cot1 *norm1sqr;
			AMixed[faces[i].c] += 0.125f *cot2 * norm2sqr;
			AMixed[faces[i].a] += 0.125f *cot2 * norm2sqr;
			AMixed[faces[i].a] += 0.125f *cot3 * norm3sqr;
			AMixed[faces[i].b] += 0.125f *cot3 * norm3sqr;
			AMixed[faces[i].b] += 0.125f *cot1 * norm1sqr;
		}
	}

}

float Operator::volume( mesh &m )
{

	int sz = m.faces.size();
	float volume=0;
	for(int i = 0; i < sz; i++){
		volume += ((m.vertices[m.faces[i].a] + m.vertices[m.faces[i].b] + m.vertices[m.faces[i].c]) * (1.f/3)).dot(
			tuple3f::cross((m.vertices[m.faces[i].b] - m.vertices[m.faces[i].a]),m.vertices[m.faces[i].c] - m.vertices[m.faces[i].a]));
	}
	return volume / 6;
}
