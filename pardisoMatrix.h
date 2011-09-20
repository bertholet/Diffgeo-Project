#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <string>


class pardisoMatrix
{
public:
	//indices of the values in the sparse matrix
	std::vector<int> ia, ja;
	//values of the elements of the sparse matrix
	std::vector<double> a;

	pardisoMatrix(void);
	~pardisoMatrix(void);

	void saveMatrix(std::string file){
		std::ofstream myFile;
		int internI;
		myFile.open(file.c_str());

		myFile << "i= [";
		internI = 0;
		for(int i = 0; i < a.size(); i++){
			if(i+1 >= ia[internI+1]){
				internI++;
				myFile << "...\n";
			}
			myFile << boost::lexical_cast<std::string>(internI+1);
			if(i!= a.size() -1){
				myFile <<", ";
			}
		}

		myFile << "];\n\n j=[";

		internI = 0;
		for(int i = 0; i < a.size(); i++){
			if(i+1 >= ia[internI+1]){
				internI++;
				myFile << "...\n";
			}
			myFile << boost::lexical_cast<std::string>(ja[i]);
			if(i!= a.size() -1){
				myFile <<", ";
			}
		}

		myFile << "];\n\n a=[";
		internI = 0;
		for(int i = 0; i < a.size(); i++){
			if(i+1 >= ia[internI+1]){
				internI++;
				myFile << "...\n";
			}
			myFile << boost::lexical_cast<std::string>(a[i]);
			if(i!= a.size() -1){
				myFile <<", ";
			}
		}
		myFile << "];";

		myFile.close();
	}

};
