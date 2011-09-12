#include "StdAfx.h"
#include "drawing2d.h"


drawing2d::drawing2d( std::vector<tuple3f> & pos, std::vector<tuple3i> & facs)
{
	tex = pos;
	faces = facs;
}

drawing2d::~drawing2d(void)
{
}

void drawing2d::glTexDisplay()
{
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++){
		glTexCoord2fv((GLfloat *) &tex[faces[i].a]);
		glVertex2fv( (GLfloat *) & tex[faces[i].a]);

		glTexCoord2fv((GLfloat *) &tex[faces[i].b]);
		glVertex2fv( (GLfloat *) & tex[faces[i].b]);


		glTexCoord2fv((GLfloat *) &tex[faces[i].c]);
		glVertex2fv( (GLfloat *) & tex[faces[i].c]);
	}
	glEnd();

	glColor3f(1.f,0.f,0.f);
	glDisable(GL_TEXTURE_2D);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		//glVertex3fv( (GLfloat *) & vertices[i]);
		glVertex2fv( (GLfloat *) & tex[faces[i].a]);

		glVertex2fv((GLfloat *) & tex[faces[i].b]);

		glVertex2fv((GLfloat *) & tex[faces[i].c]);
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
