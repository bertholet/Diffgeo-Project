// openGl_try3.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

//#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Vertex.h"
#include "OBIFileReader.h"
#include "matrixf.h"
#include "matrixFactory.h"
#include "mesh.h"
#include "curvVisualizingMesh.h"
#include "colorMap.h"
#include "ball.h"
#include "torus.h"
#include "Smoothing.h"
#include "SmoothingImplicitEuler.h"

#define HEIGHT 1000
#define WIDTH 1000

using namespace std;

void renderScene(void);
void idle(void);
int glutAWindow(void);

Vertex * points;
Vertex actualPoint;

vector<tuple3f> normals;
vector<tuple3f> vertices;
vector<tuple3f> faces;
mesh bunny;
curvColormap * cMap;
//curvColormap * cMap;
Smoothing s;

void smoothingDemo( int argc, _TCHAR* * argv ) 
{
	//bunny= mesh("C:/Users/Petje/workspace/RA/objfiles/venusm.obj", tuple3f(1.f,0.f,0.f), 1.f/1000);
	//bunny= mesh("C:/Users/bertholet/Dropbox/workspace/RA/objfiles/cow.obj", tuple3f(1.f,0.f,0.f), 3);
	//bunny = ball(2, 20);
	bunny = torus(2.f,1.f, 200, 500);
	bunny.addNormalNoise(0.1f);
	cMap = new curvColormap(bunny);
	//cMap = new gaussColormap(bunny);

	cout << cMap->additionalInfo() << "\n";
	cout << "volume is : " << Operator::volume(bunny) << " \n";

	glutInit(&argc, (char **) argv);
	glutAWindow();
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0,GLdouble(WIDTH)/HEIGHT, 2.0, 10000.0);
	//glOrtho(0,WIDTH ,0, HEIGHT,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();

	delete cMap;
}


void implicitEulerTests(void){
	bunny = torus(2.f,1.f, 20, 50);
	ImplicitEulerSmoothing smoother(bunny,0.1f, 0.1f);
}


int _tmain(int argc, _TCHAR* argv[])
{

	//smoothingDemo(argc, argv);
	implicitEulerTests();

	int a ;
	cout << " rhablabla";
	cin >> a;

	return 0;
}

void renderScene(void) {
	//glClear(GL_COLOR_BUFFER_BIT);
	/*glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();*/
	GLubyte c = rand() % 256;
	GLint sx = ( rand() % WIDTH );// 200 - 1;
	GLint sy = (rand() % HEIGHT );// 200 - 1;

	//cout << sx << "," << sy << ";";

	glColor3ub(c,c,c);

	glBegin(GL_LINES);
	glVertex2i(0,0);
	glVertex2i(sx,sy);
	glEnd();
	//glFlush();
	glutPostRedisplay();
}

void renderScene2(void){
	Vertex * temp;
	GLint mx ;
	GLint my ;


	glColor3ub(250,250,250);
	for(int i = 0; i < 100000; i++){
		temp = &points[rand()%3];
		mx = (actualPoint.x + temp->x)/2;
		my = (actualPoint.y + temp->y)/2;
		actualPoint.set(mx,my);
		//glClear() to clear all, glClearColor() to set the color on delete kann auch abgefragt werden.
		//glClear(Mask)
		glBegin(GL_POINTS);
		glVertex2iv((GLint *) &actualPoint);;
		glEnd();
	}
	glFlush();
	glutPostRedisplay();
	cout <<".";
}

float offst = 0;
void renderScene3(void){
	offst+=1;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(250,0,0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-12.f, 30.f,-40.f -offst);
		glVertex3f(-30.f, 12.f,-40.f-offst);
		glVertex3f(12.f, -30.f,-40.f-offst);
		glVertex3f(30.f, -12.f,-40.f-offst);
	glEnd();
	glFlush();
	glutPostRedisplay();
}

void renderScene4(void){
	//works only for convex stuff ... glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//define if clockwise or countercloickwise numeration is used
	//glFrontFace(GL_CCW);
	//LOLglShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny.glDisplay();
	glFlush();

	glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void renderScene5(void){
	//works only for convex stuff ... glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//define if clockwise or countercloickwise numeration is used
	//glFrontFace(GL_CCW);
	//LOLglShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny.glDisplay((colorMap&) *cMap);
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void renderScenePoints(void){
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny.glDisplayLines();
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();

}

void smoothing(void){
	s.smootheMesh_explicitEuler(bunny);
		cMap->setNormals(s.normals);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny.glDisplay((colorMap&)*cMap);
	glFlush();

	cout << "Volume is : " << Operator::volume(bunny) <<"\n";

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void idle(void){
	glutPostRedisplay();
}

int glutAWindow(){
	glutInitWindowPosition(300, 100); 
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //GLUT_DOUBLE for double buffering.

	int a = glutCreateWindow("Mein erstes GLUT fenster");
	//glutDisplayFunc(renderScene5);
	glutDisplayFunc(smoothing);
	//glutIdleFunc(idle);
	return a;
}

