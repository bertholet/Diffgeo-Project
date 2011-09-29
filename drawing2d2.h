#pragma once
#include "tuple3.h"
#include "squareTexture.h"
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "mesh.h"
#include "curvVisualizingMesh.h"

class drawing2d2
{
	class circle{
	public:
		float x,y;
		float r;
		int orientation;
	};

private: 
	mesh * bunny;
	std::vector<std::vector<int>> bordr;
	int size_x, size_y;
	squareTexture * sqTex;
	std::vector<circle> circles;
	int actual_circleIdx;
	borderMarkupMap * cmap;
	std::vector<std::vector<tuple3f>> borderPos;


public:
	drawing2d2( mesh &m, borderMarkupMap & cmap,
		std::vector<std::vector<int>> & border,
		squareTexture * sqtex, int szx, int szy);
	~drawing2d2(void);


	void moveCircleTo(int x, int y);
	void scaleCircle(float f);
	void newCircle(int x, int y);
	void nextCircle();
	void prevCircle();
	void calcActualBorder();
	void switchOrientation();

	void initGLParams(squareTexture * textur, GLuint tex_id);
	void glTexDisplay();
	void glBorderDefDisplay();
	void calcTexturePos();
};


namespace d2d2CallBack{
	void mouseCallback_borderSelection(int button, int state,
		int x, int y);
	void mouseMotion(int x, int y);
	void processNormalKeys(unsigned char key, int x, int y);
}
