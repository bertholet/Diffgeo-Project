#include "StdAfx.h"
#include "TutteDemo2.h"
#include "glWindowHelper.h"
#include "meshOperation.h"


namespace tutDemo2{
	bool spacePressed =false;
	TutteDemo2 * actualDemo;
	vector<int> windows;

	void processNormalKeys(unsigned char key, int x, int y) {

		if (key == ' ')
			spacePressed = !spacePressed;

		if(spacePressed){
			d2d2CallBack::processNormalKeys(key,x,y);
		}
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
		else if (key == 'm'){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, actualDemo->tex->szx,
				actualDemo->tex->szy,0,GL_RGBA, GL_FLOAT, 
				&(actualDemo->tex->checkboard[0]));
		}
	}

	void callback(void){
		actualDemo->loop();
	}

	void callback_sub(void){
		actualDemo->loopBorder();
	}

}

TutteDemo2::TutteDemo2(void)
{
	tutDemo2::actualDemo = this;
}

TutteDemo2::~TutteDemo2(void)
{
}

void TutteDemo2::run( mesh &m, 
		double (*weights ) (int, int,mesh &, 
			vector<int>& /*nbr_i*/, vector<int>&/*fc_i*/, 
			vector<int>& /*border*/))
{
	vector<vector<int>> border;
	meshOperation::getBorder(m,border);
	
	tex = new squareTexture();
	cmap = new borderMarkupMap(border);
	display(m,border);

	delete tex, cmap;
}

mesh * TutteDemo2::bunny = NULL;
drawing2d2 * TutteDemo2::texMesh = NULL;

void TutteDemo2::display( mesh &m,vector<vector<int>> & border ) 
{
	this->bunny = &m;
	int window1 = glWindowHelper::glWindow(450,450, tutDemo2::callback, tutDemo2::processNormalKeys);

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

	this->texMesh = new drawing2d2(m,*cmap,border, tex,190,190);
	this->texMesh->newCircle(0,0);
	glutDisplayFunc(tutDemo2::callback_sub);
	glutMouseFunc(d2d2CallBack::mouseCallback_borderSelection);
	glutMotionFunc(d2d2CallBack::mouseMotion);
	glutKeyboardFunc(d2d2CallBack::processNormalKeys);

	this->texMesh->initGLParams(tex,tex_id);

	glutMainLoop();
	delete this->texMesh;
}


void TutteDemo2::loopBorder()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	texMesh->glBorderDefDisplay();
	glFlush();
	glutPostRedisplay();
}

/*void TutteDemo2::loopTex()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	texMesh->glTexDisplay();
	glFlush();
	glutPostRedisplay();
}*/

void TutteDemo2::loop()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//bunny->glTexDisplay();
	glDisable(GL_TEXTURE_2D);
	bunny->glDisplay((colorMap&)*cmap);
	glEnable(GL_TEXTURE_2D);
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}
