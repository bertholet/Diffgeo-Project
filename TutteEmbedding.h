#pragma once
#include "mesh.h"
#include <vector>
#include <algorithm>
#include "pardisoMatrix.h"

class TutteEmbedding
{
public:
	TutteEmbedding(void);
	~TutteEmbedding(void);
	void calcTexturePos(mesh &m);
	void calcTexturePos(mesh &m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/));
	void getBorderPos( vector<tuple3f> & outerPos, int sz);
	void setUp( pardisoMatrix &mat, vector<int> &border, 
		mesh & m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/));

	void setUpX( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz );
	void setUpY( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz );
};


