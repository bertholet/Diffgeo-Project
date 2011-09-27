#include "StdAfx.h"
#include "drawing2d2.h"
#include "TutteWeights.h"

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

}


void drawing2d2::glBorderDefDisplay()
{
	glDisable(GL_TEXTURE_2D);
	for(unsigned int i =0; i < circles.size(); i++){
		if(i == actual_circleIdx ){
			glColor3f(0.f,0 ,1.f);
		}
		else{
			glColor3f(0.5f,0 ,0.1f);
		}
		glBegin(GL_LINE_LOOP);
		glVertex2f( circles[i].x-circles[i].r, circles[i].y-circles[i].r);
		glVertex2f( circles[i].x-circles[i].r, circles[i].y+circles[i].r);
		glVertex2f( circles[i].x+circles[i].r, circles[i].y+circles[i].r);
		glVertex2f( circles[i].x+circles[i].r, circles[i].y-circles[i].r);
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

	cmap->markedBorder = actual_circleIdx;
}

void drawing2d2::scaleCircle( float f )
{
	circles[actual_circleIdx].r+=f;
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
	TutteWeights::circleBorder()etc 
}



//////////////////////////////////////////////////////////////////////////
//callbacks
//////////////////////////////////////////////////////////////////////////

void d2d2CallBack::mouseCallback_borderSelection( int button, int state, int x, int y )
{

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
		actualDrawing->scaleCircle(0.005f);
	}
	else if(key == 'l'){
		actualDrawing->scaleCircle(-0.005f);
	}
	else if(key == 'u'){
		actualDrawing->prevCircle();
	}
	else if(key == 'i'){
		actualDrawing->nextCircle();
	}

}