#pragma once

#include <vector>
#include "Scene.h"
#include "KeyListener.h"

class GLWindow
{
private:
	static Scene* scene;
	int width;
	int height;

	static std::vector<KeyListener> listener;

	static void processNormalKeys(unsigned char key, int x, int y)
		{
			/*for (std::vector<KeyListener>::iterator it = listener.begin(); it!=listener.end(); ++it) {
				((KeyListener) *it).processNormalKeys(key,x,y);
			}*/
		}

	static void innerLoop(void)
	{
		GLWindow::scene->glLoop();
	}
public:
	GLWindow(int width, int height);
	~GLWindow(void);

	void display();
	void GLWindow::setScene( Scene &s )
	{
		GLWindow:scene = &s;
	}
	void GLWindow::addKeyListener( KeyListener &l )
	{
		listener.push_back(l);
	}
};
