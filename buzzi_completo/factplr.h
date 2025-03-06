#ifndef FACTOREDPLR_HPP
#define FACTOREDPLR_HPP

//   =====================< FACTPLR.HPP >=======================
//   * Class FactoredPLR derived from Factored                 *
//   * Class FactoredGauss derived from FactoredPLR            *
//   * Class FactoredCrout derived from FactoredPLR            *
//   * Description: Chapter 15                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   * These classes permit linear system solutions            *
//   * and solutions to related problems only with             *
//   * square matrices                                         *
//   * These are the ones that require the least time for      *
//   * calculation.                                            *
//   ***********************************************************


#include <factored.h>
#include <matrix.hpp>
#include <utility.hpp>
#include <vector.hpp>


//   ===========================================================
//   ============   prototype PLR functions  ===================
//   ===========================================================
//   char GaussFactorization
//       (int n,double **a,int *indx,int *signd);
//   char CroutFactorization
//       (int n,double **a,int *indx,int *signd);
//   void PLRSolution
//       (int n,double **a,float *b,int *indx);
//   void PLRTransposeSolution
//       (int n,double **a,double *b,int *indx);
//   double PLRNormInvEst
//       (int n,double **a,int *indx);


//   ===========================================================
//   ================   class FactoredPLR   ====================
//   ===========================================================


template <class D> class Te_Vector;
template <class D> class Te_Matrix;


template <class D>
class FactoredPLR : public Factored<D>
{

protected:

	int *indx;
	int signd;

	// initialisation of constructors
	void FurtherInit(void);

	//  initialisation of special vectors (indx)
	virtual void SpecificInitialize(void)
	{
		if(numRows != numColumns)
			Error("%s%s%sSpecificInitialize",Factored::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
		indx = new int[numRows + 1];
		if(!indx)
			Error("%s%s%sSpecificInitialize",Factored::ERROR,ERR_SPACE,ERR_FUNCTION);
	}

	//  deinitialisation of special vectors (indx)
	virtual void SpecificDeinitialize(void)
	{
		if(indx) {
			delete [] indx;
			indx = NULL;
		}
		signd = 0;
	}

	// constructor A('*',3,5);
	FactoredPLR(char ch,int rows,int columns) : Factored<D>(ch,rows,columns)
	{
		FurtherInit();
	}

	//   ===========================================================
	//   *************   Functions for linear algebra **************
	//   ===========================================================
	virtual void Factorization(void) = 0;
	virtual void Solution(Te_Vector<D> *bx)
	{
		PLRSolution(numRows,factored,bx->vector,indx);
	}

	virtual void Solution(const Te_Vector<D> &b,Te_Vector<D> *x)
	{
		*x = b;
		PLRSolution(numRows,factored,x->vector,indx);
	}
 
	virtual void TransposeSolution(Te_Vector<D> *bx)
	{
		PLRTransposeSolution(numRows,factored,bx->vector,indx);
	}

	virtual void TransposeSolution(const Te_Vector<D> &b,Te_Vector<D> *x)
	{
		*x = b;
		PLRTransposeSolution(numRows,factored,x->vector,indx);
	}

	virtual void Solution(Te_Matrix<D> *BX);
	virtual void Solution(const Te_Matrix<D> &B,Te_Matrix<D> *X)
	{
		*X = B;
		Solution(X);
	}

	virtual D Condition(void)
	{
		return Control((double)norm*PLRNormInvEst(numRows,factored,indx));
	}

	virtual D DeterminantEvaluation(void)
	{
		double determinant = (double)signd;
		for(int i=1; i<=numRows; i++)
			determinant *= factored[i][i];
		return determinant;
	}

public:
	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
	// default constructor type Factored A;
	FactoredPLR(void) : Factored<D>() { FurtherInit(); }

	// copy constructor
	FactoredPLR(const Factored<D> &rval) : Factored<D>(rval) { FurtherInit(); }

	// constructor A(3,5);
	FactoredPLR(int rows,int columns) : Factored<D>(rows,columns) { FurtherInit(); }

	// constructor A(3,5,w);
	FactoredPLR(int rows,int columns,D *initvalues) : Factored<D>(rows,columns,initvalues) { FurtherInit(); }

	// constructor from Te_Matrix
	FactoredPLR(const Te_Matrix<D> &rval) : Factored<D>(rval) { FurtherInit(); }

	// makes a submatrix with rows,columns
	FactoredPLR(int rows,int columns,const Te_Matrix<D> &rval) : Factored<D>(rows,columns,rval) { FurtherInit(); }

	// as above, commencing from irow,jcol
	FactoredPLR(int rows,int columns,int irow,int jcol,const Te_Matrix<D> &rval) : Factored<D>(rows,columns,irow,jcol,rval) { FurtherInit(); }

	//   ===========================================================
	//   ********************   destructor   ***********************
	//   ===========================================================
	~FactoredPLR(void);
};


//   ===========================================================
//   ===============   class FactoredGauss   ===================
//   ===========================================================
template <class D>
class FactoredGauss : public FactoredPLR<D>
{
public:
	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
	// default constructor type Factored A;
	FactoredGauss(void) : FactoredPLR<D>() { }

	// copy constructor
	FactoredGauss(const Factored<D> &rval) : FactoredPLR<D>(rval) { }

	// constructor A(3,5);
	FactoredGauss(int rows,int columns) : FactoredPLR<D>(rows,columns) { }

	// constructor A(2,2,1.,2.,3.,4.);
	FactoredGauss(int rows,int columns,D a11,...);

	// constructor A(3,5,w);
	FactoredGauss(int rows,int columns,D *initvalues) : FactoredPLR<D>(rows,columns,initvalues) { }

	// constructor from Te_Matrix
	FactoredGauss(const Te_Matrix<D> &rval) : FactoredPLR<D>(rval) { }

	// makes a submatrix with rows,columns
	FactoredGauss(int rows,int columns,const Te_Matrix<D> &rval) : FactoredPLR<D>(rows,columns,rval) { }

	// as above, commencing from irow,jcol
	FactoredGauss(int rows,int columns,int irow,int jcol,const Te_Matrix<D> &rval) : FactoredPLR<D>(rows,columns,irow,jcol,rval) { }

	//   ===========================================================
	//   ********************   destructor   ***********************
	//   ===========================================================
	~FactoredGauss(void);

	//   ===========================================================
	//   ***************   assignment operators   ******************
	//   ===========================================================
	void operator = (const FactoredGauss<D> &rval);
	FactoredGauss<D> &operator = (const Te_Matrix<D> &rval);

	//   ===========================================================
	//   ************   Functions for linear algebra ***************
	//   ===========================================================
	virtual void Factorization(void)
	{
		singular = GaussFactorization(numRows,factored,indx,&signd);
	}
}; // fine class FactoredGauss


//   ===========================================================
//   ===============   class FactoredCrout   ===================
//   ===========================================================
template <class D>
class FactoredCrout : public FactoredPLR<D>
{
public:
	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
	// default constructor type Factored A;
	FactoredCrout(void) : FactoredPLR<D>() { }

	// copy constructor
	FactoredCrout(const Factored<D> &rval) : FactoredPLR<D>(rval) { }

	// constructor A(3,5);
	FactoredCrout(int rows,int columns) : FactoredPLR<D>(rows,columns) { }

	// constructor A(2,2,1.,2.,3.,4.);
	FactoredCrout(int rows,int columns,D a11,...);

	// constructor A(3,5,w);
	FactoredCrout(int rows,int columns,D *initvalues) : FactoredPLR<D>(rows,columns,initvalues) { }

	// constructor from Te_Matrix
	FactoredCrout(const Te_Matrix<D> &rval) : FactoredPLR<D>(rval) { }

	// make a submatrix with rows,columns
	FactoredCrout(int rows,int columns,const Te_Matrix<D> &rval) : FactoredPLR<D>(rows,columns,rval) { }

	// as above, commencing from irow,jcol
	FactoredCrout(int rows,int columns,int irow,int jcol,const Te_Matrix<D> &rval) : FactoredPLR<D>(rows,columns,irow,jcol,rval) { }

	//   ===========================================================
	//   ********************   destructor   ***********************
	//   ===========================================================
	~FactoredCrout(void);

	//   ===========================================================
	//   ***************   assignment operators  *******************
	//   ===========================================================
	void operator = (const FactoredCrout<D> &rval);
	FactoredCrout<D> &operator = (const Te_Matrix<D> &rval);

	//   ===========================================================
	//   *************   Functions for linear algebra **************
	//   ===========================================================
	virtual void Factorization(void);
}; // fine class FactoredCrout


//   =====================< FACTPLR.CPP >=======================
//   * Class FactoredPLR derived from Factored                 *
//   * Class FactoredGauss derived from FactoredPLR            *
//   * Class FactoredCrout derived from FactoredPLR            *
//   * Description: Chapter 15                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================


//   ===========================================================
//   ================   protected functions   ==================
//   ===========================================================

//   *********************< FurtherInit >***********************
//   * Purpose: Terminating initialisation                     *
//   * Description: Adds specific elements                     *
//   * Example: Used internally                                *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::FurtherInit(void)
{
	signd = 0;
	indx = NULL;
}


//   *****************< SpecificInitialize >********************
//   * Purpose: Initialising specific elements                 *
//   * Description: Adds specific elements                     *
//   * Example: Used internally                                *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::SpecificInitialize(void)
{
	if(numRows != numColumns)
		Error("%s%s%sSpecificInitialize",Factored::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	indx = new int[numRows + 1];
	if(!indx)
		Error("%s%s%sSpecificInitialize",Factored::ERROR,ERR_SPACE,ERR_FUNCTION);
}


//   ****************< SpecificDeinitialize >*******************
//   * Purpose: Deinitialisation of specific elements          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::SpecificDeinitialize(void)
{
	if(indx) {
		delete [] indx;
		indx = NULL;
	}
	signd = 0;
}


//   ===========================================================
//   =================   public functions   ====================
//   ===========================================================

//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================

//   ********************< destructor >*************************
//   * Purpose: Eliminating a specific part                    *
//   ***********************************************************
template <class D>
FactoredPLR<D>::~FactoredPLR(void)
{
	if(indx) {
		delete [] indx;
		indx = NULL;
	}
}


//   ===========================================================
//   ==========   Functions for linear algebra   ===============
//   ===========================================================

//   **********************< Solution >*************************
//   * Purpose: Solving the system                             *
//   * Description: The matrix must be PLR factorised          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::Solution(Vector<D> *bx)
{
	PLRSolution(numRows,factored,bx->vector,indx);
}


//   **********************< Solution >*************************
//   * Purpose: Solving the system                             *
//   * Description: The matrix must be PLR factorised          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::Solution(const Vector<D> &b,Vector<D> *x)
{
	*x = b;
	PLRSolution(numRows,factored,x->vector,indx);
}


//   *****************< TransposeSolution >*********************
//   * Purpose: Solving the system  for A transposed           *
//   * Description: The matrix must be PLR factorised          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::TransposeSolution(Vector<D> *bx)
{
	PLRTransposeSolution(numRows,factored,bx->vector,indx);
}


//   *****************< TransposeSolution >*********************
//   * Purpose: Solving the system  for A transposed           *
//   * Description: The matrix must be PLR factorised          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::TransposeSolution(const Vector<D> &b,Vector<D> *x)
{
	*x = b;
	PLRTransposeSolution(numRows,factored,x->vector,indx);
}


//   **********************< Solution >*************************
//   * Purpose: Solving the system  for matrices               *
//   * Description: The matrix must be PLR factorised          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::Solution(Te_Matrix<D> *BX)
{
	Te_Vector<D> b(numColumns);
	for(int j = 1;j <= BX->Columns();j++) {
		b = BX->GetColumn(j);
		PLRSolution(numColumns,factored,b.vector,indx);
		BX->SetColumn(j,b);
	}
}


//   **********************< Solution >*************************
//   * Purpose: Solving the system  for matrices               *
//   * Description: The matrix must be PLR factorised          *
//   ***********************************************************
template <class D>
void FactoredPLR<D>::Solution(const Matrix<D> &B,Matrix<D> *X)
{
	*X = B;
	Solution(X);
}


//   *********************< Condition >*************************
//   * Purpose: Calculating the condition number               *
//   ***********************************************************
template <class D>
D FactoredPLR<D>::Condition(void)
{
	return Control((double)norm*PLRNormInvEst(numRows,factored,indx));
}


//   ****************< DeterminantEvaluation >******************
//   * Purpose: Calculating the determinant                    *
//   ***********************************************************
template <class D>
D FactoredPLR<D>::DeterminantEvaluation(void)
{
	double determinant = (double)signd;
	for(int i = 1;i <= numRows;i++)
		determinant *= factored[i][i];
	return determinant;
}


//   ===========================================================
//   ===================   Algorithms   ========================
//   ===========================================================

//   *******************< PLRSolution >*************************
//   * Purpose: Solving a system with matrix factorisation     *
//   *          with the PLR method                            *
//   ***********************************************************
template <class D>
void PLRSolution(int n,D **a,D *b,int *indx)
{
	int i,j,pivot;
	double sum;
	D *ai;
	for(i = 1;i <= n;i++) {
		pivot = indx[i];
		sum = b[pivot];
		b[pivot] = b[i];
		if(a[i][i] == 0.) {
			Message("\nSingular matrix");
			if(b[i] != 0.)
				Message(" and incompatible system");
			b[i] = 0.;
		} else {
			ai = a[i];
			for(j = 1;j <= i-1;j++)
				sum -= (*++ai)*b[j];
			b[i] = Control(sum);
		}
	}
	for(i = n;i >= 1;i--) {
		sum = b[i];
		ai = a[i] + i + 1;
		if(a[i][i] != 0.) {
			for(j = i+1;j <= n;j++)
				sum -= (*ai++)*b[j];
			b[i] = Control(sum/a[i][i]);
		} else
			b[i] = 0.;
	}
}


//   ***************< PLRTransposeSolution >********************
//   * Purpose: Solving a system with factorised matrix        *
//   *          with the PLR method on the transposed matrix   *
//   ***********************************************************
template <class D>
void PLRTransposeSolution(int n,D **a,D *b,int *indx)
{
	double temp;
	int i,k,j;
	for(k=1; k<=n; k++) {
      if(a[k][k] != 0.) {
    	  temp = b[k];
    	  for(i=1; i<k; i++)
    		  temp -= a[i][k]*b[i];
    	  b[k] = Control(temp/a[k][k]);
      } else {
    	  Message("\nSingular matrix");
    	  b[k] = 0.;
      }
	}
	for(k=n; k>=1; k--) {
		temp = b[k];
		for(i=k+1; i<=n; i++)
			temp -= a[i][k]*b[i];
		b[k] = Control(temp);
	}
	for(k=n; k>=1; k--) {
		j = indx[k];
		if(j != k)
			Swap(&b[j],&b[k]); // in UTILITY.CPP
	}
}


//   *******************< PLRNormInvEst >***********************
//   * Purpose: Estimating the matrix inverse norm             *
//   * Description: Serves for the condition number            *
//   ***********************************************************
template <class D>
D PLRNormInvEst(int n,D **a,int *indx)
{
	D yp,ym,tempp,tempm,temp;
	D ynorm,znorm,normaiest;
	int i,j,k;
	for(j=1; j<=n; j++)
		if(a[j][j] == 0.)
			return BIG_FLOAT;
	D *y = new D [n + 1];
	Te_Vector<D> pp(n),pm(n);
	y[1] = 1./a[1][1];
	for(i=2; i<=n; i++)
		pp[i] = a[1][i]*y[1];
	for(j=2; j<=n; j++) {
		yp = (1.-pp[j])/a[j][j];
		ym = (-1.-pp[j])/a[j][j];
		tempp = Abs(yp);
		tempm = Abs(ym);
		for(i=j+1; i<=n; i++) {
			pm[i] = pp[i]+a[j][i]*ym;
			tempm += Abs(pm[i]/a[i][i]);
			pp[i] += a[j][i]*yp;
			tempp += Abs(pp[i]/a[i][i]);
		}
		if(tempp >= tempm)
			y[j] = yp;
		else {
			y[j] = ym;
			for(i=j+1; i<=n; i++)
				pp[i] = pm[i];
		}
	}
	// Lty = y
	for(k=n; k>=1; k--) {
		double tmp = y[k];
		for(i=k+1; i<=n; i++)
			tmp -= a[i][k]*y[i];
		y[k] = Control(tmp);
	}
	// Pty = y
	for(k=n; k>=1; k--) {
		j = indx[k];
		if(j != k)
			Swap(&y[j],&y[k]);
	}
	// norm 1 of y
	for(i=1, ynorm=0.; i<=n; i++)
		ynorm += Abs(y[i]);
	if(ynorm==0.) {
		delete [] y;
		return BIG_FLOAT;
	}
	PLRSolution(n,a,y,indx);
	// norm 1
	for(i=1, znorm=0.; i<=n; i++)
      znorm += Abs(y[i]);
	normaiest = znorm/ynorm;
	delete [] y;
	return normaiest;
}


//   ===========================================================
//   ===============   class FactoredGauss   ===================
//   ===========================================================

//   ********************< constructor >************************
//   * Purpose: Sizing and initialising                        *
//   * Example: FactoredGauss A(2,2,1.,2.,3.,4.);              *
//   ***********************************************************
template <class D>
FactoredGauss<D>::FactoredGauss(int rows,int columns,D a11,...)
: FactoredPLR<D>('*',rows,columns)
{
	D *w=matrix[0] + 1;
	va_list puntaList;
	va_start(puntaList,a11);
	int i;
	*w = Control(a11);
	for(i = 2;i < size;i++)
		*++w = Control(va_arg(puntaList,D));
	va_end(puntaList);
}


//   ********************< destructor >*************************
//   * Purpose: Must not deinitialise anything                 *
//   ***********************************************************
template <class D>
FactoredGauss<D>::~FactoredGauss(void)
{ }


//   ===========================================================
//   ***************   assignment operators   ******************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment of a FactoredGauss                  *
//   * Description: Use forbidden                              *
//   ***********************************************************
template <class D>
void FactoredGauss<D>::operator =(const FactoredGauss<D> &rval)
{
	Error("\n%sAn assignment is being used %s\n with Factored N. %d",Factored::ERROR,rval.whoAmI);
}


//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix                         *
//   ***********************************************************
template <class D>
FactoredGauss<D> &FactoredGauss<D>::operator = (const Te_Matrix<D> &rval)
{
	CopyFromMatrix(rval);
	return *this;
}


//   ===========================================================
//   *************   Functions for linear algebra **************
//   ===========================================================

//   ******************< Factorization >************************
//   * Purpose: Factorisation with Gauss                       *
//   ***********************************************************
template <class D>
void FactoredGauss<D>::Factorization(void)
{
	singular = GaussFactorization(numRows,factored,indx,&signd);
}


//   ===========================================================
//   ===================   Algorithms   ========================
//   ===========================================================

//   ****************< GaussFactorization >*********************
//   * Purpose: Factorisation with the Gauss method            *
//   ***********************************************************
template <class D>
char GaussFactorization(int n,D **a,int *indx,int *signd)
{
	Te_Vector<D> d(n);
	double sum;
	char sing = 0;
	int i,r,j,k;
	const D EPS2 = 2.*MACH_EPS;
	*signd = 1; // for the sign of the determinant

	for(i = 1;i <= n;i++) {
		d[i] = MaxAbs(n,a[i]+1); // for implicit balance
		if(d[i] == 0)
			d[i] = BIG_FLOAT; // put at the end!
	}

	for(r = 1;r < n;r++) {
		double max = Abs(a[r][r]/d[r]); // balanced pivot !
		int pivot = r;

		for(i = r + 1;i <= n;i++) {
			double temp = Abs(a[i][r]/d[i]);
			if(temp > max) {
				max = temp;
				pivot = i;
			}
		}

        if(r != pivot) {
        	Swap(&a[pivot],&a[r]); // swaps pointers!
        	Swap(&d[pivot],&d[r]); // in UTILITY.CPP
        	*signd = -*signd; // changes sign
        	indx[r] = pivot;
        } else
        	indx[r] = r;
        D *ar = a[r];
        D *arr = ar + r;
        if(*arr == 0.)
        	sing = 1;
        else {
        	for(i = r + 1;i <=n;i++) {
        		D *air = a[i] + r;
        		if(*air != 0.) {
        			double temp = *air/(*arr);
        			*air = Control(temp);
        			for(j = r+1;j <= n;j++) {
        				sum = *++air;
        				sum -= temp * (*(ar + j));
        				if(Abs(sum) < EPS2 * Abs(*air))
        					*air = 0.;
        				else
        					*air = Control(sum);
        			}
        		}
        	}
        }
	}
	indx[n] = n;
	if(a[n][n] == 0.)
		sing = 1;
	return sing;
}


//   ===========================================================
//   ===============   class FactoredCrout   ===================
//   ===========================================================

//   ********************< constructor >************************
//   * Purpose: Sizing and initialising                        *
//   * Example: FactoredCrout A(2,2,1.,2.,3.,4.);              *
//   ***********************************************************
template <class D>
FactoredCrout<D>::FactoredCrout(int rows,int columns,D a11,...)
: FactoredPLR<D>('*',rows,columns)
{
	D *w=matrix[0] + 1;
	va_list puntaList;
	va_start(puntaList,a11);
	int i;
	*w = Control(a11);
	for(i = 2;i < size;i++)
		*++w = Control(va_arg(puntaList,D));
	va_end(puntaList);
}


//   ********************< destructor >*************************
//   * Purpose: Must not deinitialise anything                 *
//   ***********************************************************
template <class D>
FactoredCrout<D>::~FactoredCrout(void)
{ }


//   ===========================================================
//   ***************   assignment operators   ******************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment of a FactoredCrout                  *
//   * Description: Use forbidden                              *
//   ***********************************************************
template <class D>
void FactoredCrout<D>::operator =(const FactoredCrout<D> &rval)
{
	Error("\n%sAn assignment is used %s\n con Factored N. %d",Factored::ERROR,rval.whoAmI);
}


//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix                         *
//   ***********************************************************
template <class D>
FactoredCrout<D> &FactoredCrout<D>::operator = (const Te_Matrix<D> &rval)
{
	CopyFromMatrix(rval);
	return *this;
}


//   ===========================================================
//   ************   Functions for linear algebra  **************
//   ===========================================================

//   ******************< Factorization >************************
//   * Purpose: Interface for factorisation                    *
//   ***********************************************************
template <class D>
void FactoredCrout<D>::Factorization(void)
{
	singular = CroutFactorization(numRows,factored,indx,&signd);
}


//   ===========================================================
//   ===================   Algorithms  =========================
//   ===========================================================

//   ****************< CroutFactorization >*********************
//   * Purpose: Factorisation with Crout method                *
//   ***********************************************************
template <class D>
char CroutFactorization(int n,D **a,int *indx,int *signd)
{
	Te_Vector<D> d(n);
	double sum;
	char sing = 0;
	int i,r,j,k;
	*signd = 1.; // for the sign of the determinant

	for(i = 1;i <= n;i++) {
		d[i] = MaxAbs(n,a[i]+1); // for implicit balance
		if(d[i] == 0.)
			d[i] = BIG_FLOAT; //put at the end!
	}

	for(r = 1;r <= n;r++) {
		for(i = r;i <= n;i++) { // construct L
			sum = a[i][r];
			D *ai = a[i];  // avoids double index
			for(k = 1;k <= r-1;k++)
				sum -= *++ai * a[k][r];
			a[i][r] = Control(sum);
		}

		double max = Abs(a[r][r]/d[r]); // balanced pivot!
		int pivot = r;
		for(i = r + 1;i <= n;i++) {
			double temp = Abs(a[i][r]/d[i]);
			if(temp > max) {
				max = temp;
				pivot = i;
			}
		}
        if(r != pivot) {
        	Swap(&a[pivot],&a[r]); // swap pointers!
        	Swap(&d[pivot],&d[r]); // in UTILITY.CPP
        	*signd = -*signd; // changes sign
        	indx[r] = pivot;
        } else
        	indx[r] = r;

        if(a[r][r] == 0.)
        	sing = 1;
        else {
        	for(j = r + 1;j <=n;j++) { // constructs R
        		sum = a[r][j];
        		D *ar = a[r];
        		for(k = 1;k <= r -1;k++)
        			sum -= *++ar * a[k][j];
        		a[r][j] = Control(sum/a[r][r]);
        	}
        }
	}

	// Transform in Doolittle
	for(r = 1;r < n;r++) {
		if(a[r][r] != 0.) {
			for(i = r + 1;i<=n;i++) {
				a[r][i] = Control(a[r][i]*a[r][r]);
				a[i][r] = Control(a[i][r]/a[r][r]);
			}
		}
	}
	return sing;
}


#endif
