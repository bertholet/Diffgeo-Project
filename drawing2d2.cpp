#include "StdAfx.h"
#include "drawing2d2.h"
#include "TutteWeights.h"
#include "meshOperation.h"
#include "TutteEmbedding.h"

namespace d2d2CallBack{
	drawing2d2 * actualDrawing = NULL;
}


drawing2d2::~drawing2d2(void)
{
}
drawing2d2::drawing2d2( mesh &m, borderMarkupMap & cmap,
					 std::vector<std::vector<int>> & border,
					 squareTexture * sqtex, int szx, int szy)
{
	this->cmap = &cmap;
	this->bordr = border;
	this->bunny = &m;
	size_x = szx;
	size_y = szy;
	sqTex = sqtex;
	d2d2CallBack::actualDrawing = this;
	for(unsigned int i = 0; i < border.size(); i++){
		borderPos.push_back(vector<tuple3f>());
	}

}


void drawing2d2::glBorderDefDisplay()
{
	glDisable(GL_TEXTURE_2D);
	for(unsigned int i =0; i < circles.size(); i++){
		if(i == actual_circleIdx ){
			if(circles[i].orientation >0){
				glColor3f(0.f,0 ,1.f);
			}
			else{
				glColor3f(0.f,1 ,0.f);
			}
		}
		else{
			if(circles[i].orientation >0){
				glColor3f(0.5f,0 ,0.1f);
			}
			else{
				glColor3f(0.f,0.5f ,0.1f);
			}
		}
		glBegin(GL_LINE_LOOP);
		glVertex2f( circles[i].x-circles[i].r, circles[i].y-circles[i].r);
		glVertex2f( circles[i].x-circles[i].r, circles[i].y+circles[i].r);
		glVertex2f( circles[i].x+circles[i].r, circles[i].y+circles[i].r);
		glVertex2f( circles[i].x+circles[i].r, circles[i].y-circles[i].r);
		glEnd();

		glBegin(GL_LINE_LOOP);
		for(unsigned int j = 0; j < borderPos[i].size(); j++){
			glVertex2fv( (GLfloat *) & (borderPos[i][j]));
		}
		glEnd();

	}
	glEnable(GL_TEXTURE_2D);
}

void drawing2d2::glTexDisplay()
{
	vector<tuple3f> & tex = bunny->getTexCoords();
	vector<tuple3i> & faces = bunny->getFaces();
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < faces.size(); i++){
		glTexCoord2fv((GLfloat *) &(tex)[faces[i].a]);
		glVertex2fv( (GLfloat *) & (tex)[(faces)[i].a]);

		glTexCoord2fv((GLfloat *) & tex[(faces)[i].b]);
		glVertex2fv( (GLfloat *) & tex[(faces)[i].b]);


		glTexCoord2fv((GLfloat *) &(tex)[(faces)[i].c]);
		glVertex2fv( (GLfloat *) & (tex)[(faces)[i].c]);
	}
	glEnd();

	glColor3f(1.f,0.f,0.f);
	glDisable(GL_TEXTURE_2D);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		//glVertex3fv( (GLfloat *) & vertices[i]);
		glVertex2fv( (GLfloat *) & (tex)[(faces)[i].a]);

		glVertex2fv((GLfloat *) & (tex)[(faces)[i].b]);

		glVertex2fv((GLfloat *) & (tex)[(faces)[i].c]);
		glEnd();
	}
	for(unsigned int i =0; i < bordr.size(); i++){
		glColor3f(0.f,0.f+i%2,(0.f+(i)%3)/2);
		glBegin(GL_LINE_LOOP);
		for(unsigned j = 0; j < bordr[i].size(); j++){
			glVertex2fv( (GLfloat *) & (tex)[bordr[i][j]]);
		}
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);
}

void drawing2d2::initGLParams(squareTexture * textur, GLuint tex_id)
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


void drawing2d2::moveCircleTo( int x, int y )
{
	float fx = ((0.f + x)/size_x );
	float fy =  ((0.f + y)/size_y );

	fx = (fx <0 ? 0 : fx);
	fy = (fy >1 ? 0 : 1-fy);

	circles[actual_circleIdx].x= fx;
	circles[actual_circleIdx].y = fy;

	meshOperation::normalizeTexture(borderPos[actual_circleIdx], circles[actual_circleIdx].x,
		circles[actual_circleIdx].y, circles[actual_circleIdx].r);
}

void drawing2d2::newCircle( int x, int y )
{
	if(circles.size() >= bordr.size()){
		return;
	}
	circles.push_back(circle());
	actual_circleIdx = circles.size()-1;
	moveCircleTo(x,y);
	circles[actual_circleIdx].r = 0.1f;
	circles[actual_circleIdx].orientation = 1;
	cmap->markedBorder = actual_circleIdx;

	calcActualBorder();
}

void drawing2d2::scaleCircle( float f )
{
	circles[actual_circleIdx].r+=f;
	meshOperation::normalizeTexture(borderPos[actual_circleIdx], circles[actual_circleIdx].x,
		circles[actual_circleIdx].y, circles[actual_circleIdx].r);
}

void drawing2d2::nextCircle()
{
	actual_circleIdx++;
	actual_circleIdx= actual_circleIdx% circles.size();
	cmap->markedBorder = actual_circleIdx;
}

void drawing2d2::prevCircle()
{
	actual_circleIdx--;
	actual_circleIdx= (actual_circleIdx + circles.size())% circles.size();
	cmap->markedBorder = actual_circleIdx;
}

void drawing2d2::calcActualBorder()
{

	borderPos[actual_circleIdx].clear();
	TutteWeights::circleBorder(borderPos[actual_circleIdx], bordr[actual_circleIdx], *bunny);
	meshOperation::normalizeTexture(borderPos[actual_circleIdx], circles[actual_circleIdx].x,
		circles[actual_circleIdx].y, circles[actual_circleIdx].r);
	circles[actual_circleIdx].orientation = 1;
}

void drawing2d2::calcTexturePos()
{
	TutteEmbedding embedding;
	embedding.calcTexturePos_multiBorder(*bunny,borderPos,bordr, TutteWeights::unnormed_meanvalue_weights);
}

void drawing2d2::switchOrientation()
{
	meshOperation::mirrorX(borderPos[actual_circleIdx], circles[actual_circleIdx].x);
	circles[actual_circleIdx].orientation *=-1;
}



//////////////////////////////////////////////////////////////////////////
//callbacks
//////////////////////////////////////////////////////////////////////////

void d2d2CallBack::mouseCallback_borderSelection( int button, int state, int x, int y )
{
	actualDrawing->moveCircleTo(x,y);
}

void d2d2CallBack::mouseMotion( int x, int y )
{
	actualDrawing->moveCircleTo(x,y);
}

void d2d2CallBack::processNormalKeys( unsigned char key, int x, int y )
{
	if(key == 'n'){
		actualDrawing->newCircle(x,y);
	}

	else if(key == 'p'){
		actualDrawing->scaleCircle(0.01f);
	}
	else if(key == 'l'){
		actualDrawing->scaleCircle(-0.01f);
	}
	else if(key == 'u'){
		actualDrawing->prevCircle();
	}
	else if(key == 'i'){
		actualDrawing->nextCircle();
	}
	else if(key == 'c'){
		actualDrawing->calcActualBorder();
	}
	else if(key == 'x'){
		actualDrawing->calcTexturePos();
	}
	else if(key == 'q'){
		actualDrawing->switchOrientation();
	}

}