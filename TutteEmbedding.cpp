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
	vector<vector<float>> angles, lambdas;
	vector<double> b;
	pardisoMatrix mat;
	double * xy = new double[2*m.getVertices().size()];
	for(int i = 0; i < 2*m.getVertices().size();i++){
		xy[i] = 0.0;
	}

	meshOperation::getBorder(m, border);
	int outBorder = outerBorder(border,m);
	TutteWeights::angleApproxBorder(outerPos,border[outBorder],m);

	TutteWeights::angles_lambdas(angles,lambdas,border,outBorder,m);
	setUp_multiBorder(mat,border,outerPos,outBorder,angles, lambdas, m,weights);

//	mat.saveMatrix("C:/Users/bertholet/Dropbox/matrix_multiBorder.m");
	
	pardisoSolver s(pardisoSolver::MT_ANY, pardisoSolver::SOLVER_ITERATIVE,2);

	s.checkMatrix(pardisoSolver::MT_ANY,mat);
	s.setMatrix(mat,1);
	setUpXY_reflex(b, border[outBorder],outerPos, angles[outBorder], m.getVertices().size());

	s.setPrintStatistics(true);
	s.solve(xy,&(b[0]));

	m.setTextures_perVertex(xy);
	delete[] xy;
}

/************************************************************************/
/* Calculate stuff for fixed border                                     */
/************************************************************************/
void TutteEmbedding::calcTexturePos_multiBorder( mesh &m, 
												vector<vector<tuple3f>> & pos,
												vector<vector<int>> & border,
												double (*weights ) (int, int,
												mesh &,
												vector<int>& /*nbr_i*/,
												vector<int>&/*fc_i*/,
												vector<int>& /*border*/) )
{

	vector<double> b;
	pardisoMatrix mat;
	double * xy = new double[2*m.getVertices().size()];
	for(int i = 0; i < 2*m.getVertices().size();i++){
		xy[i] = 0.0;
	}

	setUp_multiBorder(mat,border,m,weights);

//	mat.saveMatrix("C:/Users/bertholet/Dropbox/matrix_multiBorder2.m");

	pardisoSolver s(pardisoSolver::MT_ANY, pardisoSolver::SOLVER_ITERATIVE,2);

	s.checkMatrix(pardisoSolver::MT_ANY,mat);
	s.setMatrix(mat,1);
	setUpXY_fixBorder(b, border, pos, m.getVertices().size());

	s.setPrintStatistics(true);
	s.solve(xy,&(b[0]));

	m.setTextures_perVertex(xy);
	delete[] xy;
}

/************************************************************************/
/* Set up Matrix for a single border                                                                    */
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


void TutteEmbedding::setUpXY_reflex( vector<double>& b, vector<int> & border, vector<tuple3f> & outerPos, vector<float> & angles, int sz )
{
	vector<int>::iterator el;
	b.clear();
	b.reserve(2*sz);
	int idx;
	for(int i = 0; i < sz; i++){
		b.push_back(0);
		if((el=find(border.begin(), border.end(), i))!=border.end()){
			idx= el-border.begin();
			if(angles[idx] < 0){
				b[*el] = 0;
			}
			else{
				b[*el]= outerPos[idx].x;
			}
		}
	}
	for(int i = 0; i < sz; i++){
		b.push_back(0);
		if((el=find(border.begin(), border.end(), i))!=border.end()){
			idx= el-border.begin();
			if(angles[idx] < 0){
				b[*el+sz] = 0;
			}
			else{
				b[*el+sz]= outerPos[idx].y;
			}
		}
	}
}



void TutteEmbedding::setUpXY_fixBorder( vector<double>& b, vector<vector<int>> & border, vector<vector<tuple3f>> & pos, int sz )
{
	b.clear();
	b.reserve(2*sz);
	int borderInd, brdr;
	
	for(int i = 0; i < sz; i++){
		brdr = meshOperation::borderComponent(i,border,borderInd);
		b.push_back(0);
		if(brdr >=0){
			b[i]= pos[brdr][borderInd].x;
		}
	}
	for(int i = 0; i < sz; i++){
		brdr = meshOperation::borderComponent(i,border,borderInd);
		b.push_back(0);
		if(brdr >=0){
			b[i+sz]= pos[brdr][borderInd].y;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void TutteEmbedding::setUp_multiBorder( pardisoMatrix &mat, vector<vector<int>> &border, 
					vector<tuple3f> & outerPos, int outBorder, 
					vector<vector<float>> & angles,
					vector<vector<float>> & lambdas,
					mesh & m,
					double (*weights ) (int /*i*/, int /*j*/, mesh & , 
						   vector<int>& /*neighbors_i*/,vector<int>& /*neighbor_faces_i*/,
						   vector<int>& /*border*/))
{
	vector<vector<int>> & neighbors = m.getNeighbors();
	vector<vector<int>> & neighbor_faces = m.getNeighborFaces();
	vector<int> NULLBORDER;


	vector<std::pair<int,int>> vertexIndices;
	int nrVertices = m.getVertices().size(), count;
	int myBorder, borderIndex, bordersz, offset;
	bool a_ii_added = false;
	double factor;
	vector<int>::iterator j;
	

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
		if (myBorder < 0 || ((angles[myBorder][borderIndex] < 0) && myBorder == outBorder) 
			//orientation of inner circles will be swapped
			||((angles[myBorder][borderIndex] < 0) && myBorder != outBorder)){

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

			calcOrdered(vertexIndices,border[myBorder],borderIndex);
			for(int k = 0; k < 3; k++){
				mat.ja.push_back(vertexIndices[k].first +1);
				mat.a.push_back(TutteWeights::turningWeight(borderIndex +(i<nrVertices? 0:bordersz), 
					(borderIndex + vertexIndices[k].second+bordersz)%bordersz , 
					angles[myBorder], lambdas[myBorder]));
			}
			for(int k = 0; k < 3; k++){
				mat.ja.push_back(vertexIndices[k].first +1 +nrVertices);
				mat.a.push_back(TutteWeights::turningWeight(borderIndex+(i<nrVertices? 0:bordersz), 
					(borderIndex + vertexIndices[k].second+bordersz)%bordersz +bordersz, 
					angles[myBorder], lambdas[myBorder]));
			}

		}
		mat.ia.push_back(mat.ja.size() +1);
	}


	if(mat.ja.size()+1 != mat.ia.back()){ //last index + 1 
		throw std::runtime_error("Assertion failed, matrix malformed");
	}

}

/************************************************************************/
/* Set up matrix for fixed precalculated border                         */
/************************************************************************/
void TutteEmbedding::setUp_multiBorder( pardisoMatrix &mat, vector<vector<int>> &border, 
					mesh & m,
					double (*weights ) (int /*i*/, int /*j*/, mesh & , 
						   vector<int>& /*neighbors_i*/,vector<int>& /*neighbor_faces_i*/,
						   vector<int>& /*border*/))
{

	vector<vector<int>> & neighbors = m.getNeighbors();
	vector<vector<int>> & neighbor_faces = m.getNeighborFaces();
	vector<int> NULLBORDER;

	vector<std::pair<int,int>> vertexIndices;
	int nrVertices = m.getVertices().size(), count;
	int myBorder, borderIndex, bordersz, offset;
	bool a_ii_added = false;
	double factor;
	vector<int>::iterator j;
	

//	TutteWeights::angles_lambdas(angles,lambdas, border,m);
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
		if (myBorder < 0 ){
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
		else {
			mat.ja.push_back(i+1);
			mat.a.push_back(1);
		}
		mat.ia.push_back(mat.ja.size() +1);
	}


	if(mat.ja.size()+1 != mat.ia.back()){ //last index + 1 
		throw std::runtime_error("Assertion failed, matrix malformed");
	}

}

int TutteEmbedding::outerBorder( vector<vector<int>> &border, mesh & m )
{
	float max = 0,length;
	int max_i = -1;
	for(unsigned int i = 0; i < border.size(); i++){
		length = meshOperation::getLength(border[i],m);
		if(length > max){
			max = length;
			max_i = int(i);
		}
	}
	return max_i;
}

//stores the indices of -2...+2 border neighbors in ascending order and the corresponding offset
//-2....+2 in offsets
void TutteEmbedding::calcOrdered( vector<std::pair<int,int>> & vertices, vector<int> & border, int borderIndex )
{
	/*vertices.clear();
	for(int i = 0; i < 5; i++){
		vertices.push_back(std::pair<int,int>());
		vertices[i].first = border[(borderIndex-2 +i+border.size()) % border.size()];
		vertices[i].second = i-2;
	}*/
	vertices.clear();
	for(int i = 0; i < 3; i++){
		vertices.push_back(std::pair<int,int>());
		vertices[i].first = border[(borderIndex-1 +i +border.size()) % border.size()];
		vertices[i].second = i-1;
	}

	sort(vertices.begin(),vertices.end(),pairComp::comparator);
}

bool pairComp::comparator( const std::pair<int,int>& l, 
		const std::pair<int,int>& r )
{
	return l.first < r.first; 
}
