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

	void loop();
};
