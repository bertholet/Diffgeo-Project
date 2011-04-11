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
#include "simplestCube.h"

#define HEIGHT 1000
#define WIDTH 1000

using namespace std;

//void renderScene(void);
void idle(void);
int glutAWindow(void);
void displayScene( int argc, _TCHAR* * argv );
void smoothingDemo( int argc, _TCHAR* * argv );
void implicitSmoothingDemo( int argc, _TCHAR* * argv );

Vertex * points;
Vertex actualPoint;
mesh bunny;
curvColormap * cMap;
ImplicitEulerSmoothing * smoother;

//curvColormap * cMap;
Smoothing s;

void implicitEulerTests(void){
	//bunny = torus(2.f,1.f, 20, 50);
	//smoother(bunny,0.1f, 0.1f);
	cout << "Volume before smoothing:" << Operator::volume(bunny) <<"\n";
	//smoother->smootheMesh(bunny);

}

void implicitSmoothing(void){
	smoother->smootheMesh(bunny);

	Operator::calcAllCurvNormals(bunny,cMap->curvNormals);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny.glDisplay((colorMap&) *cMap);
	//bunny.glDisplay();
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}


int _tmain(int argc, _TCHAR* argv[])
{

	//bunny= mesh("C:/Users/Petje/workspace/RA/objfiles/venusm.obj", tuple3f(1.f,0.f,0.f), 1.f/1000);
	//bunny= mesh("C:/Users/bertholet/Dropbox/workspace/RA/objfiles/cow.obj", tuple3f(1.f,0.f,0.f), 3);
	//bunny = ball(1, 40);
	bunny = torus(2.f,1.f, 30, 60);
	//bunny = simplestCube();
	bunny.addNormalNoise(0.4f);
	
	cMap = new curvColormap(bunny);
	//cMap = new gaussColormap(bunny);

	smoother = new ImplicitEulerSmoothing(bunny,1, 1);
	implicitSmoothingDemo(argc,argv);
	//implicitEulerTests();
	delete smoother;
	//smoothingDemo(argc, argv);
	//displayScene(argc, argv);
	
	//implicitEulerTests();

	int a ;
	cout << " rhablabla";
	cin >> a;

	delete cMap;
	return 0;
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

int glutAWindow(void (*callback)(void)){
	glutInitWindowPosition(300, 100); 
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //GLUT_DOUBLE for double buffering.

	int a = glutCreateWindow("Mein erstes GLUT fenster");
	//glutDisplayFunc(renderScene5);
	glutDisplayFunc(callback);
	//glutIdleFunc(idle);
	return a;
}

void displayScene( int argc, _TCHAR* * argv ) 
{

	cout << cMap->additionalInfo() << "\n";
	cout << "volume is : " << Operator::volume(bunny) << " \n";

	glutInit(&argc, (char **) argv);
	glutAWindow(renderScene4);
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0,GLdouble(WIDTH)/HEIGHT, 2.0, 10000.0);
	//glOrtho(0,WIDTH ,0, HEIGHT,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();

}

void smoothingDemo( int argc, _TCHAR* * argv ) 
{
	cout << cMap->additionalInfo() << "\n";
	cout << "volume is : " << Operator::volume(bunny) << " \n";

	glutInit(&argc, (char **) argv);
	glutAWindow(smoothing);
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0,GLdouble(WIDTH)/HEIGHT, 2.0, 10000.0);
	//glOrtho(0,WIDTH ,0, HEIGHT,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}

void implicitSmoothingDemo( int argc, _TCHAR* * argv ) 
{
	cout << cMap->additionalInfo() << "\n";
	cout << "volume is : " << Operator::volume(bunny) << " \n";

	glutInit(&argc, (char **) argv);
	glutAWindow(implicitSmoothing);
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0,GLdouble(WIDTH)/HEIGHT, 2.0, 10000.0);
	//glOrtho(0,WIDTH ,0, HEIGHT,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
}
