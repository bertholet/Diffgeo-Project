#pragma once
#include "StdAfx.h"
#include "mesh.h"
#include "squareTexture.h"

class TutteDemo
{
public:
	static mesh * bunny;
	squareTexture * tex;
	TutteDemo(void);
	~TutteDemo(void);
	void run(mesh &m);
	void run( mesh &m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/) );

	void loop();
};
