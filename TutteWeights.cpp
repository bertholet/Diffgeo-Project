#include "stdafx.h"
#include "tutteWeights.h"
#include <algorithm>

double TutteWeights::uniform_weights(int i, int j, mesh & m, vector<int> & neighbors_i, vector<int> & border)
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

double TutteWeights::unnormed_meanvalue_weights( int i, int j, mesh & m, vector<int> & neighbors_i, vector<int> & border )
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

double TutteWeights::cotan_weights( int i, int j, mesh & m, vector<int> & neighbors_i, vector<int> & border )
{
	return 0;
}
