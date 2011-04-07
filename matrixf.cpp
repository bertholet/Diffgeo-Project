#include "StdAfx.h"
#include "matrixf.h"

matrixf::matrixf(void)
{
	//buff = new arr_44();
}

matrixf::matrixf( float a11,float a12,float a13,float a14,
				 float a21,float a22,float a23,float a24,
				 float a31,float a32,float a33,float a34,
				 float a41,float a42,float a43,float a44 )
{
	//buff = new arr_44();
	mat[0][0] = a11;
	mat[0][1] = a12;
	mat[0][2] = a13;
	mat[0][3] = a14;
	mat[1][0] = a21;
	mat[1][1] = a22;
	mat[1][2] = a23;
	mat[1][3] = a24;
	mat[2][0] = a31;
	mat[2][1] = a32;
	mat[2][2] = a33;
	mat[2][3] = a34;
	mat[3][0] = a41;
	mat[3][1] = a42;
	mat[3][2] = a43;
	mat[3][3] = a44;
}

matrixf::matrixf( float arr[4][4] )
{
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			mat[i][j] = arr[i][j];
		}
	}
}

/*matrixf::matrixf( const matrixf& par )
{
	//buff = new arr_44();
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			mat[i][j] = par.mat[i][j];
		}
	}
}*/

matrixf::~matrixf(void)
{
	//delete buff;
}

matrixf matrixf::operator*( matrixf &other )
{
	float temp[4][4];

	for(int i = 0; i < 4; i++){
		for(int j= 0; j < 4; j++){
			temp[i][j] = mat[i][0]*other.mat[0][j] +
				mat[i][1]*other.mat[1][j] +
				mat[i][2]*other.mat[2][j] +
				mat[i][3]*other.mat[3][j];
		}
	}
	return matrixf(temp);
}

tuple3f matrixf::operator*( tuple3f &other )
{
	return tuple3f(
		mat[0][0]* other.x + mat[1][0]* other.y+ mat[2][0]* other.z + mat[3][0],
		mat[0][1]* other.x + mat[1][1]* other.y+ mat[2][1]* other.z + mat[3][1],
		mat[0][2]* other.x + mat[1][2]* other.y+ mat[2][2]* other.z + mat[3][2]);
}

matrixf matrixf::operator*( float other )
{
	float temp[4][4];
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			temp[i][j] = other * mat[i][j];
		}
	}
	return matrixf(temp);
}

matrixf matrixf::operator+( matrixf &other )
{	
	float temp[4][4];
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			temp[i][j] = other.mat[i][j] + mat[i][j];
		}
	}
	return matrixf(temp);
}

matrixf matrixf::transpose( void )
{
	float temp[4][4];
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			temp[i][j] = mat[j][i];
		}
	}
	return matrixf(temp);
}

/*arr_44* matrixf::operator+( matrixf &other )
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			buff->myArr[i][j] = other.mat[i][j] + mat[i][j];
		}
	}
	return buff;
}

arr_44* matrixf::operator+( arr_44 * other )
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			buff->myArr[i][j] = other->myArr[i][j] + mat[i][j];
		}
	}
	return buff;
}

arr_44* matrixf::operator*( matrixf &other )
{

	for(int i = 0; i < 4; i++){
		for(int j= 0; j < 4; j++){
			buff->myArr[i][j] = mat[i][0]*other.mat[0][j] +
				mat[i][1]*other.mat[1][j] +
				mat[i][2]*other.mat[2][j] +
				mat[i][3]*other.mat[3][j];

		}
	}
	return buff;
}

arr_44* matrixf::operator*( arr_44* other )
{
	for(int i = 0; i < 4; i++){
		for(int j= 0; j < 4; j++){
			buff->myArr[i][j] = mat[i][0]*other->myArr[0][j] +
				mat[i][1]*other->myArr[1][j] +
				mat[i][2]*other->myArr[2][j] +
				mat[i][3]*other->myArr[3][j];

		}
	}
	return buff;
}



void matrixf::operator=( arr_44 * other )
{
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			mat[i][j] = other->myArr[i][j];
		}
	}
}

void matrixf::operator=( const matrixf& other )
{

	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++){
			mat[i][j] = other.mat[i][j];
		}
	}
}*/
