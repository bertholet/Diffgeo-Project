#include "StdAfx.h"
#include "GLWindow.h"
#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>


GLWindow::GLWindow( int wid, int heigh )
{
	this->width = wid;
	this->height = heigh;
}

GLWindow::~GLWindow(void)
{
}



void GLWindow::display()
{
	int i = 0;
	char* c;
	glutInit(&i, &c);
	glutInitWindowPosition(300, 100); 
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //GLUT_DOUBLE for double buffering.

	int a = glutCreateWindow("GL Window");
	//glutDisplayFunc(renderScene5);
//	glutDisplayFunc(&GLWindow::innerLoop);
	glutKeyboardFunc(&GLWindow::processNormalKeys);

	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0,GLdouble(width)/height, 2.0, 10000.0);
	//glOrtho(0,WIDTH ,0, HEIGHT,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}
