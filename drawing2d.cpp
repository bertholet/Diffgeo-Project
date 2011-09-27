#include "StdAfx.h"
#include "drawing2d.h"




namespace d2dCallBack{
	drawing2d * actualDrawing = NULL;
}

drawing2d::drawing2d( mesh &m, 
					 std::vector<std::vector<int>> & border,
					 squareTexture * sqtex, int szx, int szy)
{
	this->bordr = border;
	this->bunny = &m;
	tex = &(bunny->getTexCoords());
	faces = &(bunny->getFaces());
	size_x = szx;
	size_y = szy;
	sqTex = sqtex;
	d2dCallBack::actualDrawing = this;
	
}

drawing2d::~drawing2d(void)
{
	d2dCallBack::actualDrawing = NULL;
}

void drawing2d::glTexDisplay()
{
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces->size(); i++){
		glTexCoord2fv((GLfloat *) &(*tex)[(*faces)[i].a]);
		glVertex2fv( (GLfloat *) & (*tex)[(*faces)[i].a]);

		glTexCoord2fv((GLfloat *) &(*tex)[(*faces)[i].b]);
		glVertex2fv( (GLfloat *) & (*tex)[(*faces)[i].b]);


		glTexCoord2fv((GLfloat *) &(*tex)[(*faces)[i].c]);
		glVertex2fv( (GLfloat *) & (*tex)[(*faces)[i].c]);
	}
	glEnd();

	glColor3f(1.f,0.f,0.f);
	glDisable(GL_TEXTURE_2D);
	for (unsigned int i = 0; i < faces->size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		//glVertex3fv( (GLfloat *) & vertices[i]);
		glVertex2fv( (GLfloat *) & (*tex)[(*faces)[i].a]);

		glVertex2fv((GLfloat *) & (*tex)[(*faces)[i].b]);

		glVertex2fv((GLfloat *) & (*tex)[(*faces)[i].c]);
		glEnd();
	}
	for(unsigned int i =0; i < bordr.size(); i++){
		glColor3f(0.f,0.f+i%2,(0.f+(i)%3)/2);
		glBegin(GL_LINE_LOOP);
		for(unsigned j = 0; j < bordr[i].size(); j++){
			glVertex2fv( (GLfloat *) & (*tex)[bordr[i][j]]);
		}
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);
}

void drawing2d::initGLParams(squareTexture * textur, GLuint tex_id)
{
	//tex stuff
	//	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textur->szx,textur->szy,0,GL_RGBA, GL_FLOAT, 
		&(textur->checkboard[0]));
	glEnable(GL_TEXTURE_2D);
	//tex stuff ends

	//	glEnable(GL_DEPTH_TEST);
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void drawing2d::draw( int x, int y )
{
	int fx = (int) ((0.f + x)/size_x * sqTex->szx);
	int fy = (int) ((0.f + y)/size_y * sqTex->szy);
	fx = (fx >= sqTex->szx ? sqTex->szx -1 : fx);
	fy = (fy >= sqTex->szy ? sqTex->szy -1 : fy);
	fx = (fx <0 ? 0 : fx);
	fy = sqTex->szy -1 -(fy <0 ? 0 : fy);
	
	sqTex->draw(fx,fy);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sqTex->szx,sqTex->szy,0,GL_RGBA, GL_FLOAT, 
		&(sqTex->checkboard[0]));
}



//////////////////////////////////////////////////////////////////////////
//callbacks
//////////////////////////////////////////////////////////////////////////

void d2dCallBack::mouseCallback( int button, int state, int x, int y )
{
	actualDrawing->draw(x,y);

}




