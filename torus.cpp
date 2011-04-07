#include "StdAfx.h"
#include "torus.h"
#include "ball.h"
#include "tuple3.h"
#include <vector>
#include <iostream>
#include <math.h>
#ifndef PI
#define PI 3.14159265f
#endif


torus::torus(float r1, float r2,int nrVertices_smallRing)
{
	int N = nrVertices_smallRing; //elements on small circle
	int M = 5;
	int n = N;
	int m = M;
	int lastRing_startIndex, actualRing_startIndex, lastRing_size, actualRing_size, firstRing_size;
	float x,y,z, psi, phi, temp_r, minDelta, delta;
//	float r1 = 2;
//	float r2 = 1.f;
	tuple3f temp_normal;

	minDelta = 2.f/n * PI * 4 /3;

	//first ring
	z = r2;
	temp_r = r1;
	m = M;
	delta = temp_r * 2.f/m * PI;
	while(delta > minDelta){
		m = 2*m;
		delta = delta / 2;
	}
	for(int j = 0; j < m; j++){
		phi = 2.f/m * j * PI;
		y = temp_r * cos(phi);
		x = temp_r * sin(phi);
		vertices.push_back(tuple3f(x,y,z));
		temp_normal = (tuple3f(x,y,z) - tuple3f(r1* sin(phi),r1*cos(phi), 0));
		temp_normal.normalize();
		normals.push_back(temp_normal);
	}

	lastRing_size = firstRing_size = m;
	lastRing_startIndex = 0;
	

	//the other rings
	for(int i = 1; i < n; i++){

		psi = 2.f/n *i * PI;
		z = cos(psi) * r2;
		
		temp_r = r1 + sin(psi);		
		m = M;
		delta = temp_r * 2.f/m * PI;
		while(delta > minDelta){
			m = 2*m;
			delta = delta / 2;
		}
		
		actualRing_size = m;
		actualRing_startIndex = vertices.size();

		//vertices and normals
		for(int j = 0; j < m; j++){
			phi = 2.f/m * j * PI;
			y = temp_r * cos(phi);
			x = temp_r * sin(phi);
			vertices.push_back(tuple3f(x,y,z));
			temp_normal = (tuple3f(x,y,z) - tuple3f(r1* sin(phi),r1*cos(phi), 0));
			temp_normal.normalize();
			normals.push_back(temp_normal);
		}

		//faces

		if(lastRing_size == 2* actualRing_size){
			for(int k = 0; k < actualRing_size; k++){
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + 2*k,
					lastRing_startIndex + 2*k+1
				));
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + 2*k+1,
					actualRing_startIndex + (k+1)%actualRing_size
					));
				faces.push_back(tuple3i(
					lastRing_startIndex + 2*k+1,
					actualRing_startIndex + (k+1)%actualRing_size,
					lastRing_startIndex + (2*k+2)%lastRing_size
					));
			}
		}
		else if(2*lastRing_size == actualRing_size){
			for(int k = 0; k < lastRing_size; k++){
				faces.push_back(tuple3i(
					actualRing_startIndex + 2*k,
					lastRing_startIndex + k,
					actualRing_startIndex + 2*k+1
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + 2*k+1,
					lastRing_startIndex + k,
					lastRing_startIndex + (k+1)%lastRing_size
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + 2*k+1,
					lastRing_startIndex + (k+1)%lastRing_size,
					actualRing_startIndex + (2*k+2)%actualRing_size
					));
			}
		}
		else if(lastRing_size == actualRing_size){
			for(int k = 0; k < lastRing_size; k++){
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + k,
					lastRing_startIndex + (k+1)%lastRing_size
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + (k+1)%lastRing_size,
					actualRing_startIndex + (k+1)%actualRing_size
					));
			}
		}


		lastRing_size = actualRing_size;
		lastRing_startIndex = actualRing_startIndex;
	}


	//faces end

	if(2*lastRing_size == firstRing_size){
		for(int k = 0; k < lastRing_size; k++){
			faces.push_back(tuple3i(
				0 + 2*k, //firstRing_startIndex  = 0
				lastRing_startIndex + k,
				0 + 2*k+1
				));
			faces.push_back(tuple3i(
				0+ 2*k+1,
				lastRing_startIndex + k,
				lastRing_startIndex + (k+1)%lastRing_size
				));
			faces.push_back(tuple3i(
				0 + 2*k+1,
				lastRing_startIndex + (k+1)%lastRing_size,
				0 + (2*k+2)%firstRing_size
				));
		}
	}
	else if(lastRing_size == firstRing_size){
		for(int k = 0; k < lastRing_size; k++){
			faces.push_back(tuple3i(
				0+ k,
				lastRing_startIndex + k,
				lastRing_startIndex + (k+1)%lastRing_size
				));
			faces.push_back(tuple3i(
				0 + k,
				lastRing_startIndex + (k+1)%lastRing_size,
				0 + (k+1)%firstRing_size
				));
		}
	}

	face_normals_perVertex = faces;
	initFaceNormals();
}

torus::torus(float r1, float r2,int nrVertices_smallRing,int nrVertices_BigRing)
{
	int N = nrVertices_smallRing; //elements on small circle
	int M = nrVertices_BigRing;
	int n = N;
	int m = M;
	int lastRing_startIndex, actualRing_startIndex, lastRing_size, actualRing_size, firstRing_size;
	float x,y,z, psi, phi, temp_r;
	//	float r1 = 2;
	//	float r2 = 1.f;
	tuple3f temp_normal;

	//first ring
	z = r2;
	temp_r = r1;

	for(int j = 0; j < m; j++){
		phi = 2.f/m * j * PI;
		y = temp_r * cos(phi);
		x = temp_r * sin(phi);
		vertices.push_back(tuple3f(x,y,z));
		temp_normal = (tuple3f(x,y,z) - tuple3f(r1* sin(phi),r1*cos(phi), 0));
		temp_normal.normalize();
		normals.push_back(temp_normal);
	}

	lastRing_size = firstRing_size = m;
	lastRing_startIndex = 0;


	//the other rings
	for(int i = 1; i < n; i++){

		psi = 2.f/n *i * PI;
		z = cos(psi) * r2;

		temp_r = r1 + sin(psi);		

		actualRing_size = m;
		actualRing_startIndex = vertices.size();

		//vertices and normals
		for(int j = 0; j < m; j++){
			phi = 2.f/m * j * PI;
			y = temp_r * cos(phi);
			x = temp_r * sin(phi);
			vertices.push_back(tuple3f(x,y,z));
			temp_normal = (tuple3f(x,y,z) - tuple3f(r1* sin(phi),r1*cos(phi), 0));
			temp_normal.normalize();
			normals.push_back(temp_normal);
		}

		//faces

		if(lastRing_size == 2* actualRing_size){
			for(int k = 0; k < actualRing_size; k++){
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + 2*k,
					lastRing_startIndex + 2*k+1
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + 2*k+1,
					actualRing_startIndex + (k+1)%actualRing_size
					));
				faces.push_back(tuple3i(
					lastRing_startIndex + 2*k+1,
					actualRing_startIndex + (k+1)%actualRing_size,
					lastRing_startIndex + (2*k+2)%lastRing_size
					));
			}
		}
		else if(2*lastRing_size == actualRing_size){
			for(int k = 0; k < lastRing_size; k++){
				faces.push_back(tuple3i(
					actualRing_startIndex + 2*k,
					lastRing_startIndex + k,
					actualRing_startIndex + 2*k+1
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + 2*k+1,
					lastRing_startIndex + k,
					lastRing_startIndex + (k+1)%lastRing_size
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + 2*k+1,
					lastRing_startIndex + (k+1)%lastRing_size,
					actualRing_startIndex + (2*k+2)%actualRing_size
					));
			}
		}
		else if(lastRing_size == actualRing_size){
			for(int k = 0; k < lastRing_size; k++){
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + k,
					lastRing_startIndex + (k+1)%lastRing_size
					));
				faces.push_back(tuple3i(
					actualRing_startIndex + k,
					lastRing_startIndex + (k+1)%lastRing_size,
					actualRing_startIndex + (k+1)%actualRing_size
					));
			}
		}


		lastRing_size = actualRing_size;
		lastRing_startIndex = actualRing_startIndex;
	}


	//faces end

	if(2*lastRing_size == firstRing_size){
		for(int k = 0; k < lastRing_size; k++){
			faces.push_back(tuple3i(
				0 + 2*k, //firstRing_startIndex  = 0
				lastRing_startIndex + k,
				0 + 2*k+1
				));
			faces.push_back(tuple3i(
				0+ 2*k+1,
				lastRing_startIndex + k,
				lastRing_startIndex + (k+1)%lastRing_size
				));
			faces.push_back(tuple3i(
				0 + 2*k+1,
				lastRing_startIndex + (k+1)%lastRing_size,
				0 + (2*k+2)%firstRing_size
				));
		}
	}
	else if(lastRing_size == firstRing_size){
		for(int k = 0; k < lastRing_size; k++){
			faces.push_back(tuple3i(
				0+ k,
				lastRing_startIndex + k,
				lastRing_startIndex + (k+1)%lastRing_size
				));
			faces.push_back(tuple3i(
				0 + k,
				lastRing_startIndex + (k+1)%lastRing_size,
				0 + (k+1)%firstRing_size
				));
		}
	}

	face_normals_perVertex = faces;
	initFaceNormals();
}

torus::~torus(void)
{
}
