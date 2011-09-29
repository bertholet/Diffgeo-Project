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

float meshOperation::sumAnglesWheel( int from, int center, int to, mesh & m )
{
	vector<int> & nbrs = m.getNeighbors()[center];
	vector<int> & nbr_fcs = m.getNeighborFaces()[center];


	int actual = from, next, lps = 0;
	float angle = 0;

	do{
		next = meshOperation::getPrevious(center,actual, m);
		if(next < 0 || lps > int(nbrs.size())){
			throw std::runtime_error("Assertion failed at sum Angle Wheel");
		}
		angle += tuple3f::angle( m.vertices[actual], m.vertices[center], m.vertices[next]);
		actual = next;
		lps++;
	}
	while (actual != to);

	return angle;
}

int meshOperation::getFirst( int center_index, int v, mesh& m )
{
	int actual = v, previous;
	while((previous =getPrevious(center_index,actual,m))>=0){
		actual = previous;
	}
	return actual;
}

int meshOperation::getLast( int center_index, int v, mesh& m )
{
	int actual = v, next;
	while((next =getNext(center_index,actual,m))>=0){
		actual = next;
	}
	return actual;
}

int meshOperation::getPrevious_bc( int center_index, int v, mesh& m )
{
	int prev = getPrevious(center_index,v,m);
	if(prev < 0){
		return getLast(center_index,v,m);
	}
	return prev;
}

int meshOperation::getNext_bc( int center_index, int v, mesh& m )
{

	int next = getNext(center_index,v,m);
	if(next < 0){
		return getFirst(center_index,v,m);
	}
	return next;
}

void meshOperation::mirrorX( std::vector<tuple3f> & borderPos, float x )
{
	for(unsigned int i = 0; i < borderPos.size(); i++){
		borderPos[i].x = 2*x-borderPos[i].x;
	}
}


