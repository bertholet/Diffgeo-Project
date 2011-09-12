#pragma once
#include "StdAfx.h"
#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "TutteDemo.h"
#include "glWindowHelper.h"
#include "TutteEmbedding.h"
#include "tutteWeights.h"

namespace tutDemo{
	bool spacePressed =false;
	TutteDemo * actualDemo;

	
	void processNormalKeys(unsigned char key, int x, int y) {

		if (key == ' ')
			spacePressed = !spacePressed;

		else if (key == 'd')
			actualDemo->bunny->rotX(0.05f);

		else if (key == 'a')
			actualDemo->bunny->rotX(-0.05f);

		else if (key == 'w')
			actualDemo->bunny->rotY(0.05f);

		else if (key == 's')
			actualDemo->bunny->rotY(-0.05f);

		else if (key == 'z'){
			actualDemo->bunny->scaleXYZ(1.05f);
		}
		else if (key == 'u'){
			actualDemo->bunny->scaleXYZ(0.95f);
		}
	}

	void callback(void){
		actualDemo->loop();
	}

	void callback_sub(void){
		actualDemo->loopTex();
	}

}

mesh * TutteDemo::bunny = NULL;
drawing2d * TutteDemo::texMesh = NULL;

TutteDemo::TutteDemo(void)
{
	tutDemo::actualDemo = this;
	this->tex = new squareTexture();
}

TutteDemo::~TutteDemo(void)
{
	tutDemo::actualDemo = NULL;
	delete tex;
}

void TutteDemo::loop()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny->glTexDisplay();
	//bunny.glDisplay();
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void TutteDemo::loopTex()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	texMesh->glTexDisplay();
	//texMesh->glDisplayLines();
	//bunny.glDisplay();
	glFlush();
//	glutSwapBuffers();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void TutteDemo::run( mesh &m )
{

	this->bunny = &m;
	TutteEmbedding embedding;
	embedding.calcTexturePos(m);

	glWindowHelper::glWindow(450,450, tutDemo::callback, tutDemo::processNormalKeys);

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

void TutteDemo::run( mesh &m, double (*weights ) (int, int,mesh &, vector<int>& /*nbr_i*/,
		vector<int>&/*fc_i*/, vector<int>& /*border*/) )
{
	run(m,weights, TutteWeights::circleBorder);
}

void TutteDemo::run( mesh &m, 
					double (*weights ) (int, int,mesh &, 
						vector<int>& /*nbr_i*/, vector<int>&/*fc_i*/, 
						vector<int>& /*border*/), 
					void (*getBorderPos ) (vector<tuple3f> & /*outerPos*/, 
					vector<int> & /*border*/, vector<int> & /*loops*/, 
					mesh & /*m*/) )
{
	//m.faces[-1];
	this->bunny = &m;
	TutteEmbedding embedding;
	embedding.calcTexturePos(m, weights, getBorderPos);

	int window1 = glWindowHelper::glWindow(450,450, tutDemo::callback, tutDemo::processNormalKeys);

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

	//////////////////////////////////////////////////////////////////////////
	//subwindow
	//////////////////////////////////////////////////////////////////////////
	int subwindow1 = glutCreateSubWindow(window1,10,10,200,200);

	this->texMesh = new drawing2d(m.getTexCoords(),m.getFaces());
	glutDisplayFunc(tutDemo::callback_sub);
	this->texMesh->initGLParams(tex,tex_id);

	glutMainLoop();
	delete this->texMesh;

}

