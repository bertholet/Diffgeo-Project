#include "StdAfx.h"
#include "curvVisualizingMesh.h"
#include <math.h>
#include <iostream>
#include <sstream>
#include <algorithm>


curvColormap::curvColormap(mesh &myMesh)
{
	using namespace std;


//init curve normals.
	theNormals = & curvNormals;
	Operator::calcAllCurvNormals(myMesh,curvNormals);
	min = curvNormals[0].norm();
	for(unsigned int i = 0; i < curvNormals.size(); i++){
		min = (min > curvNormals[i].norm()?curvNormals[i].norm(): min);
	}

	max = curvNormals[0].norm();
	for(unsigned int i = 0; i < curvNormals.size(); i++){
		max = (max < curvNormals[i].norm()? curvNormals[i].norm() : max);
	}
	
	stringstream ss;
	ss << "Min / Max of normal curvatures are: ";
	ss << min/2 << "/" << max/2;
	min = log(min + 1);
	max = log(max + 1);
	info = ss.str();

}

curvColormap::~curvColormap(void)
{
}

tuple3f curvColormap::color( float val )
{
	if(val < (min + max)/2)
	{
		return tuple3f(0,(val-min)*2/(max - min),1-(val-min)*2/(max - min));
	}
	return tuple3f((max-val)*2/(max-min), 1-(max-val)*2/(max-min),0);
	
}

tuple3f curvColormap::color( int vertexNr )
{
	return color(log((*theNormals)[vertexNr].norm() + 1));	
}

std::string curvColormap::additionalInfo( void )
{
	return info;
}

void curvColormap::setNormals( vector<tuple3f> & normals )
{
	theNormals = & normals;
}


//------------------------------------------------------------------------------------------------

gaussColormap::gaussColormap( mesh & myMesh_ )
{
	Operator::calcAllGaussCurvs(myMesh_, gaussCurvs);
	min = gaussCurvs[0];
	for(unsigned int i = 0; i < gaussCurvs.size(); i++)
	{
		min = (min > gaussCurvs[i]?gaussCurvs[i]: min);
	}

	max = gaussCurvs[0];
	for(unsigned int i = 0; i < gaussCurvs.size(); i++){
		max = (max < gaussCurvs[i]? gaussCurvs[i] : max);
	}

	stringstream ss;
	ss << "Min / Max of normal curvatures are: ";
	ss << min/2 << "/" << max/2;
	info = ss.str();

}

gaussColormap::~gaussColormap()
{

}

tuple3f gaussColormap::color( float val )
{
	if(val < (min + max)/2)
	{
		return tuple3f(0,(val-min)*2/(max - min),1-(val-min)*2/(max - min));
	}
	return tuple3f((max-val)*2/(max-min), 1-(max-val)*2/(max-min),0);
}

tuple3f gaussColormap::color( int vertexNr )
{
	return color(gaussCurvs[vertexNr]);
}

std::string gaussColormap::additionalInfo( void )
{
	return info;
}

std::string borderColorMap::additionalInfo( void )
{
	return info;
}

tuple3f borderColorMap::color( int vertexNr )
{

	if(vertexNr < sz && border[vertexNr] >-1){
		if(border[vertexNr] %4 ==0 )
			return tuple3f(0.f, 0, 1.f);
		if(border[vertexNr] %4 ==1 )
			return tuple3f(0,1.f,0);
		if(border[vertexNr] %4 ==2 )
			return tuple3f(0, 1.f, 1.f);
		if(border[vertexNr] %4 ==3 )
			return tuple3f(1.f, 1.f, 1.f);
	}

	return col2;
}


tuple3f borderMarkupMap::color( int vertexNr )
{
	if(vertexNr < sz && border[vertexNr]==markedBorder){
		return tuple3f(0.f,0.f,1.f);
	}
	return tuple3f(1.f,0.f,0.f);
}

std::string borderMarkupMap::additionalInfo( void )
{
	return "";
}
