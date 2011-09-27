// openGl_try3.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

//#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include<windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
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
#include "glWindowHelper.h"
#include "squareTexture.h"
#include "meshOperation.h"
#include "cube.h"
#include "TextureDemo.h"
#include "TutteDemo.h"
#include "TutteDemo2.h"
#include "TutteWeights.h"

#define HEIGHT 1000
#define WIDTH 1000

using namespace std;

//void renderScene(void);
void idle(void);
int glutAWindow(void);
void displayScene( int argc, _TCHAR* * argv );
void smoothingDemo( int argc, _TCHAR* * argv );
void implicitSmoothingDemo( int argc, _TCHAR* * argv );
void textureDemo(squareTexture & tex);

mesh * bunny;
colorMap * cMap;
ImplicitEulerSmoothing * smoother;

//curvColormap * cMap;
Smoothing s;

bool spacePressed =false;


void processNormalKeys(unsigned char key, int x, int y) {

	if (key == ' ')
		spacePressed = !spacePressed;

	else if (key == 'd')
		bunny->rotX(0.05f);

	else if (key == 'a')
		bunny->rotX(-0.05f);

	else if (key == 'w')
		bunny->rotY(0.05f);

	else if (key == 's')
		bunny->rotY(-0.05f);

	else if (key == 'z'){
		bunny->scaleXYZ(1.05f);
	}
	else if (key == 'u'){
		bunny->scaleXYZ(0.95f);
	}

}

void implicitEulerTests(void){
	//bunny = torus(2.f,1.f, 20, 50);
	//smoother(bunny,0.1f, 0.1f);
	cout << "Volume before smoothing:" << Operator::volume(*bunny) <<"\n";
	//smoother->smootheMesh(bunny);

}

void implicitSmoothing(void){
	if(spacePressed){
		smoother->smootheMesh(*bunny);
		//Operator::calcAllCurvNormals(bunny,cMap->curvNormals); //TODO UNCOMMENT!! REFACTOR!
	}
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny->glDisplay((colorMap&) *cMap);
	//bunny.glDisplay();
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}


int _tmain(int argc, _TCHAR* argv[])
{

		
	//bunny = cube(2.f, 10);
	//bunny= new mesh("C:/Users/Petje/Documents/My Dropbox/workspace/RA/objfiles/cow.obj", tuple3f(1.f,0.f,0.f), 2.f);
//	bunny = new mesh("C:/Users/Petje/Documents/My Dropbox/workspace/RA/objfiles/dragon.obj", tuple3f(1.f,0.f,0.f), 2.f);
//	bunny= new mesh("C:/Users/bertholet/Dropbox/workspace/RA/objfiles/dragon.obj", tuple3f(1.f,0.f,0.f), 2);
	bunny = new ball(1,40,20);
	//	bunny = ball(1, 80,40);
	//bunny = torus(2.f,1.f, 30, 60);
	//bunny = simplestCube();
	//bunny = cube(2.f,20);


	bunny->normalize();

//	meshOperation::getHalf(*bunny,*bunny, tuple3f(-1,0,0),-0.05f); //dragon.obj config

	//meshOperation::getHalf(*bunny,*bunny, tuple3f(-0.25f,1,0), 0.1f); //dragon.obj config
/*	meshOperation::getHalf(*bunny,*bunny, tuple3f(0,0.3,1), -0.35f); //nice ball with two holes
	meshOperation::getHalf(*bunny,*bunny, tuple3f(0,-0.3f,-1), -0.35f);*/
	meshOperation::getHalf(*bunny,*bunny, tuple3f(-1,0,0), -0.8f);
	meshOperation::getHalf(*bunny,*bunny, tuple3f(1,0,0), -0.8f);//*/
/*	meshOperation::getHalf(*bunny,*bunny, tuple3f(0,-1,0), -0.7f);
	meshOperation::getHalf(*bunny,*bunny, tuple3f(0,1.1,0), -0.9f);
	meshOperation::getHalf(*bunny,*bunny, tuple3f(1.1,0,0), -0.9f);
	meshOperation::getHalf(*bunny,*bunny, tuple3f(-1.1,0,0), -0.9f);
	//meshOperation::getHalf(*bunny,*bunny, tuple3f(-1,0,0), -0.7f);*/
//	meshOperation::getHalf(*bunny,*bunny, tuple3f(0,0,1), 0.05f); // dragon.obj: 2 border
//	meshOperation::undangle(*bunny);
/*	meshOperation::reduceToLargestComponent(*bunny);*/

	vector<vector<int>> border;
	//vector<int> borderStarts;
	meshOperation::getBorder(*bunny,border);
	//bunny.addNormalNoise(0.05f);
	
	//cMap = new curvColormap(bunny);
	//cMap = new gaussColormap(bunny);
		
	//	bunny->setShowOrientation(true);
/*	vector<vector<float>> angles, lambdas;
	TutteWeights::angles_lambdas(angles,lambdas,border,0,*bunny);
	cMap = (colorMap *) new borderColorMap(border,tuple3f(0,0,1), tuple3f(1,0,0),angles);*/

/*	cMap = (colorMap *) new borderColorMap(border,tuple3f(0,0,1), tuple3f(1,0,0));
	smoother = new ImplicitEulerSmoothing(*bunny,1, 0.1f);
	implicitSmoothingDemo(argc,argv);
	//implicitEulerTests();
	delete smoother;//*/
	
	//smoothingDemo(argc, argv);
	
	//displayScene(argc, argv);


/*	TextureDemo demo;
	demo.run(*bunny);//*/
/*	TutteDemo demo;
	demo.run_multiBorder(*bunny, TutteWeights::cotan_weights_divAvor);
	demo.run(*bunny, TutteWeights::cotan_weights_divAvor, TutteWeights::angleApproxBorder);
/*	squareTexture s = squareTexture();
	textureDemo(s);//*/
	TutteDemo2 demo;
	demo.run(*bunny, NULL);
//*/	

	//GLWindow window = GLWindow(300,150);

	delete cMap;
	delete bunny;
	int a ;
	cout << " rhablabla";
	cin >> a;
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
	bunny->glDisplay();
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
	bunny->glDisplay((colorMap&) *cMap);
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void renderScenePoints(void){
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny->glDisplayVertices();
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();

}

void smoothing(void){
	if(spacePressed){
		s.smootheMesh_explicitEuler(*bunny);
//			cMap->setNormals(s.normals); //TODO UNCOMMENT; REFACTOR
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny->glDisplay((colorMap&)*cMap);
	glFlush();

	cout << "Volume is : " << Operator::volume(*bunny) <<"\n";

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

	glutKeyboardFunc(processNormalKeys);
	//glutIdleFunc(idle);
	return a;
}

void displayScene( int argc, _TCHAR* * argv ) 
{

	cout << cMap->additionalInfo() << "\n";
	cout << "volume is : " << Operator::volume(*bunny) << " \n";

	glutInit(&argc, (char **) argv);
	glutAWindow(renderScene4);//Scene4
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
	cout << "volume is : " << Operator::volume(*bunny) << " \n";

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
	cout << "volume is : " << Operator::volume(*bunny) << " \n";

	glWindowHelper::glWindow(450,450,implicitSmoothing, processNormalKeys); 
	glutMainLoop();
	/*glutInit(&argc, (char **) argv);
	glutAWindow(implicitSmoothing);
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0,GLdouble(WIDTH)/HEIGHT, 2.0, 10000.0);
	//glOrtho(0,WIDTH ,0, HEIGHT,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();*/
}


void texture(void){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	bunny->glTexDisplay();
	//bunny.glDisplay();
	glFlush();

	//glEnable(GL_DEPTH_TEST);
	glutPostRedisplay();
}

void textureDemo(squareTexture &tex){
	glWindowHelper::glWindow(450,450,texture, processNormalKeys); 

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.szx,tex.szy,0,GL_RGBA, GL_FLOAT, &(tex.checkboard[0]));
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glutMainLoop();
}