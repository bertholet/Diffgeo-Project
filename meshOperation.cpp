#include "StdAfx.h"
#include "meshOperation.h"

meshOperation::meshOperation(void)
{
}

meshOperation::~meshOperation(void)
{
}

int meshOperation::getPrevious( int center_index, vector<int> & neighbor_faces, int v, mesh  & m)
{
	vector<tuple3i> & faces = m.getFaces(); 
	tuple3i & face = faces[0];
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

int meshOperation::getNext( int center_idx, vector<int> & neighbor_faces, int v, mesh  & m )
{
	vector<tuple3i> & faces = m.getFaces(); 
	tuple3i & face = faces[0];
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


