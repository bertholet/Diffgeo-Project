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
	void calcTexturePos(mesh &m, 
		double (*weights ) (int, int,mesh &, 
			vector<int>& /*nbr_i*/,
			vector<int>&/*fc_i*/, 
			vector<int>& /*border*/),
		void (*getBorderPos ) (vector<tuple3f> & /*outerPos*/, 
			vector<int> & /*border*/, 
//			vector<int> & /*loops*/, 
			mesh & /*m*/));

	void calcTexturePos_multiBorder(mesh &m, 
		double (*weights ) (int, int,mesh &, 
		vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, 
		vector<int>& /*border*/));

/*	void getBorderPos( vector<tuple3f> & outerPos, int sz);
	void getBorderPos( vector<tuple3f> & outerPos , vector<int> & border, mesh & m);*/
	void setUp( pardisoMatrix &mat, vector<int> &border, 
		mesh & m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/));

	void setUp_multiBorder(pardisoMatrix &mat, vector<vector<int>> &border,
		vector<tuple3f> & outerPos, int outBorder, 
		vector<vector<float>> & angles,
		vector<vector<float>> & lambdas,
		mesh & m,
		double (*weights ) (int /*i*/, int /*j*/, mesh & , 
				vector<int>& /*neighbors_i*/,
				vector<int>& /*neighbor_faces_i*/,
				vector<int>& /*border*/));

	void setUpX( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz );
	void setUpY( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz );
	void setUpXY( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz );
	void setUpXY_reflex( vector<double>& b, vector<int> & border, 
		vector<tuple3f> & outerPos,
		vector<float> & angles, int sz );

private:
	int outerBorder(vector<vector<int>> &border, mesh & m);
	void calcOrdered( vector<std::pair<int,int>> & vertices, vector<int> & border, int borderIndex );
};

namespace pairComp {
	bool comparator ( const std::pair<int,int>& l, const std::pair<int,int>& r);
}


