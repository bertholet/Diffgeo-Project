#pragma once

#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>

class glWindowHelper
{
public:
	static int glWindow(int width, int height, void (*glLoop)(void),void (*keyProcessing)(unsigned char, int, int)){
		int argc = 0;
		char ** argv = 0;
		glutInit(&argc, (char **) argv);
		glutInitWindowPosition(300, 100); 
		glutInitWindowSize(width,height);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //GLUT_DOUBLE for double buffering.

		int a = glutCreateWindow("A GL Window");
		glutDisplayFunc(glLoop);

		glutKeyboardFunc(keyProcessing);
		glewInit();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(100.0,GLdouble(width)/height, 2.0, 10000.0);
		glMatrixMode(GL_MODELVIEW);

		//glutMainLoop();
		return a;
	}
};
