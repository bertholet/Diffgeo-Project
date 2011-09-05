#include "stdafx.h"
#include "tutteWeights.h"
#include <algorithm>
#include "meshOperation.h"
#include <math.h>
#include "tuple3.h"
#include <iostream>

double TutteWeights::uniform_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
									 vector<int> & neighbor_fc_i, vector<int> & border)
{
	vector<int>::iterator idx;

	//ith vertex is on the border.
	if((idx = find(border.begin(), border.end(), i))!= border.end()){
		if(i!=j){
			return 0;
		}
		return 1;
	}
	//i is not on the border
	if(i==j){
		return -1;
	}
	//jth vertex is a neighbor of i
	if(find(neighbors_i.begin(), neighbors_i.end(), j)!= neighbors_i.end()){
		return 1.0/ neighbors_i.size();			
	}
	return 0;
}

/************************************************************************
* neigbor_i: the neighbor vertices of i
*  NBR_FC_I: the indices of the neighbor faces of i.
*  Note: The "unnormed" relates to the fact, that the rows do not sum up to 1.
*  This because it is not efficient to calculate the normation factor (i.e. the
*  row sum) each time.
*
/************************************************************************/
double TutteWeights::unnormed_meanvalue_weights( int i, int j, mesh & m, vector<int> & neighbors_i,
												vector<int> & nbr_fc_i, vector<int> & border )
{
	vector<int>::iterator idx;
	int prev, next;
	//ith vertex is on the border.
	if((idx = find(border.begin(), border.end(), i))!= border.end()){
		if(i!=j){
			return 0;
		}
		return 1;
	}
	//i is not on the border
	if(i==j){
		return -1;
	}
	//jth vertex is a neighbor of i
	vector<tuple3f> & verts =m.getVertices();
	float tan_alpha1_2, tan_alpha2_2;
	
	if(find(neighbors_i.begin(), neighbors_i.end(), j)!= neighbors_i.end()){

		prev = meshOperation::getPrevious(i,nbr_fc_i, j, m);	
		next = meshOperation::getNext(i,nbr_fc_i, j, m);
		if(prev == -1 || next == -1){
			prev = meshOperation::getPrevious(i,nbr_fc_i, j, m);	
			next = meshOperation::getNext(i,nbr_fc_i, j, m);
		}

		tan_alpha1_2 = (1- tuple3f::cosPoints(verts[prev], verts[i], verts[j]))/
			tuple3f::sinPoints(verts[prev], verts[i], verts[j]);
		tan_alpha2_2 = (1- tuple3f::cosPoints(verts[next], verts[i], verts[j]))/
			tuple3f::sinPoints(verts[next], verts[i], verts[j]);

		return (tan_alpha1_2 + tan_alpha2_2)/(verts[i]-verts[j]).norm();
		
	}

	return 0;
}

double TutteWeights::cotan_weights( int i, int j, mesh & m, vector<int> & neighbors_i,
								   vector<int> & neighbor_fc_i, vector<int> & border )
{
	return 0;
}
