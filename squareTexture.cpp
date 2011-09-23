#include "StdAfx.h"
#include "squareTexture.h"

squareTexture::squareTexture(void)
{
	szx=300;
	szy = 300;
	checkboard = new float[szx*szy*4];
	int side = 20;
	int side2 = 2*side;

	for(int i = 0; i < szx; i++){
		for(int j = 0; j < szy; j++){
			if(i%side2 < side){
				if(j%side2 < side){
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.1f;
						if(k==2){
							checkboard[i*szy*4 + j*4+k] = 0.1f + (0.f +i)/szx/2;
						}
						if(k==1){
							checkboard[i*szy*4 + j*4+k] = 0.1f + (0.f +j)/szy/2;
						}
					}
				}
				else{
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.9f;
					}
				}
			}
			else{
				if(j%side2 > side){
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.1f;

						if(k==2){
							checkboard[i*szy*4 + j*4+k] = 0.1f + (0.f +i)/szx/2;
						}
						if(k==1){
							checkboard[i*szy*4 + j*4+k] = 0.1f + (0.f +j)/szy/2;
						}
					}

				}
				else{
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.9f;
					}
				}
			}
		}
	}
}

squareTexture::~squareTexture(void)
{
	delete[] checkboard;
}

void squareTexture::draw( int x, int y )
{
	int startx = (x-5 >=0? x-5 :0);
	int endx = (startx + 10 >= szx ? szx:startx +10);
	int starty = (y-5 >=0? y-5 :0);
	int endy = (starty + 10 >= szy ? szy:starty +10);
	int j,k;

	for(int i = startx ; i < endx; i++){
		for(j = starty ; j < endy; j++){
			checkboard[j*szy*4 + i*4]=0.9f;
			for(k=1; k<4; k++){
				checkboard[j*szy*4 + i*4+k]=0;
			}
		}
	}
}
