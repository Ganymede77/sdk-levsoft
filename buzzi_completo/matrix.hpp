#ifndef MATRIX_HPPX
#define MATRIX_HPPX

//   ======================< MATRIX.HPP >=======================
//   * Matrix Class:                                           *
//   * for operations between matrices and vectors             *
//   * Description: Chapter 12                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   ****** Constructors by Matrix:                            *
//   * Matrix A; // default                                    *
//   * Matrix A = B; // copy-initializer                       *
//   * Matrix A(m,n); // sizes and places at 0                 *
//   * Matrix A(2,3,1.,2.,3.,4.,5.,6.); // matrix 2X3          *
//   * T x[6]={1.,2.,3.,4.,5.,6.};                             *
//   * Matrix A(2,3,x); // from array                          *
//   * Matrix A("MAT.DAT"); // Formatted  file                 *
//   * Matrix A('*',MAT.BIN"); // Binary file                  *
//   * Matrix A(m,n,B); // submatrix of B                      *
//   * Matrix A(m,n,i,j,B);// submatrix of B from i,j          *
//   * Matrix A = v; // from Te_Vector<T>                      *
//   * Matrix A = R; // from Te_MatrixRight<T>                 *
//   * Matrix A = L; // from Te_MatrixLeft<T>                  *
//   * Matrix A = R; // from Te_MatrixSymm<T>                  *
//   ***********************************************************
//   ***** Access functions:                                   *
//   * i = A.Rows(); // numRows                                *
//   * i = A.Columns(); // numColumns                          *
//   * who = A.WhoAmI();                                       *
//   * xf = A.GetValue(i,j);                                   *
//   * v = GetRow(i);                                          *
//   * v = GetColumn(j);                                       *
//   * xf = A(i,j);                                            *
//   * A(i,j) = xf;                                            *
//   * xf = A[i][j];                                           *
//   * A[i][j] = xf;                                           *
//   * A.SetValue(i,j,xf);                                     *
//   * A.SetRow(i,v);                                          *
//   * A.SetColumn(j,v);                                       *
//   ***********************************************************
//   ***** Assignments:                                        *
//   * A = B;  // Matrix                                       *
//   * A = v;  // Te_Vector<T>                                 *
//   * A = R;  // Te_MatrixRight<T>                            *
//   * A = L;  // Te_MatrixLeft<T>                             *
//   * A = S;  // Te_MatrixSymm<T>                             *
//   ***********************************************************
//   *****  Operators for composing matrices:                  *
//   * A = B&&C; // Adds C beneath B                           *
//   * A = B||C; // Adds C onto the side of B                  *
//   ***********************************************************
//   *****  Operators for tests:                               *
//   * if(A == B)                                              *
//   * if(A != B)                                              *
//   ***********************************************************
//   ***** Implemented operations :                            *
//   * Sum(A,B,&C); // C = A + B;                              *
//   * C = A + B;   // C = A + B;                              *
//   * Sum(A,B,&A); // A = A + B;                              *
//   * Sum(&A,B);   // A = A + B;                              *
//   * A += B;      // A = A + B;                              *
//   * Sum(A,B,&B); // B = A + B;                              *
//   * Sum(A,&B);   // B = A + B;                              *
//   * Sum(A,A,&A); // A = A + A;                              *
//   * Sum(&A);     // A = A + A;                              *
//   * Difference(A,B,&C); // C = A - B;                       *
//   * C = A - B;          // C = A - B;                       *
//   * Difference(A,B,&A); // A = A - B;                       *
//   * Difference(&A,B);   // A = A - B;                       *
//   * A -= B;             // A = A - B;                       *
//   * Difference(A,B,&B); // B = A - B;                       *
//   * Difference(A,&B);   // B = A - B;                       *
//   * Difference(A,A,&A); // A = A - A;                       *
//   * Difference(&A);     // A = A - A;                       *
//   * Minus(A,&B);       // B = -A;                           *
//   * B = -A;            // B = -A;                           *
//   * Minus(&A);         // A = -A;                           *
//   * Product(A,B,&C);   // C = A*B;                          *
//   * C = A*B;                                                *
//   * Product(A,B,&A);   // A = A*B;                          *
//   * Product(&A,B);     // A = A*B;                          *
//   * A *= B;            // A = A*B;                          *
//   * Product(A,B,&B);   // B = A*B;                          *
//   * Product(A,&B);     // B = A*B;                          *
//   * Product(A,A,&A);   // A = A*A;                          *
//   * Product(&A);       // A = A*A;                          *
//   * Product(A,x,&y);   // y = A*x;                          *
//   * y = A*x;                                                *
//   * Product(A,x,&x);   // x = A*x;                          *
//   * Product(A,&x);     // x = A*x;                          *
//   * Product(3.,A,&B);  // B = 3.*A;                         *
//   * B = 3.*A;                                               *
//   * Product(3.,&A);    // A = 3.*A;                         *
//   * A *= 3.;           // A = 3.*A;                         *
//   * TProduct(A,B,&C);  // C = ATB;                          *
//   * C = A%B;           // C = ATB;                          *
//   * TProduct(A,B,&A);  // A = ATB;                          *
//   * TProduct(&A,B);    // A = ATB;                          *
//   * A %= B;            // A = ATB;                          *
//   * TProduct(A,B,&B);  // B = ATB;                          *
//   * TProduct(A,&B);    // B = ATB;                          *
//   * TProduct(A,A,&A);  // A = ATA;                          *
//   * TProduct(&A);      // A = ATA;                          *
//   * A %= A;            // A = ATA;                          *
//   * y = A%x;           // y = ATx;                          *
//   * ProductT(x,y,&A);  // A = xyT;                          *
//   * A = x->*y;         // A = xyT;                          *
//   * ProductT(&A);      // A = AAT                           *
//   * Division(B,3.,&A); // A = B/3.;                         *
//   * A = B/3.;                                               *
//   * Division(&A,3.);   // A = A/3.;                         *
//   * A /= 3.;           // A = A/3.                          *
//   ***********************************************************
//   * Other functions:                                        *
//   * A.Print("Comment");                                     *
//   * A.Save("MAT.DAT", int=0);  // formatted                        *
//   * A.Save('*',"MAT.DAT");  // unformatted                  *
//   * xf = A.Max(&imax,&jmax);                                *
//   * xf = A.MaxAbs(&imax,&jmax);                             *
//   * xf = A.Min(&imin,&jmin);                                *
//   * xf = A.MinAbs(&imin,&jmin);                             *
//   * xf = A.NormT();                                         *
//   * xf = A.NormR();                                         *
//   * xf = A.NormC();                                         *
//   * xf = A.NormF();                                         *
//   * xf = A.NormI();                                         *
//   * xf = A.Norm1();                                         *
//   * Delete(&A);                                             *
//   * ChangeDimensions(newr,newc,&A,0); ///mcc                *
//   * ChangeDimensions(newr,newc);                            *
//   * Recover(&A,"MAT.DAT");                                  *
//   * Recover(&A,'*',"MAT.DAT");                              *
//   * Transpose(&A);                                          *
//   * SumRankOne(u,vT,&A);      // A = A + uvT;               *
//   * SumRankOne(3.,u,vT,&A);   // A = A + 3.*uvT;            *
//   * SumRankOne(u,vT,3.,&A);   // A = A + uvT/3.;            *
//   * Swap(&A,&B);                                            *
//   ***********************************************************


#include <utility.hpp>
#include <vector.hpp>
#include <right.hpp>
#include <left.hpp>
#include <symm.hpp>
#include <spars.hpp>


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
template <class D> class FactoredSymm;


#if 10
template <class D> Te_Matrix<D> operator || (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> char operator == (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> char operator != (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> Te_Matrix<D> operator + (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> void Sum (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
template <class D> void Sum (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
template <class D> void Sum (const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);
template <class D> void Sum (Te_Matrix<D> *lvalRvalAndResult);
template <class D> void Difference (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator - (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> void Difference (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
template <class D> void Difference (const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);
template <class D> void Difference (Te_Matrix<D> *lvalRvalAndResult);
template <class D> void Minus (const Te_Matrix<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator - (const Te_Matrix<D> &rval);
template <class D> void Minus (Te_Matrix<D> *rvalAndResult);
template <class D> void Product (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator * (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> void Product (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
template <class D> void Product (const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);
template <class D> void Product (Te_Matrix<D> *lvalRvalAndResult);
template <class D> void Product (const Te_Matrix<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);
template <class D> void Product (D lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator * (D lval,const Te_Matrix<D> &rval);
template <class D> void Product (D lval,Te_Matrix<D> *rvalAndResult);
template <class D> void TProduct (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator % (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
template <class D> void TProduct(const Te_Matrix<D> &lval, const Te_Vector<D> &rval,Te_Vector<D> *result);
template <class D> Te_Vector<D> operator % (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);
template <class D> void TProduct (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
template <class D> void TProduct(const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);
template <class D> void TProduct(Te_Matrix<D> *lvalRvalAndResult);
template <class D> void TProduct (const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndresult);
template <class D> void ProductT(const Te_Vector<D> &lval, const Te_Vector<D> &rval,Te_Matrix<D> *result);
template <class D> Te_Matrix<D> operator ->* (const Te_Vector<D> &lval,const Te_Vector<D> &rval);
template <class D> void ProductT (Te_Matrix<D> *lvalRvalAndResult);
template <class D> Te_Matrix<D> operator / (const Te_Matrix<D> &lval,double rval);
template <class D> void Division(const Te_Matrix<D> &lval,double rval,Te_Matrix<D> *result);
template <class D> void Division(Te_Matrix<D> *lvalAndResult,double rval);
template <class D> void Delete(Te_Matrix<D> *A);
template <class D> void ChangeDimensions(int rows,int columns, Te_Matrix<D> *result, char zero);
template <class D> void ChangeDimensions(int rows,int columns, char zero=0);
template <class D> void Recover(Te_Matrix<D> *A,char *filevector);
template <class D> void Recover(Te_Matrix<D> *A,char,char *filevector);
template <class D> void Transpose(Te_Matrix<D> *A);
template <class D> char Inverse(Te_Matrix<D> *A);  // Gauss Jordan
template <class D> void SumRankOne(const Te_Vector<D> &u, const Te_Vector<D> &vT,Te_Matrix<D> *result);
template <class D> void SumRankOne(double product,const Te_Vector<D> &u, const Te_Vector<D> &vT,Te_Matrix<D> *result);
template <class D> void SumRankOne(const Te_Vector<D> &u, const Te_Vector<D> &vT,double divisor,Te_Matrix<D> *result);
template <class D> void Swap(Te_Matrix<D> *lval,Te_Matrix<D> *rval);
template <class D> void Solve(const Te_MatrixRight<D> &R,Te_Matrix<D> *BX);
template <class D> void Solve(const Te_MatrixLeft<D> &R,Te_Matrix<D> *BX);
template <class D> Te_Matrix<D>  operator && (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
#endif

//   ===========================================================
//   ===================   class Matrix   ======================
//   ===========================================================


template <class D>
class Te_Matrix {
   friend class Te_Vector<D>;
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
   friend class FactoredSymm<D>;

private:
   static const char *const ERRORMESSAGEMATRIX;
   static int count; // for whoAmI Por algum motivo (??) estao usando um contador externo!!!!!

   D **matrix;
   int numRows,numColumns;
   int size;
   int whoAmI;

   // initialisation constructors
   void Initialize(int mrows,int ncolumns);

   // deinitialisation
   void Deinitialize(void);

   // prepares assignments
   void PrepCopy(int rRows,int rColumns);

   // for assigning and initialising from Te_MatrixRight
   void CopyRight(const Te_MatrixRight<D> & rval);
   // for assigning and initialising from Te_MatrixLeft
   void CopyLeft(const Te_MatrixLeft<D> & rval);

   // for assigning and initialising from Te_MatrixSymm
   void CopySymm(const Te_MatrixSymm<D> & rval);

   // private constructor Te_Matrix A type('*',3,5);
   Te_Matrix(char,int mrows,int columns);

public:
	//   ===========================================================
	//   *******************   constructors   **********************
	//   ===========================================================
   // default // Te_Matrix A;
   Te_Matrix (void);

   // copy-initializer // Te_Matrix A = B;
   Te_Matrix(const Te_Matrix <D> &rval);

   // sizes and initialises at 0 // Te_Matrix A(3,5);
   Te_Matrix (int rows,int columns);


   // sizes and initialises
   // Te_Matrix A(2,3,1.,2.,3.,4.,5.,6.);
   Te_Matrix(int rows, int columns,D a11,...);

   // initialises from array
   // float w[6]={1.,2.,3.,4.,5.6}; Matrix A(3,5,w);
   Te_Matrix(int rows,int columns,D *initvalues);

   // initialises from formatted File
   // Te_Matrix A("MAT.DAT");
   Te_Matrix(char *filematrix);

   // initialises from binary File
   // Te_Matrix A('*',"MAT.BIN"); See Save
   Te_Matrix(char,char *filematrix);

   // makes a submatrix of rows,columns
   Te_Matrix(int rows,int columns,const Te_Matrix <D> &rval);

   // likewise starting from irow,jcol
   Te_Matrix(int rows,int columns,int irow,int jcol,const Te_Matrix <D> &rval);

   // from Te_Vector
   Te_Matrix(const Te_Vector<D> &rval);

   // from Te_MatrixRight
   Te_Matrix(const Te_MatrixRight<D> &rval);

   // from Te_MatrixLeft
   Te_Matrix(const Te_MatrixLeft<D> &rval);

   // from Te_MatrixSymm
   Te_Matrix(const Te_MatrixSymm<D> &rval);

   //   ===========================================================
   //   ********************   destructor   ***********************
   //   ===========================================================
   ~Te_Matrix (void);


   //   ===========================================================
   //   ********   Non-modifying access functions   ***************
   //   ===========================================================
   // number of rows
   int Rows(void) const {
      return numRows;
   }

   // number of columns
   int Columns(void) const {
      return numColumns;
   }

   int WhoAmI(void) const {
      return whoAmI;
   }

   // receives the values with control
   D GetValue(int row,int col) const;


   //takes the row i of a matrix
   Te_Vector<D> GetRow(int i) const;

   //takes the column j of a matrix
   Te_Vector<D> GetColumn(int j) const;

   //   ===========================================================
   //   ************   Modifying access functions   ***************
   //   ===========================================================
   //   assigns and receives vector values with control
   D &operator () (int row,int col);

   // assigns and receives vector values without control
   D *operator [] (int r) {
      return matrix[r];
   }

   // assigns values with control
   void SetValue(int row,int column,D val);

   //substitutes column i with rval
   void SetRow(int i,const Te_Vector<D> &rval);

   // substitutes column j with rval
   void SetColumn(int j,const Te_Vector<D> &rval);

   //   ===========================================================
   //   **************  assignment operators **********************
   //   ===========================================================
   Te_Matrix<D> &operator = (const Te_Matrix<D> &rval);

   Te_Matrix<D> &operator = (const Te_Vector<D> &rval);

   Te_Matrix<D> &operator = (const Te_MatrixLeft<D> &rval);

   Te_Matrix<D> &operator = (const Te_MatrixRight<D> &rval);

   Te_Matrix<D> &operator = (const Te_MatrixSymm<D> &rval);


   //   ===========================================================
   //   ***********   operators for composing matrices   **********
   //   ===========================================================
   //   adds a matrix beneath another
   friend Te_Matrix<D> lev_math::operator && <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);

   //   add a matrix to the side of another
   friend Te_Matrix<D> lev_math::operator || <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);

   //   ===========================================================
   //   *****************    test operators   *********************
   //   ===========================================================
   friend char lev_math::operator == <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);

   friend char lev_math::operator != <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);

   //   ===========================================================
   //   ====================   OPERATIONS   =======================
   //   ===========================================================

   //   ===========================================================
   //   ***********************   Sum   ***************************
   //   ===========================================================

   //   Sum(A,B,&C); C = A + B;
   friend void lev_math::Sum <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
   // C = A + B;
   friend Te_Matrix<D> lev_math::operator + <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
   // Sum(&A,B); A = A + B;
   friend void lev_math::Sum <>
      (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
   // A += B; A = A + B;
   Te_Matrix<D> & operator +=
      (const Te_Matrix<D> &rval);
   // Sum(B,&A); A = B + A;
   friend void lev_math::Sum <>
      (const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);
   // Sum(&A); A = A + A;
   friend void lev_math::Sum <>
      (Te_Matrix<D> *lvalRvalAndResult);

	//   ===========================================================
	//   ********************   Difference   ***********************
	//   ===========================================================
   // Difference(A,B,&C); C = A - B;
   friend void lev_math::Difference <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);

   // C = A - B;
   friend Te_Matrix<D> lev_math::operator - <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);

   // Difference(&A,B); A = A - B;
   friend void lev_math::Difference <>
      (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);

   // A -= B; A = A - B;
   Te_Matrix<D> & operator -=
      (const Te_Matrix<D> &rval);

   // Difference(B,&A); A = B - A;
   friend void lev_math::Difference <>
      (const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);

   // Difference(&A); A = A - A;
   friend void lev_math::Difference <>
      (Te_Matrix<D> *lvalRvalAndResult);

   //   ===========================================================
   //   ***********************   Minus   *************************
   //   ===========================================================
   friend void lev_math::Minus <>
      (const Te_Matrix<D> &rval,Te_Matrix<D> *result);

   friend Te_Matrix<D> lev_math::operator - <>
      (const Te_Matrix<D> &rval);

   friend void lev_math::Minus <>
      (Te_Matrix<D> *rvalAndResult);

   //   ===========================================================
   //   **********************   Product   ************************
   //   ===========================================================
   friend void lev_math::Product <>            // C=A*B;
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
   friend Te_Matrix<D> lev_math::operator * <> // C=A*B;
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval);
   friend void lev_math::Product <>            // A = A * B;
      (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
   Te_Matrix<D> & operator *=             // A = A * B;
      (const Te_Matrix<D> &rval);
   friend void lev_math::Product <>            // B = A * B;
      (const Te_Matrix<D> & lval,Te_Matrix<D> *rvalAndResult);
   friend void lev_math::Product <>            // A = A * A;
      (Te_Matrix<D> *lvalRvalAndResult);
   friend void lev_math::Product <>            // y =A*x;
      (const Te_Matrix<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);
   friend Te_Vector<D> lev_math::operator * <> // y = A*x;
      (const Te_Matrix<D> & lval,const Te_Vector<D> &rval);
   friend void lev_math::Product <>            // A = 3.*B;
      (D lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);
   friend Te_Matrix<D> lev_math::operator * <> // A = 3.*B;
      (D lval,const Te_Matrix<D> &rval);
   friend void lev_math::Product <>            // A = 3.* A;
      (D lval,Te_Matrix<D> *rvalAndResult);
   Te_Matrix<D> &operator *=             // A = 3.*A;
      (D rval);

   //   ===========================================================
   //   *********************   TProduct   ************************
   //   ===========================================================
   friend void lev_math::TProduct <>           // C=ATB;
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result);

   friend Te_Matrix<D> lev_math::operator % <>
      (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval); // ATB

   // TProduct(A,x,&y); y =ATx; y =A%x;
   friend void lev_math::TProduct <> (const Te_Matrix &lval,
      const Te_Vector<D> &rval,Te_Vector<D> *result);

   friend Te_Vector<D> lev_math::operator % <>
      (const Te_Matrix<D> &lval,const Te_Vector<D> &rval);

   friend void lev_math::TProduct <>     // A = ATB;
      (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);

   Te_Matrix<D> & operator %=
      (const Te_Matrix<D> &rval);

   friend void lev_math::TProduct <>      // B = ATB;
      (const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult);

   friend void lev_math::TProduct <>      // A = A % A;
      (Te_Matrix<D> *lvalRvalAndResult);

   // TProduct(A,&x); x = ATx;
   friend void lev_math::TProduct <>
      (const Te_Matrix<D> &lval,Te_Vector<D> *rvalAndresult);


   //   ===========================================================
   //   *********************   ProductT   ************************
   //   ===========================================================
   // ProductT(x,y,&A); A = xyT; A = x->*y;
   friend void lev_math::ProductT <> (const Te_Vector<D> &lval,
      const Te_Vector<D> &rval,Te_Matrix<D> *result);

   // A = x->*y; A = xyT;
   friend lev_math::Te_Matrix<D> operator ->* <>
      (const Te_Vector<D> &lval,const Te_Vector<D> &rval);

   friend void lev_math::ProductT <>      // A = AAT ;
      (Te_Matrix<D> *lvalRvalAndResult);

	//   ===========================================================
	//   **********************   Division   ***********************
	//   ===========================================================
   friend void lev_math::Division <>               // A =B/3.;
      (const Te_Matrix<D> &lval,double rval,Te_Matrix<D> *result);

   friend lev_math::Te_Matrix<D> operator / <>     // A= B/3.;
      (const Te_Matrix<D> &lval,double rval);

   friend void lev_math::Division <>               // A = A/3.;
      (Te_Matrix<D> *lvalAndResult,double rval);

   Te_Matrix<D> & operator /=           // A /= 3.;
      (double rval);

   //   ===========================================================
   //   ===========   Non-modifying functions   ===================
   //   ===========================================================

   //   ***********************< Print >***************************
   void Print(char *msg="");

   //   ************************< Save >***************************
   void Save(char *filematrix, int = 0); // formatted allow append
   //   void Save(char *filematrix); // formatted *AP* 23012007
   void Save(char,char *filematrix);// binary

   //   *********************< Max and Min >***********************
   //to have the position Max(&im,&jc)
   D Max(int *imax=0,int *jmax=0);
   D MaxAbs(int *imax=0,int *jmax=0);
   D Min(int *imin=0,int *jmin=0);
   D MinAbs(int *imin=0,int *jmin=0);

   //   ***********************< Norms >***************************
   double NormT(void);
   double NormR(void);
   double NormC(void);
   double NormF(void);
   double NormI(void){return NormR();}
   double Norm1(void){return NormC();}

   //   ===========================================================
   //   =============   Modifying Functions =======================
   //   ===========================================================

   void ChangeDimensions (int rows, int columns, char zero=0) {
      PrepCopy(rows,columns);
      if(zero == 0 && size != 0)
        memset(matrix[0],0,size*sizeof(D));
   }

   /* *OFLN* *RYT* */
   // Necessario para se passar a matriz para funcao de biblioteca fortran
   // Acesso direto
   void MatrixToPointer(D* copia, int N)
   {
	   if (N >= size)
		   memcpy (copia, matrix[1], sizeof(D)*size);
	   else
		   memcpy(copia, matrix[1],sizeof(D)*N);
   }

   D* MatrixToPointer(void)
   {
          D *w=matrix[0];
          ++w;
          D *copia = new D [size];
          if (copia)
            memcpy (copia, w, sizeof(D)*size);
          return copia;
   }

   friend void lev_math::Delete <> (Te_Matrix<D> *A); // eliminate Matrix
   friend void lev_math::ChangeDimensions <> (int rows, int columns,
      Te_Matrix<D> *result,char zero);

   // recovery of formatted Save
   friend void lev_math::Recover <> (Te_Matrix<D> *A,char *filevector);
   // recovery of binary Save
   friend void lev_math::Recover <> (Te_Matrix<D> *A,char,char *filevector);
   friend void lev_math::Transpose <> (Te_Matrix<D> *A);
   friend char lev_math::Inverse <> (Te_Matrix<D> *A);  // Gauss Jordan
   friend void lev_math::SumRankOne <> (const Te_Vector<D> &u,
      const Te_Vector<D> &vT,Te_Matrix<D> *result);
   friend void lev_math::SumRankOne <> (double product,const Te_Vector<D> &u,
      const Te_Vector<D> &vT,Te_Matrix<D> *result);
   friend void lev_math::SumRankOne <> (const Te_Vector<D> &u,
      const Te_Vector<D> &vT,double divisor,Te_Matrix<D> *result);
   friend void lev_math::Swap <> (Te_Matrix<D> *lval,Te_Matrix<D> *rval);
   friend void lev_math::Solve <> (const Te_MatrixRight<D> &R,Te_Matrix<D> *BX);
   friend void lev_math::Solve <> (const Te_MatrixLeft<D> &R,Te_Matrix<D> *BX);
};


//   =====================< MATRIX.CPP >========================
//   * Matrix Class:                                           *
//   * for operations between matrices and vectors             *
//   * Description: Chapter 12                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================


template <class D> const char *const Te_Matrix<D>::ERRORMESSAGEMATRIX="\n========>>> Matrix error!!!!!!\n";
template <class D> int Te_Matrix<D>::count = 0;


//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   **********************< Initialize >***********************
//   * Purpose: Common function for initialisation             *
//   * Description: Chapter 12                                 *
//   * Example: Initialize(rows,columns)                       *
//   ***********************************************************
template <class D>
void Te_Matrix <D>::Initialize(int rows,int columns) {
   count++;
   whoAmI = count;
   if(rows < 1 || columns < 1) {
      numRows = numColumns = size = 0;
      matrix = NULL;
      return;
   }
   numRows = rows;
   numColumns = columns;
   size = numRows * numColumns + 1;
   matrix = new D *[numRows + 1];
   if(!matrix) {
      printf ("Erro na matriz\n");
      lev_math::utility::Error((char*)("%s%s%sInitialize"),
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_SPACE,utility::ERR_FUNCTION);
   }

   matrix[0]=new D[size];
   if(!matrix[0]) {
      printf ("Erro na matriz\n");
      lev_math::utility::Error((char*)("%s%s%sInitialize"),
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_SPACE,utility::ERR_FUNCTION);
   }
   matrix[1] = matrix[0];
   for(int i = 2;i <= numRows;i++)
      matrix[i] = matrix[i-1]+numColumns;
}


//   **********************< Deinitialize >*********************
//   * Purpose: Common function for deinitialisation           *
//   * Description: Chapter 12                                 *
//   * Example: Deinitialize();                                *
//   ***********************************************************
template <class D>
void Te_Matrix <D>::Deinitialize(void)
{
	if(matrix) {
		delete [] matrix[0]; matrix[0] = NULL;
		delete [] matrix; matrix = NULL;
	}
}


//   ***********************< PrepCopy >************************
//   * Purpose: Preparing an assignment                        *
//   ***********************************************************
template <class D>
void Te_Matrix <D>::PrepCopy(int rRows,int rColumns) {
   long int who = whoAmI;
   if(numRows != rRows || numColumns != rColumns) {
      Deinitialize();
      Initialize(rRows,rColumns);
      count--;
   }
   whoAmI = who;
}


//   *********************< CopyRight >*************************
//   * Purpose: Copying a MatrixRight                          *
//   ***********************************************************
template <class D>
void Te_Matrix <D>::CopyRight(const Te_MatrixRight<D> &rval) {
   if(numRows != 0) {
      for(int row = 1;row <= numRows;row++) {
         for(int col = 1;col <= numColumns;col++) {
            if(col < row)
               matrix[row][col]=0.;
            else
               matrix[row][col] = rval.matrix[row][col];
         }
      }
   }
}


//   *********************< CopyLeft >**************************
//   * Purpose: Copying a MatrixLeft                           *
//   ***********************************************************
template <class D>
void Te_Matrix <D>::CopyLeft(const Te_MatrixLeft<D> &rval) {
   if(numRows != 0) {
      for(int row = 1;row <= numRows;row++) {
         for(int col = 1;col <= numColumns;col++) {
            if(col > row)
               matrix[row][col] = 0.;
            else
               matrix[row][col] = rval.matrix[row][col];
         }
      }
   }
}


//   *********************< CopySymm >**************************
//   * Purpose: Copying a MatrixSymm                           *
//   ***********************************************************
template <class D>
void Te_Matrix <D>::CopySymm(const Te_MatrixSymm<D> &rval) {
   if(numRows != 0) {
      for(int row = 1;row <= numRows;row++) {
         for(int col = 1;col <= numColumns;col++) {
            if(col > row)
               matrix[row][col] = rval.matrix[col][row];
            else
               matrix[row][col] = rval.matrix[row][col];
         }
      }
   }
}

//   ****************< private constructor >********************
//   * Purpose: Set size of  Matrix without initialisation     *
//   *          Serving internally                             *
//   * Example: Matrix A('*',4,5);                             *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(char,int rows,int columns) {
   Initialize(rows,columns);
}


//   ===========================================================
//   ================== Public functions =======================
//   ===========================================================

//   ===========================================================
//   ******************   constructors   ***********************
//   ===========================================================

//   ***********************< default >*************************
//   * Purpose: Defining a Matrix which can then receive       *
//   *          an assignment                                  *
//   * Examples: Matrix A;                                     *
//   *           A = B;                                        *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(void) {
   Initialize(0,0);
}


//   ******************< Copy-Initialize >**********************
//   * Purpose: Defining and initialising from a Matrix        *
//   *          Used in returns                                *
//   * Examples: Matrix A=B;                                   *
//   *           Matrix A=B+2.*C;                              *
//   *           return A; (used implicitly)                   *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(const Te_Matrix <D> &rval) {
   Initialize(rval.numRows,rval.numColumns);
   if(numRows != 0)
      memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
}


//   **********************< sizing >***************************
//   * Purpose: Sizing a Matrix and initialising it to zero    *
//   *          Must then be assigned other values             *
//   * Example: Matrix A(4,5);                                 *
//   *          A(1,1)=3.; A(1,2)=7.;....                      *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(int rows,int columns) {
   Initialize(rows,columns);
   if(size != 0)
      memset(matrix[0],0,size*sizeof(D));
}


//   **************< sizing and initialising >******************
//   * Purpose: Initialising a Matrix                          *
//   * Example: Matrix A(2,3,1.,2.,3.,4.,5.,6.);               *
//   ***********************************************************
template <class D>
Te_Matrix<D>::Te_Matrix(int rows,int columns,D a11,...) {
   Initialize(rows,columns);
   D *w=matrix[0] + 1;
   va_list puntaList;
   va_start(puntaList,a11);
   int i;
   *w = utility::Control(a11);
   for(i = 2;i < size;i++)
      *++w = utility::Control(va_arg(puntaList,D));
   va_end(puntaList);
}


//   **********************< from array >***********************
//   * Purpose: Initialising a Matrix from an array            *
//   * Example: D w[]={1.,2.,3.,4.,5.,6.};                     *
//   *          Matrix A(2,3,w);                               *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(int rows,int columns,D *initvalues) {
   Initialize(rows,columns);
   D *w=matrix[0];
   ++w;
   if(numRows != 0)
      memcpy(w,initvalues,(size-1)*sizeof(D));
}


//   ********************< ASCII FILE >*************************
//   * Purpose: Initialising a Matrix from ASCII FILE          *
//   * Example: Matrix A("MAT.DAT");                           *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"r"))==NULL)
       lev_math::utility::Error("%s%s%s%sFILE",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_CONSTRUCTOR);
   int rows,columns;
   fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
   if(rows < 1 || columns < 1)
       lev_math::utility::Error("%s%s%sFILE",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   D *w = matrix[0];
   for(int i = 1;i < size;i++)fscanf(utility::bzzFileIn,"%x",++w);
   fclose(utility::bzzFileIn);
}


//   ********************< from binary FILE >*******************
//   * Purpose: Initialising a Matrix from binary FILE         *
//   *          obtained with Save('*',file name)              *
//   * Example: Matrix A('*',"MAT.BIN");                       *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(char,char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"rb"))==NULL)
       lev_math::utility::Error("%s%s%s%sFILE",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_CONSTRUCTOR);
   int rows,columns;
   if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%s%s%sFILE",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_READING_FILE,utility::ERR_CONSTRUCTOR);
   if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%s%s%sFILE",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_READING_FILE,utility::ERR_CONSTRUCTOR);
   if(rows < 1 || columns < 1)
       lev_math::utility::Error("%s%s%sFILE",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   if(fread(matrix[1],sizeof(double)*size,1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%s%s%sFILE",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_READING_FILE,utility::ERR_CONSTRUCTOR);
   fclose(utility::bzzFileIn);
}


//   ********************<  from submatrix >********************
//   * Purpose: From a submatrix with identical beginning      *
//   * Example: Matrix B(4,5,A);                               *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(int rows,int columns,const Te_Matrix <D> &rval) {
   if(rows < 1 || rows > rval.numRows)
      lev_math::utility::Error("%s%s%sSubMatrix",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   if(columns < 1 || columns > rval.numColumns)
       lev_math::utility::Error("%s%s%sSubMatrix",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   for(int row=1;row<=numRows;row++)
      memcpy(matrix[row]+1,rval.matrix[row]+1,
      numColumns*sizeof(D));
}

//   *****************< from internal submatrix >***************
//   * Purpose: From internal submatrix                        *
//   * Example: Matrix B(m,n,i,j,A);      B(m,n) from i,j      *
//   ***********************************************************
template <class D>
Te_Matrix <D>::Te_Matrix(int rows,int columns,int irow,int jcol,
   const Te_Matrix <D> &rval) {
   if(rows < 1 || irow < 1 || irow>rval.numRows || rows>(rval.numRows-irow+1))
       lev_math::utility::Error("%s%s%sSubMatrix",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_CONSTRUCTOR);
   if(columns < 1 || jcol < 1 || jcol>rval.numColumns ||
       columns>(rval.numColumns-jcol+1))
       lev_math::utility::Error("%s%s%sSubMatrix",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   for(int row=1;row<=numRows;row++)
       memcpy(matrix[row]+1,rval.matrix[row+irow-1]+jcol, numColumns*sizeof(D));
}


//   *********************< from Vector >***********************
//   * Purpose: Vector constructor                             *
//   * Example: Matrix A(y);                                   *
//   *  else:   Matrix A = y;                                  *
//   ***********************************************************
template <class D>
Te_Matrix<D>::Te_Matrix(const Te_Vector<D> &rval) {
   Initialize(rval.dimensions,1);
   if(numRows != 0)
      memcpy(matrix[0],rval.vector,size*sizeof(D));
}


//   **********************< from Right >***********************
//   * Purpose: From MatrixRight                               *
//   * Example: Matrix A(R);                                   *
//   *  else:   Matrix A = R;                                  *
//   ***********************************************************
template <class D>
Te_Matrix<D>::Te_Matrix(const Te_MatrixRight<D> &rval) {
   Initialize(rval.numRows,rval.numColumns);
   CopyRight(rval);
}


//   ************************< from Left >**********************
//   * Purpose: From MatrixLeft                                *
//   * Example: Matrix A(L);                                   *
//   *  else:   Matrix A = L;                                  *
//   ***********************************************************
template <class D>
Te_Matrix<D>::Te_Matrix(const Te_MatrixLeft<D> &rval) {
   Initialize(rval.numRows,rval.numColumns);
   CopyLeft(rval);
}


//   ************************< from Symm >**********************
//   * Purpose: From MatrixSymm                                *
//   * Example: Matrix A(S);                                   *
//   *  else:   Matrix A = S;                                  *
//   ***********************************************************
template <class D>
Te_Matrix<D>::Te_Matrix(const Te_MatrixSymm<D> &rval) {
   Initialize(rval.numRows,rval.numColumns);
   CopySymm(rval);
}


//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================
template <class D>
Te_Matrix<D>::~Te_Matrix(void) {
   Deinitialize();
}


//   ===========================================================
//   **********  Non-modifying access functions    *************
//   ===========================================================

//   *********************< GetValue >**************************
//   * Purpose: Receiving values with control                  *
//   * Description: Must be used when the Matrix               *
//   *              is defined const                           *
//   * Example: double x = A.GetValue(3,5);  // A const         *
//   ***********************************************************
template <class D>
D Te_Matrix<D>::GetValue(int row,int col) const
{
	if( (row < 1 || row > numRows) || (col < 1 || col > numColumns) )
		lev_math::utility::Error((char*)"%s%s%sGetValue", Te_Matrix<D>::ERRORMESSAGEMATRIX, utility::ERR_RANGE, utility::ERR_FUNCTION);
	return matrix[row][col];
}


//   **********************< GetRow >***************************
//   * Purpose: Returning a row of a Matrix                    *
//   * Description: Places the row in a Vector                 *
//   * Example: Vector x = A.GetRow(3);                        *
//   ***********************************************************
template <class D>
Te_Vector<D> Te_Matrix<D>::GetRow(int i) const
{
	if(i < 1 || i > numRows)
		lev_math::utility::Error((char*)("%s%s%sGetRow"), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_FUNCTION);
	Te_Vector<D> result('*',numColumns);
	memcpy(result.vector+1,matrix[i]+1, numColumns*sizeof(D));
	return result;
}


//   *********************< GetColumn >*************************
//   * Purpose: Returning a column of a Matrix                 *
//   * Description: Places the column in a Vector              *
//   * Example: Vector v = GetColumn(j);                       *
//   ***********************************************************
template <class D>
Te_Vector<D> Te_Matrix<D>::GetColumn(int j) const {
   if(j < 1 || j > numColumns)
      lev_math::utility::Error((char*)("%s%s%sGetColumn"),
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_FUNCTION);
   Te_Vector<D> result('*',numRows);
   D *r = result.vector;
   for(int k = 1;k <= numRows;k++)
       *++r = matrix[k][j];
   return result;
}


//   ===========================================================
//   **************    Modifying access functions **************
//   ===========================================================

//   ********************< operator () >************************
//   * Purpose: Receiving and assigning values with control    *
//   * Description: As with A[i][j] but with control           *
//   * Example: x = A(1,5); A(3,7) = 5.;                       *
//   ***********************************************************
template <class D>
D &Te_Matrix<D>::operator () (int row,int col) {
   if( (row < 1 || row > numRows) || (col < 1 || col > numColumns) )
      lev_math::utility::Error((char*)("%s%s%s()"), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_OPERATOR);
   return matrix[row][col];
}


//   *********************< SetValue >**************************
//   * Purpose: Assigning values with control                  *
//   * Example:  A.SetValue(3,4,5.);                           *
//   ***********************************************************
template <class D>
void Te_Matrix<D>::SetValue(int row,int col,D val) {
   if( (row < 1 || row > numRows) || (col < 1 || col > numColumns) )
     lev_math::utility::Error((char*)("%s%s%sSetValue"), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_FUNCTION);
   matrix[row][col] = val;
}


//   **********************< SetRow >***************************
//   * Purpose: Setting a row of a matrix equal to a Vector    *
//   * Description: Uses memcpy                                *
//   * Example: A.SetRow(3,v);                                 *
//   ***********************************************************
template <class D>
void Te_Matrix<D>::SetRow(int i,const Te_Vector<D> &rval)
{
	if( (i < 1 || i > numRows) || (rval.dimensions != numColumns ) )
		lev_math::utility::Error((char*)"%s%s%sSetRow", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_FUNCTION);
	memcpy(matrix[i]+1,rval.vector+1, numColumns*sizeof(D));
}


//   *********************< SetColumn >*************************
//   * Purpose: Setting a column of a matrix equal to a Vector *
//   * Example: A.SetColumn(2,v);                              *
//   ***********************************************************
template <class D>
void Te_Matrix<D>::SetColumn(int j,const Te_Vector<D> &rval)
{
	if( (j < 1 || j > numColumns) || (rval.dimensions != numRows) )
		lev_math::utility::Error((char*)"%s%s%sSetColumn", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_RANGE,utility::ERR_FUNCTION);
	D *r = rval.vector;
	for(int k = 1;k <= numRows;k++)
		matrix[k][j] = *++r;
}


//   ===========================================================
//   ***************   assignment operators ********************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix                         *
//   * Description: Uses memcpy                                *
//   * Example: A=B;      // simple                            *
//   *          A=C+D;    // with operations                   *
//   *          A=B=C+D;  // multiple                          *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator =(const Te_Matrix<D> &rval) {
   PrepCopy(rval.numRows,rval.numColumns);
   if(numRows != 0)
      memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
   return *this;
}


//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix from Vector             *
//   * Description: Uses memcpy                                *
//   * Example: A = v;         // simple                       *
//   *          A = 2.*v;      //  with operations             *
//   *          A = B = 2.*v;  // multiple                     *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator =(const Te_Vector<D> &rval) {
   PrepCopy(rval.dimensions,1);
   if(numRows != 0)
      memcpy(matrix[0],rval.vector,size*sizeof(D));
   return *this;
}


//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix from MatrixRight        *
//   * Example: A = R;          // simple                      *
//   *          A = 2.*R;       // with operations             *
//   *          A = B = 2.*R;   // multiple                    *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator =(const Te_MatrixRight<D> &rval) {
   PrepCopy(rval.numRows,rval.numColumns);
   CopyRight(rval);
   return *this;
}


//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix from MatrixLeft         *
//   * Example: A = L;         // simple                       *
//   *          A = 2.*L;      // with operations              *
//   *          A = B = 2.*L;  // multiple                     *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator =(const Te_MatrixLeft<D> &rval) {
   PrepCopy(rval.numRows,rval.numColumns);
   CopyLeft(rval);
   return *this;
}


//   *************************< = >*****************************
//   * Purpose: Assignment of a Matrix from MatrixSymm         *
//   * Example: A = S;          // simple                      *
//   *          A = 2.*S;       // with operations             *
//   *          A = B = 2.*S;   // multiple                    *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator =(const Te_MatrixSymm<D> &rval) {
   PrepCopy(rval.numRows,rval.numColumns);
   CopySymm(rval);
   return *this;
}


//   ===========================================================
//   **********   operators for composing matrices   ***********
//   ===========================================================

//   ************************< && >*****************************
//   * Purpose: Constructs one Matrix from two Matrices        *
//   *          inserting the second matrix beneath the first  *
//   *          They must have the same number of columns      *
//   * Description: Uses memcpy                                *
//   * Example: A = B&&C;                                      *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator && (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   if(lval.numColumns != rval.numColumns)
       lev_math::utility::Error("%s%s%s&&", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_Matrix<D> result('*',lval.numRows+rval.numRows, lval.numColumns);
   if(lval.numRows != 0) {
       memcpy(result.matrix[0],lval.matrix[0], lval.size*sizeof(D));
      D *r = &result[lval.numRows][lval.numColumns] + 1;
      memcpy(r,rval.matrix[0]+1, (rval.size-1)*sizeof(D));
   }
   return result;
}


//   ************************< || >*****************************
//   * Purpose: Constructs one Matrix from two Matrices        *
//   *          inserting the second matrix to the side        *
//   *          of the first                                   *
//   *          They must have the same number of rows         *
//   * Description: Uses memcpy                                *
//   * Example: A = B||C;                                      *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator || (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   if(lval.numRows != rval.numRows)
      lev_math::utility::Error("%s%s%s||", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_Matrix<D> result('*',lval.numRows, lval.numColumns+rval.numColumns);
   D **s=lval.matrix;
   D **d=rval.matrix;
   D **r=result.matrix;
   if(result.numRows != 0) {
      for(int i=1;i<=lval.numRows;i++) {
         memcpy(r[i]+1,s[i]+1, (lval.numColumns)*sizeof(D));
         memcpy(r[i]+lval.numColumns+1,d[i]+1, (rval.numColumns)*sizeof(D));
      }
   }
   return result;
}


//   ===========================================================
//   ******************   test operators ***********************
//   ===========================================================

//   ************************< == >*****************************
//   * Purpose: If it is == return 1 otherwise 0               *
//   * Example: if( A == B )                                   *
//   ***********************************************************
template <class D>
char operator == (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   if(lval.whoAmI == rval.whoAmI) return 1;
   char ch = 1;
   if(   lval.numRows != rval.numRows || lval.numColumns != rval.numColumns) ch = 0;
   else {
      if(memcmp(lval.matrix[0]+1,rval.matrix[0]+1, (rval.size-1)*sizeof(D)) == 0) ch = 1;
      else ch = 0;
   }
   return ch;
}


//   ************************< != >*****************************
//   * Purpose: If it is != return 1 otherwise 0               *
//   * Example: if( A != B)                                    *
//   ***********************************************************
template <class D>
char operator != (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   if(lval.whoAmI == rval.whoAmI) return 0;
   char ch=0;
   if(lval.numRows != rval.numRows || lval.numColumns != rval.numColumns) ch = 1;
   else ch = memcmp(lval.matrix[0]+1,rval.matrix[0]+1, (rval.size-1)*sizeof(D));
   return ch;
}


//   ===========================================================
//   ====================   OPERATIONS   =======================
//   ===========================================================

//   ===========================================================
//   ***********************   Sum   ***************************
//   ===========================================================

//   ************************< Sum >****************************
//   * Purpose: Addition between matrices                      *
//   * Description: Uses Sum from utility.cpp                  *
//   * Example: Sum(A,B,&C); C = A + B;                        *
//   ***********************************************************
template <class D>
void Sum(const Te_Matrix<D> &lval,const Te_Matrix<D> &rval, Te_Matrix<D> *result) {
   if(lval.numColumns != rval.numColumns || lval.numRows != rval.numRows)
   { //obs: 05/07/2006 *NMA* *AP* *GNM*: estavam faltando os delimitadores do bloco if, isto deve ser mantido
      printf ("Erro na matriz\n");
      lev_math::utility::Error((char*)"%s%s%sSum", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
	}

   if(result->whoAmI == lval.whoAmI)
      (*result) += rval;
   else if(result->whoAmI == rval.whoAmI)
	   Sum(lval,result);
   else {
      ChangeDimensions(lval.numRows,lval.numColumns,result,1);
      utility::Sum(rval.size-1,lval.matrix[1]+1,rval.matrix[1]+1, result->matrix[1]+1);
   }
}


//   *************************< + >*****************************
//   * Purpose: Addition of matrices                           *
//   * Example: A = B + C;                                     *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator + (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   Te_Matrix<D> result(lval.Rows(),lval.Columns()); //obs: 05/07/2006 *NMA* *AP* *GNM*: foi substituido construtor e o acesso ao numero de linhas e colunas (problema na compilacao), isto deve ser mantido
   Sum(lval,rval,&result);
   return result;
}


//   ************************< Sum >****************************
//   * Purpose: Addition of matrices                           *
//   * Description: The result goes in the left matrix         *
//   * Example: Sum(&A,B); A += B;                             *
//   ***********************************************************
template <class D>
void Sum(Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval) {
   (*lvalAndResult) += rval;
}


//   *************************< += >****************************
//   * Purpose: Adds a matrix which is also on the right       *
//   * Description: Uses Sum from utility.cpp                  *
//   * Example: A += B; // A = A + B;                          *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator += (const Te_Matrix<D> &rval) {
   if(numColumns != rval.numColumns || numRows != rval.numRows)
	   lev_math::utility::Error((char*)("%s%s%s+="), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(whoAmI == rval.whoAmI)
      Sum(this);
   else
	  utility::Sum(rval.size-1,matrix[1]+1,rval.matrix[1]+1);
   return *this;
}


//   ************************< Sum >****************************
//   * Purpose: Addition between matrices                      *
//   * Description: The result goes in the right matrix        *
//   * Example: Sum(B,&A); A = B + A;                          *
//   ***********************************************************
template <class D>
void Sum(const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult) {
   (*rvalAndResult) += lval;
}


//   ************************< Sum >****************************
//   * Purpose: Addition of two identical matrices             *
//   * Description: The result replaces the matrix             *
//   * Example: Sum(&A); A = A + A;                            *
//   ***********************************************************
template <class D>
void Sum(Te_Matrix<D> *lvalRvalAndResult) {
	utility::Sum(lvalRvalAndResult->size-1, lvalRvalAndResult->matrix[1]+1);
}


//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================

//   **********************< Difference >***********************
//   * Purpose: Difference between matrices                    *
//   * Description: Uses Difference from utility.cpp           *
//   * Example: Difference(B,C,&A); A = B - C;                 *
//   ***********************************************************
template <class D>
void Difference(const Te_Matrix<D> &lval,const Te_Matrix<D> &rval, Te_Matrix<D> *result) {
   if(lval.numColumns != rval.numColumns || lval.numRows != rval.numRows)
       lev_math::utility::Error("%s%s%sSum", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   if(result->whoAmI == lval.whoAmI)
       (*result) -= rval;
   else if(result->whoAmI == rval.whoAmI)
       Difference(lval,result);
   else {
       ChangeDimensions(lval.numRows,lval.numColumns,result,1);
       Difference(rval.size-1,lval.matrix[1]+1, rval.matrix[1]+1,result->matrix[1]+1);
   }
}


//   *************************< - >*****************************
//   * Purpose: Difference between matrices                    *
//   * Description: Uses Difference                            *
//   * Example: A = B - C;                                     *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator - (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   Te_Matrix<D> result(lval.Rows(),lval.Columns()); //obs: 05/07/2006 *NMA* *AP* *GNM*: foi substituido construtor e o acesso ao numero de linhas e colunas (problema na compilacao), isto deve ser mantido
   Difference(lval,rval,&result);
   return result;
}


//   **********************< Difference >***********************
//   * Purpose: Difference between matrices                    *
//   * Description: The result goes in the left matrix         *
//   * Example: Difference(&A,B); A = A - B;                   *
//   ***********************************************************
template <class D>
void Difference(Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval) {
   (*lvalAndResult) -= rval;
}


//   *************************< -= >****************************
//   * Purpose: Difference from a matrix also on the right     *
//   * Description: Uses Sum from utility.cpp                  *
//   * Example: A -= B; // A = A - B;                          *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator -= (const Te_Matrix<D> &rval) {
   if(numColumns != rval.numColumns || numRows != rval.numRows)
      lev_math::utility::Error("%s%s%s+=", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(whoAmI == rval.whoAmI)
      Difference(this);
   else
      Difference(rval.size-1,matrix[1]+1,rval.matrix[1]+1);
   return *this;
}


//   **********************< Difference >***********************
//   * Purpose: Difference between matrices                    *
//   * Description: The result goes in the right matrix        *
//   * Example: Difference(A,&B); B= A - B;                    *
//   ***********************************************************
template <class D>
void Difference(const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult) {
   if(lval.numColumns != rvalAndResult->numColumns || lval.numRows != rvalAndResult->numRows)
      lev_math::utility::Error("%s%s%sDiffernece", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   Difference(lval.size-1,lval.matrix[1]+1, rvalAndResult->matrix[1]+1,1);
}


//   **********************< Difference >***********************
//   * Purpose: Difference between identical matrices          *
//   * Description:  Null matrix                               *
//   * Example: Difference(&A); A = A - A;                     *
//   ***********************************************************
template <class D>
void Difference(Te_Matrix<D> *lvalRvalAndResult) {
   D *w = lvalRvalAndResult->matrix[0];
   for(int i = 1;i < lvalRvalAndResult->size;i++)
       *++w = 0;
}


//   ===========================================================
//   ***********************   Minus   *************************
//   ===========================================================

//   ************************< Minus >**************************
//   * Purpose: Unary minus                                    *
//   * Example: Minus(A,&B); B = -A;                           *
//   ***********************************************************
template <class D>
void Minus (const Te_Matrix<D> &rval,Te_Matrix<D> *result) {
   ChangeDimensions(rval.numRows,rval.numColumns,result,1);
   D *w=rval.matrix[1];
   D *r=result->matrix[1];
   for(int i = 1;i < result->size;i++)
   r[i] = -w[i];
}


//   *************************< - >*****************************
//   * Purpose: Unary minus                                    *
//   * Example: A = -B;                                        *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator -(const Te_Matrix<D> &rval) { // unary minus
   Te_Matrix<D> result('*',rval.numRows,rval.numColumns);
   Minus(rval,&result);
   return result;
}


//   ************************< Minus >**************************
//   * Purpose: Unary minus                                    *
//   * Example: Minus(&A); A = -A;                             *
//   ***********************************************************
template <class D>
void Minus (Te_Matrix<D> *rvalAndResult) {
   D *w=rvalAndResult->matrix[1];
   for(int i = 1;i < rvalAndResult->size;i++)
   w[i] = -w[i];
}


//   ===========================================================
//   **********************   Product   ************************
//   ===========================================================

//   ***********************< Product >*************************
//   * Purpose: Product between matrices                       *
//   * Description: Pointers are used to eliminate the         *
//                  use of double indices                      *
//   * Example: Product(A,B,&C); C = A * B;                    *
//   ***********************************************************
template <class D>
void Product(const Te_Matrix<D> &lval,const Te_Matrix<D> &rval, Te_Matrix<D> *result) {
   if(lval.Columns() != rval.Rows())
	   lev_math::utility::Error((char*)("%s%s%sProduct"), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(result->whoAmI == lval.whoAmI)
       (*result) *= rval;
   else if(result->whoAmI == rval.whoAmI)
       Product(lval,result);
   else {
      ChangeDimensions(lval.Rows(),rval.Columns(), result,1);
      D *v = result->matrix[1];
      D *b;
      int nc = rval.numColumns;
      for(int row = 1;row <= lval.numRows;row++) {
         for(int column = 1;column <= rval.numColumns; column++) {
            b = rval.matrix[1] + column;
            D *w = lval.matrix[row];
            // double sum=0.;
            D sum=0;
            for(int i=1;i<=lval.numColumns;i++) {
               sum += *++w * (*b);
               b += nc;
            }
            *++v = utility::Control(sum);
         }
      }
   }
}


//   *************************< * >*****************************
//   * Purpose: Product between matrices                       *
//   * Description: Uses Product(A,B,&C)                       *
//   * Example: C = A * B;                                     *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator * (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   Te_Matrix<D> result(lval.Rows(),rval.Columns());
   Product(lval,rval,&result);
   return result;
}


//   ***********************< Product >*************************
//   * Purpose: Product between matrices                       *
//   * Description: The result goes in the left matrix         *
//   * Example:  Product(&A,B); A *= B;                        *
//   ***********************************************************
template <class D>
void Product(Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval) {
   (*lvalAndResult) *= rval;
}


//   ************************< *= >*****************************
//   * Purpose: The product of one matrix with another,        *
//   *          stored replacing the first                     *
//   * Description: Always favour this alternative             *
//   *               when possible!                            *
//   * Example:  A *= B; // equivalent to  A = A * B           *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator *= (const Te_Matrix<D> &rval) {
   if(numColumns != rval.numRows)
	   lev_math::utility::Error((char*)("%s%s%s*="), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(whoAmI == rval.whoAmI)
      Product(this); // A = A*A;
   else {
      // if rval is not square *this changes dimensions
      if(rval.numRows == rval.numColumns) {
         D *b;
         int nc = rval.numColumns;
         D *v = matrix[1];
         D *w = new D[numColumns+1];
         for(int row = 1;row <= numRows;row++) {
            memcpy(w,matrix[row], (numColumns+1)*sizeof(D));
            for(int column = 1;column <= numColumns; column++) {
               b = rval.matrix[1] + column;
               // double sum=0.;
               D sum=0;
               for(int i=1;i<=numColumns;i++) {
                  sum += w[i] * (*b);
                  b += nc;
               }
               *++v = utility::Control(sum);
            }
         }
         delete [] w;
      }
      else {
         Te_Matrix result;
         Product(*this,rval,&result);
         Swap(this,&result);// avoids copy
      }
   }
   return *this;
}


//   ***********************< Product >*************************
//   * Purpose: Product between matrices                       *
//   * Description: The result goes in the right matrix        *
//   * Example: Product(B,&A); A = B * A;                      *
//   ***********************************************************
template <class D>
void Product(const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult) {
   Te_Matrix<D> C;
   Product(lval,*rvalAndResult,&C);
   Swap(rvalAndResult,&C);
}


//   ***********************< Product >*************************
//   * Purpose: Product between identical matrices             *
//   * Description: The result replaces the matrix             *
//   * Example: Product(&A); A = A * A;                        *
//   ***********************************************************
template <class D>
void Product(Te_Matrix<D> *lvalRvalAndResult) {
   Te_Matrix<D> C;
   Product(*lvalRvalAndResult,*lvalRvalAndResult,&C);
   Swap(lvalRvalAndResult,&C);
}


//   ***********************< Product >*************************
//   * Purpose: Product of a double with a Matrix              *
//   * Description: Uses Product from utility.cpp              *
//   * Example: A = 3.5 * B;                                   *
//   ***********************************************************
template <class D>
void Product(D lval,const Te_Matrix<D> &rval,Te_Matrix<D> *result) {
   if(rval.whoAmI == result->whoAmI)
      utility::Product(result->size - 1,lval,result->matrix[1]+1);
   else {
      ChangeDimensions(rval.numRows,rval.numColumns, result,1);
      utility::Product(rval.size - 1,lval,rval.matrix[1]+1, result->matrix[1]+1);
   }
}


//   *************************< * >*****************************
//   * Purpose: Product of a double with a Matrix              *
//   * Description: Uses Product from utility.cpp              *
//   * Example: A = 3.5 * B;                                   *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator * (D lval,const Te_Matrix<D> &rval) {
	//int f = rval.Rows();
   Te_Matrix<D> result(rval.Rows(),rval.Columns());
   Product(lval,rval,&result);
   return result;
}


//   ***********************< Product >*************************
//   * Purpose: Product of a double with a Matrix              *
//   * Description: Uses Product from utility.cpp              *
//   * Example: Product(&A,3.); A = 3.* A;                     *
//   ***********************************************************
template <class D>
void Product(D lval,Te_Matrix<D> *rvalAndResult) {
   Product(rvalAndResult->size - 1,lval,
     rvalAndResult->matrix[1]+1);
}


//   ************************< *= >*****************************
//   * Purpose: Multiplies and modifies a Matrix by a double   *
//   * Example: A *= 3.; // A =  3.*A;                         *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator *= (D lval) {
   utility::Product(size - 1,lval,matrix[1] + 1);//in UTILITY.CPP
   return *this;
}


//   ===========================================================
//   *********************   TProduct   ************************
//   ===========================================================

//   **********************< TProduct >*************************
//   * Purpose: Product of a transposed matrix with a          *
//   *          normal matrix                                  *
//   * Example: TProduct(A,B,&C);  // C = ATB                  *
//   ***********************************************************
template <class D>
void TProduct(const Te_Matrix<D> &lval,const Te_Matrix<D> &rval, Te_Matrix<D> *result) {
   if(lval.numRows!=rval.numRows)
       lev_math::utility::Error("%s%s%s %", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(result->whoAmI == lval.whoAmI)
      (*result) %= rval;
   else if(result->whoAmI == rval.whoAmI)
      TProduct(lval,result);
   else {
      ChangeDimensions(lval.numColumns, rval.numColumns,result,1);
      if(lval.whoAmI != rval.whoAmI) {
         D *p,*b;
         int np = lval.numColumns;
         int nc = rval.numColumns;
         D *v = result->matrix[1];
         for(int cc = 1;cc <= lval.numColumns;cc++) {
            for(int column = 1;column <= rval.numColumns;column++) {
               D sum = 0;
               // double sum=0.;
               p = lval.matrix[1] + cc;
               b = rval.matrix[1] + column;
               for(int i = 1;i <= lval.numRows;i++) {
                  sum += (*p) * (*b);
                  p += np;
                  b += nc;
               }
               *++v = utility::Control(sum);
            }
         }
      }
      else { // identical matrices ATA
         int n = lval.numColumns;
         int m = lval.numRows;
         int i,j,k;
         D **a = lval.matrix;
         D **aTa = result->matrix;
         for(i = 1;i <=n;i++) {
            for(j = i;j <= n;j++) {
               D sum = 0;
               // double sum = 0.;
               for(k = 1;k <= m;k++)
                  sum += a[k][i]*a[k][j];
               aTa[i][j] = utility::Control(sum);
            }
         }
         for(i = 1;i <= n;i++)
            for(j = 1;j < i;j++)
               aTa[i][j] = aTa[j][i];
      }
   }
}


//   *************************< % >*****************************
//   * Purpose: Product of a transposed matrix with a          *
//   *          normal matrix                                  *
//   * Example: C = A % B; // C = ATB                          *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator % (const Te_Matrix<D> &lval,const Te_Matrix<D> &rval) {
   Te_Matrix<D> result('*',lval.numColumns,rval.numColumns);
   TProduct(lval,rval,&result);
   return result;
}


//   **********************< TProduct >*************************
//   * Purpose: Executes A = ATB                               *
//   * Description: The result goes in the left matrix         *
//   * Example: TProduct(&A,B); A %= B;                        *
//   ***********************************************************
template <class D>
void TProduct(Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval) {
   (*lvalAndResult) %= rval;
}


//   ************************< %= >*****************************
//   * Purpose: Executes A = ATB                               *
//   * Example: A %= B; A %= A;                                *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator %=(const Te_Matrix<D> &rval) {
   if(numRows != rval.numRows)
       lev_math::utility::Error("%s%s %=", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(*this == rval) TProduct(this);  // A=ATA
   else if(numRows == numColumns && rval.numRows == rval.numColumns) { // to optimise if square
      Te_Matrix<D> result;
      TProduct(*this,rval,&result);
      Swap(this,&result);
   } else { // general case
      Te_Matrix<D> result;
      TProduct(*this,rval,&result);
      Swap(this,&result);
   }
   return *this;
}


//   ***********************< TProduct >************************
//   * Purpose: Calculates A = BTA                             *
//   * Example: TProduc(B,&A); A = BTA;                        *
//   ***********************************************************
template <class D>
void TProduct(const Te_Matrix<D> &lval,Te_Matrix<D> *rvalAndResult) {
   Te_Matrix<D> C;
   TProduct(lval,*rvalAndResult,&C);
   Swap(rvalAndResult,&C);
}


//   **********************< TProduct >*************************
//   * Purpose: Calculates ATA and puts it in A                *
//   * Example: TProduct(&A);                                  *
//   ***********************************************************
template <class D>
void TProduct(Te_Matrix<D> *lvalRvalAndResult) {
   if(lvalRvalAndResult->numRows ==  lvalRvalAndResult->numColumns) { // square matrices
      int i,j,k,n = lvalRvalAndResult->numRows;
      Te_Vector<D> s(n);
      D **aTa = lvalRvalAndResult->matrix;
      for(j = 1;j <= n;j++) {
         for(i = j;i <= n;i++) {
            D sum = 0;
            // double sum = 0.;
            for(k = 1;k <= n;k++)
               sum += aTa[k][i]*aTa[k][j];
            s[i] = utility::Control(sum);
         }
         for(i = j;i <= n;i++) aTa[i][j] = s[i];
      }
      for(j = 1;j <= n;j++)
         for(i = j+1;i <= n;i++)
            aTa[j][i] = aTa[i][j];
   } else { // rectangular matrices
      Te_Matrix<D> result;
      TProduct(*lvalRvalAndResult,*lvalRvalAndResult, &result);
      Swap(lvalRvalAndResult,&result);
   }
}


//   ===========================================================
//   *********************   ProductT   ************************
//   ===========================================================

//   **********************< ProductT *>************************
//   * Purpose: Executes AATand puts it in A                   *
//   * Description: Symmetrical matrix                         *
//   * Example: ProductT(&A); A = AAT;                         *
//   ***********************************************************
template <class D>
void ProductT (Te_Matrix<D> *lvalRvalAndResult) {
   if(lvalRvalAndResult->numRows ==  lvalRvalAndResult->numColumns) { // square matrices
      int i,j,k,n = lvalRvalAndResult->numRows;
      Te_Vector<D> s(n);
      D **a = lvalRvalAndResult->matrix;
      for(i = 1;i <= n;i++) {
         for(j = i;j <= n;j++) {
            D sum = 0;
            // double sum = 0.;
            D *ai = a[i];
            D *aj = a[j];
            for(k = 1;k <= n;k++) sum += (*++ai) * (*++aj);
            s[j] = utility::Control(sum);
         }
         memcpy(a[i]+i,&s[0]+i,(n-i+1)*sizeof(D));
      }
      for(i = 1;i <= n;i++)
         for(j = i+1;j <= n;j++) a[j][i] = a[i][j];
   } else { // rectangular matrices
      int n = lvalRvalAndResult->numRows;
      int m = lvalRvalAndResult->numColumns;
      int i,j,k;
      Te_Matrix<D> result('*',n,n);
      D **a = lvalRvalAndResult->matrix;
      D **aaT = result.matrix;
      for(i = 1;i <= n;i++) {
         for(j = i;j <= n;j++) {
            D sum = 0;
            // double sum = 0.;
            D *ai = a[i];
            D *aj = a[j];
            for(k = 1;k <= m;k++) sum += (*++ai) * (*++aj);
            aaT[i][j] = utility::Control(sum);
         }
      }
      for(i = 1;i <= n;i++)
         for(j = 1;j < i;j++)
            aaT[i][j] = aaT[j][i];
      Swap(lvalRvalAndResult,&result);
   }
}


//   ===========================================================
//   **********************   Division   ***********************
//   ===========================================================

//   ***********************< Division >************************
//   * Purpose: Dividing the coefficients of a matrix          *
//   *          by a double                                    *
//   * Description: Uses Division from utility.cpp             *
//   * Example: Division(A,3.,&B); B = A/3.;                   *
//   ***********************************************************
template <class D>
void Division(const Te_Matrix<D> &lval,double rval,Te_Matrix<D> *result) {
   if(lval.whoAmI == result->whoAmI)
	   utility::Division(result->size - 1,result->matrix[1]+1,rval);

   else {
      ChangeDimensions(lval.numRows,lval.numColumns, result,1);
      utility::Division(lval.size - 1,lval.matrix[1]+1, rval,result->matrix[1]+1);
   }
}


//   *************************< / >*****************************
//   * Purpose: Dividing the coefficients of a matrix          *
//   *          by a double                                    *
//   * Description: Uses Division from utility.cpp             *
//   * Example: A = B/3.;                                      *
//   ***********************************************************
template <class D>
Te_Matrix<D> operator / (const Te_Matrix<D> &lval,double rval) {
   Te_Matrix<D> result(lval.Rows(),lval.Columns()); //obs: 05/07/2006 *NMA* *AP* *GNM*: foi substituido construtor e o acesso ao número de linhas e colunas (problema na compilação), isto deve ser mantido
   Division(lval,rval,&result);
   return result;
}


//   ***********************< Division >************************
//   * Purpose: Dividing the coefficients of a matrix          *
//   *          by a double. Nao deve ser usada para D = int    *
//   * Description: Uses Division from utility.cpp             *
//   * Example: Division(&A,3.); A = A/3.;                     *
//   ***********************************************************
template <class D>
void Division(Te_Matrix<D> *lvalAndResult,double rval) {
   Division(lvalAndResult->size - 1, lvalAndResult->matrix[1]+1,rval);
}


//   ************************< /= >*****************************
//   * Purpose: Dividing and modifying a Matrix by a double     *
//	 * 			Nao deve ser usada para D = int.			   *
//   * Description: Uses Division from utility.cpp             *
//   * Example: A /= 2.; // A = A/2.;                          *
//   ***********************************************************
template <class D>
Te_Matrix<D> &Te_Matrix<D>::operator /= (double rval) {
   utility::Division(size - 1,matrix[1] + 1,rval);//in UTILITY.CPP
   return *this;
}


//   ===========================================================
//   *              Non-modifying functions                    *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Printing out the elements of a matrix          *
//   * Description: Uses the file utility::bzzFileOut global            *
//   *              in utility.hpp (Chapter 8)                 *
//   * Example: A.Print("Matrix A");                           *
//   ***********************************************************
template <class D>
void Te_Matrix<D>::Print(char *msg) {
   fprintf(utility::bzzFileOut,"\nMatrix No.%d",whoAmI);
   if(*msg) fprintf(utility::bzzFileOut,"  %s",msg);
   fprintf(utility::bzzFileOut,"\nrows %d columns %d\n", numRows,numColumns);
   for(int row = 1;row <= numRows;row++) {
      for(int column = 1;column <= numColumns;column++)
         fprintf(utility::bzzFileOut," %12.4e",matrix[row][column]);
      fprintf(utility::bzzFileOut,"\n");
   }
}

//   ************************< Save >***************************
//	 *NMA*:23/01/2007: inclui matriz em arquivo (append)	   *
//   ************************< SaveAppend >*********************
//   * Purpose: Saving  a Matrix on formatted file             *
//   * op=0-> "w" (write) op=1->"a" (append) ex: Save("elemento.dat",1)
//   * op == 0 is default                                      *
//   * Description: Can be read if op == 0 is used.            *
//   *              See TempFile in utility                    *
//   * Example: A.Save("Mat.Dat", 1);                          *
//   ***********************************************************

template <class D>
void Te_Matrix<D>::Save(char *filematrix, int mode) { // formatted
   FILE *bzz;
   if (mode)
   {
	   if((bzz = fopen(filematrix,"a")) == NULL)
		   lev_math::utility::Error((char*)("%s%s%s%sSave"), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   }
   else
   {
	   if((bzz = fopen(filematrix,"w")) == NULL)
		   lev_math::utility::Error((char*)("%s%s%s%sSave"), Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   }

   fprintf(bzz," %d %d\n",numRows,numColumns);
   for(int row = 1;row <= numRows;row++)
   {
	  for(int column = 1;column <= numColumns;column++)
		 fprintf(bzz," %15.7e",matrix[row][column]);
	  fprintf(bzz,"\n");
   }
	  fprintf(bzz,"\n");
   fclose(bzz);

}

//   ************************< Save >***************************
//   * Purpose: Saving  a Matrix on formatted file             *
//   * Description: Can be read.                               *
//   *              See TempFile in utility                    *
//   * Example: A.Save("Mat.Dat");                             *
//   * *AP* 23/01/2007 function substituted by the previous one  *
//   ***********************************************************
#if 0
template <class D>
void Te_Matrix<D>::Save(char *filematrix) { // formatted
   FILE *bzz;
   if((bzz = fopen(filematrix,"w")) == NULL);
       lev_math::utility::Error("%s%s%s%sSave", Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);

   fprintf(bzz," %d %d\n",numRows,numColumns);
   for(int row = 1;row <= numRows;row++) {
      for(int column = 1;column <= numColumns;column++)
         fprintf(bzz," %15.7e",matrix[row][column]);
      fprintf(bzz,"\n");
   }
   fclose(bzz);
}
#endif

//   ************************< Save >***************************
//   * Purpose: Saving a Matrix on an unformatted file         *
//   * Description: Can be read                                *
//   * Example: A.Save('*',MAT.BIN");                          *
//   *          See also TempFile from utility.cpp             *
//   ***********************************************************
template <class D>
void Te_Matrix<D>::Save(char,char *filematrix) { // unformatted
   FILE *bzz;
   if((bzz = fopen(filematrix,"wb")) == NULL)
       lev_math::utility::Error("%s%s%s%sSave",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   if(fwrite(&numRows,sizeof(int),1,bzz) != 1)
       lev_math::utility::Error("%s%s%sSave",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   if(fwrite(&numColumns,sizeof(int),1,bzz) != 1)
       lev_math::utility::Error("%s%s%sSave",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   if(fwrite(matrix[1],sizeof(D)*size,1,bzz) != 1)
       lev_math::utility::Error("%s%s%sSave",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   fclose(bzz);
}


//   ===========================================================
//   *********************   Norms   ***************************
//   ===========================================================

//   ***********************< NormT >***************************
//   * Purpose: Total matrix NORM  (n times the max of aij)    *
//   * Example: double normT = A.NormT ();                     *
//   ***********************************************************
template <class D>
double Te_Matrix<D>::NormT(void) {
   double norm=0.,aus;
   int n=(numRows > numColumns) ? numRows : numColumns;
   D *w = matrix[0];
   for(int i = 1;i < size;i++)
      if((aus = Abs(*++w)) > norm) norm = aus;
   return (double)n*norm;
}


//   ***********************< NormR >***************************
//   * Purpose: Max of the sums of the abs values              *
//   *          of coeff. rows                                 *
//   * Example: double normR = A.NormR ();                     *
//   ***********************************************************
template <class D>
double Te_Matrix<D>::NormR(void) {
   double norm = 0.;
   D *w = matrix[0];
   for(int i = 1;i <= numRows;i++) {
       double sum = 0.;
       for(int j = 1;j <= numColumns;j++)sum += Abs(*++w);
       if(sum > norm) norm = utility::Control(sum);
   }
   return norm;
}


//   ***********************< NormC >***************************
//   * Purpose: Norm C                                         *
//   * Example: double normC = A.NormC ();                     *
//   ***********************************************************
template <class D>
double Te_Matrix<D>::NormC(void) {
   double norm = 0.;
   for(int j = 1;j <= numColumns;j++) {
      double sum = 0.;
      for(int i = 1;i <= numRows;i++)
         sum += Abs(matrix[i][j]);
      if(sum > norm) norm = utility::Control(sum);
   }
   return norm;
}


//   ***********************< NormF >***************************
//   * Purpose: Frobenius' norm (the square root of the        *
//   *          sums of squares of all the coefficients)       *
//   * Description: Uses the SqrtSumSqr from utility.cpp       *
//   * Example: double normF = A.NormF ();                     *
//   ***********************************************************
template <class D>
double Te_Matrix<D>::NormF(void) {
   return SqrtSumSqr(size-1,matrix[0]+1);
}


//   ===========================================================
//   *                       Max Min                           *
//   ===========================================================

//   ************************< Max >****************************
//   * Purpose: Maximum of a  Matrix                           *
//   * Description: Uses the function described in UTILITY.CPP *
//   * Example: double xmax = A.Max();                         *
//   *  else:    double xmax = A.Max(&imax,&jmax);             *
//   ***********************************************************
template <class D>
D Te_Matrix<D>::Max(int *imax,int *jmax) {
   int iaux,im,jm;
   D xm;
   xm = utility::Max(size-1,matrix[0] + 1,&iaux);
   if(imax == 0 && jmax == 0)
      return xm;
   im = iaux/numColumns + 1;
   jm = iaux - numColumns*(im - 1) + 1;
   if(imax != 0)*imax = im;
   if(jmax != 0)*jmax = jm;
   return xm;
}


//   **********************< MaxAbs >***************************
//   * Purpose: Absolute maximum of a Matrix                   *
//   * Description: Uses the function defined in UTILITY.CPP   *
//   * Example: double xmax = A.MaxAbs();                      *
//   *  else:   double xmax = A.MaxAbs(&imax,&jmax);           *
//   ***********************************************************
template <class D>
D Te_Matrix<D>::MaxAbs(int *imax,int *jmax) {
   int iaux,im,jm;
   D xm;
   xm = utility::MaxAbs(size-1,matrix[0] + 1,&iaux);
   if(imax == 0 && jmax == 0)
      return xm;
   im = iaux/numColumns + 1;
   jm = iaux - numColumns*(im - 1) + 1;
   if(imax != 0)*imax = im;
   if(jmax != 0)*jmax = jm;
   return xm;
}


//   ************************< Min >****************************
//   * Purpose: Minimum of a Matrix                            *
//   * Description: Uses the function defined in UTILITY.CPP   *
//   * Example: double xmin = A.Min();                         *
//   *  else:   double xmin = A.Min(&imin,&jmin);              *
//   ***********************************************************
template <class D>
D Te_Matrix<D>::Min(int *imin,int *jmin) {
   int iaux,im,jm;
   D xm;
   xm = utility::Min(size-1,matrix[0] + 1,&iaux);
   if(imin == 0 && jmin == 0)
      return xm;
   im = iaux/numColumns + 1;
   jm = iaux - numColumns*(im - 1) + 1;
   if(imin != 0)*imin = im;
   if(jmin != 0)*jmin = jm;
   return xm;
}


//   **********************< MinAbs >***************************
//   * Purpose: Absolute minimum of a Matrix                   *
//   * Description: Uses the function defined in UTILITY.CPP   *
//   * Example: double xmin = A.MinAbs();                      *
//   *  else:   double xmin = A.MinAbs(&imin,&jmin);           *
//   ***********************************************************
template <class D>
D Te_Matrix<D>::MinAbs(int *imin,int *jmin) {
   int iaux,im,jm;
   D xm;
   xm = utility::MinAbs(size-1,matrix[0] + 1,&iaux);
   if(imin == 0 && jmin == 0)
      return xm;
   im = iaux/numColumns + 1;
   jm = iaux - numColumns*(im - 1) + 1;
   if(imin != 0)*imin = im;
   if(jmin != 0)*jmin = jm;
   return xm;
}


//   ===========================================================
//   *                 Modifying functions                     *
//   ===========================================================

//   **********************< Delete >***************************
//   * Purpose: Eliminating a matrix of no use                 *
//   *          without leaving the purpose                    *
//   * Example: Delete(&A);                                    *
//   ***********************************************************
template <class D>
void Delete(Te_Matrix<D> *result) {
   result->Deinitialize();
   result->size = result->numRows = result->numColumns = 0;
   result->matrix = NULL;
}


//   ***************< ChangeDimensions >************************
//   * Purpose: Changing the dimensions of a Matrix            *
//   * Example: ChangeDimensions(row,columns,&A,0); ///mcc     *
//   ***********************************************************
template <class D>
void ChangeDimensions(int rows,int columns, Te_Matrix<D> *result,char zero) {
   result->PrepCopy(rows,columns);
   if(zero == 0 && result->size != 0)
      memset(result->matrix[0],0,
             result->size*sizeof(D));
}

#if 0
//   ***************< ChangeDimensions >************************
//   * Purpose: Changing the dimensions of a Matrix            *
//   * Example: ChangeDimensions(row,columns,0);               *
//   ***********************************************************
template <class D>
void ChangeDimensions(int rows,int columns,char zero=0) {
    PrepCopy(rows,columns);
    if(zero == 0 && size != 0)
        memset(matrix[0],0,size*sizeof(D));
}
#endif

//   **********************< Recover >**************************
//   * Purpose: Recovering a matrix stored with                *
//   *          formatted Save(file name)                      *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved matrix             *
//   * Example: Recover(&x,file name);                         *
//   ***********************************************************
template <class D>
void Recover(Te_Matrix<D> *A,char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"r"))==NULL) {
       lev_math::utility::Error("%s%s%s%sRecover",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   }
   int rows,columns;
   fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
   ChangeDimensions(rows,columns,A,1);
   D *w = A->matrix[0];
   for(int i = 1;i < A->size;i++)fscanf(utility::bzzFileIn,"%f",++w);
   fclose(utility::bzzFileIn);
}


//   **********************< Recover >**************************
//   * Purpose: Recovering a matrix stored by                  *
//   *          unformatted  Save('*', file name)              *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved matrix             *
//   * Example: Recover(&A,'*',file name);                     *
//   ***********************************************************
template <class D>
void Recover(Te_Matrix<D> *A,char,char *filematrix) {
   if((utility::bzzFileIn = fopen(filematrix,"rb")) == NULL) { }
       lev_math::utility::Error("%s%s%s%sRecover *",
       Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   int rows,columns;
   if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1) { }
      lev_math::utility::Error("%s%s%sRecover",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
   if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1) { }
      lev_math::utility::Error("%s%s%sRecover",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
   ChangeDimensions(rows,columns,A,1);
   if(rows < 1 || columns < 1) { fclose(utility::bzzFileIn); return; }
   if(fread(A->matrix[1],sizeof(D)*(A->size), 1,utility::bzzFileIn) != 1) {}
      lev_math::utility::Error("%s%s%sRecover",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
   fclose(utility::bzzFileIn);
}


//   *********************< Transpose >*************************
//   * Purpose: Modifying a matrix into its transpose          *
//   * Description: If the matrix is square it replaces itself *
//   * Example: Transpose(&A);                                 *
//   ***********************************************************
template <class D>
void Transpose(Te_Matrix<D> *A) {
   int i,j;
   D **a=A->matrix;
   if(A->numRows == A->numColumns) {
      for(i = 1;i <= A->numRows;i++)
      for(j = i+1;j <= A->numColumns;j++)
         utility::Swap(&a[i][j],&a[j][i]); // in utility.cpp
      } else {
      Te_Matrix<D> B('*',A->numColumns,A->numRows);
      for(i = 1;i <= A->numRows;i++)
         for(j = 1;j <= A->numColumns;j++)
            B[j][i] = a[i][j];
      Swap(A,&B); // avoid copying
   }
}


//   *********************< SumRankOne >************************
//   * Purpose: Sum at matrix one of rank one                  *
//   * Example: SumRankOne(u,vt,&A); A = A + u*vT              *
//   ***********************************************************
template <class D>
void SumRankOne(const Te_Vector<D> &u, const Te_Vector<D> &vT, Te_Matrix<D> *result) {
   if(u.dimensions != vT.dimensions ||
      u.dimensions != result->numRows ||
      u.dimensions != result->numColumns)
      lev_math::utility::Error("%s%s%sSumRankOne",
      Te_Matrix<D>::ERRORMESSAGEMATRIX,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   D *r = result->matrix[1] + 1;
   D *left = u.vector + 1;
   D *right = vT.vector;
   for(int i = 1;i <= u.dimensions;i++) {
      for(int j = 1;j <= u.dimensions;j++)
         *r++ = utility::Control(*r + (*left) * right[j]);
      left++;
   }
}


//   *********************< SumRankOne >************************
//   * Purpose: Sum at matrix one of rank one                  *
//   * Example: SumRankOne(a,u,vt,&A); A = A + a*u*vT          *
//   ***********************************************************
template <class D>
void SumRankOne(double alfa,const Te_Vector<D> &u, const Te_Vector<D> &vT, Te_Matrix<D> *result) {
   Te_Vector<D> au;
   Product(alfa,u,&au);
   SumRankOne(au,vT,result);
}


//   *********************< SumRankOne >************************
//   * Purpose: Sum at matrix one of rank one                  *
//   * Example: SumRankOne(u,vt,b,&A); A = A + u*vT/b          *
//   ***********************************************************
template <class D>
void SumRankOne(const Te_Vector<D> &u,const Te_Vector<D> &vT, double beta, Te_Matrix<D> *result) {
   Te_Vector<D> au;
   Division(u,beta,&au);
   SumRankOne(au,vT,result);
}


//   ************************< Swap >***************************
//   * Purpose: Swapping the elements of any two Matrices      *
//   * Description: Provides an efficient method of copying    *
//   *              when a Matrix remains in the purpose and   *
//   *              another one leaves, they swap              *
//   * Example: Swap(&A,&B);                                   *
//   ***********************************************************
template <class D>
void Swap(Te_Matrix<D> *lval,Te_Matrix<D> *rval) {
   D **temp = lval->matrix;
   lval->matrix = rval->matrix;
   rval->matrix = temp;
   utility::Swap(&lval->numColumns,&rval->numColumns);
   utility::Swap(&lval->numRows,&rval->numRows);
   utility::Swap(&lval->size,&rval->size);
}


}//end namespace lev_math
#endif //MATRIX_HPPX
