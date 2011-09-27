#pragma once
#include "tuple3.h"
#include "squareTexture.h"
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "mesh.h"
#include "curvVisualizingMesh.h"

class drawing2d
{

private:
	mesh * bunny;
	std::vector<tuple3f> * tex;
	std::vector<tuple3i> * faces;
	std::vector<std::vector<int>> bordr;
	int size_x, size_y;
	squareTexture * sqTex;


public:
	drawing2d(mesh & m ,
		std::vector<std::vector<int>> & border, squareTexture * sqtex, int szx, int szy);

	~drawing2d(void);

	void initGLParams(squareTexture * textur, GLuint tex_id);
	void glTexDisplay();
	void glBorderDefDisplay();
	void draw(int x, int y);

};

namespace d2dCallBack{
	void mouseCallback(int button, int state,
		int x, int y);
}
