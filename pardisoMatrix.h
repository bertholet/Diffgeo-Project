#pragma once
#include <vector>

class pardisoMatrix
{
public:
	//indices of the values in the sparse matrix
	std::vector<int> ia, ja;
	//values of the elements of the sparse matrix
	std::vector<double> a;

	pardisoMatrix(void);
	~pardisoMatrix(void);
};
