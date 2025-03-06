#ifndef ARN_FUNC_H
#define ARN_FUNC_H


#include <stddef.h>


namespace lev_math {

 	void arpack_aupd(int &iido,const char *bbbmat, long nn,const char* wwwhich, long nnev,
 			double &ttol, double rResid[], long nncv, double vV[],
 			long lldv, int iiparam[], int iipntr[], double wWorkd[],
 			double wWorkl[], long llworkl, int& iinfo,double fflag);

 	void  arpack_eupd(bool rvec, const char *hHowMny,  double D[], double Z[],
 			long ldz, double sigma, const char *bbmat, long n,
 			const char* wwhich, long nev, double tol, double Resid[],
 			long ncv, double V[], long ldv, int iparam[],
 			int ipntr[], double Workd[], double Workl[],
 			long lworkl, int &info,double flag );

 	void arpack_aupd(int &ido,const char *bbmat, long n,const char* wwhich, long nev,
 			float &tol, float Resid[], long ncv, float V[],
 			long ldv, int iparam[], int ipntr[], float Workd[],
 			float Workl[], long lworkl, int& info, float flag);

 	void   arpack_eupd(bool rvec, const char *hHowMny,  float D[], float Z[],
 			long ldz, float sigma, const char *bbmat, long n,
 			const char* wwhich, long nev, float tol, float Resid[],
 			long ncv, float V[], long ldv, int iparam[],
 			int ipntr[], float Workd[], float Workl[],
 			long lworkl, int &info,float flag );
}


#endif // ARN_FUNC_H
