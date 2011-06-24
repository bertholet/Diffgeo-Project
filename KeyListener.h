#pragma once

class KeyListener
{
public:
	KeyListener(void);
	~KeyListener(void);
	void processNormalKeys(unsigned char key, int x, int y);
};
