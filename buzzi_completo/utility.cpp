//   ====================< UTILITY.CPP >========================
//   * File containing the basic utilities                     *
//   * Description: Chapter 8                                  *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <climits> // constantes relativos aos valores máximos e mínimos dos tipos básicos inteiros
#include <cfloat>  // idem para ponto flutuante
#include <utility.hpp>


namespace lev_math {

//   ===========================================================
//   ======================   GLOBAL   =========================
//   ===========================================================
FILE *utility::bzzFileOut = stdout;
FILE *utility::bzzFileIn = stdin;
char utility::bzzYesNo = 'Y';
const double utility::MACH_EPS = utility::MachEps();
const float utility::BIG_FLOAT =                        FLT_MAX;//3.4e+38;
const float utility::TINY_FLOAT =                       FLT_MIN;//3.4e-38;
const double utility::BIG_DOUBLE =                      DBL_MAX;//1.7e+308;
const double utility::TINY_DOUBLE =                     DBL_MIN;//8.e-307;
const long double utility::BIG_LONG_DOUBLE =            LDBL_MAX;//3.4e+4932;
const long double utility::TINY_LONG_DOUBLE =           LDBL_MIN;//3.4e-4932;
const char utility::BIG_CHAR =                          127;
const char utility::TINY_CHAR =                         -128;
const unsigned char utility::BIG_UNSIGNED_CHAR =        255;
const unsigned char utility::TINY_UNSIGNED_CHAR =       0;
const int utility::BIG_INT =                            32767;
const int utility::TINY_INT =                           -32768;
const unsigned int utility::BIG_UNSIGNED_INT =          65535;
const unsigned int utility::TINY_UNSIGNED_INT =         0;
const long int utility::BIG_LONG  =                      LONG_MAX;//2147483647;
const long int utility::TINY_LONG =                     LONG_MIN;//-2147483648;
const unsigned long int utility::BIG_UNSIGNED_LONG  =   ULONG_MAX;//4294967295;
const unsigned long int utility::TINY_UNSIGNED_LONG =   0;


//   ===========================================================
//   ======================   ERRORS   =========================
//   ===========================================================

//========== Error Type ==============
const char *const utility::ERR_RANGE=
      "\nIndex out of range\n";
const char *const utility::ERR_SPACE=
      "\nNo room in heap\n";
const char *const utility::ERR_OPEN_FILE=
      "\nCan't open FILE\n";
const char *const utility::ERR_CLOSE_FILE=
      "\nCan't close FILE\n";
const char *const utility::ERR_READING_FILE=
      "\nCan't read FILE\n";
const char *const utility::ERR_WRITING_FILE=
      "\nCan't write FILE\n";
const char *const utility::ERR_CHECK_DIMENSION=
      "\nDimension check failure\n";
const char *const utility::ERR_OUT_OF_SCOPE=
      "\nOut of Scope\n";
const char *const utility::ERR_FACTORIZED=
      "\nFactorized Matrix\n";
const char *const utility::ERR_IMPLEMENTATION=
      "\nFunction not implemented\n";

//============ Function Type =============
const char *const utility::ERR_FUNCTION="\nFunction: ";
const char *const utility::ERR_CONSTRUCTOR="\nConstructor: ";
const char *const utility::ERR_OPERATOR="\nOperator: ";

//   ===========================================================
//   ====================   FUNCTIONS   ========================
//   ===========================================================

//   **********************< MachEps >**************************
//   * Purpose: Calculating the precision of the machine       *
//   * Description: It initialises the const MACH_EPS which    *
//   *              can be used  anywhere in the program       *
//   * Example: if( x < MACH_EPS)                              *
//   ***********************************************************
double utility::MachEps(void)
{
	double macheps = 1.,eps = 2.;
	while(eps != 1.) {
		macheps /= 2.;
		eps = 1. + macheps; // in simple rounded form
	}
	return macheps*2.;
}


//   **********************< Message >**************************
//   * Purpose: Sending messages to the FILE bzzFileOut        *
//   * Description: For handling in the same manner as printf. *
//   *              It does not interrupt the program          *
//   *              To disable: bzzYesNo = 'N'                 *
//   *              To enable: bzzYesNO = 'Y' (default)        *
//   * Example: Message("The value of i = %d",i);              *
//   ***********************************************************
void utility::Message(char *myFormat,...)
{
	if(utility::bzzYesNo != 'Y')
		return;
	va_list argPointer;
	va_start(argPointer,myFormat);
	vfprintf(utility::bzzFileOut,myFormat,argPointer);
	va_end(argPointer);
}


//   ***********************< Error >***************************
//   * Purpose: Sending error messages to the file stderr      *
//   * Description: As with Message, but it interrupts         *
//   *              the program                                *
//   * Example: if(x < 1) Error("x = %f",x);                   *
//   ***********************************************************
void utility::Error(char *myFormat,...)
{
	va_list argPointer;
	va_start(argPointer,myFormat);
	vfprintf(stderr,myFormat,argPointer);
	va_end(argPointer);
	exit(1);
}


#ifdef __COMPLEXO__
//   ***********************< sqrt  >***************************
//   * Purpose: It Calculates the square root of a complex     *
//   * Description: As presented in Numerical Recipes in C     *
//   *              pp 177, and appendix c                     *
//   * Example: a = sqrt(c);                                   *
//   ***********************************************************
complex<double> utility::sqrt(complex<double> &z)
{
	complex<double> c;
	double x,y,w,r;
	if ((real(z) == 0) && (imag(z) == 0) )
		c = complex<double> (0,0);
	else {
		x = fabs(real(z)); y = fabs(imag(z));
		if (x >= y) {
			r = y/x;
			w = sqrt(x)*sqrt(0.5*(1.+sqrt(1.+r*r)));
		} else {
			r = x/y;
			w = sqrt(y)*sqrt(0.5*(r+sqrt(1.+r*r)));
		}
		if (real(z) >= 0)
			c = complex<double>(w, imag(z)/(2.*w));
		else {
			w = (imag(z)>=0)?w:-w;
			c = complex<double>(imag(z)/(2.*w), w);
		}
	}
	return c;
}
#endif



/*
//   ********************< SqrtSumSqr >*************************
//   * Purpose: The Euclidean Norm for a double                *
//   * Description: Chapter 8                                  *
//   * Example: double x[100]; x[0]=.....                      *
//   *          norm = SqrtSumSqr(n,x);                        *
//   ***********************************************************
double SqrtSumSqr(int n,double *x)
	{
	if(n <= 0)
		Error("%s%sSqrtSumSqr",ERR_RANGE,ERR_FUNCTION);
	double aux, xmax = 0.,xmin = BIG_DOUBLE;
	for(int j = 0;j < n;j++)
		{
		aux = Abs(x[j]);
		if(xmax < aux)xmax = aux;
		if(xmin > aux)xmin = aux;
		}
	if(xmax == 0.)return xmax;
	if (xmin == 0.)xmin = TINY_DOUBLE;
	long double longaux =
		(long double)xmax/(long double)xmin;
	aux = sqrt(BIG_DOUBLE/((double)n));
	// to avoid the problems of
	if(xmax < aux &&                            // overflow
		xmax > TINY_DOUBLE/MACH_EPS &&       // small numbers
		longaux < 1./MACH_EPS)                     // sort
		{
		double norm = 0.;  // without problems: double
		for(int i = 0;i < n;i++)
		  {
		  aux = x[i];
		  norm += aux*aux;
		  }
		return sqrt(norm);
		}
	else  // if there are problems it works in long double
		{
		long double norm = 0.;
		for(int i = 0;i < n;i++)
		  {
		  longaux = x[i];
		  norm += longaux*longaux;
		  }
		if(norm < BIG_DOUBLE && norm > TINY_DOUBLE)
			return sqrt(norm);
		longaux = (long double)xmax*(long double)n;
		norm /= longaux; // avoids overflow
		norm /= longaux;
		norm = longaux*sqrt(norm); // renormalises
			  // avoids overflow
		if(norm > BIG_DOUBLE) norm = BIG_DOUBLE;
		return norm;
		}
	}
*/


//   ===========================================================
//   =====================   TempFile   ========================
//   *          Class for creating temporary files             *
//   ***********************************************************
int utility::TempFile::countTempFile = 0;


//   ********************< NewFileName >************************
//   * Purpose: Initialisation when the default                *
//   *          constructor has been used.                     *
//   * Description: Chapter 10                                 *
//   * Example: TempFile ff[10];                               *
//   *               ff[0].NewFileName("D:");                  *
//   ***********************************************************
void utility::TempFile::NewFileName(char *directoryTemp)
{
	nameTempFile = new char [strlen(directoryTemp)+10];
	strcpy(nameTempFile,directoryTemp);
	countTempFile++;
	sprintf(nameTempFile, "%sT%d.TMP", directoryTemp, countTempFile);
}

} //end lev_math namespace
