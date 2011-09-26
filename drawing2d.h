#pragma once
#include "tuple3.h"
#include "squareTexture.h"
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

class drawing2d
{
	class circle{
	public:
		float x,y;
		float r;
	};
private:
	std::vector<tuple3f> tex;
	std::vector<tuple3i> faces;
	std::vector<std::vector<int>> bordr;
	int size_x, size_y;
	squareTexture * sqTex;
	std::vector<circle> circles;
	circle * actual_circle;

public:
	drawing2d(std::vector<tuple3f> & pos, std::vector<tuple3i> & faces ,
		std::vector<std::vector<int>> & border, squareTexture * sqtex, int szx, int szy);
	~drawing2d(void);

	void initGLParams(squareTexture * textur, GLuint tex_id);
	void glTexDisplay();
	void draw(int x, int y);

	void moveCircleTo(int x, int y);
	void scaleCircle(float f);
	void newCircle(int x, int y);



};

namespace d2dCallBack{
	void mouseCallback(int button, int state,
		int x, int y);
	void mouseCallback_borderSelection(int button, int state,
		int x, int y);
	void mouseMotion(int x, int y);
	void processNormalKeys(unsigned char key, int x, int y);
}
