#pragma once
#include "tuple3.h"
/*struct arr_44 
{
	float myArr[4][4];
};*/

class matrixf
{
	//buff is the memory where eg a matrix calculation is performed
	//arr_44 * buff;
public:
	float mat[4][4];

	matrixf(void);
	//matrixf(const matrixf& par);
	matrixf(float arr[4][4]);
	matrixf(float a11,float a12,float a13,float a14,
		float a21,float a22,float a23,float a24,
		float a31,float a32,float a33,float a34,
		float a41,float a42,float a43,float a44);
	~matrixf(void);

	matrixf transpose(void);


	matrixf operator * (matrixf &other);
	matrixf operator * (float other);
	tuple3f operator * (tuple3f &other);
	matrixf operator + (matrixf &other);
	//arr_44* operator * (matrixf &other);
	//arr_44* operator * (arr_44* other);

	//arr_44* operator + (matrixf &other);
	//arr_44* operator + (arr_44 * other);
	//void operator = (arr_44 * other);
	//void operator = (const matrixf& other);
};
