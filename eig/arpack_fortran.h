#ifndef ARPACK_FORTRAN_H
#define ARPACK_FORTRAN_H


extern "C"
{

	void 	dsaupd_(int *ido,  const char* bmat, long *ordem0,
			const char* which, long *nev, double *tol,
			double *resid, long *ncv, double *v,
			long *ldv, int *iparam, int *ipntr,
			double* workd, double* workl, long *lworkl,
			int *info );

	void dseupd_(int *rvec, const char *HowMny, int *select,
			double *d, double *Z, long *ldz,
			double *sigma, const char *bmat, long *n,
			const char *which, long *nev, double *tol,
			double *resid, long *ncv, double *V,
			long *ldv, int *iparam, int *ipntr,
			double *workd, double *workl,
			long *lworkl, int *info);

	// single precision symmetric routines.
	void ssaupd_(int *ido, const char *bmat, long *n, const char *which,
			long *nev, float *tol, float *resid,
			long *ncv, float *V, long *ldv,
			int *iparam, int *ipntr, float *workd,
			float *workl, long *lworkl, int *info);

	void sseupd_(int *rvec, const char *HowMny, int *select,
			float *d, float *Z, long *ldz,
			float *sigma, const char *bmat, long *n,
			const char *which, long *nev, float *tol,
			float *resid, long *ncv, float *V,
			long *ldv, int *iparam, int *ipntr,
			float *workd, float *workl,
			long *lworkl, int *info);

	#if 0
	// double precision nonsymmetric routines.
	void dnaupd_(int *ido, char *bmat, long *n, char *which,
			long *nev, double *tol, double *resid,
			long *ncv, double *V, long *ldv,
			int *iparam, int *ipntr, double *workd,
			double *workl, long *lworkl, int *info);

	void dneupd_(logical *rvec, char *HowMny, logical *select,
			double *dr, double *di, double *Z,
			long *ldz, double *sigmar,
			double *sigmai, double *workev,
			char *bmat, long *n, char *which,
			long *nev, double *tol, double *resid,
			long *ncv, double *V, long *ldv,
			int *iparam, int *ipntr,
			double *workd, double *workl,
			long *lworkl, int *info);

	// single precision symmetric routines.
	void ssaupd_(int *ido, char *bmat, long *n, char *which,
			long *nev, float *tol, float *resid,
			long *ncv, float *V, long *ldv,
			int *iparam, int *ipntr, float *workd,
			float *workl, long *lworkl, int *info);

	void sseupd_(logical *rvec, char *HowMny, logical *select,
			float *d, float *Z, long *ldz,
			float *sigma, char *bmat, long *n,
			char *which, long *nev, float *tol,
			float *resid, long *ncv, float *V,
			long *ldv, int *iparam, int *ipntr,
			float *workd, float *workl,
			long *lworkl, int *info);

	// single precision nonsymmetric routines.
	void snaupd_(int *ido, char *bmat, long *n, char *which,
			long *nev, float *tol, float *resid,
			long *ncv, float *V, long *ldv,
			int *iparam, int *ipntr, float *workd,
			float *workl, long *lworkl, int *info);

	void sneupd_(logical *rvec, char *HowMny, logical *select,
			float *dr, float *di, float *Z,
			long *ldz, float *sigmar,
			float *sigmai, float *workev, char *bmat,
			long *n, char *which, long *nev,
			float *tol, float *resid, long *ncv,
			float *V, long *ldv, int *iparam,
			int *ipntr, float *workd, float *workl,
			long *lworkl, int *info);

	// single precision complex routines.
	void cnaupd_(int *ido, char *bmat, long *n, char *which,
			long *nev, float *tol, ComplexFloat *resid,
			long *ncv, ComplexFloat *V, long *ldv,
			int *iparam, int *ipntr, ComplexFloat *workd,
			ComplexFloat *workl, long *lworkl,
			float *rwork, int *info);

	void cneupd_(logical *rvec, char *HowMny, logical *select,
			ComplexFloat *d, ComplexFloat *Z, long *ldz,
			ComplexFloat *sigma, ComplexFloat *workev,
			char *bmat, long *n, char *which, long *nev,
			float *tol, ComplexFloat *resid, long *ncv,
			ComplexFloat *V, long *ldv, int *iparam,
			int *ipntr, ComplexFloat *workd,
			ComplexFloat *workl, long *lworkl,
			float *rwork, int *info);

	// double precision complex routines.
	void znaupd_(int *ido, char *bmat, long *n, char *which,
			long *nev, double *tol,ComplexDbl *resid,
			long *ncv,ComplexDbl *V, long *ldv,
			int *iparam, int *ipntr,ComplexDbl *workd,
			ComplexDbl *workl, long *lworkl,
			double *rwork, int *info);

	void zneupd_(logical *rvec, char *HowMny, logical *select,
			ComplexDbl *d,ComplexDbl *Z, long *ldz,
			ComplexDbl *sigma,ComplexDbl *workev,
			char *bmat, long *n, char *which, long *nev,
			double *tol,ComplexDbl *resid, long *ncv,
			ComplexDbl *V, long *ldv, int *iparam,
			int *ipntr,ComplexDbl *workd,
			ComplexDbl *workl, long *lworkl,
			double *rwork, int *info);
	#endif
}


#endif // ARCOMP_FORTRAN_H
