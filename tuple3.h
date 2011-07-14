#pragma once
#include <math.h>
#include <iostream>

class tuple3f
{
public:
	float x, y, z;
	tuple3f(void);
	tuple3f(float x_, float y_, float z_){
		x = x_;
		y=y_;
		z=z_;
	}
	~tuple3f(void);

	void set(float x_, float y_, float z_){
		x = x_;
		y=y_;
		z=z_;
	}

	void normalize(void){
		float temp = x*x + y*y + z*z;
		temp = pow(temp, 0.5f);
		x = x/temp;
		y= y/temp;
		z= z/temp;
	}

	static tuple3f cross(tuple3f &a, tuple3f &b){
		float x = a.y*b.z - a.z*b.y;
		float y = a.z*b.x - a.x*b.z;
		float z = a.x*b.y - a.y*b.x;
		return tuple3f(x,y,z);
	}

	static float crossNorm(tuple3f &a, tuple3f &b){
		float x = a.y*b.z - a.z*b.y;
		float y = a.z*b.x - a.x*b.z;
		float z = a.x*b.y - a.y*b.x;
		return pow(x*x + y*y + z*z, 0.5f);
	}

	static float cot(tuple3f &a, tuple3f &b){
		float t = crossNorm(a,b);
		return a.dot(b)/(t>0.0001f? t: 0.0001f);
	}


	static float cotPoints(tuple3f &a, tuple3f &b, tuple3f &c){
		//cot at b of the triangle a, b,c
		float a_bDotc_b =(a.x-b.x)*(c.x - b.x) + (a.y-b.y)*(c.y - b.y) + (a.z-b.z)*(c.z - b.z);
		float x = (a.y-b.y)*(c.z - b.z) - (a.z-b.z)*(c.y - b.y);
		float y = (a.z-b.z)*(c.x - b.x) - (a.x-b.x)*(c.z - b.z);
		float z = (a.x-b.x)*(c.y - b.y) - (a.y-b.y)*(c.x - b.x);
		float norm_sqr_a_bXc_b = x*x + y*y + z*z;
		return a_bDotc_b/pow(norm_sqr_a_bXc_b, 0.5f);
	}	

	float norm(void){
		float temp = x*x + y*y + z*z;
		temp = pow(temp, 0.5f);
		return temp;
	}

	float normSqr(void){
		return x*x + y*y + z*z;
	}
	
	float dot(tuple3f &other){
		return x*other.x + y * other.y + z * other.z;
	}

	//(a-b) dot(c-b)
	static float a_bDotc_b( tuple3f &a, tuple3f &b , tuple3f & c )
	{
		return (a.x - b.x)*(c.x-b.x) + (a.y - b.y)*(c.y-b.y) + (a.z - b.z)*(c.z-b.z);
	}

	static float sinPoints(tuple3f &a, tuple3f &b, tuple3f &c){
		float t = crossNorm(a-b,c-b);
		t/= (a-b).norm() * (c-b).norm();
		return t;
	}

	static float cosPoints( tuple3f& a, tuple3f& b, tuple3f& c ){
		/*float temp = a_bDotc_b(a,b,c) / (b-a).norm() /(c-b).norm();
		if(temp > 1 || temp < - 1){
			std::cout << "dammnit";
			throw std::exception("Assertion failed in tuple3h::cosPoints");
		}*/
		return a_bDotc_b(a,b,c) / (b-a).norm() /(c-b).norm();
	}

	tuple3f operator + (tuple3f &other){
		return tuple3f(x + other.x, y+ other.y, z + other.z);
	}

	void operator += (tuple3f &other){
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator *= (float &other){
		x *= other;
		y *= other;
		z *= other;
	}

	tuple3f operator * (float other){
		return tuple3f(x * other, y * other, z * other);
	}

	
	tuple3f operator - (tuple3f &other){
		return tuple3f(x - other.x, y- other.y, z - other.z);
	}

	tuple3f operator - (float &other){
		return tuple3f(x - other, y- other, z - other);
	}

	void operator -= (float &other){
		x -= other;
		y -= other;
		z -= other;
	}

};

class tuple3i
{
public:
	int a, b, c;
	tuple3i(void);
	tuple3i(int x_, int y_, int z_){
		a = x_;
		b=y_;
		c=z_;
	}
	~tuple3i(void);

	void set(int x_, int y_, int z_){
		a = x_;
		b=y_;
		c=z_;
	}
};
