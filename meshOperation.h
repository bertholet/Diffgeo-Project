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

		target.reset(vertices,faces);

	}
	
	//////////////////////////////////////////////////////////////////////////
	// Detects dangling border positions, i.e. vertices that have more than 2 
	// adjascend border edges (but 2k of them). This will k-plicate such vertices
	// and put them into the mesh such that the border 
	// (and possibly the mesh) falls in more components
	//////////////////////////////////////////////////////////////////////////
	static void undangle(mesh & m){
		vector<int> criticalPoints, borderCompRepresentants, facesToChange;
		
		int vertex;
		vector<int>::iterator nbrs;
		vector<int> * nbr_fcs;
		int borderEdges=0, nbr, nextNbr, newVertex_id, 
			nrVertices = m.getVertices().size();

		for(vertex= 0; vertex < nrVertices; vertex++){
			nbr_fcs = & (m.getNeighborFaces()[vertex]);
			borderEdges =0;
			borderCompRepresentants.clear();
			//critical neighbors will have one neighbor per "border component"
			for(nbrs = (m.getNeighbors()[vertex]).begin(); nbrs != (m.getNeighbors()[vertex]).end(); nbrs++){
				//positive oriented border edges
				if(countPosOrientedOccurrences(*nbr_fcs, vertex, *nbrs,m) -
					countNegOrientedOccurrences(*nbr_fcs, vertex, *nbrs,m) >0){//!= 0){
						borderEdges++;
						borderCompRepresentants.push_back(*nbrs);
				}
			}

			if(borderEdges > 1){//2){
				//split neighborhood
				criticalPoints.push_back(vertex);
				for(nbrs = borderCompRepresentants.begin(); nbrs!= borderCompRepresentants.end(); nbrs++){
					//first border component can keep the old vertex.
					if(nbrs!=borderCompRepresentants.begin()){
						//duplicate vertex
						newVertex_id = m.getVertices().size();
						m.getVertices().push_back(m.getVertices()[vertex]);
						
						nbr = *nbrs;
						nextNbr = getNext(vertex,nbr,m);
						while(nextNbr>-1){
							replace_vbc_newv(vertex, nbr, nextNbr,newVertex_id, m);
							nbr = nextNbr;
							nextNbr = getNext(vertex,nextNbr,m);
						}
					}
				}
			}
		}

		m.reset(m.vertices, m.faces);
		printf("Found %d dangling border points\n", criticalPoints.size());

	}

	static int countComponents(mesh &m){

	}

	static void reduceToLargestComponent(mesh &m){
		vector<vector<int>> components;
		vector<int> stack;
		vector<int> done;
		vector<int>::iterator low;
		vector<vector<int>> & nbrs = m.getNeighbors();
		int nrComponents =0;
		int node,j;

		for(unsigned int i = 0; i < m.getVertices().size();i++){
			low = lower_bound(done.begin(), done.end(),i);
			if(low == done.end() || *low != i){
				components.push_back(vector<int>()); 
				vector<int> & comp = components.back();
				stack.push_back(i);
				while(stack.size() !=0){
					node = stack.back();
					stack.pop_back();
					ifNotContainedInsert(done, node);

					low = lower_bound(comp.begin(), comp.end(),node);
					if(low == comp.end() || *low != node){
						comp.insert(low, node);
					}
					
					for(j = 0; j < nbrs[node].size(); j++){
						low = lower_bound(done.begin(), done.end(),nbrs[node][j]);
						if(low == done.end() || *low != nbrs[node][j]){
							ifNotContainedInsert(stack, nbrs[node][j]);
						}
					}
				}
				nrComponents++;
			}
		}

		printf("Found %d components. \n", nrComponents);

		int max=0, maxi = -1;
		for(int i = 0; i < components.size(); i++){
			if(components[i].size() > max){
				maxi = i;
				max = components[i].size();
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
	//scale textures to [0,1]x[0,1]
	//////////////////////////////////////////////////////////////////////////
	static void normalizeTexture(vector<tuple3f> &tex){
		float maxx= tex[0].x, minx = tex[0].x,
			maxy= tex[0].y, miny = tex[0].y;

		for(vector<tuple3f>::iterator it = tex.begin(); it != tex.end(); it++){
			if(it->x > maxx){
				maxx = it->x;
			}
			if(it->y > maxy){
				maxy = it->y;
			}
			if(it->x < minx){
				minx = it->x;
			}
			if(it->y < miny){
				miny = it->y;
			}
		}

		float scale = (maxx-minx>maxy-miny? maxx-minx:maxy-miny);
		scale = 1.f/scale;
		for(vector<tuple3f>::iterator it = tex.begin(); it != tex.end(); it++){
			it->x = it->x-minx;
			it->y = it->y-miny;
			it->x *= scale;
			it->y *=scale;
		}
	}

	static void getBorder(mesh & m, vector<int> & target, vector<int> & starts){
		
		target.clear();
/*		vector<int> *neighbors = new vector<int>[m.vertices.size()];
		vector<int> *neighbor_faces = new vector<int>[m.vertices.size()]; 
*/		vector<int> border;

/*		getNeighbors(m.faces, neighbors);
		getNeighborFaces(m.faces, neighbor_faces);
*/		//vertex i is a border iff it has: only two neigbors
		//or has one (two) neigbors connected with only one furter neigbor
		//better: it has at least one neighbor with which it sheares only one face.
		int nrVertices = m.vertices.size();
		for(int i = 0; i < nrVertices; i++){
//			if(isOnBorder(i, neighbors, neighbor_faces, m)){
			if(isOnBorder(i, m)){
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
//			while((nextVertex = nextBorderVertex(nextVertex, border, neighbors, neighbor_faces, m)) >-1){
			while((nextVertex = nextBorderVertex(nextVertex, border, m)) >-1){
				target.push_back(nextVertex);
			}

			//assertion
/*			if(find(neighbors[target.back()].begin(),
				neighbors[target.back()].end(),
				target[starts.back()])==neighbors[target.back()].end()){
					throw std::exception("Assertion failed: border not a closed loop");
			}*/
		}

//		delete[] neighbors, neighbor_faces;

	}

	//////////////////////////////////////////////////////////////////////////
	//Returns the neighbors by vertex nr, ordered in ascending order, each time.
	//Stores them in "neighbors", a preallocated and preinitialized vector of sz
	// nrVertices
	//////////////////////////////////////////////////////////////////////////
	static void getNeighbors(vector<tuple3i> & faces, vector<vector<int>> & neighbors){
			
		for(unsigned int i = 0; i < faces.size(); i++){

		/*debug[faces[i].a]++;
		debug[faces[i].b]++;
		debug[faces[i].c]++;*/
		//adds the integers such that the vector stays ordered.

			ifNotContainedInsert(neighbors[faces[i].a], faces[i].b);
			ifNotContainedInsert(neighbors[faces[i].b], faces[i].a);

			ifNotContainedInsert(neighbors[faces[i].a], faces[i].c);
			ifNotContainedInsert(neighbors[faces[i].c], faces[i].a);

			ifNotContainedInsert(neighbors[faces[i].b], faces[i].c);
			ifNotContainedInsert(neighbors[faces[i].c], faces[i].b);

		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Returns an array of vectors. neighbor_faces[i] will be the indices of all faces
	//neighboring the node i a preallocated and preinitialized vector of sz
	// nrVertices
	//////////////////////////////////////////////////////////////////////////
	static void getNeighborFaces(vector<tuple3i> & faces, vector<vector<int>> & neighbor_faces){
			
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
	//vertex (by orientation induced from the face orientation) or -1 if none exists
	//////////////////////////////////////////////////////////////////////////
	static int getPrevious( int center_index, int v, mesh& m );
	//////////////////////////////////////////////////////////////////////////
	//returns the index of the successor of v in the neighbors of the center_index
	//vertex (by orientation induced from the face orientation)
	//////////////////////////////////////////////////////////////////////////
	static int getNext( int center_idx, int v , mesh& m );

	//////////////////////////////////////////////////////////////////////////
	// Returns the length of the polygon train denoted by the vertex indices
	// "vertices"
	//////////////////////////////////////////////////////////////////////////
	static float getLength(vector<int> & vertices, mesh & m){
		float length = 0;
		for(unsigned int i = 0; i < vertices.size()-1; i++){
			length += (m.vertices[vertices[i+1]] - m.vertices[vertices[i]]).norm();
		}

		length += (m.vertices[vertices.back()] - m.vertices[vertices.front()]).norm();
		return length;
	}

	//////////////////////////////////////////////////////////////////////////
	//sums the angle (pos oriented) between the edges specified by the indices
	// (at from) and (at to)
	//////////////////////////////////////////////////////////////////////////
	static float sumAnglesWheel( int from, int at, int to, mesh & m );

private:
	static void ifNotContainedInsert( vector<int> &v, int a )
	{
		vector<int>::iterator low;
		
		if(v.size() == 0){
			v.push_back(a);
			return;
		}

		low = lower_bound(v.begin(), v.end(),a);
		if(low == v.end() || *low != a){
			v.insert(low,1,a);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Retruns the next border vertex and !ERASES! it from border.
	//////////////////////////////////////////////////////////////////////////
	static int nextBorderVertex( int vertex, vector<int> &border, mesh &m )
	{
		vector<int> & nbrs = m.getNeighbors()[vertex];
		vector<int> & nbr_fcs = m.getNeighborFaces()[vertex];
		vector<int> borderEls;
		vector<int>::iterator it = nbrs.begin(), borderEl;

		vector<tuple3i> debug;
		for(int i = 0; i< nbr_fcs.size(); i++){
			debug.push_back(m.getFaces()[nbr_fcs[i]]);
		}

		//check for each neighbor if it lies on the border until one is found.
		for(it = nbrs.begin(); it != nbrs.end(); it++){
			borderEl = lower_bound(border.begin(), border.end(),*it);
			if(borderEl != border.end() && *borderEl == *it){
				/*border.erase(borderEl);
				result = *it;
				return *it;*/
				//an element could be on the border and a neighbor, without
				//being connected to vertex by a border.
				if(countPosOrientedOccurrences(nbr_fcs, vertex, *borderEl, m) 
					- countNegOrientedOccurrences(nbr_fcs, vertex, *borderEl, m)==1)
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
	static bool isOnBorder(int vertex_nr, mesh &m){

		vector<vector<int>> & neighbors = m.getNeighbors();
		vector<vector<int>> & neighbor_faces = m.getNeighborFaces();
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
				}
			}
			if(count == 1){
				return true;
			}
			if(count >2){
				printf("meshOperation::isOnBorder: detected edge contained in %d faces!", count);
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

	//////////////////////////////////////////////////////////////////////////
	//counts nr of occurences of (a,b) as positively oriented edge
	//in the faces indicised bayface_indexes
	//////////////////////////////////////////////////////////////////////////
	static int countNegOrientedOccurrences(vector<int> & face_indexes, int a, int b, mesh & m){
		vector<int>::iterator nbr_fc_idx,end;
		tuple3i nbr_fc;
		int count = 0;
		nbr_fc_idx = face_indexes.begin();
		end = face_indexes.end();
		// counts nr of faces shared
		for(;nbr_fc_idx!= end; nbr_fc_idx++){
			nbr_fc = m.faces[*nbr_fc_idx];
			if(nbr_fc.a == a && nbr_fc.c == b || 
				nbr_fc.b == a && nbr_fc.a == b|| 
				nbr_fc.c == a && nbr_fc.b == b ){
					count ++;
			}
		}
		return count;
	}

	//////////////////////////////////////////////////////////////////////////
	// replaces vertex by newVertex in the face specified by vertex, nbr1, nbr2
	//////////////////////////////////////////////////////////////////////////
	static void replace_vbc_newv(int vertex, int nbr1, int nbr2, int newVertex, mesh& m){
		vector<int> & nbr_fcs = m.getNeighborFaces()[vertex];
		vector<tuple3i> & fcs = m.getFaces();
		bool replaced = false;

		int debug_that_fuck;
		if(m.getNeighbors().size() <= newVertex){
			m.getNeighbors().push_back(vector<int>());
			if(m.getNeighbors().size()<= newVertex){
				throw std::runtime_error("Assertion Error");
			}
		}
		//&! replaced....
		for(vector<int>::iterator fcID = nbr_fcs.begin(); fcID!= nbr_fcs.end(); fcID++){
			if((fcs[*fcID].a == vertex) && (fcs[*fcID].b == nbr1) && (fcs[*fcID].c == nbr2)){

				fcs[*fcID].a = newVertex;

				ifNotContainedInsert(m.getNeighbors()[fcs[*fcID].b], newVertex);
				ifNotContainedInsert(m.getNeighbors()[fcs[*fcID].c], newVertex);
				ifNotContainedInsert(m.getNeighbors()[newVertex], fcs[*fcID].b);
				ifNotContainedInsert(m.getNeighbors()[newVertex], fcs[*fcID].c);
				fcID = nbr_fcs.erase(fcID);
				replaced = true;
				break;
			}
			else if((fcs[*fcID].b == vertex )&& (fcs[*fcID].c == nbr1) && (fcs[*fcID].a == nbr2)){
				fcs[*fcID].b = newVertex;

				debug_that_fuck = fcs[*fcID].a;
				ifNotContainedInsert(m.getNeighbors()[fcs[*fcID].a], newVertex);
				debug_that_fuck = fcs[*fcID].c;
				ifNotContainedInsert(m.getNeighbors()[fcs[*fcID].c], newVertex);
				ifNotContainedInsert(m.getNeighbors()[newVertex], fcs[*fcID].a);
				ifNotContainedInsert(m.getNeighbors()[newVertex], fcs[*fcID].c);

				fcID = nbr_fcs.erase(fcID);
				replaced = true;
				break;
			}
			else if((fcs[*fcID].c == vertex) && (fcs[*fcID].a == nbr1) && (fcs[*fcID].b== nbr2)){
				fcs[*fcID].c = newVertex;

				ifNotContainedInsert(m.getNeighbors()[fcs[*fcID].b], newVertex);
				ifNotContainedInsert(m.getNeighbors()[fcs[*fcID].a], newVertex);
				ifNotContainedInsert(m.getNeighbors()[newVertex], fcs[*fcID].b);
				ifNotContainedInsert(m.getNeighbors()[newVertex], fcs[*fcID].a);

				fcID = nbr_fcs.erase(fcID);
				replaced = true;
				break;
			}
		}
		if(!replaced){
			throw std::runtime_error("Assertion failed in replace_v_abc");
		}
	}
};
