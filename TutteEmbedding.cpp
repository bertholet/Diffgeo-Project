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
	vector<int> border;
	vector<int> loops;
	vector<double> b;
	meshOperation::getBorder(m,border,loops);

	if(loops.size() > 1){
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
	setUp(mat, border, m, TutteWeights::uniform_weights);
	parsolver.setMatrix(mat, 1);

	TutteWeights::circleBorder(outerPos, border, loops, m);
	setUpX(b, border,outerPos, m.getVertices().size());
	parsolver.solve(x,&b[0]);

	setUpY(b, border,outerPos, m.getVertices().size());
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
										vector<int> & /*loops*/, 
										mesh & /*m*/))
{
	vector<int> border;
	vector<int> loops;
	vector<double> b;
	meshOperation::getBorder(m,border,loops);

	if(loops.size() > 1){
		printf("Error: Only a single border allowed with this Method! in tutteEmbedding::calcTexturePos");
		throw std::exception("Only a single border allowed with this Method");
	}

	vector<tuple3f> outerPos;
	double * x = new double[m.getVertices().size()];
	double * y = new double[m.getVertices().size()];

	getBorderPos(outerPos, border, loops,m);

	pardisoMatrix mat;
	pardisoSolver parsolver(pardisoSolver::MT_STRUCTURALLY_SYMMETRIC,
		pardisoSolver::SOLVER_ITERATIVE,
		2);
	setUp(mat, border, m, weights);
	parsolver.checkMatrix(parsolver.MT_STRUCTURALLY_SYMMETRIC, mat);
	parsolver.setMatrix(mat, 1);

	//getBorderPos(outerPos, border, loops,m);
	//getBorderPos(outerPos,border,m);
	setUpX(b, border,outerPos, m.getVertices().size());
	parsolver.solve(x,&b[0]);

	setUpY(b, border,outerPos, m.getVertices().size());
	parsolver.solve(y,&b[0]);

	m.setTextures_perVertex(x,y);

	delete[] x,y;
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
	vector<int> * neighbors = new vector<int>[nrVertices];
	vector<int> * neighbor_faces = new vector<int>[nrVertices];
	double factor;

	//vector<int> & nbrs_i = NULL;
	//vector<int> * nbr_fc_i = NULL;
	vector<int>::iterator j;
	meshOperation::getNeighbors(m.getFaces(), neighbors);
	meshOperation::getNeighborFaces(m.getFaces(), neighbor_faces);
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
		throw std::exception("Assertion failed, matrix malformed");
	}
	
	
	delete[] neighbors, neighbor_faces; 
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
