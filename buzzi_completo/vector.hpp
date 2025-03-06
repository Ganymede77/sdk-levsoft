#ifndef VECTOR_HPPX
#define VECTOR_HPPX

/*******************************************************************
//   ======================< VECTOR.HPP >=======================
//   * Class Vector                                            *
//   *       for operations between matrices and vectors       *
//   * Description: Chapter 12                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   ****** Constructors for Vector:                           *
//   * Vector v; // default                                    *
//   * Vector v = x; // copy-initializer                       *
//   * Vector v(n); // sizes and places at 0                   *
//   * Vector v(5,1.,2.,3.,4.,5.); // vector 1,2,3,4,5         *
//   * double x[5]={1.,2.,3.,4.,5.};                           *
//   * Vector v(5,x); // from array                            *
//   * Vector v("VET.DAT"); // Formatted  file                 *
//   * Vector v('*',VET.BIN"); // Binary file                  *
//   * Vector v(nc,w); // subvector of w                       *
//   * Vector(nc,i,w);// subvector of w; starting from i       *
//   ***********************************************************
//   ***** Access functions :                                  *
//   *  int i = v.Size();// dimensions                         *
//   *  double xf = v.GetValue(i) // element i                 *
//   *  Vector w = GetVector(nc,i,v); // subvector             *
//   *  double xf = v(i);   v(i) = 4.;                         *
//   *  double xf = v[i];   v[i] = 7.;                         *
//   *  v.SetValue(i,7.);                                      *
//   *  v.SetVector(i,w); // Vector w                          *
//   ***********************************************************
//   ***** Assignment:                                         *
//   *  v = w; // Assignment                                   *
//   ***********************************************************
//   *****  Operators for forming vectors:                     *
//   *  v = w&&y; // appends one vector to another             *
//   ***********************************************************
//   *****  Operators for tests:                               *
//   *  if(v == w)                                             *
//   *  if(v != w)                                             *
//   ***********************************************************
//   ***** Implemented operations :                            *
//   *  Sum (a,b,&c);  // c = a + b;                           *
//   *  c = a + b;     // c = a + b;                           *
//   *  Sum (a,b,&a);  // a = a + b;                           *
//   *  Sum (&a,b);    // a = a + b;                           *
//   *  a += b;        // a = a + b;                           *
//   *  Sum (a,b,&b);  // b = a + b;                           *
//   *  Sum (b,&a);    // a = b + a;                           *
//   *  Sum (a,a,&a);  // a = a + a;                           *
//   *  Sum (&a);      // a = a + a;                           *
//   *  Difference(a,b,&c);  // c = a - b;                     *
//   *  c = a - b;           // c = a - b;                     *
//   *  Difference(a,b,&a);  // a = a - b;                     *
//   *  Difference(&a,b);    // a = a - b;                     *
//   *  a -= b;              // a = a - b;                     *
//   *  Difference(a,b,&b);  // b = a - b;                     *
//   *  Difference(b,&a);    // a = b - a;                     *
//   *  Difference(a,a,&a);  // a = a - a;                     *
//   *  Difference(&a);      // a = a - a;                     *
//   *  Minus (a,&b);        // b = -a;                        *
//   *  b = -a;                                                *
//   *  Minus (&a);          // a = -a;                        *
//   *  Product(A,x,&y);     // y = A*x; A Matrix              *
//   *  y = A*x;             // y = A*x                        *
//   *  Product(A,x,&x);     // x = A*x;                       *
//   *  Product(A,&x);       // x = A*x;                       *
//   *  Product(R,x,&y);     // y = R*x; R MatrixRight         *
//   *  y =R*x;                                                *
//   *  Product(L,x,&y);     // y = L*x; L MatrixLeft          *
//   *  y = L*x;                                               *
//   *  Product(3.,x,&y);    // y = 3.*x;                      *
//   *  y = 3.*x;                                              *
//   *  Product(3.,x,&x);    // x = 3.*x;                      *
//   *  Product(3.,&x);      // x = 3.*x;                      *
//   *  x *= 3.;             // x = 3.*x;                      *
//   *  TProduct(x,y,&xf);   // xf = xTy;                      *
//   *  xf = Dot(x,y);       // xf = xTy;                      *
//   *  xf = x%y;            // xf = xTy;                      *
//   *  TProduct(A,x,&y);    // y = ATx;                       *
//   *  y = A%x;             // y = ATx;                       *
//   *  TProduct(A,x,&x);    // x = ATx;                       *
//   *  TProduct(A,&x);      // x = ATx;                       *
//   *  ProductT(x,y,&A);    // A = xyT;                       *
//   *  A = x->*y;           // A = xyT;                       *
//   *  Division(x,3.,&y);   // y = x/3.;                      *
//   *  y = x/3.;                                              *
//   *  Division(&x,3.);     //  x = x/3.;                     *
//   *  x /= 3.;             //  x = x/3.;                     *
//   ***********************************************************
//   *  Other functions:                                       *
//   *  v.Print("comment");                                    *
//   *  v.Save("Vet.DAT");                                     *
//   *  v.Save('*',"VET.BIN");                                 *
//   *  double xf = v.Max(imax);                               *
//   *  xf = v.MaxAbs(imax);                                   *
//   *  xf = v.Min(imin);                                      *
//   *  xf = v.MinAbs(imin);                                   *
//   *  xf = v.Norm1();                                        *
//   *  xf = v.Norm2();                                        *
//   *  xf = v.NormI();                                        *
//   *  Delete(&v); // eliminate a Vector                      *
//   *  ChangeDimensions(newdim,&v, 0);  ///mcc                *
//   *  Recover(&v,"VET.DAT");                                 *
//   *  Recover(&v,'*',"VET.BIN");                             *
//   *  Normalize(&v); // vTv = 1                              *
//   *  Reverse(&v);                                           *
//   *  Sort(&v);                                              *
//   *  Swap(&x,&y);                                           *
//   *  CleanVector();                                         *
//   ***********************************************************
****************************************************************/


#include <utility.hpp>


namespace lev_math {

// preventive statements
template <class D> class Te_Vector;
template <class D> class Te_Matrix;
template <class D> class Te_MatrixLeft;
template <class D> class Te_MatrixRight;
template <class D> class Te_MatrixSymm;
template <class D> class Te_MatrixSparse;
template <class D> class Factored;
template <class D> class FactoredPLR;
template <class D> class FactoredGauss;
template <class D> class FactoredQRLQ;
template <class D> class FactoredQR;
template <class D> class FactoredLQ;
template <class D> class FactoredSVD;
//template <class D> class FactoredSymm;

#if 10
template <class D> Te_Vector<D> GetVector (int nc,int ielem,const Te_Vector<D> &rval);
template <class D> Te_Vector<D> operator && (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> char operator == (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> char operator != (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> void Sum(const Te_Vector<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator + (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> void Sum(Te_Vector<D> *lvalAndResult,const Te_Vector<D> &rval);
template <class D> void Sum(const Te_Vector<D> &lval,Te_Vector<D> *rvalAndResult);
template <class D> void Sum(Te_Vector<D> *lvalRvalAndResult);
template <class D> void Difference(const Te_Vector<D> &lval, const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator - (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> void Difference(Te_Vector<D> *lvalAndResult,const Te_Vector<D> &rval);
template <class D> void Difference(const Te_Vector<D> &lval,Te_Vector<D> *rvalAndResult);
template <class D> void Difference(Te_Vector<D> *lvalRvalAndResult);
template <class D> void Minus (const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> void Minus (Te_Vector<D> *rvalAndResult);
template <class D> Te_Vector<D> operator * (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);
template <class D> void Product (const Te_Matrix<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> void Product (const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndresult);
template <class D> void Product (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval);
template <class D> void Product (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval);
template <class D> void Product (D lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (D lval,const Te_Vector<D> &rval);
template <class D> void Product (D lval,Te_Vector<D> *rvalAndResult);
// SLLV ADICIONOU
//template <class D> Te_Vector<D> operator * (double lval,const Te_Vector<D> &rval);
template <class D1, class D2> Te_Vector<D2> operator * (D1 lval,const Te_Vector<D2> &rval);
template <class D> void TProduct(const Te_Vector<D> &lval,const Te_Vector<D> &rval,D *result);
template <class D> D Dot(const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> D operator % (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> void TProduct(const Te_Matrix<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator % (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);
template <class D> void TProduct (const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndresult);
template <class D> void ProductT(const Te_Vector<D> &lval,const Te_Vector<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator ->* (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> void Division (const Te_Vector<D> &lval,D rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator / (const Te_Vector<D> &lval,D rval);
template <class D> void Division (Te_Vector<D> *lvalAndResult,D rval);
template <class D> void Delete(Te_Vector<D> *result);
template <class D> void ChangeDimensions(int dim,Te_Vector<D> *result,char zero );
template <class D> void Recover(Te_Vector<D> *result,char *filevector); // formatted
template <class D> void Recover(Te_Vector<D> *result,char,char *filevector);// binary
template <class D> void SumRankOne(const Te_Vector<D> &u,const Te_Vector<D> &vT,Te_Matrix<D> *result);
template <class D> void SumRankOne(double product,const Te_Vector<D> &u,const Te_Vector<D> &vT,Te_Matrix<D> *result);
template <class D> void SumRankOne(const Te_Vector<D> &u,const Te_Vector<D> &vT,double divisor,Te_Matrix<D> *result);
template <class D> D Normalize(Te_Vector<D> *result);
template <class D> void Reverse(Te_Vector<D> *result);
template <class D> void Sort(Te_Vector<D> *result);
template <class D> void Swap(Te_Vector<D> *lval,Te_Vector<D> *rval);
#endif


//   ===========================================================
//   ===================   class Vector   ======================
//   ===========================================================


template <class D>
class Te_Vector {

   friend class Te_Matrix<D>;
   friend class Te_MatrixRight<D>;
   friend class Te_MatrixLeft<D>;
   friend class Te_MatrixSymm<D>;
   friend class Te_MatrixSparse<D>;
   friend class Factored<D>;
   friend class FactoredPLR<D>;
   friend class FactoredGauss<D>;
   friend class FactoredQRLQ<D>;
   friend class FactoredQR<D>;
   friend class FactoredLQ<D>;
   friend class FactoredSVD<D>;
   // friend class FactoredSymm<D>;

private:

   static const char *const ERRORMESSAGE;
   static int vcount; // for whoAmI

   D *vector;
   int dimensions;
   int whoAmI;

   // initialise constructors
   void Initialize(int nc);

   // private constructor Vector('*',nc)
   Te_Vector(char,int nc);

public:

	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
    // default constructor Te_Vector v;
   	Te_Vector(void);

   	// copy-initializer
   	Te_Vector(const Te_Vector<D> &rval);

   	// sizes and initialises at 0
   	Te_Vector(int nc);

   // sizes and initialises
   Te_Vector(int nc,D v1,...);

   // data pointer constructor
   Te_Vector(int nc,D *initvalues);

   // file v(FILE) constructor;
   Te_Vector(char *filevector);

   // unformatted v('*',FILE) constructor saved with Save
   Te_Vector(char,char *filevector);

   // makes a subvector of size nc<=n
   Te_Vector(int nc,const Te_Vector<D> &rval);

   // likewise starting from ielem
   Te_Vector(int nc,int ielem,const Te_Vector<D> &rval);

//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================
   ~Te_Vector(void);

//   ===========================================================
//   **********   Non-modifying access functions   *************
//   ===========================================================
   int Size(void) const {return dimensions;} // dimensions

   int WhoAmI(void) const {return whoAmI;}

   // receives the value of the vector with control
   D GetValue(int i) const;

   // subvector (similar to Te_Vector(nc,ielem,Vector)
   friend Te_Vector<D> GetVector <D>
      (int nc,int ielem,const Te_Vector<D> &rval);

//   ===========================================================
//   *************   Modifying access functions   **************
//   ===========================================================
   // assigns and receives vector values with control
   D &operator () (int i);

   // assigns and receives values without control
   D &operator [] (int i);//{return vector[i];}

   // assigns vector values with control
   void SetValue(int i,D val);

   // modifies the vector starting from ielem with rval
   void SetVector(int ielem,const Te_Vector<D> &rval);

//   ===========================================================
//   ****************  assignment operators ********************
//   ===========================================================
   Te_Vector<D> &operator =
       (const Te_Vector<D> &rval);

//   ===========================================================
//   ***********   operators for composing vectors   ***********
//   ===========================================================
      //adds one vector to another in sequence
   friend Te_Vector<D> operator && <D>
       (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

//   ===========================================================
//   ****************   operators for tests   ******************
//   ===========================================================
   friend char operator == <D>
       (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

   friend char operator != <D>
       (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

//   ===========================================================
//   ====================   OPERATIONS   =======================
//   ===========================================================

//   ===========================================================
//   ***********************   Sum   ***************************
//   ===========================================================
   friend void Sum <D> (const Te_Vector<D> &lval,const Te_Vector<D> &rval,
       Te_Vector<D> *result); // Sum (a,b,&c); c = a + b;

   friend Te_Vector<D> operator + <D>
       (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

    // Sum (&a,b); a = a + b;
   friend void Sum <D> (Te_Vector<D> *lvalAndResult,
       const Te_Vector<D> &rval);

   Te_Vector<D> &operator +=
       (const Te_Vector<D> &rval);

   // Sum (b,&a); a = b + a;
   friend void Sum <D> (const Te_Vector<D> &lval,
       Te_Vector<D> *rvalAndResult);

    // Sum (&a); a = a + a;
   friend void Sum <D> (Te_Vector<D> *lvalRvalAndResult);

//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================
   // Difference(a,b,&c); c = a - b;
   friend void Difference <D> (const Te_Vector<D> &lval,
       const Te_Vector<D> &rval,Te_Vector<D> *result);


   // c = a - b;
   friend Te_Vector<D> operator - <D>
       (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
   /*{
   Te_Vector<D> result('*',lval.dimensions);
   Difference(lval,rval,&result);
   return result;
   }*/

   // Difference(&a,b); a = a - b;
   friend void Difference <D> (Te_Vector<D> *lvalAndResult,
       const Te_Vector<D> &rval);

   // a -= b; a = a - b;
   Te_Vector<D> &operator -=
       (const Te_Vector<D> &rval);

   // Difference(b,&a); a = b - a;
   friend void Difference <D> (const Te_Vector<D> &lval,
       Te_Vector<D> *rvalAndResult);
   // Difference(&a); a = a - a;
   friend void Difference <D> (Te_Vector<D> *lvalRvalAndResult);

//   ===========================================================
//   ***********************   Minus   *************************
//   ===========================================================
   // Minus (a,&b); b = -a;
   friend void Minus <D> (const Te_Vector<D> &rval,Te_Vector<D> *result);

   Te_Vector<D> operator -();// unary minus

   // Minus (&a); a = -a;
   friend void Minus <D> (Te_Vector<D> *rvalAndResult);

//   ===========================================================
//   **********************   Product   ************************
//   ===========================================================
   // Product(A,x,&y); y = A*x;
   friend void Product <D>
      (const Te_Matrix<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);

   // y = A*x;
   friend Te_Vector<D> operator * <>
      (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);

   // Product(A,&x); x = A*x;
   friend void Product <D>
      (const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndresult);

   // Product(R,x,&y); y = R*x;
   friend void Product <D>
      (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval,
      Te_Vector<D> *result);

   friend Te_Vector<D> operator * <D>   //y =R*x;
      (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval);

   // Product(L,x,&y); y = L*x;
   friend void Product <D>
      (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval,
      Te_Vector<D> *result);

   friend Te_Vector<D> operator * <D>   // y = L*x;
      (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval);

   // Product(3.,x,&y); y = 3.*x;
   friend void Product <D>
      (D lval,const Te_Vector<D> &rval,Te_Vector<D> *result);

   friend Te_Vector<D> operator * <D>
      (D lval,const Te_Vector<D> &rval);

   // SLLV ADICIONOU
   //   friend Te_Vector<D> operator *<D>
   //   (double lval,const Te_Vector<D> &rval);
   //template <class D1, class D2> friend Te_Vector<D2> operator * (D1 lval,const Te_Vector<D2> &rval);
   //   friend Te_Vector<D2> operator *<,>
   //   (D1 lval,const Te_Vector<D2> &rval);

   // Product(3.,&x); x = 3.*x;
   friend void Product <D>
      (D lval,Te_Vector<D> *rvalAndResult);

   Te_Vector<D> &operator *= (D rval);

	//   ===========================================================
	//   ******************   TProduct, Dot   **********************
	//   ===========================================================
    // TProduct(x,y,&c); c = xTy; c = x%y; c = Dot(x,y);
    friend void TProduct <D> (const Te_Vector<D> &lval, const Te_Vector<D> &rval,D *result);

    // c = Dot(x,y); c = xTy;
    friend D Dot <D> (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

    // c = x%y; c = xTy;
    friend D operator % <D> (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

    // TProduct(A,x,&y); y =ATx; y =A%x;
    friend void TProduct <D> (const Te_Matrix<D> &lval, const Te_Vector<D> &rval,Te_Vector<D> *result);

    // y = ATx;
    friend Te_Vector<D> operator % <D> (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);

    // TProduct(A,&x); x = ATx;
    friend void TProduct <D> (const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndresult);

	//   ===========================================================
	//   *********************   ProductT   ************************
	//   ===========================================================
    // ProductT(x,y,&A); A = xyT; A = x->*y;
    friend void ProductT <D> (const Te_Vector<D> &lval, const Te_Vector<D> &rval,Te_Matrix<D> *result);

    friend Te_Matrix<D> operator ->* <D> (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

	//   ===========================================================
	//   **********************   Division   ***********************
	//   ===========================================================

    // Division(x,3.,&y); y = x/3.;
    friend void Division <D> (const Te_Vector<D> &lval,D rval,Te_Vector<D> *result);

    // y = x/3.
    friend Te_Vector<D> operator / <D> (const Te_Vector<D> &lval,D rval);

    // Division(&x,3.); x = x/3.;
    friend void Division <D> (Te_Vector<D> *lvalAndResult,D rval);

    // x /= 3.; x = x/3.;
    Te_Vector<D> &operator /= (D rval);

	//   ===========================================================
	//   ===========   Non-modifying functions   ===================
	//   ===========================================================

    //   ***********************< Print >***************************
    void Print(char *msg="");
    void Save(char *filevector); // formatted
    void Save(char,char *filevector);// binary

    //   *********************< Max and Min >***********************
    D Max(int *imax = 0);
    D MaxAbs(int *imax=0);

    D Min(int *imin=0);
    D MinAbs(int *imin=0);

    //   ***********************< Norms >***************************
    D Norm1(void);
    D Norm2(void);
    D NormI(void);

	//   ===========================================================
	//   =============   Modifying Functions =======================
	//   ===========================================================
    friend void Delete <D> (Te_Vector<D> *result); // eliminates Vector
    friend void ChangeDimensions <D> (int dim,Te_Vector<D> *result, char zero);

    void ChangeDimension(int dim, char zero=0) {
    	int who = whoAmI;
    	if (dim != dimensions) {
    		delete [] vector;
    		Initialize(dim);
    		Te_Vector<D>::vcount--;
    	}
    	whoAmI = who;
    	if (zero == 0 && dimensions != 0)
    		memset(vector,0,(dimensions + 1) *sizeof(D));
    }

    // recovery from Save
    friend void Recover <D> (Te_Vector<D> *result,char *filevector); // formatted
    friend void Recover <D> (Te_Vector<D> *result,char,char *filevector);// binary
    friend void SumRankOne <D> (const Te_Vector<D> &u, const Te_Vector<D> &vT,Te_Matrix<D> *result);
    friend void SumRankOne <D> (double product,const Te_Vector<D> &u, const Te_Vector<D> &vT,Te_Matrix<D> *result);
    friend void SumRankOne <D> (const Te_Vector<D> &u, const Te_Vector<D> &vT,double divisor,Te_Matrix<D> *result);

    // normalises (xTx=1) and returns norm2
    // friend double Normalize<D>(Te_Vector<D> *result);
    friend D Normalize <D> (Te_Vector<D> *result);
    friend void Reverse <D> (Te_Vector<D> *result);  //inverts the vector
    friend void Sort <D> (Te_Vector<D> *result); // orders the vector
    // swaps the contents of two vectors
    friend void Swap <D> (Te_Vector<D> *lval,Te_Vector<D> *rval);
    // Zera todos os termos de um vetor
    void CleanVector();
};


template <class D>
const char *const Te_Vector<D>::ERRORMESSAGE = "\n========>>> Vector error!!!!!!\n";
template <class D>
int Te_Vector<D>:: vcount = 0;


//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   ********************< Initialize >*************************
//   * Purpose: For initialising various constructors          *
//   * Description: Internal use                               *
//   * Example: Initialize(nc);                                *
//   ***********************************************************
template <class D>
void Te_Vector<D>::Initialize(int size)
{
	vcount++;
	whoAmI = vcount;
	if(size < 1) {
		dimensions = 0;
		vector = NULL;
	} else {
		dimensions = size;
		vector = new D [size+1];
		if(!vector)
			utility::Error((char*)("%s%s%sInitialize"), Te_Vector<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
	}
}


//   ****************< private constructor >********************
//   * Purpose: Sizing a Vector without initialisation         *
//   *         for internal use                                *
//   * Example: Vector x('*',3);                               *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(char,int nc)
{
	Initialize(nc);
}


//   ===========================================================
//   ================== Public functions =======================
//   ===========================================================

//   ===========================================================
//   *******************   constructors   **********************
//   ===========================================================

//   **********************< default >**************************
//   * Purpose: Initialising a Vector which can then receive   *
//   *               an assignment                             *
//   * Example: Vector x;                                      *
//   *          x = y;                                         *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(void)
{
	Initialize(0);
}


//   ******************< Copy-Initializer >*********************
//   * Purpose: Initialising from Vector                       *
//   *          Used in returns                                *
//   * Example: Te_Vector x = b; // simple initialisation      *
//   *          Te_Vector x = b+2.*y;  // with operations      *
//   *          return x; (used implicitly)                    *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(const Te_Vector<D> &rval)
{
	Initialize(rval.dimensions);
	if(dimensions != 0)
		memcpy(vector,rval.vector,(dimensions+1)*sizeof(D));
}


//   ********************< sizing >*****************************
//   * Purpose: Sizing a Vector                                *
//   *          Initialises it to zero                         *
//   * Example: Te_Vector x(3);                                *
//   *          x(1) = 2.; x(2) = 4.; x(3) = 5.;               *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(int nc)
{
	Initialize(nc);
	if(dimensions != 0)
		memset(vector,0,(dimensions+1)*sizeof(D));
}


//   **************< sizing and initialising >******************
//   * Purpose: Initialising a Vector                          *
//   * Example: Te_Vector x(3,1.,2.,3.);                          *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(int nc,D v1,...)
   {
   Initialize(nc);
   D *w = vector + 1;
   va_list puntaList;
   va_start(puntaList,v1);
   int i;
   *w = utility::Control(v1);
   for(i = 2;i <= nc;i++)
      *++w = utility::Control(va_arg(puntaList,D));
   va_end(puntaList);
   }


//   *********************< from array >************************
//   * Purpose: Initialising a Vector from an array            *
//   * Example: double w[] = {1.,2.,3.,4.,5.,6.};               *
//   *          Vector x(6,w);                                 *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(int nc,D *initvalues)
   {
   Initialize(nc);
   if(dimensions != 0)
      {
      D *w = vector;
      w++;
      memcpy(w,initvalues,nc*sizeof(D));
      }
   }


//   ********************< ASCII FILE >*************************
//   * Purpose: Initialising a Vector from FILE                *
//   * Example: Vector x("VECT.DAT");                          *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(char *filevector)
   {
   if((utility::bzzFileIn = fopen(filevector,"r"))==NULL)
       utility::Error("%s%s%s%sConstructor",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filevector,utility::ERR_FUNCTION);
   int nc;
   fscanf(utility::bzzFileIn,"%d",&nc);
   Initialize(nc);
   D *w = vector;
   int i;
   if(dimensions != 0)
      for(i = 1;i <= nc;i++)
        fscanf(utility::bzzFileIn,"%f",++w);
   fclose(utility::bzzFileIn);
   }

//   ********************< from binary FILE >*******************
//   * Purpose: Initialising a Vector from unformatted FILE    *
//   * Description: Constructor for recovering a Vector        *
//   *              saved on FILE with Save('*',file name)     *
//   * Example: Te_Vector v('*',"VET.BIN");                       *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(char,char *filevector)
   {
   if((utility::bzzFileIn=fopen(filevector,"rb"))==NULL)
       utility::Error("%s%s%s%sFILE",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filevector,utility::ERR_CONSTRUCTOR);
   int nc;
   if(fread(&nc,sizeof(int),1,utility::bzzFileIn) != 1)
      utility::Error("when reading dimensions");
   Initialize(nc);
   if(dimensions != 0)
      if(fread(vector+1,sizeof(D)*nc,1,utility::bzzFileIn) != 1)
         utility::Error("%s%s%sConstructor",
         Te_Vector<D>::ERRORMESSAGE,utility::ERR_READING_FILE,utility::ERR_CONSTRUCTOR);
   fclose(utility::bzzFileIn);
   }


//   ********************< subvector >**************************
//   * Purpose: Subvector with the same beginning and nc <= n  *
//   * Example: Te_Vector vNew(5,vOld);                           *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(int nc,const Te_Vector &rval)
   {
   if(nc < 1 || nc > rval.dimensions)
      utility::Error("%s%s%s subvector",
      Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   Initialize(nc);
   memcpy(vector,rval.vector,(nc+1)*sizeof(D));
   }

//   ********************< subvector >**************************
//   * Purpose: Subvector with beginning at ielem              *
//   *          and length nc<=n-ielem+1                       *
//   * Example: Te_Vector vNew(3,2,vOld);                         *
//   ***********************************************************
template <class D>
Te_Vector<D>::Te_Vector(int nc,int ielem,const Te_Vector<D> &rval)
   {
   if(nc < 1 || ielem < 1 || (nc+ielem-1) > rval.dimensions)
      utility::Error("%s%s%sSubvector ",
      Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   Initialize(nc);
   D *r = rval.vector+ielem-1;
   memcpy(vector,r,(dimensions+1)*sizeof(D));
   }

//   ********************< destructor >*************************
//   * Purpose: Deinitialising Vector                          *
//   ***********************************************************
template <class D>
Te_Vector<D>::~Te_Vector(void)
{
	if(vector != NULL) {
		delete [] vector; vector = NULL;
	}
}


//   ===========================================================
//   ***********  Non-modifying access functions    ************
//   ===========================================================

//   *********************< GetValue >**************************
//   * Purpose: Receiving values with control                  *
//   * Description: Return is not a reference!                 *
//   *              The user cannot access the data.           *
//   * Example: x = y.GetValue(3);                             *
//   ***********************************************************
template <class D>
D Te_Vector<D>::GetValue(int i) const
   {
   if(i < 1 || i > dimensions )
      utility::Error((char*)"%s%s%sValue",
      Te_Vector<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
   return vector[i];
   }

//   ********************< GetVector >**************************
//   * Purpose: Creating a subvector                           *
//   *        (similar to constructor Te_Vector(nc,ielem,Vector)  *
//   *        It is better to use constructor                  *
//   * Description: nc: dimensions of new vector               *
//   *              ielem: the element from where it starts    *
//   *              rval: Vector to be used                    *
//   * Example: x = GetVector(5,2,y);                          *
//   *          x has dimension 5                              *
//   *          and starts from element 2 of y                 *
//   ***********************************************************
template <class D>
Te_Vector<D> GetVector(int nc,int ielem,const Te_Vector<D> &rval)
   {
   if(nc < 1 || ielem < 1 || (nc+ielem-1) > rval.dimensions)
      utility::Error((char*)"%s%s%sGetVector",
      Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   Te_Vector<D> result('*',nc);
   D *r = rval.vector+ielem-1;
   memcpy(result.vector,r,(nc+1)*sizeof(D));
   return result;
   }

//   ===========================================================
//   *************    Modifying access functions ***************
//   ===========================================================

//   ********************< operator () >************************
//   * Purpose: Receiving and assigning values with control    *
//   * Example: x = v(5); v(7) = 3.;                           *
//   ***********************************************************
template <class D>
D &Te_Vector<D>::operator () (int i)
       {
       if(i < 1 || i > dimensions )
          utility::Error((char*)("%s%s%s()"),
          Te_Vector<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_OPERATOR);
       return vector[i];
       }

template <class D>
D & Te_Vector <D> :: operator [] (int i) {return vector[i];}



//   *********************< SetValue >**************************
//   * Purpose: Assigning values with control                  *
//   * Description: Not with a reference!                      *
//   *              The user cannot access the data.           *
//   * Example: x.SetValue(3,3.14);                            *
//   ***********************************************************
template <class D>
void Te_Vector<D>::SetValue(int i,D val)
   {
   if(i < 1 || i > dimensions)
      utility::Error((char*)"%s%s%sSetValue",
      Te_Vector<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
   vector[i] = val;
   }

//   ********************< SetVector >**************************
//   * Purpose: Assigning coefficients for a portion           *
//   * Description: v.SetVector(3,w);                          *
//   *         substitutes w starting from coefficient 3 of v  *
//   ***********************************************************
template <class D>
void Te_Vector<D>::SetVector(int ielem,const Te_Vector<D> &rval)
   {
   if(ielem < 1 || ielem > dimensions
                  ||
      rval.dimensions > dimensions-ielem+1)
        utility::Error((char*)"%s%s%sSetVector",
        Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
      D*r = vector+ielem-1;
      memcpy(r,rval.vector,
           (rval.dimensions+1)*sizeof(D));
   }

//   ===========================================================
//   ****************   assignment operator ********************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment                                     *
//   * Example: x = b;    // simple                            *
//   *          x = y + z;    // with operations               *
//   *          x = b = c + d;  // multiple                    *
//   ***********************************************************
template <class D>
Te_Vector<D> &Te_Vector<D>::operator = (const Te_Vector<D> &rval)
{
	int who = whoAmI;
	if(dimensions != rval.dimensions) {
		delete [] vector;
		vector = NULL;
		Initialize(rval.dimensions);
		vcount--;
	}
	whoAmI = who;
	memcpy(vector,rval.vector,(dimensions+1)*sizeof(D));
	return *this;
}


//   ===========================================================
//   ************   operator for creating vectors   ************
//   ===========================================================

//   ************************< && >*****************************
//   * Purpose: Constructing one Vector from two Vectors       *
//   *          Putting them in series                         *
//   * Description: I use && and not || because Vector         *
//   *              is a column vector                         *
//   * Example:  x = a&&b&&c;                                  *
//   ***********************************************************
template <class D>
Te_Vector<D> operator && (const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   // sizing without initialisation
   Te_Vector<D> result('*',lval.dimensions+rval.dimensions);
   D *w = lval.vector;
   D *z = rval.vector;
   D *r = result.vector;
   memcpy(r,w,(lval.dimensions+1)*sizeof(D));
   r = result.vector+lval.dimensions+1;
   z = z+1;
//  memcpy(r,z,(rval.dimensions + 1)*sizeof(D));  	// Esta linha foi alterada porque dava problemas de
   memcpy(r,z,(rval.dimensions)*sizeof(D));
   return result;
   }

//   ===========================================================
//   ****************   operators for tests   ******************
//   ===========================================================

//   ************************< == >*****************************
//   * Purpose: Comparing two Vectors                          *
//   * Description: Uses the function memcmp                   *
//   * Example: if(w == v)                                     *
//   ***********************************************************
template <class D>
char operator == (const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   if(lval.whoAmI == rval.whoAmI) return 1;
   char ch = 1;
   if(lval.dimensions != rval.dimensions) ch = 0;
   else
      {
      if(memcmp(lval.vector+1,rval.vector+1,
              rval.dimensions*sizeof(D))==0)ch=1;
      else ch=0;
      }
   return ch;
   }

//   ************************< != >*****************************
//   * Purpose: Testing whether two Vectors are different      *
//   * Description: Uses memcmp                                *
//   * Example: if(v != w)                                     *
//   ***********************************************************
template <class D>
char operator != (const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   if(lval.whoAmI == rval.whoAmI) return 0;
   char ch = 0;
   if(lval.dimensions != rval.dimensions)ch = 1;
   else
      ch = memcmp(lval.vector+1,rval.vector+1,
              rval.dimensions*sizeof(D));
   return ch;
   }

//   ===========================================================
//   ====================   OPERATIONS   =======================
//   ===========================================================

//   ===========================================================
//   ***********************   Sum   ***************************
//   ===========================================================

//   ************************< Sum >****************************
//   * Purpose: Addition between vectors                       *
//   * Description: Uses Sum from utility.cpp                  *
//   * Example: Sum(a,b,&c); c = a + b;     OK                   *
//   ***********************************************************
template <class D>
void Sum(const Te_Vector<D> &lval,const Te_Vector<D> &rval,
        Te_Vector<D> *result)
   {
   if(lval.dimensions != rval.dimensions)
       utility::Error("%s%s%s+",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(result->whoAmI == lval.whoAmI)
      (*result) += rval;
   else if(result->whoAmI == rval.whoAmI)
      Sum(lval,result);
   else
      {
      ChangeDimensions(lval.dimensions,result,1);
      utility::Sum(result->dimensions,lval.vector+1,
         rval.vector+1,result->vector+1); // UTILITY.CPP
      }
   }

//   *************************< + >*****************************
//   * Purpose: Addition of two Vectors                        *
//   * Description: Uses Sum(x,y,&z);                          *
//   * Example: y = a + x;          OK                           *
//   ***********************************************************
template <class D>
Te_Vector<D> operator + (const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   Te_Vector<D> result('*',lval.dimensions);
   Sum(lval,rval,&result);
   return result;
   }

//   ************************< Sum >****************************
//   * Purpose: Addition between vectors                       *
//   * Description: The result goes in the left vector         *
//   * Example: Sum(&a,b); a += b;     OK                        *
//   ***********************************************************
template <class D>
void Sum(Te_Vector<D> *lvalAndResult,const Te_Vector<D> &rval)
   {
   (*lvalAndResult) += rval;
   }

//   ************************< += >*****************************
//   * Purpose: Addition of two Vectors with +=                *
//   * Description: Better than a = a + x;                     *
//   * Example: a += x;                    OK                    *
//   ***********************************************************
template <class D>
Te_Vector<D> &Te_Vector<D>::operator += (const Te_Vector<D> &rval)
   {
   if(dimensions != rval.dimensions)
       utility::Error((char*)("%s%s%s+="),
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(whoAmI == rval.whoAmI)
      Sum(this);
   else
      utility::Sum(dimensions,vector+1,rval.vector+1); // UTILITY.CPP
   return *this;
   }

//   ************************< Sum >****************************
//   * Purpose: Addition between vectors                       *
//   * Description: The result goes in the right vector        *
//   * Example: Sum(b,&a); a = b + a;    OK                      *
//   ***********************************************************
template <class D>
void Sum(const Te_Vector<D> &lval,Te_Vector<D> *rvalAndResult)
   {
   (*rvalAndResult) += lval;
   }

//   ************************< Sum >****************************
//   * Purpose: Addition between two identical vectors         *
//   * Description: The result replaces the vector             *
//   * Example: Sum(&a); a = a + a;         OK                   *
//   ***********************************************************
template <class D>
void Sum(Te_Vector<D> *lvalRvalAndResult)
   {
   utility::Sum(lvalRvalAndResult->dimensions,
       lvalRvalAndResult->vector+1);
   }

//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================

//   **********************< Difference >***********************
//   * Purpose: Difference between vectors                     *
//   * Description: Uses Difference from utility.cpp           *
//   * Example: Difference(a,b,&c); c = a - b;       OK          *
//   ***********************************************************
template <class D>
void Difference(const Te_Vector<D> &lval,const Te_Vector<D> &rval,
              Te_Vector<D> *result)
   {
   if(lval.dimensions!=rval.dimensions)
		utility::Error((char*)"%s%s%s-",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(result->whoAmI == lval.whoAmI)
      (*result) -= rval;
   else if(result->whoAmI == rval.whoAmI)
     Difference(lval,result);
   else
      {
      ChangeDimensions(lval.dimensions,result,1);
      utility::Difference(result->dimensions,lval.vector + 1,
         rval.vector + 1,result->vector + 1); //UTILITY.CPP
      }
   }

//   *************************< - >*****************************
//   * Purpose: Difference between two vectors                 *
//   * Example: y = z - x;        OK                             *
//   ***********************************************************

template <class D>
Te_Vector<D> operator - (const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   Te_Vector<D> result('*',lval.dimensions);
   Difference(lval,rval,&result);
   return result;
   }

//   **********************< Difference >***********************
//   * Purpose: Difference between vectors                     *
//   * Description: The result goes in the left vector         *
//   * Example: Difference(&a,b); a = a - b;      OK             *
//   ***********************************************************
template <class D>
void Difference(Te_Vector<D> *lvalAndResult,const Te_Vector<D> &rval)
   {
   (*lvalAndResult) -= rval;
   }

//   ************************< -= >*****************************
//   * Purpose: Difference between two vectors with -=         *
//   * Description: Better than a = a - x;                     *
//   * Example: a -= x;                     OK                   *
//   ***********************************************************
template <class D>
Te_Vector<D> &Te_Vector<D>::operator -= (const Te_Vector<D> &rval)
   {
   if(dimensions != rval.dimensions)
	   utility::Error((char*)"%s%s%s+=",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(whoAmI == rval.whoAmI)
      Difference(this);
   else
      utility::Difference(dimensions,vector + 1,
             rval.vector +1); // in UTILITY.CPP
   return *this;
   }

//   **********************< Difference >***********************
//   * Purpose: Difference between vectors                     *
//   * Description: The result goes in the right vector        *
//   * Example: Difference(a,&b); b = a - b;   OK                *
//   ***********************************************************
template <class D>
void Difference(const Te_Vector<D> &lval,Te_Vector<D> *rvalAndResult)
   {
   if(lval.dimensions != rvalAndResult->dimensions)
       utility::Error((char*)"%s%s%sDiffernece",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   utility::Difference(lval.dimensions,lval.vector+1,
             rvalAndResult->vector+1,1);
   }

//   **********************< Difference >***********************
//   * Purpose: Difference between identical vectors           *
//   * Description: null Vector                                *
//   * Example: Difference(&a); a = a - a;     OK                *
//   ***********************************************************
template <class D>
void Difference(Te_Vector<D> *lvalRvalAndResult)
   {
   D *w = lvalRvalAndResult->vector;
   for(int i = 1;i <= lvalRvalAndResult->dimensions;i++)
       *++w = 0;
   }

//   ===========================================================
//   ***********************   Minus   *************************
//   ===========================================================

//   ************************< Minus >**************************
//   * Purpose: Unary minus           *
//   * Example: Minus(a,&b); b = -a;      OK                     *
//   ***********************************************************
template <class D>
void Minus (const Te_Vector<D> &rval,Te_Vector<D> *result)
   {
   ChangeDimensions(rval.dimensions,result,1);
   D *w = rval.vector;
   D *r = result->vector;
   for(int i = 1;i <= rval.dimensions;i++)
       *++r = -(*++w);
   }

//   *************************< - >*****************************
//   * Purpose: Changing the sign of a Vector                  *
//   * Example: y = -x;            OK                            *
//   ***********************************************************
template <class D>
Te_Vector<D> Te_Vector<D>::operator -() // unary minus
   {
   Te_Vector<D> result('*',dimensions);
   Minus(*this,&result);
   return result;
   }

//   ************************< Minus >**************************
//   * Purpose: Unary minus           *
//   * Example: Minus(&a); a = -a;       OK                      *
//   ***********************************************************
template <class D>
void Minus (Te_Vector<D> *rvalAndResult)
   {
   D *r = rvalAndResult->vector;
   for(int i = 1;i <= rvalAndResult->dimensions;i++)
       r[i] = -r[i];
   }

//   ===========================================================
//   ***********************   Product   ***********************
//   ===========================================================

//   ***********************< Product >*************************
//   * Purpose: The product between a matrix and a vector      *
//   * Example: Product(A,x,&y); y = A*x;                      *
//   ***********************************************************
template <class D>
void Product(const Te_Matrix<D> &lval,const Te_Vector<D> &rval,
            Te_Vector<D> *result)
   {
   if(lval.numColumns!=rval.dimensions)
       utility::Error("%s%s%s*",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(rval.whoAmI == result->whoAmI)
      {
      Te_Vector<D> aux;
      Product(lval,rval,&aux); // recursive call
      Swap(&aux,result); // avoids copying
      }
   else
      {
      ChangeDimensions(lval.numRows,result,1);
      D *r = result->vector;
      for(int i = 1;i <= lval.numRows;i++)
        r[i] = utility::Dot(rval.dimensions,lval.matrix[i] +1,
                rval.vector + 1);
      }
   }

//   *************************< * >*****************************
//   * Purpose: The product between a matrix and a vector      *
//   * Example: y = A*x;                                       *
//   ***********************************************************
template <class D>
Te_Vector<D> operator * (const Te_Matrix<D> &lval,const Te_Vector<D> &rval) {
   Te_Vector<D> result(lval.Rows());//obs: 05/07/2006 *NMA* *AP* *GNM*: foi substituido construtor e o acesso ao numero de linhas e colunas (problema na compilação), isto deve ser mantido
   Product(lval,rval,&result);
   return result;
}


//   ***********************< Product >*************************
//   * Purpose: The product between a matrix and a vector      *
//   * Example: Product(A,&x); x = A*x;                        *
//   ***********************************************************
template <class D>
void Product(const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndResult) {
   Product(lval,*rvalAndResult,rvalAndResult);
}


//   ***********************< Product >*************************
//   * Purpose: The product between a double and a vector       *
//   * Example: Product(3.,x,&y); y = 3.*x;                    *
//   ***********************************************************
template <class D>
void Product(D lval,const Te_Vector<D> &rval,Te_Vector<D> *result)
   {
   if(rval.whoAmI == result->whoAmI)
      {
   utility::Product(result->dimensions,lval,
          result->vector + 1); // in UTILITY.CPP
      }
   else
      {
      ChangeDimensions(rval.dimensions,result,1);
      utility::Product(rval.dimensions,lval,rval.vector + 1,
                   result->vector + 1); // in UTILITY.CPP
      }
   }

//   *************************< * >*****************************
//   * Purpose: The product between a double and a vector       *
//   * Example: y = 3.*x;             OK                         *
//   ***********************************************************
template <class D>
Te_Vector<D> operator * (D lval,const Te_Vector<D> &rval)
   {
   Te_Vector<D> result(rval.Size());//obs: 05/07/2006 *NMA* *AP* *GNM*: foi substituido construtor e o acesso a dimension (problema na compilação), isto deve ser mantido
   Product(lval,rval,&result);
   return result;
   }


//   ***********************< Product >*************************
//   * Purpose: The product between a double and a vector       *
//   * Example: Product(3.,&x); x = 3.*x;   OK                  *
//   ***********************************************************
template <class D>
void Product (D lval,Te_Vector<D> *rvalAndResult)
   {
   utility::Product(rvalAndResult->dimensions,lval,
          rvalAndResult->vector + 1); // in UTILITY.CPP
   }

//   ************************< *= >*****************************
//   * Purpose: The product between a double and                *
//   *          a vector with *=                               *
//   * Description: To be used preferably!                     *
//   * Example: y *= 3.;         OK                              *
//   ***********************************************************
template <class D>
Te_Vector<D> &Te_Vector<D>::operator *= (D rval)
   {
	utility::Product(dimensions,rval,vector + 1); // in UTILITY.CPP
   return *this;
   }

//   ===========================================================
//   ******************   TProduct, Dot   **********************
//   ===========================================================

//   *********************< TProduct >**************************
//   * Purpose: Dot product between two vectors (xTy)          *
//   * Description: Uses Dot of UTILITY                        *
//   * Example: TProduct(x,y,&c); c = xTy;                     *
//   ***********************************************************
template <class D>
void TProduct(const Te_Vector<D> &lval,const Te_Vector<D> &rval,
             D *result)
   {
   if(lval.dimensions!=rval.dimensions)
       utility::Error("%s%s%sDot",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   *result = utility::Dot(lval.dimensions,lval.vector + 1,
                 rval.vector +1); // in UTILITY.CPP
   }

//   ************************< Dot >****************************
//   * Purpose: Dot product between two vectors (xTy)          *
//   * Description: Uses TProduct                              *
//   * Example: c = Dot(x,y);                                  *
//   ***********************************************************


template <class D>
D Dot(const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   D result;
   TProduct(lval,rval,&result);
   return result;
   }


//   *************************< % >*****************************
//   * Purpose: Dot product between two vectors (xTy)          *
//   * Description: Uses TProduct                              *
//   * Example: w = x%y;                                       *
//   ***********************************************************
template <class D>
D operator % (const Te_Vector<D> &lval,const Te_Vector<D> &rval)
   {
   D result;
   TProduct(lval,rval,&result);
   return result;
   }

//   *********************< TProduct >**************************
//   * Purpose: ATx: Matrix A transposed and multiplied        *
//   *               by Vector x                               *
//   * Description: the matrix is scanned row by row           *
//   * Example: TProduct(A,x,&y);                              *
//   ***********************************************************
template <class D>
void TProduct(const Te_Matrix<D> &lval, const Te_Vector<D> &rval,Te_Vector<D> *result) {
   if(lval.numRows != rval.dimensions)
       utility::Error("%s%s%s%TProduct",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   if(rval.whoAmI == result->whoAmI)
      {
      Te_Vector<D> aux;
      TProduct(lval,rval,&aux); // recursive call
      Swap(&aux,result); // avoids copying
      }
   else
      {
      ChangeDimensions(lval.numColumns,result,1);
      int i,j;
      D *r = result->vector;
      D *left = lval.matrix[1] + 1;
      D *right = rval.vector + 1;
      for(j = 1;j <= lval.numColumns;j++)
        r[j] = 0;
      for(i = 1;i <= lval.numRows;i++)
        {
	for(j = 1;j<= lval.numColumns;j++)
           r[j] = utility::Control(r[j] + (*left++)*(*right));
        right++;
        }
      }
   }

//   *************************< % >*****************************
//   * Purpose: ATx: Matrix A transposed and multiplied        *
//   *               by Vector x                               *
//   * Description: The matrix is scanned row by row           *
//   * Example: y = A%x;                                       *
//   ***********************************************************
template <class D>
Te_Vector<D> operator % (const Te_Matrix<D> &lval,const Te_Vector<D> &rval)
   {
   Te_Vector<D> result('*',lval.numColumns);
   TProduct(lval,rval,&result);
   return result;
   }

//   ***********************< TProduct >************************
//   * Purpose: The product between a transposed matrix        *
//   *          and a vector                                   *
//   * Example: TProduct(A,&x); // x = ATx;                    *
//   ***********************************************************
template <class D>
void TProduct(const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndResult) {
   TProduct(lval,*rvalAndResult,rvalAndResult);
}


//   ===========================================================
//   ********************   ProductT   *************************
//   ===========================================================

//   *********************< ProductT >**************************
//   * Purpose: The product between a column vector and        *
//   *         a row vector                                    *
//   * Example: ProductT(x,y,&A); A = xyT;                     *
//   ***********************************************************
template <class D>
void ProductT(const Te_Vector<D> &lval,const Te_Vector<D> &rval, Te_Matrix<D> *result) {
   if(lval.dimensions != rval.dimensions)
       utility::Error((char*)("%s%s%sProductT"),
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   ChangeDimensions(lval.dimensions,
                  lval.dimensions,result,1);
   D *r = result->matrix[1] + 1;
   D *left = lval.vector + 1;
   D *right = rval.vector;
   for(int i = 1;i <= lval.dimensions;i++)
     {
     for(int j = 1;j <= rval.dimensions;j++)
       *r++ = utility::Control(*left * right[j]);
     left++;
     }
   }

//   ************************< ->* >****************************
//   * Purpose: The product between a Vector                   *
//   *          and a transposed Vector                        *
//   * Example: Matrix A = y ->* x;                            *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator ->* (const Te_Vector<D> &lval,const Te_Vector<D> &rval) {
   Te_Matrix<D> result('*',lval.dimensions,lval.dimensions);
   ProductT(lval,rval,&result);
   return result;
}


//   ===========================================================
//   **********************   Division   ***********************
//   ===========================================================

//   ***********************< Division >************************
//   * Purpose: Dividing the coefficients of a vector          *
//   *          by a double                                    *
//   * Description: Uses Division of utility.cpp               *
//   * Example: Division(x,3.,&y); y = x/3.;     OK            *
//   ***********************************************************
template <class D>
void Division(const Te_Vector<D> &lval,D rval,Te_Vector<D> *result) {
   if(lval.whoAmI == result->whoAmI) {
      utility::Division(result->dimensions, result->vector+1,rval); //in UTILITY.CPP
   } else {
      ChangeDimensions(lval.dimensions,result,1);
      utility::Division(lval.dimensions,lval.vector + 1,rval, result->vector + 1); // in UTILITY.CPP
   }
}


//   *************************< / >*****************************
//   * Purpose: Dividing the coefficients of a vector          *
//   *          by a double                                    *
//   * Description: Uses Division of utility.cpp               *
//   * Example: y = x/3.;       OK                             *
//   ***********************************************************
template <class D>
Te_Vector<D> operator / (const Te_Vector<D> &lval,D rval)
   {
   Te_Vector<D> result('*',lval.dimensions);
   utility::Division(lval.dimensions,lval.vector + 1,rval,
                   result.vector + 1); // in UTILITY.CPP
   return result;
   }

//   ***********************< Division >************************
//   * Purpose: Dividing the coefficients of a vector          *
//   *          by a double                                     *
//   * Description: Uses Division of utility.cpp               *
//   * Example: Division(&x,3.); x = x/3.;      OK               *
//   ***********************************************************
template <class D>
void Division(Te_Vector<D> *lvalAndResult,D rval)
   {
   Division(lvalAndResult->dimensions,
           lvalAndResult->vector+1,rval);
   }

//   ************************< /= >*****************************
//   * Purpose: Divides and modifies a Vector by a double       *
//   * Description: Uses Division of utility.cpp               *
//   * Example: x /= 2.; // x = x/2.;          OK                *
//   ***********************************************************
template <class D>
Te_Vector<D> &Te_Vector<D>::operator /= (D rval)
   {
	utility::Division(dimensions,vector + 1,rval);//in UTILITY.CPP
   return *this;
   }

//   ===========================================================
//   *                 Non-modifying functions                 *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Prints on utility::bzzFileOut the elements of a Vector  *
//   * Description: Prints on utility::bzzFileOut                       *
//   * Example: v.Print("Vector v");                           *
//   ***********************************************************
template <class D>
void Te_Vector<D>::Print(char *msg)
   {
   fprintf(utility::bzzFileOut,"\nVector No.%d",whoAmI);
   if(*msg)fprintf(utility::bzzFileOut,"  %s",msg);
   fprintf(utility::bzzFileOut,"\nSize %d\n",dimensions);
   for(int i = 1;i <= dimensions;i++)
        fprintf(utility::bzzFileOut," %12.4e",vector[i]);
   }

//   ************************< Save >***************************
//   * Purpose: Saving on file a Vector formatted in ASCII     *
//   * Description: Can be read with a constructor             *
//   *              or with Recover                            *
//   * Example: v.Save("VET.DAT");                             *
//   ***********************************************************
template <class D>
void Te_Vector<D>::Save(char *filevector) // formatted
   {
   FILE *bzz;
   if((bzz = fopen(filevector,"w")) == NULL)
       utility::Error("%s%s%s%sSave",
       Te_Vector<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filevector,utility::ERR_FUNCTION);

   fprintf(bzz," %d\n",dimensions);
   for(int i = 1;i <= dimensions;i++)
      fprintf(bzz," %15.7e\n",vector[i]);
   fclose(bzz);
   }

//   ************************< Save >***************************
//   * Purpose: Saving a Vector on an unformatted file         *
//   * Description: Can be read with a constructor             *
//   *              or with Recover                            *
//   * Example: v.Save('*',VET.BIN");                          *
//   ***********************************************************
template <class D>
void Te_Vector<D>::Save(char s,char *filevector) // unformatted
   {
   FILE *bzz;
   if((bzz = fopen(filevector,"wb")) == NULL)
       utility::Error("%s%s%s%sSave",
       Te_Vector::ERRORMESSAGE,utility::ERR_OPEN_FILE,filevector,utility::ERR_FUNCTION);

   if(fwrite(&dimensions,sizeof(int),1,bzz) != 1)
      utility::Error("I could not write ");

   if(fwrite(vector+1,sizeof(D)*dimensions,1,bzz) != 1)
      utility::Error("I could not write ");

   fclose(bzz);
   }



//   ===========================================================
//   *                       Max Min                           *
//   ===========================================================

//   ************************< Max >****************************
//   * Purpose: The maximum of a Vector                        *
//   * Description: Uses the function defined  in UTILITY.CPP  *
//   * Example: double xmax = v.Max();                         *
//   *          double xmax = v.Max(&imax);                    *
//   ***********************************************************
// *RYT* 30/10/2007 Este trecho de codigo foi descomentado. Eh
//       utilizado no QWS
template <class D>
D Te_Vector<D>::Max(int *imax)
   {
   D xmax = utility::Max(dimensions,vector + 1,imax);
   if(imax != 0)(*imax)++;
   return xmax;
   }


//   **********************< MaxAbs >***************************
//   * Purpose: The absolute maximum of a Vector               *
//   * Description: Uses the function defined  in UTILITY.CPP  *
//   * Example: double xmax = v.MaxAbs();                       *
//   *          double xmax = v.MaxAbs(&imax);                  *
//   ***********************************************************
template <class D>
D Te_Vector<D>::MaxAbs(int *imax)
   {
   D xmax = utility::MaxAbs(dimensions,vector + 1,imax);
   if(imax != 0) (*imax)++;
   return xmax;
   }



//   ************************< Min >****************************
//   * Purpose: The minimum of a Vector                        *
//   * Description: Uses the function defined  in UTILITY.CPP  *
//   * Example: double xmin = v.Min();                         *
//   *          double xmin = v.Min(&imin);                    *
//   ***********************************************************
// *RYT* 30/10/2007 Este trecho de codigo foi descomentado. Eh
//       utilizado no QWS
template <class D>
D Te_Vector<D>::Min(int *imin)
   {
   D xmin = utility::Min(dimensions,vector + 1,imin);
   if(imin != 0) (*imin)++;
   return xmin;
   }



//   *********************** MinAbs >***************************
//   * Purpose: The absolute minimum of a Vector               *
//   * Description: Uses the function described in UTILITY.CPP *
//   * Example: double xmin = v.MinAbs();                       *
//   *          double xmin = v.MinAbs(&imin);                  *
//   ***********************************************************
template <class D>
D Te_Vector<D>::MinAbs(int *imin)
   {
   D xmin = utility::MinAbs(dimensions,vector + 1,imin);
   if(imin != 0) (*imin)++;
   return xmin;
   }

//   *                        Norms                            *
//   ===========================================================

//   ***********************< Norm1 >***************************
//   * Purpose: Calculating the norm-1                         *
//   * Description: Addition of absolute values of v           *
//   * Example: double norm1 = v.Norm1();                       *
//   ***********************************************************
template <class D>
D Te_Vector<D>::Norm1(void)
{
D norm = 0.,*x = vector;
for(int i = 1;i <= dimensions;i++)
   norm+=Abs(*++x);
return norm;
}

//   ***********************< Norm2 >***************************
//   * Purpose: norm-2 or Euclidean norm                       *
//   * Description: square root of the sum of squares          *
//   *              Chapter 8                                  *
//   * Example: double norm2 = v.Norm2();                       *
//   ***********************************************************
template <class D>
D Te_Vector<D>::Norm2(void)
   {
   D *x = vector + 1;
   return utility::SqrtSumSqr(dimensions,x); //in UTILITY.CPP
   }

//   ***********************< NormI >***************************
//   * Purpose: Norm-I                                         *
//   * Description: Absolute maximum                           *
//   * Example: double normi = v.NormI();                       *
//   ***********************************************************
template <class D>
D Te_Vector<D>::NormI(void)
{
return MaxAbs();
}

//   ===========================================================
//   *                 Modifying functions                     *
//   ===========================================================

//   **********************< Delete >***************************
//   * Purpose: Eliminate a vector of no use                   *
//   *          without leaving the scope                      *
//   * Example: Delete(&x);                                    *
//   ***********************************************************
template <class D>
void Delete(Te_Vector<D> *result)
{
	delete [] result->vector;
	result->dimensions = 0;
	result->vector = NULL;
}


//   ***************< ChangeDimensions >************************
//   * Purpose: Change the dimensions of a Vector              *
//   * Example: ChangeDimensions(dim,&a, 0); //mcc             *
//   ***********************************************************
template <class D>
void ChangeDimensions(int dim,Te_Vector<D> *result,char zero)
{
	int who = result->whoAmI;
	if(dim != result->dimensions) {
		delete [] result->vector;
		result->vector = NULL;
		result->Initialize(dim);
		Te_Vector<D>::vcount--;
	}
	result->whoAmI = who;
	if(zero == 0 && result->dimensions != 0)
		memset(result->vector,0, (result->dimensions + 1)*sizeof(D));
}


//   **********************< Recover >**************************
//   * Purpose: Recovering a vector stored with                *
//   *          formatted Save(file name)                      *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved vector             *
//   * Example: Recover(&x,nomefile);                          *
//   ***********************************************************
template <class D>
void Recover(Te_Vector<D> *result,char *filevector)
{
	if((utility::bzzFileIn = fopen(filevector,"r"))==NULL) {
		utility::Error("%s%s%s%sRecover", Te_Vector<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filevector,utility::ERR_FUNCTION);
	}
	int nc;
	fscanf(utility::bzzFileIn,"%d",&nc);
	ChangeDimensions(nc,result,1);
	D *w = result->vector;
	int i;
	for(i = 1;i <= nc;i++)
		fscanf(utility::bzzFileIn,"%f",++w);
	fclose(utility::bzzFileIn);
}


//   **********************< Recover >**************************
//   * Purpose: Recovering a vector stored by                  *
//   *          unformatted Save('*', file name)               *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved vector             *
//   * Example: Recover(&x,'*',file name);                     *
//   ***********************************************************
template <class D>
void Recover(Te_Vector<D> *result,char,char *filevector)
{
	if((utility::bzzFileIn=fopen(filevector,"rb"))==NULL) {
		utility::Error("%s%s%s%sRecover", Te_Vector<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filevector,utility::ERR_FUNCTION);
	}
	int nc;
	if(fread(&nc,sizeof(int),1,utility::bzzFileIn) != 1) {
		utility::Error("reading in dimensions");
	}
	ChangeDimensions(nc,result,1);
	if(nc < 1) {
		fclose(utility::bzzFileIn);
		return;
	}
	D *v = result->vector;
	if(fread(v+1,sizeof(D)*nc,1,utility::bzzFileIn) != 1) {
		utility::Error("%s%s%sRecover", Te_Vector<D>::ERRORMESSAGE,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
	}
   fclose(utility::bzzFileIn);
}


//   *********************< Normalize >*************************
//   * Purpose: Normalising a vector (xTx = 1)                 *
//   *          Returns the norm value                         *
//   * Example: double norm = Normalize(&x);                   *
//   ***********************************************************
template <class D>
D Normalize(Te_Vector<D> *result)
{
	D norm = result->Norm2();
	if(norm==0.)
		return norm;
	D *w = result->vector;
	for(int i = 1;i <= result->dimensions;i++)
		*++w /= norm;
	return norm;
}


//   **********************< Reverse >**************************
//   * Purpose: Inverting the elements of a Vector             *
//   * Description: Swaps extreme elements                     *
//   * Example: Reverse(&x);                                   *
//   ***********************************************************
template <class D>
void Reverse(Te_Vector<D> *result)
{
	int m = (result->dimensions+1)/2;
	int n = result->dimensions;
	int i,j;
	if(n < 2) return;
	D *v = result->vector;
	if(n == 2)
		Swap(v+1,v+2);
	else
		for(i = 1,j = n;i <= m;i++,j--)
			utility::Swap(v+i,v+j); // in UTILITY.CPP
}


//   ***********************< Sort >****************************
//   * Purpose: Ordering a Vector                              *
//   * Description: Uses heap sort of UTILITY.CPP (Chapter 8)  *
//   * Example: Te_Vector x(150,w); Sort(&x);                  *
//   ***********************************************************
template <class D>
void Sort(Te_Vector<D> *result)
{
	Sort(result->dimensions,result->vector + 1);
}


//   ************************< Swap >***************************
//   * Purpose: Swapping the elements of any two Vectors       *
//   * Description: Provides an efficient method of copying    *
//   *              When a Vector remains in the purpose and   *
//   *              one leaves, they swap                      *
//   * Example: Swap(&x,&y);                                   *
//   ***********************************************************
template <class D>
void Swap(Te_Vector<D> *lval,Te_Vector<D> *rval)
{
	utility::Swap(&lval->vector,&rval->vector);
	utility::Swap(&lval->dimensions,&rval->dimensions);
}


//   ********************< CleanVector >************************
//   * Purpose: Zera todos os termos de um vetor               *
//   * Description: Faz com que um vetor tenha todos os seus   *
//   *                  termos setados em zero                 *
//   * Example: x.Clean();                                     *
//   ***********************************************************
template <class D>
void Te_Vector<D>::CleanVector()
{
	int i;
	for(i = 1; i <= dimensions; i++)
		vector[i] = 0;
}


//   *********************< Conjugate >*************************
//   * Purpose: Modifying a matrix into its conjugate          *
//   * Description:                                            *
//   * Example: Conjugate(&A);                                 *
//   ***********************************************************
template <class D>
void Conjugate(Te_Vector<D> *A)
{
	D * mat = A->vector+1;
	for (int i = 1; i <= A->dimensions; i++,mat++)
		*mat = conj(*mat);
}

} // end namespace lev_math

#endif // VECTOR_HPP
