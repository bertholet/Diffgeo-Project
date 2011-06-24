#include "StdAfx.h"
#include "squareTexture.h"

squareTexture::squareTexture(void)
{
	szx=300;
	szy = 300;
	checkboard = new float[szx*szy*4];

	for(int i = 0; i < szx; i++){
		for(int j = 0; j < szy; j++){
			if(i%60 < 30){
				if(j%60 < 30){
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.1f;
					}
				}
				else{
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.9f;
					}
				}
			}
			else{
				if(j%60 > 30){
					for(int k = 0; k < 4; k++){
						checkboard[i*szy*4 + j*4+k] = 0.1f;
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
