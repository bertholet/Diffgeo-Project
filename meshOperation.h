#pragma once
#include "mesh.h"
#include "tuple3.h"
#include <vector>
#include <algorithm>

class meshOperation
{
public:
	meshOperation(void);
	~meshOperation(void);

	static void getHalf(mesh & m, mesh & target, tuple3f direction, float dist){
		vector<int> usedVertices;
		vector<tuple3i> faces;
		vector<tuple3f> vertices;

		for(unsigned int i = 0; i < m.faces.size(); i++){

			if(((tuple3f) m.vertices[m.faces[i].a]).dot(direction)>dist ||
				((tuple3f) m.vertices[m.faces[i].b]).dot(direction)>dist||
				((tuple3f) m.vertices[m.faces[i].c]).dot(direction)>dist){
				usedVertices.push_back(m.faces[i].a);
				usedVertices.push_back(m.faces[i].b);
				usedVertices.push_back(m.faces[i].c);
				
				faces.push_back(m.faces[i]);
			}
		}

		vector<int>::iterator it;

		// using default comparison (operator <):
		std::sort (usedVertices.begin(), usedVertices.end());
		usedVertices.erase(std::unique(usedVertices.begin(), usedVertices.end()), usedVertices.end());

		int k;
		for(unsigned int i = 0; i < faces.size(); i++){
			k= (std::find(usedVertices.begin(), usedVertices.end(), faces[i].a)-usedVertices.begin());
			faces[i].a = k;
			k= (std::find(usedVertices.begin(), usedVertices.end(), faces[i].b)-usedVertices.begin());
			faces[i].b = k;
			k= (std::find(usedVertices.begin(), usedVertices.end(), faces[i].c)-usedVertices.begin());
			faces[i].c = k;

		}

		for(unsigned int i = 0; i < usedVertices.size(); i++){
			vertices.push_back(m.vertices[usedVertices[i]]);
		}

		target.faces = faces;
		target.vertices = vertices;
		target.initNormalsFromVertices();

	}

	static void getBorder(mesh & m, vector<int> & target, vector<int> & starts){
		
		target.clear();
		vector<int> *neighbors = new vector<int>[m.vertices.size()];
		vector<int> *neighbor_faces = new vector<int>[m.vertices.size()];
		vector<int> border;

		getNeighbors(m.faces, neighbors);
		getNeighborFaces(m.faces, neighbor_faces);
		//vertex i is a border iff it has: only two neigbors
		//or has one (two) neigbors connected with only one furter neigbor
		//better: it has at least one neighbor with which it sheares only one face.
		int nrVertices = m.vertices.size();
		for(int i = 0; i < nrVertices; i++){
			if(isOnBorder(i, neighbors, neighbor_faces, m)){
				border.push_back(i);
				//target.push_back(i);
			}
		}


		int nextVertex;
		while(!border.empty()){
			nextVertex = (border.back());
			border.pop_back();
			
			starts.push_back((target.size()));
			target.push_back(nextVertex);
			while((nextVertex = nextBorderVertex(nextVertex, border, neighbors, neighbor_faces, m)) >-1){
				target.push_back(nextVertex);
			}

			//assertion
/*			if(find(neighbors[target.back()].begin(),
				neighbors[target.back()].end(),
				target[starts.back()])==neighbors[target.back()].end()){
					throw std::exception("Assertion failed: border not a closed loop");
			}*/
		}

		delete[] neighbors, neighbor_faces;

	}

	//////////////////////////////////////////////////////////////////////////
	//Returns the neighbors by vertex nr, ordered in ascending order, each time.
	//
	//////////////////////////////////////////////////////////////////////////
	static void getNeighbors(vector<tuple3i> & faces, vector<int> * neighbors){
			
		for(unsigned int i = 0; i < faces.size(); i++){

		/*debug[faces[i].a]++;
		debug[faces[i].b]++;
		debug[faces[i].c]++;*/
		//adds the integers such that the vector stays ordered.

			ifNotContainedAdd(neighbors[faces[i].a], faces[i].b);
			ifNotContainedAdd(neighbors[faces[i].b], faces[i].a);

			ifNotContainedAdd(neighbors[faces[i].a], faces[i].c);
			ifNotContainedAdd(neighbors[faces[i].c], faces[i].a);

			ifNotContainedAdd(neighbors[faces[i].b], faces[i].c);
			ifNotContainedAdd(neighbors[faces[i].c], faces[i].b);

		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Returns an array of vectors. neighbor_faces[i] will be the indices of all faces
	//neighboring the node i
	//////////////////////////////////////////////////////////////////////////
	static void getNeighborFaces(vector<tuple3i> & faces, vector<int> * neighbor_faces){
			
		for(int i = 0; i < (int) faces.size(); i++){

		/*debug[faces[i].a]++;
		debug[faces[i].b]++;
		debug[faces[i].c]++;*/
		//adds the integers such that the vector stays ordered.

			neighbor_faces[faces[i].a].push_back(i);
			neighbor_faces[faces[i].b].push_back(i);
			neighbor_faces[faces[i].c].push_back(i);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//returns the index of the predecessor of v in the neighbors of the center_index
	//vertex (by orientation induced from the face orientation)
	//////////////////////////////////////////////////////////////////////////
	static int getPrevious( int center_index, vector<int> & neighbor_faces, int v, mesh& m );
	//////////////////////////////////////////////////////////////////////////
	//returns the index of the successor of v in the neighbors of the center_index
	//vertex (by orientation induced from the face orientation)
	//////////////////////////////////////////////////////////////////////////
	static int getNext( int center_idx, vector<int> & nbr_fc, int v , mesh& m );

	//////////////////////////////////////////////////////////////////////////
	// Returns the length of the polygon train denoted by the vertex indices
	// "vertices"
	//////////////////////////////////////////////////////////////////////////
	static float getLength(vector<int> & vertices, mesh & m){
		float length = 0;
		for(int i = 0; i < vertices.size()-1; i++){
			length += (m.vertices[vertices[i+1]] - m.vertices[vertices[i]]).norm();
		}

		length += (m.vertices[vertices.back()] - m.vertices[vertices.front()]).norm();
		return length;
	}

private:
	static void ifNotContainedAdd( vector<int> &v, int a )
	{
		vector<int>::iterator low;

		low = lower_bound(v.begin(), v.end(),a);
		if(low == v.end() || *low != a){
			v.insert(low,1,a);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Retruns the next border vertex and ERASES it from border
	//////////////////////////////////////////////////////////////////////////
	static int nextBorderVertex( int vertex, vector<int> &border, 
		vector<int> * neighbors, vector<int> * neighbor_faces, mesh &m )
	{
		vector<int> & nbrs = neighbors[vertex];
		vector<int> & nbr_fcs = neighbor_faces[vertex];
		vector<int> borderEls;
		vector<int>::iterator it = nbrs.begin(), borderEl;

		//check for each neighbor if it lies on the border until one is found.
		for(it = nbrs.begin(); it != nbrs.end(); it++){
			borderEl = lower_bound(border.begin(), border.end(),*it);
			if(borderEl != border.end() && *borderEl == *it){
				/*border.erase(borderEl);
				result = *it;
				return *it;*/
				//an element could be on the border and a neighbor, without
				//being connected to vertex by a border.
				if(countPosOrientedOccurrences(neighbor_faces[vertex], vertex, *borderEl, m) ==1)
				{
					borderEls.push_back(*it);
				}
			}
		}
		if(borderEls.size() == 0){
			return -1;
		}
		if(borderEls.size() == 1)
		{	
			border.erase(lower_bound(border.begin(), border.end(), borderEls.back()));
			return (borderEls.back());
		}
		else{
			//dangling trianlge did occur
			printf("Dangling triangle detected in MeshOpetation.h : nextBorderVertex(...), at vertexNr %d: %d matches \n", vertex, borderEls.size());
			border.erase(lower_bound(border.begin(), border.end(), borderEls.back()));
			return (borderEls.back());
			
		}

	}

	//////////////////////////////////////////////////////////////////////////
	//if all edges are part of two closed triangle paths involving vertex_nr
	//this is not a border vertex
	//////////////////////////////////////////////////////////////////////////
	static bool isOnBorder(int vertex_nr, vector<int> * neighbors, vector<int> * neighbor_faces, mesh &m){

		//is not on border exactly if it is in two faces
		//or h
		bool isBorder =false;
		vector<int> & oneRing = neighbors[vertex_nr];

		if(oneRing.size() == 2){
			return true;
		}

		vector<int>::iterator nbr, nbr_fc_idx,nbr2,nbr3, end, end3;
		tuple3i nbr_fc = (m.faces[0]);
		int count = 0;

		for(nbr = oneRing.begin(); nbr != oneRing.end(); nbr++){
			nbr_fc_idx = neighbor_faces[vertex_nr].begin();
			end = neighbor_faces[vertex_nr].end();
			count = 0; // counts nr of faces shared
			for(;nbr_fc_idx!= end; nbr_fc_idx++){
				nbr_fc = m.faces[*nbr_fc_idx];
				if(nbr_fc.a == *nbr || nbr_fc.b == *nbr || nbr_fc.c == *nbr ){
					count ++;
					if(count == 2){
						break; // two neighbors can only share 2 faces
					}
				}
			}
			if(count == 1){
				return true;
			}

		}//*/
		//old algorithm
		//for each neighbor nbr: look at its neighbors nbr2. if nbr2 is a neighbor of
		//the original node count ++
/*		for(nbr = oneRing.begin(); nbr != oneRing.end(); nbr++){
			count = 0;

			nbr2 = neighbors[*nbr].begin();	
			end  = neighbors[*nbr].end();	
			//"neighbor's neighbor
			for(;nbr2 != end; nbr2++){
				nbr3 = neighbors[*nbr2].begin();	
				end3= neighbors[*nbr2].end();
				
				//neighbor's neighbor is a neighbor of original node
				for(;nbr3 != end3; nbr3++){
					if(*nbr3 == vertex_nr){
						count++;
					}
				}
			}
			if(count < 2){
				return true;
			}
		}//*/
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//counts nr of occurences of (a,b) as positively oriented edge
	//in the faces indicised bayface_indexes
	//////////////////////////////////////////////////////////////////////////
	static int countPosOrientedOccurrences(vector<int> & face_indexes, int a, int b, mesh & m){
		vector<int>::iterator nbr_fc_idx,end;
		tuple3i nbr_fc;
		int count = 0;
		nbr_fc_idx = face_indexes.begin();
		end = face_indexes.end();
		// counts nr of faces shared
		for(;nbr_fc_idx!= end; nbr_fc_idx++){
			nbr_fc = m.faces[*nbr_fc_idx];
			if(nbr_fc.a == a && nbr_fc.b == b || 
				nbr_fc.b == a && nbr_fc.c == b|| 
				nbr_fc.c == a && nbr_fc.a == b ){
				count ++;
			}
		}
		return count;
	}

};
