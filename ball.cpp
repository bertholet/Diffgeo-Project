#include "StdAfx.h"
#ifndef PI
#define PI 3.14159265f
#endif
#include <math.h>
#include "ball.h"
#include "tuple3.h"
#include <vector>
#include <iostream>

using namespace std;
ball::ball(float r, int verticesPerHalfCycle)
{
	float x,y, z, delta, phi, psi;
	int N = 6, startIndex_actualRing, startIndex_lastRing, actualRing_size, lastRing_size,n;
	int M = verticesPerHalfCycle;
	float maxDelta = sin(1.f / M * PI) * 2 * PI / N;

	vertices.push_back(tuple3f(0,0,r));
	//lastRing_indices.push_back(0);
	startIndex_lastRing = 0;
	lastRing_size =1;

	for(int i = 1; i <M; i++){
		psi = 1.f / M * i * PI;
		z = r*cos(psi);

		n = N;
		delta = sin(psi)*2*PI /n;
		while(delta >maxDelta){
			delta = delta /2;
			n = 2 * n;
		}

		startIndex_actualRing = vertices.size();
		actualRing_size = n;

		for(int j = 0; j < n; j++)
		{
			phi = 2.f / n * j * PI;
			x = r*sin(psi)*cos(phi);
			y = r*sin(psi)*sin(phi);
			vertices.push_back(tuple3f(x,y,z));
		}

		//update faces;
		if(lastRing_size ==1){
			for(int i = 0; i < actualRing_size; i++){
				faces.push_back(tuple3i(startIndex_lastRing, 
					i+startIndex_actualRing, 
					startIndex_actualRing+(i+1)%(actualRing_size)));
			}
		}
		else if(lastRing_size == 2* actualRing_size ){
			for(int i = 0; i < actualRing_size; i++){
				faces.push_back(tuple3i(startIndex_lastRing + 2*i,
					startIndex_lastRing + (2*i + 1),
					startIndex_actualRing + i));
				faces.push_back(tuple3i(startIndex_actualRing + i,
					startIndex_lastRing + (2*i + 1),
					startIndex_actualRing + (i+1)%actualRing_size));
				faces.push_back(tuple3i(startIndex_lastRing + 2*i+1,
					startIndex_lastRing + (2*i + 2)%lastRing_size,
					startIndex_actualRing + (i+1)%actualRing_size));
			}
		}
		else if (lastRing_size * 2 == actualRing_size){
			for(int i = 0; i < lastRing_size; i++){
				faces.push_back(tuple3i(startIndex_actualRing + 2*i,
					startIndex_lastRing + i,
					startIndex_actualRing + (2*i+1)));
				faces.push_back(tuple3i(startIndex_actualRing + 2*i+1,
					startIndex_lastRing + i,
					startIndex_lastRing + (i+1)%lastRing_size));
				faces.push_back(tuple3i(startIndex_actualRing + 2*i+1,
					startIndex_lastRing + (i+1)%lastRing_size,
					startIndex_actualRing + (2*i+2)%actualRing_size));
			}
		}
		else if(lastRing_size == actualRing_size){
			for(int i = 0; i < lastRing_size; i++){
				faces.push_back(tuple3i(startIndex_lastRing + i,
					startIndex_lastRing + (i+1)%lastRing_size,
					startIndex_actualRing + i));
				faces.push_back(tuple3i(startIndex_lastRing + (i+1)%lastRing_size,
					startIndex_actualRing + i,
					startIndex_actualRing + (i+1)%actualRing_size));
			}
		}
		else{
			throw std::exception("bad programming in ball.cpp, unexpected case happened.");
		}


		//updateLastRingIndices
		startIndex_lastRing = startIndex_actualRing;
		lastRing_size = actualRing_size;
	}
	vertices.push_back(tuple3f(0,0,-r));
	startIndex_actualRing = vertices.size() -1;
	for(int i = 0; i < lastRing_size; i++){
		faces.push_back(tuple3i(startIndex_actualRing, 
			startIndex_lastRing + i,
			startIndex_lastRing + (i+1)%lastRing_size));
	}

	normals = vertices;
	face_normals_perVertex = faces;
	initFaceNormals();
}

ball::~ball(void)
{
}
