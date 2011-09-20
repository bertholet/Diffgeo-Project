#include "StdAfx.h"
#include "TutteEmbedding.h"
#include <vector>
#include "meshOperation.h"
#include "tuple3.h"
#include <math.h>
#include "pardiso.h"
#include "tutteWeights.h"


TutteEmbedding::TutteEmbedding(void)
{
}

TutteEmbedding::~TutteEmbedding(void)
{
}

void TutteEmbedding::calcTexturePos( mesh &m )
{
	vector<vector<int>> border;
	vector<double> b;
	meshOperation::getBorder(m,border);

	if(border.size() > 1){
		printf("Error in calcTexturePos");
		throw std::exception("Only a single border allowed with this Method");
	}

	vector<tuple3f> outerPos;
	double * x = new double[m.getVertices().size()];
	double * y = new double[m.getVertices().size()];

	pardisoMatrix mat;
	pardisoSolver parsolver(pardisoSolver::MT_STRUCTURALLY_SYMMETRIC,
		pardisoSolver::SOLVER_ITERATIVE,
		2);
	setUp(mat, border[0], m, TutteWeights::uniform_weights);
	parsolver.setMatrix(mat, 1);

	TutteWeights::circleBorder(outerPos, border[0], m);
	setUpX(b, border[0],outerPos, m.getVertices().size());
	parsolver.solve(x,&b[0]);

	setUpY(b, border[0],outerPos, m.getVertices().size());
	parsolver.solve(y,&b[0]);
	
	m.setTextures_perVertex(x,y);

	delete[] x,y;
}

void TutteEmbedding::calcTexturePos( mesh &m, 
									double (*weights ) (int, int, mesh &, 
									vector<int>& /*nbr_i*/, 
									vector<int>&/*fc_i*/, 
									vector<int>& /*border*/)){

	calcTexturePos(m,weights, TutteWeights::circleBorder);
}

void TutteEmbedding::calcTexturePos( mesh &m, 
									double (*weights ) (int, int, mesh &, 
										vector<int>& /*nbr_i*/, 
										vector<int>&/*fc_i*/, 
										vector<int>& /*border*/),
									void (*getBorderPos ) (vector<tuple3f> & /*outerPos*/, 
										vector<int> & /*border*/, 
										//vector<int> & /*loops*/, 
										mesh & /*m*/))
{
	vector<vector<int>> border;
	vector<double> b;
	meshOperation::getBorder(m,border);

	if(border.size() > 1){
		printf("Error: Only a single border allowed with this Method! in tutteEmbedding::calcTexturePos");
		throw std::runtime_error("Only a single border allowed with this Method");
	}

	vector<tuple3f> outerPos;
	double * x = new double[m.getVertices().size()];
	double * y = new double[m.getVertices().size()];
	for(int i = 0; i < m.getVertices().size();i++){
		x[i] =0.0;
		y[i] =0.0;
	}

	getBorderPos(outerPos, border[0], m);

	pardisoMatrix mat;
	pardisoSolver parsolver(pardisoSolver::MT_STRUCTURALLY_SYMMETRIC,
		pardisoSolver::SOLVER_ITERATIVE,
		2);
	setUp(mat, border[0], m, weights);
	parsolver.checkMatrix(parsolver.MT_STRUCTURALLY_SYMMETRIC, mat);
	parsolver.setMatrix(mat, 1);

//	mat.saveMatrix("C:/Users/bertholet/Dropbox/matrix_working.m");

	//getBorderPos(outerPos, border, loops,m);
	//getBorderPos(outerPos,border,m);
	setUpX(b, border[0],outerPos, m.getVertices().size());
	parsolver.solve(x,&b[0]);

	setUpY(b, border[0],outerPos, m.getVertices().size());
	parsolver.solve(y,&b[0]);

	m.setTextures_perVertex(x,y);

	delete[] x,y;
}



void TutteEmbedding::calcTexturePos_multiBorder( mesh &m, 
			double (*weights ) (int, int,
					mesh &,
					vector<int>& /*nbr_i*/,
					vector<int>&/*fc_i*/,
					vector<int>& /*border*/) )
{

	vector<vector<int>> border;
	vector<tuple3f> outerPos;
	vector<double> b;
	pardisoMatrix mat;
	double * xy = new double[2*m.getVertices().size()];
	for(int i = 0; i < 2*m.getVertices().size();i++){
		xy[i] = 0.0;
	}

	meshOperation::getBorder(m, border);
	int outBorder = outerBorder(border,m);
	TutteWeights::angleApproxBorder(outerPos,border[outBorder],m);

	setUp_multiBorder(mat,border,outerPos,outBorder,m,weights);

	mat.saveMatrix("C:/Users/bertholet/Dropbox/matrix_multiBorder.m");
	
	pardisoSolver s(pardisoSolver::MT_ANY, pardisoSolver::SOLVER_ITERATIVE,2);

//	s.checkMatrix(pardisoSolver::MT_ANY,mat);
	s.setMatrix(mat,1);
	setUpXY(b, border[outBorder],outerPos, m.getVertices().size());

	s.setPrintStatistics(true);
	s.solve(xy,&(b[0]));

	m.setTextures_perVertex(xy);
	delete[] xy;
}


/************************************************************************/
/* Set up Matrix                                                                     */
/************************************************************************/
void TutteEmbedding::setUp( pardisoMatrix &mat, vector<int> &border, mesh & m, 
						   double (*weights ) (int /*i*/, int /*j*/, mesh & , 
								vector<int>& /*neighbors_i*/,vector<int>& /*neighbor_faces_i*/,
								vector<int>& /*border*/))
{
	int nrVertices = m.getVertices().size(), count;
	bool a_ii_added = false;
	double factor;

	vector<int>::iterator j;

	vector<vector<int>> & neighbors = m.getNeighbors();
	vector<vector<int>> & neighbor_faces = m.getNeighborFaces();

	//set up indices some values might be zero. values are assumed to be only at (i,j) if i and j are neighbors
	
	count = 1;
	mat.ia.push_back(1);
	for(int i = 0; i < nrVertices; i++){
		count+= neighbors[i].size()+1;
		mat.ia.push_back(count); 
	}

	for(int i = 0; i < nrVertices;i++){
		vector<int> & nbrs_i = neighbors[i];
		vector<int> & nbr_fc_i = (neighbor_faces[i]);
		a_ii_added = false;

		//calculate normation factor
		factor = 0;

		for(j = nbrs_i.begin(); j!=nbrs_i.end(); j++){
			factor += weights(i,*j,m,nbrs_i,nbr_fc_i,border);
		}

		for(j = nbrs_i.begin(); j!=nbrs_i.end(); j++){
			if(i< *j &&! a_ii_added){
				mat.ja.push_back(i+1);
				a_ii_added = true;
				mat.a.push_back(weights(i,i,m,nbrs_i,nbr_fc_i,border));
			}
			mat.ja.push_back((*j) +1);
			mat.a.push_back((factor <0.0001?0:weights(i,*j,m,nbrs_i,nbr_fc_i,border)/factor));

		}
		if(!a_ii_added){
			mat.ja.push_back(i+1);
			a_ii_added = true;
			mat.a.push_back(weights(i,i,m,nbrs_i,nbr_fc_i,border));
		}
	}
	
	if(mat.ja.size()+1 != mat.ia.back()){ //last index + 1 
		throw std::runtime_error("Assertion failed, matrix malformed");
	}
	
}

/************************************************************************/
/* This sets up the parameter b in the tutte equation Ax = b
	if you are solving for the x coordinate of the tex coords
	for the equivalent y coords use the other method-
	sz: is the number of vertices, as this can not be read
	*/
/************************************************************************/
void TutteEmbedding::setUpX( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz )
{

	vector<int>::iterator el;
	b.clear();
	b.reserve(sz);
	int idx;
	for(int i = 0; i < sz; i++){
		//if i is on the border
		b.push_back(0);
		if((el=find(border.begin(), border.end(), i))!=border.end()){
			idx= el-border.begin();
			b[*el]= outerPos[idx].x;
		}
	}
}

//************************************
// Method:    setUpY
// FullName:  TutteEmbedding::setUpY
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: vector<double> & b
// Parameter: vector<int> & border
// Parameter: vector<tuple3f> & outerPos
// Parameter: int sz
//************************************
void TutteEmbedding::setUpY( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz )
{
	vector<int>::iterator el;
	b.clear();
	b.reserve(sz);
	int idx;
	for(int i = 0; i < sz; i++){
		b.push_back(0);
		if((el=find(border.begin(), border.end(), i))!=border.end()){
			idx= el-border.begin();
			b[*el]= outerPos[idx].y;
		}
	}
}

void TutteEmbedding::setUpXY( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, int sz )
{
	vector<int>::iterator el;
	b.clear();
	b.reserve(2*sz);
	int idx;
	for(int i = 0; i < sz; i++){
		b.push_back(0);
		if((el=find(border.begin(), border.end(), i))!=border.end()){
			idx= el-border.begin();
			b[*el]= outerPos[idx].x;
		}
	}
	for(int i = 0; i < sz; i++){
		b.push_back(0);
		if((el=find(border.begin(), border.end(), i))!=border.end()){
			idx= el-border.begin();
			b[*el+sz]= outerPos[idx].y;
		}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void TutteEmbedding::setUp_multiBorder( pardisoMatrix &mat, vector<vector<int>> &border, 
					vector<tuple3f> & outerPos, int outBorder, mesh & m,
					double (*weights ) (int /*i*/, int /*j*/, mesh & , 
						   vector<int>& /*neighbors_i*/,vector<int>& /*neighbor_faces_i*/,
						   vector<int>& /*border*/))
{
	vector<vector<float>> angles, lambdas;
	vector<vector<int>> & neighbors = m.getNeighbors();
	vector<vector<int>> & neighbor_faces = m.getNeighborFaces();
	vector<int> NULLBORDER;
//vector<int> & NULLBORDER = border[outBorder];


	vector<std::pair<int,int>> vertexIndices;
	int nrVertices = m.getVertices().size(), count;
	int myBorder, borderIndex, bordersz, offset;
	bool a_ii_added = false;
	double factor;
	vector<int>::iterator j;
	

	TutteWeights::angles_lambdas(angles,lambdas, border,m);
		//set up indices some values might be zero. values are assumed to be only at (i,j) if i and j are neighbors

	count = 1;
	mat.ia.push_back(1);

	//////////////////////////////////////////////////////////////////////////
	//x's
	//////////////////////////////////////////////////////////////////////////

	for(int i = 0; i < 2*nrVertices;i++){
		vector<int> & nbrs_i = neighbors[i%nrVertices];
		vector<int> & nbr_fc_i = (neighbor_faces[i%nrVertices]);
		myBorder = meshOperation::borderComponent(i%nrVertices,border, borderIndex);
		bordersz = (myBorder <0? 0: border[myBorder].size());
		offset = (i<nrVertices?0:nrVertices);
	
		//not on boder or a reflex angle
		if (myBorder < 0 || ((angles[myBorder][borderIndex] > PI)&&(myBorder != outBorder))){
//		if (myBorder < 0 || ((angles[myBorder][borderIndex] > PI) || (myBorder == outBorder))){
			a_ii_added = false;
			//calculate normation factor
			factor = 0;
			for(j = nbrs_i.begin(); j!=nbrs_i.end(); j++){
				factor += weights(i%nrVertices,*j,m,nbrs_i,nbr_fc_i,NULLBORDER);
			}

			for(j = nbrs_i.begin(); j!=nbrs_i.end(); j++){
				if(i< *j + offset &&! a_ii_added){
					mat.ja.push_back(i+1);
					a_ii_added = true;
					mat.a.push_back(weights(i%nrVertices,i%nrVertices,m,nbrs_i,nbr_fc_i,NULLBORDER));
				}
				mat.ja.push_back((*j)+offset +1);
				//note the following makes sense because the sum of vals = 0 means there is only a diagonal element..
				mat.a.push_back((factor <0.0001? 0: weights(i%nrVertices,*j,m,nbrs_i,nbr_fc_i,NULLBORDER)/factor));

			}

			if(!a_ii_added){
				mat.ja.push_back(i+1);
				a_ii_added = true;
				mat.a.push_back(weights(i%nrVertices,i%nrVertices,m,nbrs_i,nbr_fc_i,NULLBORDER));
			}
		}
		else if (myBorder == outBorder){
			mat.ja.push_back(i+1);
			mat.a.push_back(1);
		}
		else{
			//border -2 to border +2 iff it is a reflex angle
			// else act as usually., i.e. as at border < -1
			calculate(vertexIndices, border[myBorder], borderIndex);
			for(int i = 0; i < 5; i++){
				mat.ja.push_back(vertexIndices[i].first +1);
				mat.a.push_back(TutteWeights::angleMat(borderIndex, 
					borderIndex + vertexIndices[i].second , 
					angles[myBorder], lambdas[myBorder]));
			}

			for(int i = 0; i < 5; i++){
				mat.ja.push_back(vertexIndices[i].first+1);
				mat.a.push_back(TutteWeights::angleMat(borderIndex, 
					borderIndex + vertexIndices[i].second  + nrVertices, 
					angles[myBorder], lambdas[myBorder]));
			}
		}
		mat.ia.push_back(mat.ja.size() +1);
	}

	//////////////////////////////////////////////////////////////////////////
	// y's
	//////////////////////////////////////////////////////////////////////////
/*	for(int i = 0; i < nrVertices;i++){
		vector<int> & nbrs_i = neighbors[i];
		vector<int> & nbr_fc_i = (neighbor_faces[i]);
		myBorder = meshOperation::borderComponent(i,border);


		a_ii_added = false;
		//calculate normation factor
		factor = 0;
		for(j = nbrs_i.begin(); j!=nbrs_i.end(); j++){
			factor += weights(i,*j,m,nbrs_i,nbr_fc_i,border);
		}

		for(j = nbrs_i.begin(); j!=nbrs_i.end(); j++){
			if(i< *j &&! a_ii_added){
				mat.ja.push_back(i+nrVertices + 1);
				a_ii_added = true;
				mat.a.push_back(weights(i,i,m,nbrs_i,nbr_fc_i,border));
			}
			mat.ja.push_back((*j) +1);
			//note the following makes sense because the sum of vals = 0 means there is only a diagonal element..
			mat.a.push_back((factor <0.0001? 0: weights(i,*j,m,nbrs_i,nbr_fc_i,border)/factor));

		}

		if(!a_ii_added){
			mat.ja.push_back(i+ nrVertices+1);
			a_ii_added = true;
			mat.a.push_back(weights(i,i,m,nbrs_i,nbr_fc_i,border));
		}
	}*/

	if(mat.ja.size()+1 != mat.ia.back()){ //last index + 1 
		throw std::runtime_error("Assertion failed, matrix malformed");
	}

}

int TutteEmbedding::outerBorder( vector<vector<int>> &border, mesh & m )
{
	unsigned int max = 0;
	int max_i = -1;
	for(unsigned int i = 0; i < border.size(); i++){
		if(border[i].size() > max){
			max = border[i].size();
			max_i = int(i);
		}
	}
	return max_i;
}

//stores the indices of -2...+2 border neighbors in ascending order and the corresponding offset
//-2....+2 in offsets
void TutteEmbedding::calculate( vector<std::pair<int,int>> & vertices, vector<int> & border, int borderIndex )
{
	vertices.clear();
	for(int i = 0; i < 5; i++){
		vertices.push_back(std::pair<int,int>());
		vertices[i].first = border[(borderIndex-2 +i+border.size()) % border.size()];
		vertices[i].second = i-2;
	}

	sort(vertices.begin(),vertices.end(),pairComp::comparator);
}

bool pairComp::comparator( const std::pair<int,int>& l, 
		const std::pair<int,int>& r )
{
	return l.first < r.first; 
}
