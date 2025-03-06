#ifndef UTILITY_HPP
#define UTILITY_HPP


//   =====================< UTILITY.HPP >=======================
//   * Header File for UTILITY.HPP                             *
//   * Description: Chapter 8                                  *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================


#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <cstdarg>
#include <cstring>


namespace lev_math {

namespace utility {

//   ===========================================================
//   ======================   GLOBAL   =========================
//   ===========================================================

extern FILE *bzzFileOut;
extern FILE *bzzFileIn;
extern char bzzYesNo;
extern const double MACH_EPS;
extern const float BIG_FLOAT;
extern const float TINY_FLOAT;
extern const double BIG_DOUBLE;
extern const double TINY_DOUBLE;
extern const long double BIG_LONG_DOUBLE;
extern const long double TINY_LONG_DOUBLE;
extern const char BIG_CHAR;
extern const char TINY_CHAR;
extern const unsigned char BIG_UNSIGNED_CHAR;
extern const unsigned char TINY_UNSIGNED_CHAR;
extern const int BIG_INT;
extern const int TINY_INT;
extern const unsigned int BIG_UNSIGNED_INT;
extern const unsigned int TINY_UNSIGNED_INT;
extern const long int BIG_LONG;
extern const long int TINY_LONG;
extern const unsigned long int BIG_UNSIGNED_LONG;
extern const unsigned long int TINY_UNSIGNED_LONG;

//   ===========================================================
//   ======================   ERRORS   =========================
//   ===========================================================

//========== Error Type ==============
extern const char *const ERR_RANGE;
extern const char *const ERR_SPACE;
extern const char *const ERR_OPEN_FILE;
extern const char *const ERR_CLOSE_FILE;
extern const char *const ERR_READING_FILE;
extern const char *const ERR_WRITING_FILE;
extern const char *const ERR_CHECK_DIMENSION;
extern const char *const ERR_OUT_OF_SCOPE;
extern const char *const ERR_FACTORIZED;
extern const char *const ERR_IMPLEMENTATION;

//============ Function Type =============
extern const char *const ERR_FUNCTION;
extern const char *const ERR_CONSTRUCTOR;
extern const char *const ERR_OPERATOR;



//   ===========================================================
//   ====================   PROTOTYPES   =======================
//   ===========================================================

double MachEps(void);

void Message(char *myFormat,...);
void Error(char *myFormat,...);


//   ===========================================================
//   =================  Inline  Functions  =====================
//   ===========================================================

//   ************************< Abs >****************************
//   * Purpose: To have a unique function for absolute value   *
//   * Description: Overload for float, int, double, complex   *
//   * Example: float x = Abs(y);                              *
//   ***********************************************************

inline float Abs(float a) { return (float)fabs(a); }
inline double Abs(double a) { return fabs(a); }
inline int Abs(int i) { return abs(i); }
inline double Abs(std::complex<double> a) { return abs(a); }


//   ************************< Sqrt >***************************
//   * Purpose: To have a unique function for square root value*
//   * Description: Overload for float, int, double, complex   *
//   * Example: float x = Abs(y);                              *
//   ***********************************************************
#if 0
inline float Sqrt(float a)
	{return sqrt(a);}
inline double Sqrt(double a)
	{return sqrt(a);}
inline int Sqrt(int i)
	{return sqrt(i);}
inline complex Sqrt(complex a)
	{return sqrt(a);} // this function overloads that defined by BORLAND
#endif


//   ***********************< Swap >****************************
//   * Purpose: Having a unique function for Swap              *
//   * Description: Overload for float, int, double,           *
//   *              *float, Vector, Matrix,  etc.              *
//   * Example: Swap(&x,&y);                                   *
//   ***********************************************************
inline void Swap (int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

inline void Swap (float *a, float *b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}


inline void Swap(double *a, double *b)
{
	double temp = *a;
	*a = *b;
	*b = temp;
}

inline void Swap(std::complex<double> *a, std::complex<double> *b)
{
	std::complex<double> temp = *a;
	*a = *b;
	*b = temp;
}


inline void Swap(int **a, int **b)
{
	int *temp = *a;
	*a = *b;
	*b = temp;
}


inline void Swap(float **a, float **b)
{
	float *temp = *a;
	*a = *b;
	*b = temp;
}


inline void Swap (double **a, double **b)
{
	double *temp = *a;
	*a = *b;
	*b = temp;
}


inline void Swap(std::complex<double> **a, std::complex<double> **b)
{
	std::complex<double> *temp = *a;
	*a = *b;
	*b = temp;
}


//   ===========================================================
//   =================  templates  =============================
//   ===========================================================

//   *************< Max, Min, MaxAbs, MinAbs >******************
//   * Purpose: Maximum and minimum for a pair of numbers      *
//   * Description: Functions overload float, int, double      *
//   * Example: x=Max(a,b);i=Min(l,k); x=MaxAbs(a,b);          *
//   ***********************************************************
template <class Q> inline Q Max (Q a, Q b)
	{return (a > b ? a : b);}
template <class Q> inline Q Min (Q a, Q b)
	{return (a > b ? a : b);}
template <class Q> inline Q MaxAbs(Q a, Q b)
	{return (Abs(a) > Abs(b) ? Abs(a) : Abs(b));}
template <class Q> inline Q MinAbs(Q a, Q b)
	{return (Abs(a) < Abs(b) ? Abs(a) : Abs(b));}
template <class Q> inline Q Control(Q  value){return value;} // to mantain compatibility with previous version


//   ************************< Max >****************************
//   * Purpose: Max of an array                                *
//   * Description: It returns the Max of an array.            *
//   *              If the index im is put in the argument     *
//   *              it also returns the position.              *
//   * Example: y = Max(10,x,&im);                             *
//   ***********************************************************
template <class Q> Q Max(int n,Q *x,int *im = 0)
{
	if(n < 0)
		return x[0];
	Q temp = x[0];
	if(im != 0) *im = 0;
	for(int i = 1;i < n;i++)
		if(temp < x[i]) {
			temp = x[i];
			if(im != 0)
				*im = i;
		}
	return temp;
}


//   **********************< MaxAbs >***************************
//   * Purpose: Max Abs of an array                            *
//   * Example: y = MaxAbs(10,x,&im);                          *
//   ***********************************************************
template <class Q> Q MaxAbs(int n,Q *x,int *im=0)
	{
	double temp = Abs(x[0]);
	if(n < 0) return temp;
	if(im != 0) *im = 0;
	for(int i = 1;i < n;i++)
	  if(temp < Abs(x[i]))
		 { temp = Abs(x[i]); if(im != 0) *im = i; }
	return temp;
	}


//   ************************< Min >****************************
//   * Purpose: Min of an array                                *
//   * Example: y = Min(10,x,&im);                             *
//   ***********************************************************
template <class Q> Q Min(int n,Q *x,int *im=0)
{
	if(n < 0)
		return x[0];
	Q temp = x[0];
	if(im != 0)
		*im = 0;
	for(int i = 1;i < n;i++)
		if(temp > x[i]) {
			temp = x[i];
			if(im != 0)
				*im = i;
		}
	return temp;
}


//   **********************< MinAbs >***************************
//   * Purpose: Min abs of an array                            *
//   * Example: y = MinAbs(10,x,&im);                          *
//   ***********************************************************
template <class Q> Q MinAbs(int n,Q *x,int *im=0)
	{
	double temp = Abs(x[0]);
	if(n < 0) return temp;
	if(im != 0) *im = 0;
	for(int i = 1;i < n;i++)
	  if(temp > Abs(x[i]))
		 { temp = Abs(x[i]); if(im != 0) *im = i; }
	return temp;
	}


//   ************************< Sum >****************************
//   * Purpose: The sum of two vectors                         *
//   * Description: The sum of two arrays with control         *
//   * Example: Sum(n,x,y,z); z = x + y;                       *
//   ***********************************************************
template <class Q> void Sum(long n,Q *lval,Q *rval,Q *result)
{
	for(int i=0;i < n;i++) {
		*result++ = Control((*lval++) + (*rval++));
	}
}


//   **************************< Sum >**************************
//   * Purpose: The sum of two vectors                         *
//   * Description: The sum substitutes the first vector       *
//   * Example: Sum(n,x,y) x = x + y                           *
//   ***********************************************************
template <class Q> void Sum(long n,Q *lvalAndResult,Q *rval)
{
	for(int i=0;i < n;i++) {
		*lvalAndResult++ += (*rval++);
	}
}


//   **************************< Sum >**************************
//   * Purpose: The sum of two equal vectors                   *
//   * Description: The sum substitutes the vector             *
//   * Example: Sum(n,x) x = x + x                             *
//   ***********************************************************
template <class Q> void Sum(long n,Q *lvalRvalAndResult)
{
	for(int i=0;i < n;i++) {
		*lvalRvalAndResult  +=   *lvalRvalAndResult;
		lvalRvalAndResult++;
	}
}


//   *********************< Difference >************************
//   * Purpose: The difference between two vectors             *
//   * Description: The difference between two arrays          *
//   *              with control                               *
//   * Example: Difference(n,x,y,z); z = x - y;                *
//   ***********************************************************
template <class Q> void Difference(int n,Q *lval,Q *rval,Q *result)
{
	for(int i=0;i < n;i++) {
		*result++ = Control((*lval++) - (*rval++));
	}
}


//   *********************< Difference >************************
//   * Purpose: The difference between two vectors             *
//   * Description: The result substitutes                     *
//   *              the first vector                           *
//   * Example: Difference(n,x,y) x = x - y                    *
//   ***********************************************************
template <class Q> void Difference(int n,Q *lvalAndResult,Q *rval)
{
	for(int i=0;i < n;i++) {
		*lvalAndResult++  -=  *rval++;
	}
}


//   ******************< Difference >***************************
//   * Purpose: The difference between two vectors             *
//   * Description: The result substitutes                     *
//   *              the second vector                          *
//   * Example: Difference(n,x,y,1) y = x - y                  *
//   ***********************************************************
template <class Q> void Difference(int n,Q *lval,Q *rvalAndResult, int)
{
	for(int i=0;i < n;i++) {
		*rvalAndResult  = Control((*lval++) - (*rvalAndResult));
		rvalAndResult++;
	}
}


//   ************************< Dot >****************************
//   * Purpose: The scalar (dot) product of two vectors        *
//   * Description: The sum of the products of the coefficients*
//   * Example: float dot = Dot(n,x,y);                        *
//   ***********************************************************
template <class Q> Q Dot(int n,Q *lval,Q *rval)
	{
	Q result = 0.;
	for(int i=0;i < n;i++)
		 result += (*lval++) * (*rval++);
	return Control(result);
	}


//   **********************< Product >**************************
//   *Purpose: The product of a scalar with a vector           *
//   *Description: It multiplies n terms of the vector         *
//   *             v by c, resulting in r                      *
//   *Comments: The scalar dominion (class T) must be a subset *
//   *          of the matrix's element set (class Q).         *
//   *Example: Product(n,c,v,r)                                *
//   ***********************************************************
template <class Q, class T>
void Product(int n,T lval,Q *rval,Q *result)
	{
	for(int i=0;i < n;i++)
		 *result++ = Control(lval * (*rval++));
	}


//   **********************< Product >**************************
//   *Purpose: The product of a complex scalar with a vector   *
//   *Description: It multiplies n terms of the vector         *
//   *             v by c, resulting in r                      *
//   *Comments: The v dominion must be a subset                *
//   *          of the scalar and result matrix's element set  *
//   *          (complex).                                     *
//   *Example: Product(n,c,v,r)                                *
//   ***********************************************************
#ifdef __COMPLEXO__
template <class Q>
void Product(char, int n , complex<Q> lval,Q *rval,complex<Q> *result)
{
	for(int i=0;i < n;i++)
		*result++ = Control(lval * (*rval++));
}
#endif


//   **********************< Product>***************************
//   *Purpose: The product of a float with a vector            *
//   *Description: It multiplies n terms of the vector         *
//   *             v by c, substituting in v                   *
//   *Comments: The scalar dominion (class T) must be a subset *
//   *          of the matrix's element set (class Q).         *
//   *Example: Product(n,c,v)                                  *
//   ***********************************************************
template <class Q, class T>
void Product(int n,T lval,Q *rvalAndResult)
{
	for(int i=0;i < n;i++)
		*rvalAndResult = Control(lval * (*rvalAndResult));
	rvalAndResult++;
}


//   *********************< Division >**************************
//   *Purpose: Dividing a vector by a float                    *
//   *Description: It divides n terms of the vector v by c,    *
//   *               resulting in r                            *
//   *Comments: The scalar dominion (class T) must be a subset *
//   *          of the matrix's element set (class Q).         *
//   *Example: Division(n,v,c,r)                               *
//   ***********************************************************
template <class Q, class T>
void Division(int n,Q *lval,T rval,Q *result)
	{
	if(rval == 0.)rval = TINY_FLOAT;
	for(int i=0;i < n;i++)
		 *result++ = Control((*lval++)/rval);
	}


//   **********************< Division >*************************
//   *Purpose: Dividing a vector by a float                    *
//   *Description: It divides n terms of the vector v by c,    *
//   *             substituting in v                           *
//   *Comments: The scalar dominion (class T) must be a subset *
//   *          of the matrix's element set (class Q).         *
//   *Example: Division(n,c,v)                                 *
//   ***********************************************************
template <class Q, class T>
void Division(int n,Q *lval,T rval)
{
	if(rval == 0.)
		rval = TINY_FLOAT;
	for(int i=0;i < n;i++)
		*lval = Control((*lval)/rval);
	lval++;
}


//   ********************< SqrtSumSqr >*************************
//   * Purpose: The Euclidean Norm calculation for a float     *
//   * Description: Chapter 8                                  *
//   * Example: float x[100]; x[0]=.....                       *
//   *          float norm = SqrtSumSqr(n,x);                  *
//   ***********************************************************
template <class Q>
Q SqrtSumSqr(int n,Q *x)
{
	if(n <= 0)
		Error("%s%sSqrtSumSqr",ERR_RANGE,ERR_FUNCTION);
	Q norm = 0;
	for(int i = 0;i < n;i++)
		norm += x[i]*x[i];
	norm=sqrt(norm);
	return norm;
}


//   ***********************< Sort >****************************
//   * Purpose: To sort an array of floats                     *
//   * Description: Chapter 5                                  *
//   * Example:                                                *
//   *        float x[5]={3.,2.,5.,1.,4.};                     *
//   *        Sort(5,x);                                       *
//   ***********************************************************
#if 0
template <class Q>
void Sort(int n,Q *x)
	{
	if(n <= 1)return;
	int node,i,j,k,ik,jk;
	for(node = 1;node < n;node++)
		{
		i = node;
		j = ((i+1)/2)-1;
		while(i != 0 && x[j] <= x[i])
		  {
		  Swap(x+j,x+i);
		  i=j;
		  j=((i+1)/2)-1;
		  }
		}
	for(i = n-1;i >= 1;i--)
		{
		Swap(x+i,x);
		k = i-1;
		ik = 0;
		jk = 1;
		if(k >= 2 && x[2] > x[1])jk=2;
		while(jk <= k && x[jk] > x[ik])
		  {
		  Swap(x+jk,x+ik);
		  ik = jk;
		  jk = (2*(ik+1))-1;
		  if(jk+1 <= k)
		  if(x[jk+1] > x[jk])jk++;
		  }
		}
	}
#endif   // problemas com operador >= ou <= com tipos complexo.....


#ifdef __COMPLEXO__
 std::complex<double> sqrt(std::complex<double> &);
#endif


// funcoes usadas para generalizar o uso dos operadores de acesswo a numeros complexos
inline double real(double a) { return a; }
inline double imag(double a) { double zero = 0.; return zero; }
inline double conj(const double &a) { return a; }

#endif


//   *********************< TempFile >**************************
//   * Class for creating temporary files                      *
//   ***********************************************************

#ifndef TEMPFILE_HPP
#define TEMPFILE_HPP


//   ===========================================================
//   ==================   class TempFile   =====================
//   ===========================================================
class TempFile
{
private:
	static int countTempFile;
	char *nameTempFile;
public:
	// default constructor TempFile ff[10];
	TempFile(void) { nameTempFile = NULL; }

	// constructor TempFile f1("D:"),f2("C:\\TEMP\\");
	TempFile(char *directoryTemp) { NewFileName(directoryTemp); }

	// destructor
	~TempFile(void)
	{
		delete [] nameTempFile; nameTempFile = NULL;
	}

	// provides the unique name f1.FileName
	char *FileName(void){return nameTempFile;}

	// for using after default ff[0].NewFileName("D:");
	void NewFileName(char *directoryTemp);
};

} // Fim namespace

} // End lev_math namespace

#endif
