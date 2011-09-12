#pragma once

#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>

namespace glWindowHelper
{
//public:
	int glWindow(int width, int height, void (*glLoop)(void),void (*keyProcessing)(unsigned char, int, int));


}

