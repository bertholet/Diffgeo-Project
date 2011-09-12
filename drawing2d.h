#pragma once
#include "tuple3.h"
#include "squareTexture.h"
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>

class drawing2d
{
private:
	std::vector<tuple3f> tex;
	std::vector<tuple3i> faces;
public:
	drawing2d(std::vector<tuple3f> & pos, std::vector<tuple3i> & faces );
	~drawing2d(void);

	void initGLParams(squareTexture * textur, GLuint tex_id);
	void glTexDisplay();

};
