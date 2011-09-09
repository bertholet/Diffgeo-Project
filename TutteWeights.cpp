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

		prev = meshOperation::getPrevious(i, j, m);	
		next = meshOperation::getNext(i, j, m);
		if(prev == -1 || next == -1){
			prev = meshOperation::getPrevious(i, j, m);	
			next = meshOperation::getNext(i, j, m);
		}

		tan_alpha1_2 = (1- tuple3f::cosPoints(verts[prev], verts[i], verts[j]))/
			tuple3f::sinPoints(verts[prev], verts[i], verts[j]);
		tan_alpha2_2 = (1- tuple3f::cosPoints(verts[next], verts[i], verts[j]))/
			//tuple3f::sinPoints(verts[next], verts[i], verts[j]);
			tuple3f::sinPoints(verts[j], verts[i], verts[next]);

		return (tan_alpha1_2 + tan_alpha2_2)/(verts[i]-verts[j]).norm(); //norm
		
	}

	return 0;
}

double TutteWeights::cotan_weights( int i, int j, mesh & m, vector<int> & neighbors_i,
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
	float cot_alpha1, cot_alpha2;

	if(find(neighbors_i.begin(), neighbors_i.end(), j)!= neighbors_i.end()){

		prev = meshOperation::getPrevious(i, j, m);	
		next = meshOperation::getNext(i, j, m);
		if(prev == -1 || next == -1){
			prev = meshOperation::getPrevious(i, j, m);	
			next = meshOperation::getNext(i, j, m);
		}

		cot_alpha1 = tuple3f::cotPoints(verts[j], verts[prev], verts[i]);
		cot_alpha2 = tuple3f::cotPoints(verts[i], verts[next], verts[j]);

		return (cot_alpha1 + cot_alpha2)/(verts[i]-verts[j]).normSqr(); //norm

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// The weights following  the paper Discrete Diff geo
//////////////////////////////////////////////////////////////////////////
double TutteWeights::cotan_weights_divAvor( int i, int j, mesh & m, 
										  vector<int> & neighbors_i, 
										  vector<int> & nbr_fc_i, 
										  vector<int> & border )
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
	float cot_alpha1, cot_alpha2, tempcot1, tempcot2;
	int nbr;

	if(find(neighbors_i.begin(), neighbors_i.end(), j)!= neighbors_i.end()){

		prev = meshOperation::getPrevious(i, j, m);	
		next = meshOperation::getNext(i, j, m);
		if(prev == -1 || next == -1){
			printf("Error in Tutteweights::cotanweights_adiv");
		}

		cot_alpha1 = tuple3f::cotPoints(verts[j], verts[prev], verts[i]);
		cot_alpha2 = tuple3f::cotPoints(verts[i], verts[next], verts[j]);

		float Avornoi = 0;
		for(int k = 0; k < neighbors_i.size(); k++){
			/*if(tuple3f::a_bDotc_b(verts[k], verts[i], verts[prev]) <0 )
			{
				
			}
			else if(tuple3f::a_bDotc_b(verts[k], verts[prev], verts[i]) <0 ||
				tuple3f::a_bDotc_b(verts[k], verts[prev], verts[i])<0)
			{

			}
			else 
			{

			}
			else{
				Avornoi += (tuple3f::cotPoints(verts[k], verts[prev], verts[i]) +
					tuple3f::cotPoints(verts[i], verts[next], verts[k])) *
					(verts[i]-verts[k]).normSqr();
			}*/
			nbr = neighbors_i[k];
			prev = meshOperation::getPrevious(i, nbr, m);	
			next = meshOperation::getNext(i,nbr, m);
			tempcot1 = tuple3f::cotPoints(verts[nbr], verts[prev], verts[i]);
			tempcot1 = (tempcot1 >0 ? tempcot1: -tempcot1);
			tempcot2 = tuple3f::cotPoints(verts[i], verts[next], verts[nbr]);
			tempcot2 = (tempcot2 >0 ? tempcot2: -tempcot2);
			Avornoi += (tempcot1 +
				tempcot2) *
				(verts[i]-verts[nbr]).normSqr();
		}

		return (cot_alpha1 + cot_alpha2)/Avornoi;//(verts[i]-verts[j]).normSqr(); //norm

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// The weights following  the paper Discrete Diff geo
//////////////////////////////////////////////////////////////////////////
double TutteWeights::cotan_weights_divAmix( int i, int j, mesh & m, 
										  vector<int> & neighbors_i, 
										  vector<int> & nbr_fc_i, 
										  vector<int> & border )
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
	float cot_alpha1, cot_alpha2, tempcot1, tempcot2;
	int nbr;

	if(find(neighbors_i.begin(), neighbors_i.end(), j)!= neighbors_i.end()){

		prev = meshOperation::getPrevious(i, j, m);	
		next = meshOperation::getNext(i, j, m);
		if(prev == -1 || next == -1){
			printf("Error in Tutteweights::cotanweights_adiv");
		}

		cot_alpha1 = tuple3f::cotPoints(verts[j], verts[prev], verts[i]);
		cot_alpha2 = tuple3f::cotPoints(verts[i], verts[next], verts[j]);

		float Amix = 0, areaT;
		for(int k = 0; k < neighbors_i.size(); k++){
			nbr = neighbors_i[k];
			prev = meshOperation::getPrevious(i, nbr, m);	
		//	next = meshOperation::getNext(i,nbr_fc_i,nbr, m);
			areaT = tuple3f::cross((verts[nbr] - verts[i]),(verts[prev] - verts[i])).norm()/2;

			if(tuple3f::a_bDotc_b(verts[nbr], verts[i], verts[prev]) <0 )
			{
				Amix += areaT/2;
			}
			else if(tuple3f::a_bDotc_b(verts[nbr], verts[prev], verts[i]) <0 ||
				tuple3f::a_bDotc_b(verts[prev], verts[nbr], verts[i])<0)
			{
				Amix += areaT/4;
			}
			else{
				tempcot1 = tuple3f::cotPoints(verts[nbr], verts[prev], verts[i]);
				tempcot1 = (tempcot1 >0 ? tempcot1: -tempcot1);
				tempcot2 = tuple3f::cotPoints(verts[i], verts[nbr], verts[prev]);
				tempcot2 = (tempcot2 >0 ? tempcot2: -tempcot2);
				Amix += tempcot1 * (verts[i]-verts[nbr]).normSqr() +
					tempcot2 * (verts[i]-verts[prev]).normSqr();
			}

		}

		return (cot_alpha1 + cot_alpha2)/Amix;//(verts[i]-verts[j]).normSqr(); //norm

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//borders
//////////////////////////////////////////////////////////////////////////


void TutteWeights::circleBorder( vector<tuple3f> & outerPos , vector<int> & border, vector<int> & loops, mesh & m)
{
	int sz = border.size();
	for(int i = 0; i < sz;i++){
		outerPos.push_back(tuple3f((sin(i*2* PI/sz)+1.f)/2.f,(cos(i*2* PI/sz)+1.f)/2.f,0.f));
	}
}

void TutteWeights::distWeightCircBorder( vector<tuple3f> & outerPos , vector<int> & border, vector<int> & loops, mesh & m)
{
	int sz = border.size();
	float length = meshOperation::getLength(border, m);
	float ln = 0;
	for(int i = 0; i < sz;i++){
		outerPos.push_back(tuple3f((sin(ln/length * 2* PI)+1.f)/2.f,(cos(ln/length * 2* PI)+1.f)/2.f,0.f));
		if(i < sz-1){
			ln += (m.vertices[border[i+1]] - m.vertices[border[i]]).norm();
		}
	}
}

void TutteWeights::angleApproxBorder( vector<tuple3f> & outerPos , vector<int> & border
			, vector<int> & loops, mesh & m)
{
	vector<float> angles, lengthQuota; //length next segment over last segment
	angles.reserve(border.size());
	lengthQuota.reserve(border.size());

	int bdr, loopsz, loopSt, prev, next;
	float angle, length, sum, scale;

	for(int lp = 0; lp < loops.size(); lp++){

		loopSt = loops[lp];
		loopsz = (lp == loops.size() -1? border.size(): loops[lp+1]) 
			- loopSt;
		sum = 0;

		for(bdr =0; bdr < loopsz; bdr++){
			prev = (bdr == 0? loopsz-1 : bdr-1);
			next = (bdr == loopsz-1? 0 : bdr+1);
			
			angle = meshOperation::sumAnglesWheel(border[prev], 
				border[bdr], border[next], m);
			sum+= angle;
			angles.push_back(angle);

			length = (m.getVertices()[border[next]] - m.getVertices()[border[bdr]]).norm()/
				(m.getVertices()[border[bdr]] - m.getVertices()[border[prev]]).norm();
			lengthQuota.push_back(length);

		}
		scale = (loopsz-2) * PI / sum;
		for(bdr =0; bdr < loopsz; bdr++){
			angles[loopSt + bdr] = angles[loopSt + bdr] * scale;
		}


		printf("");
	}
}


