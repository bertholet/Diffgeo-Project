#pragma once

class squareTexture
{
public:
	int szx, szy;
	float *checkboard;
	squareTexture(void);
	~squareTexture(void);

	void draw(int x, int y);
};
