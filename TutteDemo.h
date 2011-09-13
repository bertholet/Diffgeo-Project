#pragma once
#include "StdAfx.h"
#include "mesh.h"
#include "squareTexture.h"
#include "drawing2d.h"

class TutteDemo
{
public:
	static mesh * bunny;
	static drawing2d * texMesh;
	squareTexture * tex;
	TutteDemo(void);
	~TutteDemo(void);
	void run(mesh &m);
	void run( mesh &m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/) );
	void run( mesh &m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/),
		void (*getBorderPos ) (vector<tuple3f> & /*outerPos*/, 
		vector<int> & /*border*/, 
		vector<int> & /*loops*/, 
		mesh & /*m*/));


	void loop();
	void loopTex(void);
};
