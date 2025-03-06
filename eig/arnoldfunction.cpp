/*
 * arnoldfunction.cpp
 *
 *  Created on: 30/09/2008
 *      Author: onofre
 */


#include <arpack_fortran.h>
#include <arnoldfunction.h>


namespace lev_math {

void arpack_aupd(int &iido,const char *bbbmat, long nn,const char* wwwhich, long nnev,
		double &ttol, double rResid[], long nncv, double vV[],
		long lldv, int iiparam[], int iipntr[], double wWorkd[],
		double wWorkl[], long llworkl, int& iinfo,double fflag)
{
	dsaupd_(&iido,bbbmat, &nn,wwwhich, &nnev, &ttol, &rResid[1], &nncv,&vV[1],
			&lldv, &iiparam[1], &iipntr[1], &wWorkd[1],&wWorkl[1],
			&llworkl, &iinfo);
}


void  arpack_eupd(bool rvec, const char *hHowMny,  double D[], double Z[],
		long ldz, double sigma, const char *bbmat, long n,
		const char* wwhich, long nev, double tol, double Resid[],
		long ncv, double V[], long ldv, int iparam[],
		int ipntr[], double Workd[], double Workl[],
		long lworkl, int &info,double flag )
{
	int   	irvec;
	int  		*iselect;
	double 	*iZ;
	irvec   = (int)rvec;
	iselect = new int[ncv];
	iZ = (Z == NULL) ? &V[1] : Z;

	dseupd_(&irvec, hHowMny, iselect, &D[1], iZ, &ldz,
			&sigma, bbmat,&n, wwhich, &nev, &tol, &Resid[1], &ncv,
			&V[1], &ldv, &iparam[1], &ipntr[1], &Workd[1], &Workl[1], &lworkl, &info );

	delete [] iselect;
	iselect = NULL;
}


void arpack_aupd(int &ido,const char *bbmat, long n,const char* wwhich, long nev,
		float &tol, float Resid[], long ncv, float V[],
		long ldv, int iparam[], int ipntr[], float Workd[],
		float Workl[], long lworkl, int& info, float flag)
{
	ssaupd_(&ido,bbmat, &n,wwhich, &nev, &tol, &Resid[1], &ncv,&V[1],
			&ldv, &iparam[1], &ipntr[1], &Workd[1],&Workl[1],
			&lworkl, &info);
}


void   arpack_eupd(bool rvec, const char *hHowMny,  float D[], float Z[],
		long ldz, float sigma, const char *bbmat, long n,
		const char* wwhich, long nev, float tol, float Resid[],
		long ncv, float V[], long ldv, int iparam[],
		int ipntr[], float Workd[], float Workl[],
		long lworkl, int &info,float flag)
{
	int   	irvec;
	int  		*iselect;
	float 	*iZ;
	irvec   = (int)rvec;
	iselect = new int[ncv];
	iZ = (Z == NULL) ? &V[1] : Z;

	sseupd_(&irvec, hHowMny, iselect, &D[1], iZ, &ldz,
			&sigma, bbmat,&n, wwhich, &nev, &tol, &Resid[1], &ncv,
			&V[1], &ldv, &iparam[1], &ipntr[1], &Workd[1], &Workl[1], &lworkl, &info );

	delete [] iselect;
	iselect = NULL;
}


} // Final do namespace

