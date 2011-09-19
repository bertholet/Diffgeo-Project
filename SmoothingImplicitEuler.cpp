#include "StdAfx.h"
#include "SmoothingImplicitEuler.h"
#include <algorithm>
#include <Windows.h>
#include "Operator.h"
#include <math.h>
#include "pardiso.h"

using namespace std;


ImplicitEulerSmoothing::ImplicitEulerSmoothing( mesh &m, float lambda, float dt)
{
	vector<tuple3f> & vertices = m.vertices;
	vector<int>  * neighbors = new vector<int>[vertices.size()];
	vector<int>::iterator low;

	//debug values..

	//vector<int> & neighbors[] = *neighborsptr;


	/*n = 10;
	for(int i =0; i < 10; i++){
		ia.push_back(2*i+1);
		ja.push_back(i+1);
		a.push_back(i+5);
		if(i<9){
			ja.push_back(i+2);
			a.push_back(i*0.1 + 2);
		}
		
	}
	ia.push_back(20);
	b= new double[n][1];
	x= new double[n][1];

	for(int i = 0; i < 10; i++){
		b[i][0] = i;
	}*/


	//size of the matrix 

	n = m.vertices.size();
	//Note: if x or b are matrices i.e. have multiple columns pardiso expects
	//a parameter vector where the COLUMNS are enumerated one after the other.
	//NOT & b[0][0] with b n x m array.
	b = new double[vertices.size()*NRHS];
	for(unsigned int i = 0; i < vertices.size(); i++){
		b[i] = vertices[i].x; //b[i][0]
		b[vertices.size()+i] = vertices[i].y;
		b[2*vertices.size()+i] = vertices[i].z;

	}
	x= new double[vertices.size()*NRHS];

	//find neighbors and sotre them in neighbors
	findNeighbors(m.faces, neighbors);
	setUpSparseMatrix(vertices, neighbors, lambda, dt);
	//correct up to here! ia, ja, a.

	delete[] neighbors;

	pardisoInitFactorize();
}

ImplicitEulerSmoothing::~ImplicitEulerSmoothing(void)
{

	// Release Pardiso memory...
	phase = -1;                 // Release internal memory. 

	pardiso (pt, &maxfct, &mnum, &matrix_type, &phase,
		&n, NULL, &ia[0], &ja[0], NULL, &nr_eqs,
		int_params, &print_stats, NULL, NULL, &error,  double_params);

	delete[] b;
	delete[] x;

}


void ImplicitEulerSmoothing::pardisoInitFactorize( void )
{
	matrix_type = 1;        /* Real STRUCTURALLY symmetric matrix : 1*/

	nr_eqs = NRHS;          /* Number of right hand sides. */



	///////////////Put this stuff in the constructor...
	error = 0;

	//set parameter here...
	solver = 1; /* 0: direct solving, 1 iteratvie solving*/
	maxfct = 1;		/* Maximum number of numerical factorizations.  */
	mnum   = 1;         /* Which factorization to use. */

	print_stats = 0;         /* Print statistical information  */

	SYSTEM_INFO sysinfo; 	GetSystemInfo( &sysinfo );  	
	num_procs = sysinfo.dwNumberOfProcessors;    
	int_params[0] = 0; //use default params..
	int_params[2]  = num_procs;

	//all parameters have to be set after init!. (but for param 2 = num_procs)
	//int_params[3] = 91;	//hmhmhm
	int_params[7] = 1;//1       /* Max numbers of iterative refinement steps. */
	int_params[6] = 0;	//int_params[6] = 1; then the result is placed in b.
	int_params[32] = 1; // compute determinant 


	pardisoinit (pt,  &matrix_type, &solver, int_params, double_params, &error); 

	error = 0;
	if (error != 0) 
	{
		if (error == -10 )
			printf("No license file found \n");
		if (error == -11 )
			printf("License is expired \n");
		if (error == -12 )
			printf("Wrong username or hostname \n");

		int ah;
		cin >> ah;
	}
	else
		printf("[PARDISO]: License check was successful ... \n");

	n = ia.size() -1;
	pardiso_chkmatrix  (&matrix_type, &n, & a[0], & ia[0], &ja[0], &error);
	if (error != 0) {
		printf("\nERROR in consistency of matrix: %d", error);

		exit(1);
	}

	//Phases 1-2
	phase = 12;
	//may be this is even correct....


	pardiso (pt, &maxfct, &mnum, &matrix_type, &phase,
		&n, &a[0], &ia[0], &ja[0], NULL, &nr_eqs,
		int_params, &print_stats, NULL, NULL, &error, double_params);


}


void ImplicitEulerSmoothing::smootheMesh( mesh &m )
{
	float oldVolume = Operator::volume(m);
	//refactor this stuff asap
	//put killing this annoying library in the destructor. init stuff in the constructor.
	//(muhahaha destoy it, destroy it...)

//	int      matrix_type = 1;        /* Real STRUCTURALLY symmetric matrix : 1*/

//    int      nrhs = NRHS;          /* Number of right hand sides. */

	/* Internal solver memory pointer pt,                  */
	/* 32-bit: int pt[64]; 64-bit: long int pt[64]         */
	/* or void *pt[64] should be OK on both  architectures  */ 
//	void    *pt[64]; 

	/* Pardiso control parameters. */
//	int      int_params[64];
//	double   double_params[64];
//	int      maxfct, mnum, phase, error, msglvl, solver;

	/* Number of processors. */
//	int      num_procs;



	///////////////Put this stuff in the constructor...
	error = 0;

//set parameter here...
//	solver = 1; /* 0: direct solving, 1 iteratvie solving*/
//	maxfct = 1;		/* Maximum number of numerical factorizations.  */
//	mnum   = 1;         /* Which factorization to use. */

//	msglvl = 0;         /* Print statistical information  */

//	SYSTEM_INFO sysinfo; 	GetSystemInfo( &sysinfo );  	
//	num_procs = sysinfo.dwNumberOfProcessors;    
//	int_params[0] = 0; //use default params..
//	int_params[2]  = num_procs;


	

	//all parameters have to be set after init!. (but for param 2 = num_procs)
	//int_params[3] = 91;	//hmhmhm
//	int_params[7] = 1;//1       /* Max numbers of iterative refinement steps. */
//	int_params[6] = 0;
	//int_params[6] = 1; then the result is placed in b.
//	int_params[32] = 1; // compute determinant 

/*	pardisoinit (pt,  &matrix_type, &solver, int_params, double_params, &error); 

	if (error != 0) 
	{
		if (error == -10 )
			printf("No license file found \n");
		if (error == -11 )
			printf("License is expired \n");
		if (error == -12 )
			printf("Wrong username or hostname \n");

		int ah;
		cin >> ah;
	}
	else
		printf("[PARDISO]: License check was successful ... \n");
*/


/*	n = ia.size() -1;
	pardiso_chkmatrix  (&matrix_type, &n, & a[0], & ia[0], &ja[0], &error);
	if (error != 0) {
		printf("\nERROR in consistency of matrix: %d", error);

		exit(1);
	}
//	else
//		printf("Matrice is well formed...\n");

	/*pardiso_printstats (&mtype, &n, &a[0], &ia[0], &ja[0], &nrhs, &b[0][0], &error);
	if (error != 0) {
		printf("\nERROR right hand side: %d", error);
		exit(1);
	}*/


	/* -------------------------------------------------------------------- */
	/* ..  Reordering and Symbolic Factorization.  This step also allocates */
	/*     all memory that is necessary for the factorization.              */
	/* -------------------------------------------------------------------- */
/*	phase = 11; 

	pardiso (pt, &maxfct, &mnum, &mtype, &phase,
		&n, &a[0], &ia[0], &ja[0], NULL, &nrhs,
		int_params, &msglvl, NULL, NULL, &error, double_params);

	if (error != 0) {
		printf("\nERROR during symbolic factorization: %d", error);
		exit(1);
	}
	printf("\nReordering completed ... ");
	printf("\nNumber of nonzeros in factors  = %d", int_params[17]);
	printf("\nNumber of factorization MFLOPS = %d", int_params[18]);


	/* -------------------------------------------------------------------- */
	/* ..  Numerical factorization.                                         */
	/* -------------------------------------------------------------------- */    
/*	phase = 22;
	int_params[32] = 1; // compute determinant 

	pardiso (pt, &maxfct, &mnum, &mtype, &phase,
		&n, &a[0], &ia[0], &ja[0], NULL, &nrhs,
		int_params, &msglvl, NULL, NULL, &error,  double_params);

	if (error != 0) {
		printf("\nERROR during numerical factorization: %d", error);
		exit(2);
	}
	printf("\nFactorization completed ...\n ");

	/* -------------------------------------------------------------------- */    
	/* ..  Back substitution and iterative refinement.                      */
	/* -------------------------------------------------------------------- */    
/*	phase = 33;

	int_params[7] = 1;       // Max numbers of iterative refinement steps. 

	pardiso (pt, &maxfct, &mnum, &mtype, &phase,
		&n, &a[0], &ia[0], &ja[0], NULL, &nrhs,
		int_params, &msglvl, b, x, &error,  double_params);

	if (error != 0) {
		printf("\nERROR during solution: %d", error);
		exit(3);
	}

*/

	//Phases 1-3
	//phase = 13;
	//may be this is even correct....

	phase = 33;

	pardiso (pt, &maxfct, &mnum, &matrix_type, &phase,
		&n, &a[0], &ia[0], &ja[0], NULL, &nr_eqs,
		int_params, &print_stats, b, x, &error, double_params);

	// Release memory...
/*	phase = -1;                 // Release internal memory. 

	pardiso (pt, &maxfct, &mnum, &matrix_type, &phase,
		&n, NULL, &ia[0], &ja[0], NULL, &nrhs,
		int_params, &msglvl, NULL, NULL, &error,  double_params);
*/


	//testing...

	int i = 0;

	double err_= calcBoxNormError();
	cout << "MAXIMUM ERROR: " << err_ << "\n";
	
	//print first values of x;
	/*cout <<"x(1:8,:)\n";
	for(i = 0; i < 8; i++){
		for (int j = 0; j < nrhs; j++){
			cout<<x[i + j * n] <<",";
		}
		cout << "\n";
	}

	//print first values of b;
	cout <<"b(1:8,:)\n";
	for(i = 0; i < 8; i++){
		for (int j = 0; j < nrhs; j++){
			cout<<b[i + j *n] <<",";
		}
		cout << "\n";
	}//*/

	updateVerticesAndB(m);

	float factor = pow(oldVolume / Operator::volume(m), 0.33333333f);
	m.scaleVertices(factor);

	cout<< "Volume: " << Operator::volume(m)<< "\n";
}


void ImplicitEulerSmoothing::ifNotContainedAdd( vector<int> &v, int a )
{
	vector<int>::iterator low;

	low = lower_bound(v.begin(), v.end(),a);
	if(low == v.end() || *low != a){
		v.insert(low,1,a);
	}
}

void ImplicitEulerSmoothing::findNeighbors( vector<tuple3i> &faces, vector<int> * neighbors ) 
{
	for(unsigned int i = 0; i < faces.size(); i++){

		/*debug[faces[i].a]++;
		debug[faces[i].b]++;
		debug[faces[i].c]++;*/

		//adds the integers such that the vector stays ordered.

		ifNotContainedAdd(neighbors[faces[i].a], faces[i].b);
		ifNotContainedAdd(neighbors[faces[i].b], faces[i].a);

		ifNotContainedAdd(neighbors[faces[i].a], faces[i].c);
		ifNotContainedAdd(neighbors[faces[i].c], faces[i].a);

		ifNotContainedAdd(neighbors[faces[i].b], faces[i].c);
		ifNotContainedAdd(neighbors[faces[i].c], faces[i].b);

	}
}

void ImplicitEulerSmoothing::setUpSparseMatrix( vector<tuple3f> &vertices, vector<int> * neighbors, float lambda, float dt ) 
{

	float minus_lambdaDt_div_m;
	//set up ia
	ia.reserve(vertices.size()+1);
	ia.push_back(0);
	int j = 0;
	int nb;
	bool added_i;
	for(unsigned int i = 0; i < vertices.size(); i++){
		nb = neighbors[i].size();

	/*	if(nb == 0){
			throw new std::exception("Assertion failed. The mesh is malformed: it contains unconnected nodes");
		}*/
		ia.push_back((ia.back()) + nb +1);
	}

	//set up ja
	ja.reserve(vertices.size()*6);
	int sz;
	int vsize = vertices.size();
	for(int i = 0; i < vsize; i++){
		j=0;
		added_i = false;
		sz = neighbors[i].size();
		if(sz != 0){
			nb = neighbors[i][j];
			for(j = 0; j < sz; j++){
				nb = neighbors[i][j];
				if(added_i == false && i < nb){
					added_i = true;
					ja.push_back(i);
				}
				ja.push_back(nb);
			}
		}

		if(added_i == false){
			added_i = true;
			ja.push_back(i);
		}


	}

	//set up values of the sparse matrix
	a.reserve(ja.size());
	for(int i = 0; i < vsize; i++){

		j=0;
		added_i = false;
		sz = neighbors[i].size();
		if(sz != 0){
			nb = neighbors[i][j];
			minus_lambdaDt_div_m = -lambda * dt /sz;

			for(j = 0; j < sz; j++){
				nb = neighbors[i][j];
				if(added_i == false && i < nb){
					added_i = true;
					a.push_back(1 + lambda * dt);
				}
				a.push_back(minus_lambdaDt_div_m);
			}
		}

		if(added_i == false){

			added_i = true;
			a.push_back(1 + lambda * dt);
		}

	}

	/* -------------------------------------------------------------------- */
	/* ..  Convert matrix from 0-based C-notation to Fortran 1-based        */
	/*     notation.                                                        */
	/* -------------------------------------------------------------------- */
	for (unsigned int i = 0; i < ia.size(); i++) {
		ia[i] += 1;
	}
	for (unsigned int i = 0; i < ja.size(); i++) {
		ja[i] += 1;
	}
}


void ImplicitEulerSmoothing::updateVerticesAndB( mesh & m )
{
	int sz = m.vertices.size();
	for(int i = 0; i < m.vertices.size(); i++){
		m.vertices[i].x = (float)x[i + 0 * sz];
		m.vertices[i].y = (float)x[i + 1 * sz];
		m.vertices[i].z = (float) x[i + 2* sz];
	}

	//swap x,b
	double * temp;
	temp = b;
	b = x;
	x= temp;
	/*for(int i = 0; i < NRHS*sz; i++){
		b[i] = x[i];
	}*/
}

double ImplicitEulerSmoothing::calcBoxNormError(void) 
{
	int i;
	double res;
	double err_ =0;
	for(int k = 0; k < NRHS; k++){
		for(i = 0; i < n; i++){
			res = 0;
			for(int j = 0 ; j < ia[i+1]-ia[i]; j++){
				res += a[ia[i] + j -1]*x[ja[ia[i]+ j -1]-1 + k*n];
			}
			res = res -b[i + k* n];
			res = (res >0 ?  res: -res);
			err_ = (res > err_? res: err_);
		}
	}
	return err_;
}
