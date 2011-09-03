#include "stdafx.h"
#include "OBIFileReader.h"

#include <stdlib.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>


#define VERTEX "v"
#define COMMENT "#"
#define VNORMAL "vn"
#define VTEX "vt"
#define FACE "f"

OBIFileReader::OBIFileReader()
{

}

OBIFileReader::~OBIFileReader(void)
{
}


void OBIFileReader::parse(const char * file){
	using namespace boost;
	ifstream myStream(file, ifstream::in);
	if(!myStream){
		printf("File not found!");
		throw std::exception("File was not found.");
	}
	string buff("");
	char_separator<char> sep(" /");
	typedef tokenizer<char_separator<char>> tokenizer_; 
	tokenizer_ t(buff, sep);
	vector<std::string> tokens;

	float val1, val2, val3;
	//int vl1,vl2,vl3, vl4, vl5, vl6;
	tuple3f tuple;
	tuple3i tuplei;
	char what1, what2;
	int matches;


	while(myStream.good()){
		getline(myStream, buff);
		matches = sscanf(buff.c_str(), "%c%c%*", &what1, &what2); 
		
		if(matches != 2){
			//skip everything
			cout << ">> Could not parse line: "<<buff <<std::endl;
		}
		else if(what1 == 'v' && what2 == ' '){
			sscanf(buff.c_str(), "v %f %f %f", &val1, &val2, &val3);
			//cout << val1 << " " << val2 << " " << val3 << " is my vertex\n";
			tuple.set(val1, val2, val3);
			vertices.push_back(tuple);
		}
		else if(what1 == 'v' && what2 == 'n'){
			sscanf(buff.c_str(), "vn %f %f %f", &val1, &val2, &val3);
			//cout << val1 << " " << val2 << " " << val3 << " is my normal\n";
			tuple.set(val1, val2, val3);
			tuple.normalize();
			normals.push_back(tuple);
		}
		else if(what1 == 'v' && what2 == 't'){
			sscanf(buff.c_str(), "vt %f %f", &val1, &val2);
			//cout << val1 << " " << val2 << " " << val3 << " is my tex\n";
			tuple.set(val1, val2, 0.f);
			tex.push_back(tuple);
		}
		else if(what1 == 'f' && what2 == ' '){
			//sscanf(buff.c_str(), "f %i//%i %i//%i %i//%i", &vl1, &vl2, &vl3, &vl4, &vl5, &vl6);
			//cout << val1 << " " << val2 << " " << val3 << " is my tex\n";
			//tuplei.set(vl1-1, vl3-1, vl5-1);
			//faces.push_back(tuplei);

			tokens.clear();
			t= tokenizer_(buff, sep);
			for(tokenizer_::iterator beg = t.begin(); beg!=t.end(); ++beg){
				tokens.push_back(beg.current_token());
			}

			if(tokens.size() == 4){ //only face coordinates
				tuplei.set(lexical_cast<int>(tokens[1]) - 1,
					lexical_cast<int>(tokens[2])-1,
					lexical_cast<int>(tokens[3])-1);
				faces.push_back(tuplei);
			}
			else if(tokens.size() == 7){//assume there are onlyvertices and normals
				tuplei.set(lexical_cast<int>(tokens[1])-1,
					lexical_cast<int>(tokens[3]) -1,
					lexical_cast<int>(tokens[5]) -1);
				faces.push_back(tuplei);

				tuplei.set(lexical_cast<int>(tokens[2])-1,
					lexical_cast<int>(tokens[4]) -1,
					lexical_cast<int>(tokens[6]) -1);
				faces_normals.push_back(tuplei);
			}
			else if(tokens.size() == 10){//assume there are onlyvertices and normals
				tuplei.set(lexical_cast<int>(tokens[1])-1,
					lexical_cast<int>(tokens[4]) -1,
					lexical_cast<int>(tokens[7]) -1);
				faces.push_back(tuplei);

				tuplei.set(lexical_cast<int>(tokens[2])-1,
					lexical_cast<int>(tokens[5]) -1,
					lexical_cast<int>(tokens[8]) -1);
				faces_textures.push_back(tuplei);

				tuplei.set(lexical_cast<int>(tokens[3])-1,
					lexical_cast<int>(tokens[6]) -1,
					lexical_cast<int>(tokens[9]) -1);
				faces_normals.push_back(tuplei);
			}
			else{
				cout <<"Undefined face specification!";
				throw std::exception("Unimplemented face specification found while reading obj file");
			}

			//cout << tuplei.x << " " << tuplei.y << " " << tuplei.z << " is my tex\n";
		}
		else{
			cout << ">> Didnt read line: "<<buff <<std::endl;
		}

				
		//cout << tokens[1].c_str() << " size \n";

		/*if(tokens[0].compare(VERTEX) == 0 && tokens.size() == 4){
			sscanf("0.991", "%f", &val1);
			cout << " vertex\n"<< " value 1:";
			cout << val1;
		}
		else{
			cout << "wtf..?";
		}*/

	}
	myStream.close();
	cout << "read : \n" << vertices.size() << " vertices \n"
		<< normals.size() << " normals \n"
		<< tex.size() << " textures \n"
		<< faces.size() << " faces. :-*\n";

	checkAllVerticesUsed();
}

void OBIFileReader::checkAllVerticesUsed( void )
{
	bool * arr = new bool[vertices.size()];
	vector<int>  unusedVertices;

		//if thou passes here, thee takest a moment to comemorate
		//the exact place, were I, for the first time,
		//overwrote memory not belonging to me
		//resulting in a wunderfull unpredictable code behaviour...

	for(unsigned int i = 0; i < vertices.size(); i++){
		arr[i] = false;
	}


	for(unsigned int i = 0; i < faces.size(); i++){
		arr[faces[i].a] = true;
		arr[faces[i].b] = true;
		arr[faces[i].c] = true;
	}

	for(int i = 0; i < vertices.size(); i++){
		if(!arr[i]){
			unusedVertices.push_back(i);
		}
	}
	
	cout << "Analysed parsed Mesh... " << unusedVertices.size() << " Vertices unused\n";

/*	int unused;
	for(int i = 0; i < unusedVertices.size()){
		unused = unusedVertices[i];
		vertices.erase(unused);

		for(int j = 0; j < faces.size(); j++){
			if(faces[j].a > unusedVertices[i]){

			}
			if(faces[j].a > unusedVertices[i]){

			}
			if(faces[j].a > unusedVertices[i]){

			}
		}
	}*/

	delete[] arr;
}

/*vector<tuple3i> OBIFileReader::getFaceNormals()
{
	throw std::exception("The method or operation is not implemented.");
}*/

/*void OBIFileReader:: operator>>(Face * other){
	;
}*/
