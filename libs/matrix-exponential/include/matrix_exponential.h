#ifndef __cplusplus
#include <complex.h>
/*
  Complex function.
*/
double complex *c8mat_expm1 ( int n, double complex a[] );
#endif
/*
  Real functions.
*/
#ifdef __cplusplus
extern "C" 
{
#endif
	double *r8mat_expm1 ( int n, double a[] );
	double *r8mat_expm2 ( int n, double a[] );
	double *r8mat_expm3 ( int n, double a[] );
#ifdef __cplusplus
}
#endif
