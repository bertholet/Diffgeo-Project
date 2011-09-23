#include "stdafx.h"
#include "tutteWeights.h"
#include <algorithm>
#include "meshOperation.h"
#include <math.h>
#include "tuple3.h"
#include <iostream>
#include "pardiso.h"

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
		if(prev == -1){
			prev = meshOperation::getLast(i, j, m);	
		}
		if(next == -1){
			next = meshOperation::getFirst(i, j, m);
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

		prev = meshOperation::getPrevious_bc(i, j, m);	
		next = meshOperation::getNext_bc(i, j, m);
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
			prev = meshOperation::getPrevious_bc(i, nbr, m);	
			next = meshOperation::getNext_bc(i,nbr, m);

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

		prev = meshOperation::getPrevious_bc(i, j, m);	
		next = meshOperation::getNext_bc(i, j, m);
		if(prev == -1 || next == -1){
			printf("Error in Tutteweights::cotanweights_adiv");
		}

		cot_alpha1 = tuple3f::cotPoints(verts[j], verts[prev], verts[i]);
		cot_alpha2 = tuple3f::cotPoints(verts[i], verts[next], verts[j]);

		float Amix = 0, areaT;
		for(int k = 0; k < (int) neighbors_i.size(); k++){
			nbr = neighbors_i[k];
			prev = meshOperation::getPrevious_bc(i, nbr, m);	
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


void TutteWeights::circleBorder( vector<tuple3f> & outerPos , vector<int> & border, mesh & m)
{
	int sz = border.size();
	for(int i = 0; i < sz;i++){
		outerPos.push_back(tuple3f((sin(i*2* PI/sz)+1.f)/2.f,(cos(i*2* PI/sz)+1.f)/2.f,0.f));
	}

}

void TutteWeights::distWeightCircBorder( vector<tuple3f> & outerPos , vector<int> & border, mesh & m)
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


void TutteWeights::angles_lambdas( vector<float> &angles, vector<float> &lambdas, vector<int> & border, mesh & m)
{
	int prev, next, loopsz = border.size();
	float angle, length, scale_factor, sum = 0;

	angles.reserve(border.size());
	lambdas.reserve(border.size());
	for(int bdr =0; bdr < loopsz; bdr++){
		prev = (bdr == 0? loopsz-1 : bdr-1);
		next = (bdr == loopsz-1? 0 : bdr+1);

		angle = meshOperation::sumAnglesWheel(border[prev], 
			border[bdr], border[next], m);
		sum+= angle;
		angles.push_back(angle); //was pi -angle...

		length = (m.getVertices()[border[next]] - m.getVertices()[border[bdr]]).norm()/
			(m.getVertices()[border[bdr]] - m.getVertices()[border[prev]]).norm();
		lambdas.push_back(length);

	}

	scale_factor = (0.f +loopsz-2) * PI / sum;
	for(int bdr =0; bdr < loopsz; bdr++){
		angles[bdr] = PI - (angles[bdr] * scale_factor);
	}
}

/************************************************************************/
/* Returns aussenwinkel pi-sum(wheel), scaled to realistic vlue         */
/************************************************************************/
void TutteWeights::angles_lambdas( vector<vector<float>> &angles, vector<vector<float>> &lambdas, 
								  vector<vector<int>> & border, mesh & m)
{
	for(unsigned int i = 0; i < border.size(); i++){
		angles.push_back(vector<float>());
		lambdas.push_back(vector<float>());
		angles_lambdas(angles.back(),lambdas.back(),border[i],m);
	}
}

void TutteWeights::angleApproxBorder( vector<tuple3f> & outerPos , vector<int> & border
			, mesh & m)
{
	vector<float> angles, lambdas; //length next segment over last segment
	outerPos.clear();

	int loopsz= border.size();//, prev, next;
//	float angle, length, sum, scale_factor;

	angles_lambdas(angles,lambdas,border,m);
/*	sum = 0;


	for(bdr =0; bdr < loopsz; bdr++){
		prev = (bdr == 0? loopsz-1 : bdr-1);
		next = (bdr == loopsz-1? 0 : bdr+1);

		angle = meshOperation::sumAnglesWheel(border[prev], 
			border[bdr], border[next], m);
		sum+= angle;
		angles.push_back(angle); //was pi -angle...

		length = (m.getVertices()[border[next]] - m.getVertices()[border[bdr]]).norm()/
			(m.getVertices()[border[bdr]] - m.getVertices()[border[prev]]).norm();
		lambdas.push_back(length);

	}
	scale_factor = (loopsz-2) * PI / sum;
	for(bdr =0; bdr < loopsz; bdr++){
		angles[bdr] = PI - angles[bdr] * scale_factor;
	}*/

	//setUp Matrix;
	pardisoMatrix mat;

	setUp_angleMat(angles, lambdas, mat);
	pardisoSolver solver = pardisoSolver(pardisoSolver::MT_STRUCTURALLY_SYMMETRIC,
		pardisoSolver::SOLVER_ITERATIVE,2);
//		solver.checkMatrix(pardisoSolver::MT_STRUCTURALLY_SYMMETRIC, mat);
	solver.setMatrix(mat,1);

	//setup x,b
	vector<double> x,b;
	x.reserve(2*loopsz);
	b.reserve(2*loopsz);
	for(int k=0; k < 2*loopsz; k++){
		b.push_back(0);
		x.push_back(0);
	}
	b[1] = (m.getVertices()[border[1]]-m.getVertices()[border[0]]).norm();
	b[loopsz+1] = (m.getVertices()[border[1]]-m.getVertices()[border[0]]).norm();

	solver.solve(&(x[0]),&(b[0]));


	
	for(int k=0; k < loopsz; k++){
		outerPos.push_back(tuple3f(x[k], x[k+loopsz],0));
	}
	meshOperation::normalizeTexture(outerPos);

}

//////////////////////////////////////////////////////////////////////////
//0 based notation. weights for angle based border
//////////////////////////////////////////////////////////////////////////
float TutteWeights::angleMat( int ind1, int ind2, vector<float> & angles, vector<float> & lambdas )
{
	int sz = angles.size();
	int i = (ind1<ind2?ind1:ind2);
	int j= (ind1<ind2?ind2:ind1);
	float alpha_im1 =angles[(i-1+sz)% sz];
	float alpha_i = angles[i% sz];
	float alpha_ip1=angles[(i+1)% sz];
	float lambda_im1 =lambdas[(i-1+sz)% sz];
	float lambda_i = lambdas[ i % sz];
	float lambda_ip1=lambdas[(i+1)% sz];

	float out = 0;
	if(i<sz &&j < sz){
		if((i-2 + sz)%sz == j){
			out = lambda_im1 * cos(alpha_im1);
		}
		else if( (i-1 + sz)% sz == j){
			out = -lambda_im1*cos(alpha_im1)-1-lambda_i*cos(alpha_i) -(lambda_i)*(lambda_i);
		}
		else if(i == j){
			out = 2 + 2*lambda_i*cos(alpha_i) +(lambda_i)*(lambda_i) +(lambda_ip1)*(lambda_ip1);
		}
		else if((i+1)% sz == j){
			out = -lambda_i*cos(alpha_i)-1-lambda_ip1*cos(alpha_ip1) -(lambda_ip1)*(lambda_ip1);
		}
		else if((i+2)% sz == j){
			out = lambda_ip1*cos(alpha_ip1);
		}
	}
	else if(i<sz && j >=sz){
		//changed all signs!!!!!!!!!
		if((i-2 +sz)%sz + sz== j){
			out = -lambda_im1*sin(alpha_im1);
		}
		else if( (i-1+sz)% sz + sz == j){
			out = +lambda_im1*sin(alpha_im1)+lambda_i*sin(alpha_i);
		}
		else if(i + sz == j){
			out = 0;
		}
		else if((i+1)% sz + sz == j){
			out = -lambda_i*sin(alpha_i) - lambda_ip1*sin(alpha_ip1);
		}
		else if( (i+2)% sz + sz == j){
			out = +lambda_ip1*sin(alpha_ip1);
		}
	}
	else if(i>=sz && j >=sz){
		if((i-2+sz)% sz + sz== j){
			out = lambda_im1*cos(alpha_im1);
		}
		else if((i-1 + sz)% sz + sz == j){
			out = -lambda_im1*cos(alpha_im1)-1-lambda_i*cos(alpha_i) -(lambda_i)*(lambda_i);
		}
		else if(i == j){
			out = 2 + 2*lambda_i*cos(alpha_i) +(lambda_i*lambda_i) + lambda_ip1*lambda_ip1;
		}
		else if((i+1)% sz + sz == j){
			out = -lambda_i*cos(alpha_i)-1-lambda_ip1*cos(alpha_ip1) -lambda_ip1*lambda_ip1;
		}
		else if((i+2)% sz + sz == j){
			out = lambda_ip1*cos(alpha_ip1);
		}
	}

	if((i==0&&j==0)|| (i==1 &&j==1)||(i==sz &&j==sz)||(i==sz+1 &&j==sz+1) ){
		out = out+1;
	}

	return out;
}

void TutteWeights::setUp_angleMat( vector<float> &angles, vector<float> &lambdas, pardisoMatrix & target )
{
	int sz = angles.size();
	int par_j,j;
	target.ia.clear();
	target.ja.clear();
	target.a.clear();

	for(int i= 0; i< 2*sz; i++){
		target.ia.push_back(target.a.size()+1);
		if(i%sz-2>=0 && i%sz+2 < sz){
			for(par_j=i-2; par_j <=i+2; par_j++){
				j= (par_j+sz)%sz; //% of negatives is ill defined
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
			for(par_j=i-2; par_j <=i+2; par_j++){
				j= (par_j+sz)%sz + sz;
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
		}
		//stupid indices have to be ordered ascendingly. just stupid.
		else if(i%sz-2 <0){

			for(par_j=0; par_j <=i%sz+2; par_j++){
				j= (par_j+sz)%sz; //% of negatives is ill defined
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
			for(par_j=i%sz-2; par_j <0; par_j++){
				j= (par_j+sz)%sz; //% of negatives is ill defined
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}

			for(par_j=0; par_j <=i%sz+2; par_j++){
				j= (par_j+sz)%sz + sz;
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
			for(par_j=i%sz-2; par_j <0; par_j++){
				j= (par_j+sz)%sz + sz;
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}

		}
		else if(i%sz+2>=sz){
			for(par_j=sz; par_j <=i%sz+2; par_j++){
				j= (par_j+sz)%sz; //% of negatives is ill defined
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
			for(par_j=i%sz-2; par_j <sz; par_j++){
				j= (par_j+sz)%sz; //% of negatives is ill defined
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}

			for(par_j=sz; par_j <=i%sz+2; par_j++){
				j= (par_j+sz)%sz + sz;
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
			for(par_j=i%sz-2; par_j <sz; par_j++){
				j= (par_j+sz)%sz + sz;
				target.ja.push_back(j+1);
				target.a.push_back(angleMat(i,j,angles,lambdas));
			}
		}
	}
	target.ia.push_back(target.a.size()+1);
}

void TutteWeights::setUp_fullAndAngleBased( vector<tuple3f> & outerPos ,
		vector<int> & border, vector<int> & loops, 
		mesh & m, pardisoMatrix & target )
{

}

double TutteWeights::mulitBorderWeights( int i, int j, mesh & m, vector<int> & neighbors_i, vector<int> & nbr_fc_i, vector<vector<int>> & border )
{
	throw std::runtime_error("not implemented, dude!");
}

/*
* A weight function for matrix values on additional (INNER) borders.
* will make constrictions for the produced border to be negatively oriented!!!
*/
float TutteWeights::turningWeight( int i, int j, vector<float> &angles, vector<float> &lambdas )
{
	int sz = angles.size();
	if(i>=2*sz || j>=2*sz|| i<0 ||j<0){
		throw std::runtime_error("Assertion Failed");
	}
	/*if(i==j){
		return 1;
	}
	if((i<sz&&j<sz )|| (i>sz&&j>sz)){
		if(j%sz == (i -1+sz)%sz){
			return -(1+lambdas[(i -1+sz)%sz]*cos(angles[(i -1+sz)%sz]));
		}
		else if(j%sz== (i-2+sz)%sz){
			return lambdas[(i -1+sz)%sz]*cos(angles[(i -1+sz)%sz]);
		}
	}
	else if(j==sz +(i-1+sz)%sz){
		return lambdas[(i -1+sz)%sz]*sin(angles[(i -1+sz)%sz]);
	}
	else if(j==sz +(i-2+sz)%sz){
		return -lambdas[(i -1+sz)%sz]*sin(angles[(i -1+sz)%sz]);
	}
	else if(i==sz+(j-1+sz)%sz){
		return lambdas[(i-1)%sz]*sin(angles[(i-1)%sz]);
	}
	else if(i==sz+(j-2+sz)%sz){
		return -lambdas[(i-1)%sz]*sin(angles[(i-1)%sz]);
	}
	else if(i+sz==j || j+sz ==i){
		return 0;
	}
	return 0;*/
	if((i<sz&&j<sz )|| (i>=sz&&j>=sz)){
		if(j%sz==(i+1)%sz){
			return 1;
		}
		else if(j==i){
			return -(1+lambdas[i%sz]*cos(-angles[i%sz]));
		}
		else if(j%sz== (i-1+sz)%sz){
			return lambdas[i%sz]*cos(-angles[i%sz]);
		}
	}
	else if(j==sz +i){
		return lambdas[i]*sin(-angles[i]);
	}
	else if(j==sz +(i-1+sz)%sz){
		return -lambdas[i]*sin(-angles[i]);
	}
	else if(j== i-sz ){
		return -lambdas[i%sz]*sin(-angles[i%sz]);
	}
	else if(j== (i-1+sz)%sz){
		return lambdas[i%sz]*sin(-angles[i%sz]);
	}
	else if(j== (i+1)%sz +sz || (i +1)%sz == j){
		return 0;
	}
	return 0;
}


