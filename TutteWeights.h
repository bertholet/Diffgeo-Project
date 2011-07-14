#pragma once
#ifndef UNIFORM
#define UNIFORM
#include "mesh.h"
#include <vector>


namespace TutteWeights{
	double uniform_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i,vector<int> & border);
	double unnormed_meanvalue_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i, vector<int> & border);
	double cotan_weights(int i, int j, mesh & m, vector<int> & neighbors_i,
		vector<int> & neighbor_fc_i, vector<int> & border);
}
#endif