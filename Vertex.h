#pragma once
#include<windows.h>
#include <GL/glew.h>

class Vertex
{

public:
	GLint x, y;
	Vertex(void);
	Vertex(int, int);
	~Vertex(void);

	void set(int cx, int cy){x = cx; y = cy;}
};
