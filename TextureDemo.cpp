#include "StdAfx.h"
#include "TextureDemo.h"
#include "glWindowHelper.h"
#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>

namespace texDemo{
	bool spacePressed =false;

	void processNormalKeys(unsigned char key, int x, int y) {

		if (key == ' ')
			spacePressed = !spacePressed;

		if (key == 'd')
			TextureDemo::bunny->rotX(0.05f);

		if (key == 'a')
			TextureDemo::bunny->rotX(-0.05f);

		if (key == 'w')
			TextureDemo::bunny->rotY(0.05f);

		if (key == 's')
			TextureDemo::bunny->rotY(-0.05f);
	}

	void texture(void){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		TextureDemo::bunny->glTexDisplay();
		//bunny.glDisplay();
		glFlush();

		//glEnable(GL_DEPTH_TEST);
		glutPostRedisplay();
	}
}

mesh * TextureDemo::bunny = NULL;

TextureDemo::TextureDemo(void)
{
	this->tex = new squareTexture();
}

TextureDemo::~TextureDemo(void)
{
	delete tex;
}

void TextureDemo::run(mesh &_mesh){
	this->bunny = & _mesh;

	glWindowHelper::glWindow(450,450, texDemo::texture, texDemo::processNormalKeys);

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->szx,tex->szy,0,GL_RGBA, GL_FLOAT, 
		&(tex->checkboard[0]));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glutMainLoop();
}

