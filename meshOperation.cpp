#include "StdAfx.h"
#include "meshOperation.h"

meshOperation::meshOperation(void)
{
}

meshOperation::~meshOperation(void)
{
}

int meshOperation::getPrevious( int center_index, int v, mesh  & m)
{
	vector<tuple3i> & faces = m.getFaces(); 
	vector<int> & neighbor_faces = m.getNeighborFaces()[center_index];
	tuple3i face;
	
	for(vector<int>::iterator it = neighbor_faces.begin(); it != neighbor_faces.end(); it++){
		face = faces[*it];
		if(face.a == v && face.b == center_index){
			return face.c;
		}
		if(face.b == v && face.c == center_index){
			return face.a;
		}
		if(face.c == v && face.a == center_index){
			return face.b;
		}
	}

	return -1;
}

int meshOperation::getNext( int center_idx, int v, mesh  & m )
{
	vector<tuple3i> & faces = m.getFaces(); 
	vector<int> & neighbor_faces = m.getNeighborFaces()[center_idx];
	tuple3i  face;

	for(vector<int>::iterator it = neighbor_faces.begin(); it != neighbor_faces.end(); it++){
		face = faces[*it];
		if(face.a == v && face.c == center_idx){
			return face.b;
		}
		if(face.b == v && face.a == center_idx){
			return face.c;
		}
		if(face.c == v && face.b == center_idx){
			return face.a;
		}
	}

	return -1;
}

float meshOperation::sumAnglesWheel( int from, int at, int to, mesh & m )
{
	vector<int> & nbrs = m.getNeighbors()[at];
	vector<int> & nbr_fcs = m.getNeighborFaces()[at];
	int actual = from, next, lps = 0;
	float angle = 0;

	do{
		next = meshOperation::getNext(at,from, m);
		if(next < 0 || lps > nbrs.size()){
			throw std::runtime_error("Assertion failed at sum Angle Wheel");
		}
		angle += tuple3f::angle( m.vertices[actual], m.vertices[at], m.vertices[next]);
		actual = next;
	}
	while (actual != to);
	
	return angle;
}


