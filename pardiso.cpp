#include "StdAfx.h"
#include "pardiso.h"
#include <Windows.h>

void pardisoSolver::init_intParams(int nrRefinementSteps)
{
	SYSTEM_INFO sysinfo; 	GetSystemInfo( &sysinfo );  	
	int num_procs = sysinfo.dwNumberOfProcessors;    
	int_params[0] = 0; //use default params..
	int_params[2]  = num_procs;

	//all parameters have to be set after init!. (but for param 2 = num_procs)
	//int_params[3] = 91;	//hmhmhm
	int_params[7] = nrRefinementSteps;//1       /* Max numbers of iterative refinement steps. */
	int_params[6] = 0;	//int_params[6] = 1; then the result is placed in b.
	int_params[32] = 1; // compute determinant 
}
