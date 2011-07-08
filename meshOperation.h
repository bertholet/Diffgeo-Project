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
		vector<int> border;

		getNeighbors(m.faces, neighbors);
		//vertex i is a border iff it has: only two neigbors
		//or has one (two) neigbors connected with only one furter neigbor
		
		for(int i = 0; i < m.vertices.size(); i++){
			if(isOnBorder(i, neighbors)){
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
			while((nextVertex = nextBorderVertex(nextVertex, border, neighbors)) >-1){
				target.push_back(nextVertex);
			}
		}

		delete[] neighbors;

	}

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

private:
	static void ifNotContainedAdd( vector<int> &v, int a )
	{
		vector<int>::iterator low;

		low = lower_bound(v.begin(), v.end(),a);
		if(low == v.end() || *low != a){
			v.insert(low,1,a);
		}
	}

	static int nextBorderVertex( int vertex, vector<int> &border, vector<int> * neighbors )
	{
		vector<int> & nbrs = neighbors[vertex];

		vector<int>::iterator it = nbrs.begin(), borderEl;
		int result;

		//check for each neighbor if it lies on the border until one is found.
		for(it = nbrs.begin(); it != nbrs.end(); it++){
			borderEl = lower_bound(border.begin(), border.end(),*it);
			if(borderEl != border.end() && *borderEl == *it){
				border.erase(borderEl);
				result = *it;
				return *it;
			}
		}
		return -1;

	}

	//////////////////////////////////////////////////////////////////////////
	//if all edges are part of two closed triangle paths involving vertex_nr
	//this is not a border vertex
	//////////////////////////////////////////////////////////////////////////
	static bool isOnBorder(int vertex_nr, vector<int> * neighbors){

		//is not on border exactly if it is in two faces
		//or h
		bool isBorder =false;
		vector<int> & oneRing = neighbors[vertex_nr];

		if(oneRing.size() == 2){
			return true;
		}

		vector<int>::iterator nbr, nbr2,nbr3, end, end3;
		int count = 0;

		//for each neighbor nbr: look at its neighbors nbr2. if nbr2 is a neighbor of
		//the original node count ++
		for(nbr = oneRing.begin(); nbr != oneRing.end(); nbr++){
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
		}
		return false;
	}


};
