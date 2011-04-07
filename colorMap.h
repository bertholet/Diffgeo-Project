#pragma once
#include "tuple3.h"
#include <string>

struct colorMap
{
	virtual tuple3f color(int vertexNr) = 0;
	virtual std::string additionalInfo(void) = 0;
};
