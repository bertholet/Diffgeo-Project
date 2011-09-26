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
	double mulitBorderWeights( int i, int j, mesh & m, 
		vector<int> & neighbors_i, 
		vector<int> & nbr_fc_i, 
		vector<vector<int>> & border );


	void circleBorder( vector<tuple3f> & outerPos , vector<int> & border, mesh &);
	void distWeightCircBorder( vector<tuple3f> & outerPos , vector<int> & border, mesh &);
	void angleApproxBorder( vector<tuple3f> & outerPos , vector<int> & border, mesh & m);

	void setUp_angleMat(vector<float> &angles, vector<float> &lambdas, pardisoMatrix & target);
	float angleMat(int i, int j, vector<float> &angles, vector<float> &lambdas);
	float turningWeight(int i, int j, vector<float> &angles, vector<float> &lambdas);

	void angles_lambdas(vector<float> &angles, vector<float> &lambdas, vector<int> & border, mesh & m);
	//the angles of the inner  borders will be scaled to n-2 pi of inner borders to n+2 pi.
	void angles_lambdas( vector<vector<float>> &angles, vector<vector<float>> &lambdas, 
		vector<vector<int>> & border, int outBorder, mesh & m);
	//will norm the angles such that they sum to sum
	void angles_lambdas(vector<float> &angles, vector<float> &lambdas, vector<int> & border,float sum, mesh & m);

	void setUp_fullAndAngleBased(vector<tuple3f> & outerPos , vector<int> & border, 
		vector<int> & loops, mesh &, pardisoMatrix & target);
}
#endif