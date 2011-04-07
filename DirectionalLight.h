#pragma once
#include "tuple3.h"
class DirectionalLight
{
public:
	tuple3f direction;
	tuple3f intensity;
	DirectionalLight(void);
	DirectionalLight(tuple3f direction, tuple3f intensity){
		this->direction = direction;
		this->intensity = intensity;
	}
	~DirectionalLight(void);
};
