#include "StdAfx.h"
#include "cube.h"
#include <vector>


cube::cube(float sidelngth, int n )
{
	float delta = 2.f/n;

	int j=0;
	int i = 0;
	float base_x,base_y;
	int baseSz;

	vector<int> bottomEdge, topEdge, bottomEdgeInner, topEdgeInner;


	//the sides of the box

	for(i = 0; i < n; i++){
		base_x = -1 + i*delta;
		base_y = -1;
		for(j = 0; j < n+1; j++){
			vertices.push_back(tuple3f(base_x, base_y, -1 + j*delta));
		}
		bottomEdge.push_back(i*(n+1));
		topEdge.push_back(i*(n+1) + n);
	}

	baseSz = vertices.size();
	for(i = 0; i < n; i++){
		base_x = 1 ;
		base_y = -1+ i*delta;
		for(j = 0; j < n+1; j++){
			vertices.push_back(tuple3f(base_x, base_y, -1 + j*delta));
		}

		bottomEdge.push_back(baseSz+i*(n+1));
		topEdge.push_back(baseSz + i*(n+1) + n);
	}

	baseSz = vertices.size();
	for(i = 0; i < n; i++){
		base_x = 1 - i*delta;
		base_y = 1;
		for(j = 0; j < n+1; j++){
			vertices.push_back(tuple3f(base_x, base_y, -1 + j*delta));
		}

		bottomEdge.push_back(baseSz+i*(n+1));
		topEdge.push_back(baseSz + i*(n+1) + n);
	}

	baseSz = vertices.size();
	for(i = 0; i < n; i++){
		base_x = -1;
		base_y = 1- i*delta;
		for(j = 0; j < n+1; j++){
			vertices.push_back(tuple3f(base_x, base_y, -1 + j*delta));
		}

		bottomEdge.push_back(baseSz+i*(n+1));
		topEdge.push_back(baseSz + i*(n+1) + n);
	}


	//faces of the sides of the box.
	int lastLine = 0;
	int thisLine = n+1;

	for(i=0; i < 4*n; i++){
		for(j=0; j < n; j++){
			faces.push_back(tuple3i(lastLine +j+1, lastLine+j, thisLine + j));
			faces.push_back(tuple3i(lastLine +j+1, thisLine + j,thisLine + j+1));
		}
		lastLine = thisLine;
		thisLine+= n+1;
		thisLine = thisLine % vertices.size();
	}

	//top and bottom

	lastLine = vertices.size();
	thisLine = vertices.size() +n-1;
	baseSz = vertices.size();
	for(i = 1; i < n; i++){
		base_x = -1;
		base_y = -1;
		for(j = 1; j < n; j++){
			vertices.push_back(tuple3f(base_x + i*delta, base_y + j*delta, -1));
		}
	}

	//inner square edge to connect with the other borders
	for(i=0; i < n-1;i++){
		bottomEdgeInner.push_back(baseSz + i*(n-1));
	}
	for(i=0; i < n-1;i++){
		bottomEdgeInner.push_back(baseSz + (n-2)*(n-1)+i);
	}
	for(i=0; i < n-1;i++){
		bottomEdgeInner.push_back(baseSz + (n-1 -i)*(n-1) -1);
	}
	for(i=0; i < n-1;i++){
		bottomEdgeInner.push_back(baseSz + (n-1)-i-1);
	}

	//faces bottom
	for(i=1; i < n-1; i++){

		for(j=0; j < n-2; j++){
			faces.push_back(tuple3i(lastLine+j,lastLine +j+1, thisLine + j));
			faces.push_back(tuple3i(thisLine + j,lastLine +j+1,thisLine + j+1));
		}
		lastLine = thisLine;
		thisLine+= n-1;
	}

	//faces connection
	int baseOuter=0,baseInner = 0;
	faces.push_back(tuple3i(bottomEdge[baseOuter+1],bottomEdge[baseOuter],  bottomEdgeInner[baseInner]));
	faces.push_back(tuple3i( bottomEdgeInner[baseInner],bottomEdge[baseOuter],bottomEdge[(baseOuter + 4*(n) -1)]));

	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i( bottomEdge[i+1],bottomEdge[i], bottomEdgeInner[i-1]));
		faces.push_back(tuple3i(bottomEdgeInner[i],bottomEdge[i+1], bottomEdgeInner[i-1]));
	}

	baseOuter=n,baseInner = n-1;
	faces.push_back(tuple3i( bottomEdge[baseOuter+1],bottomEdge[baseOuter], bottomEdgeInner[baseInner]));
	faces.push_back(tuple3i( bottomEdgeInner[baseInner],bottomEdge[baseOuter],bottomEdge[(baseOuter-1)]));
	
	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i( bottomEdge[baseOuter+i+1],bottomEdge[baseOuter+i], bottomEdgeInner[baseInner+i-1]));
		faces.push_back(tuple3i( bottomEdgeInner[baseInner+i],bottomEdge[baseOuter+i+1],bottomEdgeInner[baseInner+i-1]));
	}

	baseOuter=2*n,baseInner = 2*(n-1);
	faces.push_back(tuple3i( bottomEdge[baseOuter+1],bottomEdge[baseOuter], bottomEdgeInner[baseInner]));
	faces.push_back(tuple3i( bottomEdgeInner[baseInner],bottomEdge[baseOuter],bottomEdge[(baseOuter-1)]));
	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i( bottomEdge[baseOuter+i+1],bottomEdge[baseOuter+i], bottomEdgeInner[baseInner+i-1]));
		faces.push_back(tuple3i( bottomEdgeInner[baseInner+i],bottomEdge[baseOuter+i+1],bottomEdgeInner[baseInner+i-1]));
	}

	baseOuter=3*n,baseInner = 3*(n-1);
	faces.push_back(tuple3i( bottomEdge[baseOuter+1],bottomEdge[baseOuter], bottomEdgeInner[baseInner]));
	faces.push_back(tuple3i( bottomEdgeInner[baseInner],bottomEdge[baseOuter],bottomEdge[(baseOuter-1)]));
	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i( bottomEdge[baseOuter+i+1],bottomEdge[baseOuter+i], bottomEdgeInner[baseInner+i-1]));
		faces.push_back(tuple3i( bottomEdgeInner[baseInner+i],bottomEdge[baseOuter+i+1],bottomEdgeInner[baseInner+i-1]));
	}

	//last but not least: the top.

	lastLine = vertices.size();
	thisLine = vertices.size() +n-1;
	baseSz = vertices.size();
	for(i = 1; i < n; i++){
		base_x = -1;
		base_y = -1;
		for(j = 1; j < n; j++){
			vertices.push_back(tuple3f(base_x + i*delta, base_y + j*delta, 1));
		}
	}

	for(i=0; i < n-1;i++){
		topEdgeInner.push_back(baseSz + i*(n-1));
	}
	for(i=0; i < n-1;i++){
		topEdgeInner.push_back(baseSz + (n-2)*(n-1)+i);
	}
	for(i=0; i < n-1;i++){
		topEdgeInner.push_back(baseSz + (n-1 -i)*(n-1) -1);
	}
	for(i=0; i < n-1;i++){
		topEdgeInner.push_back(baseSz + (n-1)-i-1);
	}

//wrong orientation
	//faces top
	for(i=1; i < n-1; i++){

		for(j=0; j < n-2; j++){
			faces.push_back(tuple3i( lastLine +j+1,lastLine+j, thisLine + j));
			faces.push_back(tuple3i(lastLine +j+1,thisLine + j,thisLine + j+1));
		}
		lastLine = thisLine;
		thisLine+= n-1;
	}



	//faces connection
	baseOuter=0,baseInner = 0;
	faces.push_back(tuple3i(topEdge[baseOuter], topEdge[baseOuter+1], topEdgeInner[baseInner]));
	faces.push_back(tuple3i(topEdge[baseOuter], topEdgeInner[baseInner],topEdge[(baseOuter + 4*(n) -1)]));

	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i(topEdge[i], topEdge[i+1], topEdgeInner[i-1]));
		faces.push_back(tuple3i(topEdge[i+1], topEdgeInner[i],topEdgeInner[i-1]));
	}

	baseOuter=n,baseInner = n-1;
	faces.push_back(tuple3i(topEdge[baseOuter], topEdge[baseOuter+1], topEdgeInner[baseInner]));
	faces.push_back(tuple3i(topEdge[baseOuter], topEdgeInner[baseInner],topEdge[(baseOuter-1)]));

	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i(topEdge[baseOuter+i], topEdge[baseOuter+i+1], topEdgeInner[baseInner+i-1]));
		faces.push_back(tuple3i(topEdge[baseOuter+i+1], topEdgeInner[baseInner+i],topEdgeInner[baseInner+i-1]));
	}

	baseOuter=2*n,baseInner = 2*(n-1);
	faces.push_back(tuple3i(topEdge[baseOuter], topEdge[baseOuter+1], topEdgeInner[baseInner]));
	faces.push_back(tuple3i(topEdge[baseOuter], topEdgeInner[baseInner],topEdge[(baseOuter-1)]));
	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i(topEdge[baseOuter+i], topEdge[baseOuter+i+1], topEdgeInner[baseInner+i-1]));
		faces.push_back(tuple3i(topEdge[baseOuter+i+1], topEdgeInner[baseInner+i],topEdgeInner[baseInner+i-1]));
	}

	baseOuter=3*n,baseInner = 3*(n-1);
	faces.push_back(tuple3i(topEdge[baseOuter], topEdge[baseOuter+1], topEdgeInner[baseInner]));
	faces.push_back(tuple3i(topEdge[baseOuter], topEdgeInner[baseInner],topEdge[(baseOuter-1)]));
	for(i= 1; i <n-1; i++){
		faces.push_back(tuple3i(topEdge[baseOuter+i], topEdge[baseOuter+i+1], topEdgeInner[baseInner+i-1]));
		faces.push_back(tuple3i(topEdge[baseOuter+i+1], topEdgeInner[baseInner+i],topEdgeInner[baseInner+i-1]));
	}


	initNormalsFromVertices();
	initFaceNormals();
}

cube::~cube(void)
{
}
