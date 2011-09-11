#pragma once
#ifndef UNIFORM
#define UNIFORM
#include "mesh.h"
#include <vector>
#include "pardisoMatrix.h"


namespace TutteWeights{
	double uniform_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i,vector<int> & border);
	double unnormed_meanvalue_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i, vector<int> & border);
	double cotan_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i, vector<int> & border);
	double cotan_weights_divAvor(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i, vector<int> & border);
	double cotan_weights_divAmix( int i, int j, mesh & m, 
		vector<int> & neighbors_i, 
		vector<int> & nbr_fc_i, 
		vector<int> & border );


	void circleBorder( vector<tuple3f> & outerPos , vector<int> & border, 
		vector<int> & loops, mesh &);
	void distWeightCircBorder( vector<tuple3f> & outerPos , vector<int> & border, 
		vector<int> & loops, mesh &);
	void angleApproxBorder( vector<tuple3f> & outerPos , vector<int> & border, 
		vector<int> & loops, mesh & m);

	void setUp_angleMat(vector<float> &angles, vector<float> &lambdas, pardisoMatrix & target);
	float angleMat(int i, int j, vector<float> &angles, vector<float> &lambdas);
}
#endif