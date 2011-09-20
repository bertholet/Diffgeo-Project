/*
* Pardiso Header for Visual C++
*
*/

/* PARDISO prototype. */
#pragma once
#include "pardisoMatrix.h"


extern "C" __declspec(dllimport) void pardisoinit (void   *, int    *,   int *, int *, double *, int *);
extern "C" __declspec(dllimport) void pardiso     (void   *, int    *,   int *, int *,    int *, int *, 
												   double *, int    *,    int *, int *,   int *, int *,
												   int *, double *, double *, int *, double *);
extern "C" __declspec(dllimport) void pardiso_chkmatrix  (int *, int *, double *, int *, int *, int *);
extern "C" __declspec(dllimport) void pardiso_chkvec     (int *, int *, double *, int *);
extern "C" __declspec(dllimport) void pardiso_printstats (int *, int *, double *, int *, int *, int *,
														  double *, int *);


/************************************************************************/
/* A wrapper class for the pardiso solver. Note: there are more
*  possible Attributes and configurations than accessible by this wrapper
*/
/************************************************************************/
class pardisoSolver{

private: 
	int int_params[64];
	double double_params[64];
	void *intern_memory[64];

	int matrix_type;
	int print_stats;
	int error;
	int nrhs;

	void init_intParams(int nrRefinementSteps);
	void checkError_init() 
	{
		//error = 0;
		if (error != 0) 
		{
			if (error == -10 )
				printf("No license file found \n");
			else if (error == -11 )
				printf("License is expired \n");
			else if (error == -12 )
				printf("Wrong username or hostname \n");
			else
				printf("Error %d has occurred\n", error);

		}
		else
			printf("[PARDISO]: License check was successful ... \n");
	}



public:
	//other matrix types do exist, see manual.
	static const int MT_ANY=11,MT_SYMMETRIC=-2,MT_STRUCTURALLY_SYMMETRIC=1;
	//solver types
	static const int SOLVER_DIRECT = 0, SOLVER_ITERATIVE=1;
	pardisoMatrix * matrix;
	pardisoSolver(
			int  matrix_typ, int solver,
			int nr_refinement_steps){
		error = 0;
		matrix = NULL; 
		matrix_type = matrix_typ;
		init_intParams(nr_refinement_steps);
		pardisoinit(intern_memory, &matrix_type, &solver, int_params, double_params, &error);
		checkError_init();

	}


	~pardisoSolver(void){
		int phase = -1;
		int maxfct =1; /*max nr of factorizations*/
		int mnum =1; /* Which factorization to use. */
		int n = matrix->ia.size()-1;

		pardiso (intern_memory, &maxfct, &mnum, &matrix_type, &phase,
			&n, &matrix->a[0], &matrix->ia[0], &matrix->ja[0], NULL, &nrhs,
			int_params, &print_stats, NULL, NULL, &error, double_params);
	}

	/************************************************************************/
	/* nr_righthandsides is the number of rows of b in Ax = b.                                                                     */
	/************************************************************************/
	void setMatrix(pardisoMatrix & mat, int nr_righthandsides)
	{
		matrix = &mat;
		nrhs = nr_righthandsides;
		checkMatrix(matrix_type, mat);

		//factorization: symbolic and numerical
		int phase = 12;
		int maxfct =1; /*max nr of factorizations*/
		int mnum =1; /* Which factorization to use. */
		int n = mat.ia.size()-1;

		pardiso (intern_memory, &maxfct, &mnum, &matrix_type, &phase,
			&n, &mat.a[0], &mat.ia[0], &mat.ja[0], NULL, &nr_righthandsides,
			int_params, &print_stats, NULL, NULL, &error, double_params);
	}


	void setPrintStatistics(bool print){
		if(print){
			print_stats = 1;
		}
		else{
			print_stats =0;
		}
	}

	void solve(double *x, double * b){
		int phase = 33;
		int maxfct =1; /*max nr of factorizations*/
		int mnum =1; /* Which factorization to use. */
		int n = matrix->ia.size()-1;

		error = 0;
		pardiso (intern_memory, &maxfct, &mnum, &matrix_type, &phase,
			&n, &matrix->a[0], &matrix->ia[0], &matrix->ja[0], NULL, &nrhs,
			int_params, &print_stats, b, x, &error, double_params);

		if(error != 0){
			throw std::exception("Exception in pardiso solve-");
		}
	}

	static void checkMatrix( int matrix_type, pardisoMatrix & mat ) 
	{
		int n = mat.ia.size() -1;
		int err;
		pardiso_chkmatrix  (&matrix_type, &n, & mat.a[0], & mat.ia[0], 
			& mat.ja[0], &err);

		if (err != 0) {
			printf("\nERROR in consistency of matrix: %d", err);

			throw std::runtime_error("Pardiso::checkmatrix error");
		}
	}


};

