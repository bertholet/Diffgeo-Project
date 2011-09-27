#pragma once
#include "mesh.h"
#include "drawing2d2.h"
#include <vector>
#include "curvVisualizingMesh.h"

/************************************************************************/
/* Allow the setting of position of the inner Border.                                                                     */
/************************************************************************/
class TutteDemo2
{
public:
	static mesh * bunny;
	static drawing2d2 * texMesh;
	squareTexture * tex;
	borderMarkupMap * cmap;

	TutteDemo2(void);
	~TutteDemo2(void);

	void run( mesh &m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/));

//	void loopTex();
	void loopBorder();
	void loop();

private:
	void display( mesh &m, 	vector<vector<int>> &border) ;
};
