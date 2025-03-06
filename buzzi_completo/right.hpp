#ifndef RIGHT_HPP
#define RIGHT_HPP

//   ======================< RIGHT.HPP >========================
//   * Class Te_MatrixRight                                    *
//   * Description: Chapter 12                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   ****** Constructions for Te_MatrixRight:                  *
//   * Te_MatrixRight R; // default                            *
//   * Te_MatrixRight R = right; // copy-initializer           *
//   * Te_MatrixRight L(3,3); // sizes and places at 0         *
//   * Te_MatrixRight R(3,3,                                   *
//   *       1.,2.,3.,                                         *
//   *          4.,5.,                                         *
//   *             6.);// sizes and initialises                *
//   * float x[]=                                              *
//   *     {                                                   *
//   *     1.,2.,3.,                                           *
//   *        4.,5.,                                           *
//   *           6.                                            *
//   *     };                                                  *
//   * Te_MatrixRight R(3,3,x); // from array                  *
//   * Te_MatrixRight R("MAT.DAT"); // Formatted File          *
//   * Te_MatrixRight R('*',MAT.BIN"); // Binary File          *
//   ***********************************************************
//   ***** Access functions :                                  *
//   *  i = R.Rows(); // numRows                               *
//   *  i = R.Columns(); // numColumns                         *
//   *  xf = R.GetValue(i);                                    *
//   *  xf = R(i,j);                                           *
//   *  R(i,j) = xf;                                           *
//   *  xf = R[i][j];                                          *
//   *  R[i][j] = xf;                                          *
//   *  R.SetValue(i,j,xf);                                    *
//   ***********************************************************
//   ***** Assignments:                                        *
//   *  R = right; // right Te_MatrixRight                     *
//   ***********************************************************
//   *****  Operators for tests:                               *
//   * if(R1 == R2)                                            *
//   * if(R1 != R2)                                            *
//   ***********************************************************
//   ***** Implemented operations :                            *
//   * Sum(R1,R2,&R3);        // R3 = R1 + R2;                 *
//   * R3 = R1 + R2;          // R3 = R1 + R2;                 *
//   * Sum(L,R,&A);           // A = L + R;                    *
//   * Sum(R,L,&A);           // A = R + L;                    *
//   * Sum(R1,R2,&R1);        // R1 = R1 + R2;                 *
//   * Sum(&R1,R2);           // R1 = R1 + R2;                 *
//   * R1 += R2;              // R1 = R1 + R2;                 *
//   * Sum(R1,R2,&R2);        // R2 = R1 + R2;                 *
//   * Sum(R1,&R2);           // R2 = R1 + R2;                 *
//   * Sum(R1,R1,&R1);        // R1 = R1 + R1;                 *
//   * Sum(&R1);              // R1 = R1 + R1;                 *
//   * Difference(R1,R2,&R3); // R3 = R1 - R2;                 *
//   * R3 = R1 - R2;          // R3 = R1 - R2;                 *
//   * Difference(R1,R2,&R1); // R1 = R1 - R2;                 *
//   * Difference(&R1,R2);    // R1 = R1 - R2;                 *
//   * R1 -= R2;              // R1 = R1 - R2;                 *
//   * Difference(R1,R2,&R2); // R2 = R1 - R2;                 *
//   * Difference(R1,&R2);    // R2 = R1 - R2;                 *
//   * Difference(R1,R1,&R1); // R1 = R1 - R1;                 *
//   * Difference(&R1);       // R1 = R1 - R1;                 *
//   * Minus(R1,&R2);         // R2 = -R1;                     *
//   * R2 = -R1;              // R2 = -R1;                     *
//   * Minus(&R1);            // R1 = -R1;                     *
//   * Product(R1,R2,&R3);    // R3 = R1*R2;                   *
//   * R3 = R1*R2;                                             *
//   * Product(R1,R2,&R1);    // R1 = R1*R2;                   *
//   * Product(&R1,R2);       // R1 = R1*R2;                   *
//   * R1 *= R2;              // R1 = R1*R2;                   *
//   * Product(R1,R1,&R1);    // R1 = R1*R1;                   *
//   * Product(&R1);          // R1 = R1*R1;                   *
//   * Product(R1,x,&y);      // y = R1*x;                     *
//   * y = R1*x;                                               *
//   * Product(3.,R1,&R2);    // R2 = 3.*R1;                   *
//   * R2 = 3.*R1;                                             *
//   * Product(3.,&R1);       // R1 = 3.*R1;                   *
//   * R1 *= 3.;              // R1 = 3.*R1;                   *
//   ***********************************************************
//   ***** Functions for linear algebra:                       *
//   * Solve(R,b,&x);                                          *
//   * Solve(R,&bx);  else Solve(R,b,&b);                      *
//   * Solve(R,B,&X);                                          *
//   * Solve(R,&BX);  else Solve(R,B,&B);                      *
//   * TranposeSolve(R,b,&x);                                  *
//   * TransposeSolve(R,&bx); else TransposeSolve(R,b,&b);     *
//   * R.Determinant();                                        *
//   * R.ConditionNumber();                                    *
//   ***********************************************************


namespace lev_math {

// preventive declarations
template <class D> class Te_Vector;
template <class D> class Te_Matrix;
template <class D> class Te_MatrixLeft;
template <class D> class Te_MatrixRight;
template <class D> class Te_MatrixSymm;


#if 10
template <class D> void Sum();
template <class D> void Difference ();
template <class D> void Minus ();
template <class D> void Product ();
template <class D> void Division ();
template <class D> void TProduct ();
template <class D> void ProductT ();
template <class D> void Delete ();
template <class D> void ChangeDimensions ();
template <class D> void Recover ();
template <class D> Te_MatrixRight<D> Transpose ();
template <class D> char Inverse ();
template <class D> void Swap ();
template <class D> void Solve ();
template <class D> void TransposeSolve ();
template <class D> char operator == (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval);
template <class D> char operator != (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval);
template <class D> Te_MatrixRight<D> operator + (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval);
template <class D> void Sum (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval,Te_MatrixRight<D> *result);
template <class D> void Sum (Te_MatrixLeft<D> &lval,Te_MatrixRight<D> &rval,Te_Matrix <D> *result);
template <class D> void Sum (Te_MatrixRight<D> &lval,Te_MatrixLeft<D> &rval,Te_Matrix <D> *result);
template <class D> void Sum (Te_MatrixRight<D> *lvalAndResult,const Te_MatrixRight<D> &rval);
template <class D> void Sum (const Te_MatrixRight<D> &lval,Te_MatrixRight<D> *rvalAndResult);
template <class D> void Sum (Te_MatrixRight<D> *lvalRvalAndResult);
template <class D> void Difference (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval, Te_MatrixRight<D> *result);
template <class D> Te_MatrixRight<D> operator - (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval);
template <class D> void Difference  (Te_MatrixRight<D> *lvalAndResult,const Te_MatrixRight<D> &rval);
template <class D> void Difference  (const Te_MatrixRight<D> &lval,Te_MatrixRight<D> *rvalAndResult);
template <class D> void Minus (const Te_MatrixRight<D> &rval,Te_MatrixRight<D> *result);
template <class D> Te_MatrixRight<D> operator - (const Te_MatrixRight<D> &rval);
template <class D> void Minus (Te_MatrixRight<D> *rvalAndResult);
template <class D> void Product (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval, Te_MatrixRight<D> *result);
template <class D> Te_MatrixRight<D> operator * (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval);
template <class D> void Product (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
template <class D> void Product (const Te_MatrixRight<D> &lval,Te_MatrixRight<D> *rvalAndResult);
template <class D> void Product (Te_MatrixRight<D> *lvalRvalAndResult);
template <class D> void Product (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval, Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval);
template <class D> void Product (double lval,const Te_MatrixRight<D> &rval,Te_MatrixRight<D> *result);
template <class D> Te_MatrixRight<D> operator * (double lval,const Te_MatrixRight<D> &rval);
template <class D> void Product (double lval,Te_MatrixRight<D> *rvalAndResult);
template <class D> void TProduct (const Te_MatrixRight<D> &lvalAndRval, Te_MatrixSymm<D> *result);
template <class D> void ProductT (const Te_MatrixRight<D> &lvalAndRval, Te_MatrixSymm<D> *result);
template <class D> void Division (const Te_MatrixRight<D> &lval,double rval, Te_MatrixRight<D> *result);
template <class D> Te_MatrixRight<D> operator / (const Te_MatrixRight<D> &lval,double rval);
template <class D> void Division (Te_MatrixRight<D> *lvalAndResult,double rval);
template <class D> void Delete (Te_MatrixRight<D> *R);
template <class D> void ChangeDimensions (int rows,int columns,Te_MatrixRight<D> *result,char zero );
template <class D> void Recover (Te_MatrixRight<D> *A,char *filematrix);
template <class D> void Recover (Te_MatrixRight<D> *A,char,char *filematrix);
template <class D> Te_MatrixLeft<D> Transpose (const Te_MatrixRight<D> &R);
template <class D> Te_MatrixRight<D> Transpose (const Te_MatrixLeft<D> &L);
template <class D> char Inverse (Te_MatrixRight<D> *R);
template <class D> void Swap (Te_MatrixRight<D> *lval,Te_MatrixRight<D> *rval);
template <class D> void Solve (const Te_MatrixRight<D> &R,Te_Vector<D> *bx);
template <class D> void Solve (const Te_MatrixRight<D> &R,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void Solve (const Te_MatrixRight<D> &R,Te_Matrix<D> *BX);
template <class D> void Solve (const Te_MatrixRight<D> &R,const Te_Matrix<D> &B,Te_Matrix<D> *X);
template <class D> void TransposeSolve (const Te_MatrixRight<D> &R,Te_Vector<D> *bx);
template <class D> void TransposeSolve (const Te_MatrixRight<D> &R,const Te_Vector<D> &b,Te_Vector<D> *x);
#endif


//   ===========================================================
//   =================   class Te_MatrixRight   ================
//   ===========================================================
template <class D> class Te_MatrixRight
{
   friend class Te_Vector<D>;
   friend class Te_Matrix<D>;
   friend class Te_MatrixLeft<D>;
   friend class Te_MatrixSymm<D>;

private:
   static const char *const ERRORMESSAGE;
   static int count; // for whoAmI

   D **matrix;
   int numRows,numColumns;
   int size;
   int whoAmI;

   // initialisation constructors
   void Initialize(int rows,int columns);

   // deinitialisation
   void Deinitialize(void);

   // Te_MatrixRight A('*',3,3);
   Te_MatrixRight(char,int rows,int columns);

public:
   //   ===========================================================
   //   ******************   constructors   ***********************
   //   ===========================================================
   // default // Te_MatrixRight R
   Te_MatrixRight(void);

   // copy-initializer // Te_MatrixRight R = right;
   Te_MatrixRight(Te_MatrixRight<D> &rval);

   // sizes and initialises to 0 // Te_MatrixRight R(3,3);
   Te_MatrixRight(int rows,int columns);

   // sizes and initialises
   // Te_MatrixRight R(2,2,1.,2.,3.);
   Te_MatrixRight(int rows,int columns,D a11,...);

   // from array // Te_MatrixRight R(3,3,w)
   Te_MatrixRight(int rows,int columns,D *initvalues);

   // from formatted File // Te_MatrixRight R("RIGHT.DAT");
   Te_MatrixRight(char *filematrix);

   // from binary File // Te_MatrixRight R('*',"RIGHT.BIN");
   Te_MatrixRight(char,char *filematrix);

   //   ===========================================================
   //   ********************   destructor   ***********************
   //   ===========================================================
   ~Te_MatrixRight(void);

   //   ===========================================================
   //   ************   Non-modifying access functions   ***********
  //   ===========================================================
   // number of rows
   int Rows(void) const  {
      return numRows;
   }

   // number of columns
   int Columns(void) const {
      return numColumns;
   }

   int WhoAmI(void) const {
      return whoAmI;
   }

   // receive the values with control
   D GetValue(int row,int col) const;

   //   ===========================================================
   //   **************   Modifying access functions   *************
   //   ===========================================================
   //   assigns values with control
   void SetValue(int row,int col,D val);

   // assigns and receives vector values with control
   D &operator () (int row,int col);

   // assigns and receives vector values without control
   D *operator [] (int i) {
      return matrix[i];
   }

   //   ===========================================================
   //   ******************  assignment operators ******************
   //   ===========================================================
   Te_MatrixRight<D> &operator = (const Te_MatrixRight<D> &rval);

   //   ===========================================================
   //   *****************   operators for tests   *****************
   //   ===========================================================
   friend char operator == <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval);

   friend char operator != <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval);

   //   ===========================================================
   //   ====================   OPERATIONS   =======================
   //   ===========================================================

   //   ===========================================================
   //   ***********************   Sum   ***************************
   //   ===========================================================
   //   Sum(A,B,&C); C = A + B;

   // C = A + B;
   friend Te_MatrixRight<D> operator + <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval);

   friend void Sum <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval, Te_MatrixRight<D> *result);

   // Sum(L,R,&C); C = L + R;
   friend void Sum <> (Te_MatrixLeft<D> &lval,Te_MatrixRight<D> &rval,
      Te_Matrix <D> *result);

   // Sum(R,L,&C); C = R + L;
   friend void Sum <> (Te_MatrixRight<D> &lval, Te_MatrixLeft<D> &rval,
      Te_Matrix <D> *result);

   // Sum(&A,B); A = A + B;
   friend void Sum <> (Te_MatrixRight<D> *lvalAndResult,
      const Te_MatrixRight<D> &rval);

   // A += B; A = A + B;
   Te_MatrixRight<D> &operator += (const Te_MatrixRight<D> &rval);

   // Sum(B,&A); A = B + A;
   friend void Sum <> (const Te_MatrixRight<D> &lval,
      Te_MatrixRight<D> *rvalAndResult);

   // Sum(&A); A = A + A;
   friend void Sum <> (Te_MatrixRight<D> *lvalRvalAndResult);

   //   ===========================================================
   //   ********************   Difference   ***********************
   //   ===========================================================
   //   Difference(A,B,&C); C = A - B;
   friend void Difference <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval, Te_MatrixRight<D> *result);

   // C = A - B;
   friend Te_MatrixRight<D> operator - <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval);

   // Difference(&A,B); A = A - B;
   friend void Difference <> (Te_MatrixRight<D> *lvalAndResult,
      const Te_MatrixRight<D> &rval);

   // A -= B; A = A - B;
   Te_MatrixRight<D> &operator -= (const Te_MatrixRight<D> &rval);

   // Difference(B,&A); A = B - A;
   friend void Difference <> (const Te_MatrixRight<D> &lval,
      Te_MatrixRight<D> *rvalAndResult);

   //   ===========================================================
   //   ***********************   Minus   *************************
   //   ===========================================================
   friend void Minus <> (const Te_MatrixRight<D> &rval,
      Te_MatrixRight<D> *result);

   //   unary
   friend Te_MatrixRight<D> operator - <> (const Te_MatrixRight<D> &rval);

   friend void Minus <>(Te_MatrixRight<D> *rvalAndResult);

   //   ===========================================================
   //   **********************   Product   ************************
   //   ===========================================================
   friend void Product         // C=A*B;
      <> (const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval,
      Te_MatrixRight<D> *result);

   friend Te_MatrixRight<D> operator * <> (const Te_MatrixRight<D> &lval,
      const Te_MatrixRight<D> &rval);

   friend void Product <>         // A = A * B;
      (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);

   // A *= B; A = A*B
   Te_MatrixRight<D> &operator *= (const Te_MatrixRight<D> &rval);

   friend void Product    <>     // B = A * B;
      (const Te_MatrixRight<D> &lval,Te_MatrixRight<D> *rvalAndResult);

   friend void Product  <>       // A = A * A;
      (Te_MatrixRight<D> *lvalRvalAndResult);

   friend void Product   <>      // y =A*x;
      (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval,
      Te_Vector<D> *result);

   friend Te_Vector<D> operator * <> // y = A*x;
      (const Te_MatrixRight<D> &lval,const Te_Vector<D> &rval);

   friend void Product  <>       // A = 3.*B;
      (double lval,const Te_MatrixRight<D> &rval,
      Te_MatrixRight<D> *result);

   friend Te_MatrixRight<D> operator * <>  // A = 3.*B;
      (double lval,const Te_MatrixRight<D> &rval);

   friend void Product   <>      // A = 3.* A;
      (double lval,Te_MatrixRight<D> *rvalAndResult);

   Te_MatrixRight<D> &operator *=        // A = 3.*A;
      (double rval);

   //   ===========================================================
   //   *********************   TProduct   ************************
   //   ===========================================================
   friend void TProduct <> (const Te_MatrixRight<D> &lvalAndRval,
      Te_MatrixSymm<D> *result);

   //   ===========================================================
   //   *********************   ProductT   ************************
   //   ===========================================================
   friend void ProductT <> (const Te_MatrixRight<D> &lvalAndRval,
      Te_MatrixSymm<D> *result);

   //   ===========================================================
   //   **********************   Division   ***********************
   //   ===========================================================
   friend void Division <>// A =B/3.;
      (const Te_MatrixRight<D> &lval,double rval,
      Te_MatrixRight<D> *result);

   // A = B/3.;
   friend Te_MatrixRight<D> operator / <>
      (const Te_MatrixRight<D> &lval,double rval);

   friend void Division  <>        // A /= 3.;
      (Te_MatrixRight<D> *lvalAndResult,double rval);

   // A /= 3.;
   Te_MatrixRight<D> &operator /= (double rval);

   //   ===========================================================
   //   ===========   Non-modifying functions   ===================
   //   ===========================================================

   //   ***********************< Print >***************************
   void Print(char *msg="");

   //   ************************< Save >***************************
   void Save(char *filematrix); // formatted
   void Save(char,char *filematrix);// binary

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
   friend void Delete <> (Te_MatrixRight<D> *R);
   friend void ChangeDimensions <> (int rows,int columns,
      Te_MatrixRight<D> *result,char zero);

   // from formatted file
   friend void Recover <> (Te_MatrixRight<D> *A,char *filematrix);
   // from binary file
   friend void Recover <> (Te_MatrixRight<D> *A,char,char *filematrix);
   friend Te_MatrixLeft<D> Transpose <> (const Te_MatrixRight<D> &R);
   friend Te_MatrixRight<D> Transpose <> (const Te_MatrixLeft<D> &L);
   friend char Inverse <> (Te_MatrixRight<D> *R);
   friend void Swap <> (Te_MatrixRight<D> *lval,Te_MatrixRight<D> *rval);

   //   ===========================================================
   //   ==========   Functions for linear algebra  ================
   //   ===========================================================
   friend void Solve <> (const Te_MatrixRight<D> &R,Te_Vector<D> *bx);
   friend void Solve <> (const Te_MatrixRight<D> &R, const Te_Vector<D> &b,
      Te_Vector<D> *x);
   friend void Solve <> (const Te_MatrixRight<D> &R,Te_Matrix<D> *BX);
   friend void Solve <> (const Te_MatrixRight<D> &R,const Te_Matrix<D> &B,
      Te_Matrix<D> *X);
   friend void TransposeSolve <> (const Te_MatrixRight<D> &R,
      Te_Vector<D> *bx);
   friend void TransposeSolve <> (const Te_MatrixRight<D> &R,
      const Te_Vector<D> &b,Te_Vector<D> *x);

   D Determinant(void);
   D ConditionNumber();
};


template <class D>
const char *const Te_MatrixRight<D>::ERRORMESSAGE =
   "\n========>>> Te_MatrixRight error!!!!!!\n";
template <class D>
int Te_MatrixRight<D>::count = 0;


//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   ********************< Initialize >*************************
//   * Purpose: Common function for initialising constructors  *
//   ***********************************************************
template <class D>
void Te_MatrixRight<D>::Initialize(int rows,int columns) {
   count++;
   whoAmI = count;
   if(rows < 1 || columns < 1) {
      numRows = numColumns = size = 0;
      matrix = NULL;
      return;
   }
   if(rows != columns)
      utility::Error("%s%s%sInitialize",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   numRows = numColumns = rows;
   size = numRows*(numRows+1)/2+1;
   matrix = new D *[numRows+1];
   if(!matrix)
      utility::Error("%s%s%sInitialize",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
   matrix[0] = new D[size];
   if(!matrix[0])
      utility::Error("%s%s%sInitialize",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
   matrix[1] = matrix[0];
   for(int i = 2;i <= numRows;i++)
      matrix[i] = &matrix[i-1][numRows-i+1];
}


//   **********************< Deinitialize >*********************
//   * Purpose: Common function for deinitialisation           *
//   * Description: Chapter 12                                 *
//   * Example: Deinitialize();                                *
//   ***********************************************************
template <class D>
void Te_MatrixRight<D>::Deinitialize(void)
{
	if(matrix) {
		delete [] matrix[0]; matrix[0] = NULL;
		delete [] matrix; matrix = NULL;
	}
}


//   ****************< private constructor >********************
//   * Purpose: Sets size of a Te_MatrixRight<D> without       *
//   *          initialisation. Serving internally             *
//   * Example: Te_MatrixRight<D> R('*',4,5);                  *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(char,int rows,int columns)
{
	Initialize(rows,columns);
}


//   ===========================================================
//   ================== Public functions =======================
//   ===========================================================

//   ===========================================================
//   ******************   constructors   ***********************
//   ===========================================================

//   **********************< default >**************************
//   * Purpose: Defining a Te_MatrixRight<D> which can then    *
//   *          receive an assignment                          *
//   * Examples: Te_MatrixRight<D> R;                          *
//   *           R=B;                                          *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(void)
{
	Initialize(0,0);
}


//   ****************< Copy-Initializer >***********************
//   * Purpose: Definition and initialisation of MatrixRigh    *
//   *          Used in returns                                *
//   * Examples: Te_MatrixRight<D> A=B;                        *
//   *           Te_MatrixRight<D> A=B+2.*C;                   *
//   *           return A; (used implicitly)                   *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(Te_MatrixRight<D> &rval)
{
	Initialize(rval.numRows,rval.numRows);
	if(numRows != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
}


//   *************< sizing constructor >************************
//   * Purpose: Setting size and initialising to 0             *
//   *          a Te_MatrixRight<D>                            *
//   *          Must be able to assign it other values         *
//   * Description: Uses the function memset                   *
//   * Example: Te_MatrixRight<D> A(5,5);                      *
//   *          A(1,1)=3.; A(1,2)=7.;....                      *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(int rows,int columns)
{
	Initialize(rows,columns);
	if(numRows != 0)
		memset(matrix[0],0,size*sizeof(D));
}


/*
//   *************< sizing and initialising  >******************
//   * Purpose: Initialising a Te_MatrixRight<D>               *
//   * Example: Te_MatrixRight<D> A(2,2,1.,2.,3.);             *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(int rows,int columns,double a11,...) {
   Initialize(rows,columns);
   D *w=matrix[0] + 1;
   va_list puntaList;
   va_start(puntaList,a11);
   int i;
   *w = Control(a11);
   for(i = 2;i < size;i++)
      *++w = Control(va_arg(puntaList,double));
   va_end(puntaList);
}
*/


//   *********************< from array >************************
//   * Purpose: Initialising a Te_MatrixRight<D> from an array *
//   * Example: T w[]={1.,2.,3.,                               *
//   *                        4.,5.,                           *
//   *                           6.};                          *
//   *          Te_MatrixRight<D> A(3,3,w);                    *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(int rows,int columns, D *initvalues) {
   Initialize(rows,columns);
   D *w = matrix[0];
      ++w;
   if(numRows != 0)
      memcpy(w,initvalues,(size-1)*sizeof(D));
}


//   ********************< ASCII FILE >*************************
//   * Purpose: Initialising a Te_MatrixRight<D> from FILE     *
//   * Example: Te_MatrixRight<D> A("MAT.DAT");                *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"r"))==NULL)
       utility::Error("%s%s%s%sConstructor",
          Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   int rows,columns;
   fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
   if(rows < 1 || columns < 1)
       utility::Error("%s%s%sFILE",
          Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION, utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   D *w = matrix[0];
   for(int i = 1;i < size;i++)
      fscanf(utility::bzzFileIn,"%f",++w);
   fclose(utility::bzzFileIn);
}


//   *******************< binary FILE >*************************
//   * Purpose: Initialising a Te_MatrixRight<D> from          *
//   *          unformatted FILE                               *
//   * Example: Te_MatrixRight<D> A('*',"MAT.DAT");            *
//   ***********************************************************
template <class D>
Te_MatrixRight<D>::Te_MatrixRight(char,char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"rb"))==NULL)
       utility::Error("%s%s%s%sFILE",
          Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_CONSTRUCTOR);
   int rows,columns;
   if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
      utility::Error("%sFILE",Te_MatrixRight<D>::ERRORMESSAGE);
   if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
      utility::Error("%sFILE",Te_MatrixRight<D>::ERRORMESSAGE);
   if(rows < 1 || columns < 1)
       utility::Error("%s%s%sFILE",
          Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   if(fread(matrix[1],sizeof(D)*size,1,utility::bzzFileIn) != 1)
      utility::Error("%sFILE",Te_MatrixRight<D>::ERRORMESSAGE);
   fclose(utility::bzzFileIn);
}


//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================
template <class D>
Te_MatrixRight<D>::~Te_MatrixRight(void) {
   Deinitialize();
}


//   ===========================================================
//   ************  Non-modifying access functions    ***********
//   ===========================================================

//   *********************< GetValue >**************************
//   * Purpose: Receiving values with control                  *
//   * Example: x = R.GetValue(i,j);                           *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::GetValue(int row,int column) const {
   if(row < 1 || row > numRows)
      utility::Error("%s%s%sGetValue",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
   if(column < 1 || column > numRows)
      utility::Error("%s%s%sGetValue",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
   if(column < row)return 0.;
   return matrix[row][column];
}


//   ===========================================================
//   **************    Modifying access functions **************
//   ===========================================================

//   *********************< SetValue >**************************
//   * Purpose: Assigning values with control                  *
//   ***********************************************************
template <class D>
void Te_MatrixRight<D>::SetValue(int row,int column,D val) {
   if(row < 1 || row > numRows)
      utility::Error("%s%s%sSetValue",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
   if(column < 1 || column > numRows)
      utility::Error("%s%s%sSetValue",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
   if(column < row)
      utility::Error("%s\ncolumn<row%sSetValue",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_FUNCTION);
   matrix[row][column]=val;
}


//   ********************< operator () >************************
//   * Purpose: Receiving and assigning values with control    *
//   * Example: x = A(1,5); A(3,7) = 5.;                       *
//   ***********************************************************
template <class D>
D &Te_MatrixRight<D>::operator () (int row,int col) {
   if( (row < 1 || row > numRows) ||
       (col < row || col > numRows) )
      utility::Error("%s%s%s()",Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_OPERATOR);
   return matrix[row][col];
}


//   ===========================================================
//   ***************   assignment operator   *******************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment                                     *
//   * Example: A=B;   // simple                               *
//   *          A=C+T;    // with operations                   *
//   *          A=B=C+T;  // multiple                          *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> &Te_MatrixRight<D>::operator =
   (const Te_MatrixRight<D> &rval) {
   int who = whoAmI;
   if(numRows != rval.numRows) {
      Deinitialize();
      Initialize(rval.numRows,rval.numRows);
      count--;
   }
   whoAmI = who;
   if(numRows != 0)
      memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
   return *this;
}


//   ===========================================================
//   ***************   operators for tests   *******************
//   ===========================================================

//   ************************< == >*****************************
//   * Purpose: Checking if two Te_MatrixRights are identical  *
//   * Description: Returns 1 if they are identical            *
//   * Example: if(A==B)...                                    *
//   ***********************************************************
template <class D>
char operator == (const Te_MatrixRight<D> &lval,
   const Te_MatrixRight<D> &rval) {
   if(lval.whoAmI == rval.whoAmI) return 1;
   char ch = 1;
   if(lval.numRows != rval.numRows) ch = 0;
   else {
      if(memcmp(lval.matrix[0]+1,rval.matrix[0]+1,
         (rval.size-1)*sizeof(D))==0)ch=1;
      else ch = 0;
   }
   return ch;
}


//   ************************< != >*****************************
//   * Purpose: Checking if two Te_MatrixRights are different  *
//   * Description: Returns 1 if they are different            *
//   * Example: if(A!=B)...                                    *
//   ***********************************************************
template <class D>
char operator != (const Te_MatrixRight<D> &lval,
   const Te_MatrixRight<D> &rval) {
   if(lval.whoAmI == rval.whoAmI) return 0;
   char ch = 0;
   if(lval.numRows != rval.numRows) ch = 1;
   else
      ch = memcmp(lval.matrix[0]+1,rval.matrix[0]+1, (rval.size-1)*sizeof(D));
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
void Sum(const Te_MatrixRight<D> &lval,const Te_MatrixRight<D> &rval,
   Te_MatrixRight<D> *result) {
   if(lval.numRows != rval.numRows)
      utility::Error("%s%s%s+",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(result->whoAmI == lval.whoAmI)
      (*result) += rval;
   else if(result->whoAmI == rval.whoAmI)
      Sum(lval,result);
   else {
      ChangeDimensions(lval.numRows,lval.numColumns,result,1);
      Sum(rval.size,lval.matrix[1],rval.matrix[1],result->matrix[1]);
   }
}


//   *************************< + >*****************************
//   * Purpose: Addition of two Right matrices                 *
//   * Example: A = B + C;                                     *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> operator + (const Te_MatrixRight<D> &lval,
   const Te_MatrixRight<D> &rval) {
   Te_MatrixRight<D> result('*',lval.numRows,lval.numRows);
   Sum(lval,rval,&result);
   return result;
}


//   ************************< Sum >****************************
//   * Purpose: Addition between Left and Right matrices       *
//   * Example: Sum(L,R,&C); C = L + R;                        *
//   ***********************************************************
template <class D>
void Sum(Te_MatrixLeft<D> &lval,Te_MatrixRight<D> &rval,Te_Matrix<D> *result) {
   if(lval.numRows != rval.numRows)
      utility::Error("%s%s%s+",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   ChangeDimensions(lval.numRows,lval.numColumns,result,1);
   for(int i = 1;i <= lval.numRows;i++) {
      for(int j = 1;j <= lval.numColumns;j++) {
        if(i < j)
           (*result)[i][j] = lval[i][j];
        else if(i > j)
           (*result)[i][j] = rval[i][j];
        else
           (*result)[i][j] = Control(lval[i][j] + rval[i][j]);
      }
   }
}


//   ************************< Sum >****************************
//   * Purpose: Addition between Left and Right matrices       *
//   * Example: Sum(R,L,&C); C = R + L;                        *
//   ***********************************************************
template <class D>
void Sum(Te_MatrixRight<D> &lval,Te_MatrixLeft<D> &rval,Te_Matrix<D> *result) {
   if(lval.numRows != rval.numRows)
      utility::Error("%s%s%s+",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   ChangeDimensions(lval.numRows,lval.numColumns,result,1);
   for(int i = 1;i <= lval.numRows;i++)   {
      for(int j = 1;j <= lval.numColumns;j++) {
         if(i < j)
            (*result)[i][j] = rval[i][j];
         else if(i > j)
            (*result)[i][j] = lval[i][j];
         else
            (*result)[i][j] = Control(lval[i][j] + rval[i][j]);
      }
   }
}


//   ************************< Sum >****************************
//   * Purpose: Addition between matrices                      *
//   * Description: The result goes in the left matrix         *
//   * Example: Sum(&A,B); A += B;                             *
//   ***********************************************************
template <class D>
void Sum(Te_MatrixRight<D> *lvalAndResult,const Te_MatrixRight<D> &rval) {
   (*lvalAndResult) += rval;
}


//   ************************< += >*****************************
//   * Purpose: Addition of two Right matrices                 *
//   * Example: A += B;                                        *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> &Te_MatrixRight<D>::operator +=
   (const Te_MatrixRight<D> &rval) {
   if(numRows != rval.numRows)
      utility::Error("%s%s%s+",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(whoAmI == rval.whoAmI)
      Sum(this);
   else
      Sum(rval.size-1,matrix[1]+1,rval.matrix[1]+1);
   return *this;
}


//   ************************< Sum >****************************
//   * Purpose: Addition between matrices                      *
//   * Description: The result goes in the right matrix        *
//   * Example: Sum(B,&A); A = B + A;                          *
//   ***********************************************************
template <class D>
void Sum(const Te_MatrixRight<D> &lval,Te_MatrixRight<D> *rvalAndResult) {
   (*rvalAndResult) += lval;
}


//   ************************< Sum >****************************
//   * Purpose: Addition between identical matrices            *
//   * Description: The result replaces the matrix             *
//   * Example: Sum(&A); A = A + A;                            *
//   ***********************************************************
template <class D>
void Sum(Te_MatrixRight<D> *lvalRvalAndResult) {
   Sum(lvalRvalAndResult->size-1,
   lvalRvalAndResult->matrix[1]+1);
}


//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================

//   *************************< - >*****************************
//   * Purpose: Difference between Right matrices              *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> operator - (const Te_MatrixRight<D> &lval,
   const Te_MatrixRight<D> &rval) {
   if(lval.numRows != rval.numRows)
      utility::Error("%s%s%s-",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_MatrixRight<D> result('*',lval.numRows,lval.numRows);
   Difference(rval.size,lval.matrix[1],rval.matrix[1], result.matrix[1]);
   return result;
}


//   ************************< -= >*****************************
//   * Purpose: Difference between two Right matrices          *
//   * Example: A-=B;                                          *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> &Te_MatrixRight<D>::operator -=
   (const Te_MatrixRight<D> &rval) {
   if(numRows != rval.numRows)
      utility::Error("%s%s%s-",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Difference(rval.size,matrix[1],rval.matrix[1]);
   return *this;
}


//   ===========================================================
//   *************************   Minus   ***********************
//   ===========================================================

//   *************************< - >*****************************
//   * Purpose: Unary minus                                    *
//   * Example: A=-B;                                          *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> operator - (const Te_MatrixRight<D> &rval) { // unary minus
   Te_MatrixRight<D> result('*',rval.numRows,rval.numRows);
   D *w = rval.matrix[1];
   D *r = result.matrix[1];
   for(int i = 1;i < result.size;i++)
   r[i] = -w[i];
   return result;
}


//   ===========================================================
//   **********************   Product   ************************
//   ===========================================================

//   *************************< * >*****************************
//   * Purpose: Product of two Right matrices                  *
//   * Description: It produces a Te_MatrixRight               *
//   * Example: C = A*B;                                       *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> operator * (const Te_MatrixRight<D> &lval,
   const Te_MatrixRight<D> &rval) {
   if(lval.numRows != rval.numRows)
      utility::Error("%s%s%s*",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_MatrixRight<D> result('*',lval.numRows,lval.numRows);
   D **b=rval.matrix;
   D *v=result.matrix[1];
   for(int row=1;row<=lval.numRows;row++) {
      for(int column=row;column<=rval.numRows;column++)   {
         D *w = lval.matrix[row] + row;
         D sum = 0.;
         for(int i = row,k = 1;k <= column-row+1;i++,k++) {
            sum += *w++ * b[i][column];
         }
         *++v = Control(sum);
      }
   }
   return result;
}


//   *************************< * >*****************************
//   * Purpose: Product of a Right matrix with a vector        *
//   * Example: y = R*x;                                       *
//   ***********************************************************
template <class D>
Te_Vector<D> operator * (const Te_MatrixRight<D> &lval,
   const Te_Vector<D> &rval) {
   if(lval.numColumns != rval.dimensions)
      utility::Error("%s%s%s*",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_Vector<D> result('*',lval.numRows);
   D *r = result.vector;
   for(int i = 1;i <= lval.numRows;i++)
      r[i] = Dot(rval.dimensions-i+1,
      lval.matrix[i] + i,rval.vector + i);
   return result;
}


//   *************************< * >*****************************
//   * Purpose: Product of a T with a Right matrix             *
//   * Example: C = 3.*B;                                      *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> operator * (D lval,const Te_MatrixRight<D> &rval) {
   Te_MatrixRight<D> result('*',rval.numRows,rval.numColumns);
   Product(rval.size - 1,lval,rval.matrix[1]+1, result.matrix[1]+1);
   return result;
}

/*
//   ************************< *= >*****************************
//   * Purpose: Product of a T with a Right matrix             *
//   * Example: R *= 3.;                                       *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> &Te_MatrixRight<D>::operator *= (D rval) {
   Product(size - 1,rval,matrix[1] + 1);//in UTILITY.CPP
   return *this;
}
*/

//   ===========================================================
//   *********************   TProduct   ************************
//   ===========================================================

//   **********************< TProduct >*************************
//   * Purpose: Product of a transposed right matrix           *
//   *          multiplied by itself                           *
//   * Description: The result is MatrixSymm                   *
//   * Example: TProduct(R,&S);                                *
//   ***********************************************************
template <class D>
void TProduct(const Te_MatrixRight<D> &lvalAndRval,Te_MatrixSymm<D> *result) {
   D **r = lvalAndRval.matrix;
   int n = lvalAndRval.numRows;
   ChangeDimensions(n,n,result,1);
   D **s = result->matrix;
   for(int i=1;i <= n;i++) {
      for(int j=1;j <= i;j++) {
         D sum = 0.;
         for(int k=1;k <= i && k <=j;k++)
            sum += r[k][i]*r[k][j];
         s[i][j] = Control(sum);
      }
   }
}


//   ===========================================================
//   *********************   ProductT   ************************
//   ===========================================================

//   **********************< ProductT >*************************
//   * Purpose: Product of a Right matrix                      *
//   *          with its transpose                             *
//   * Description: The result is MatrixSymm                   *
//   * Example: ProductT(R,&S);                                *
//   ***********************************************************
template <class D>
void ProductD(const Te_MatrixRight<D> &lvalAndRval,Te_MatrixSymm<D> *result) {
   D **r = lvalAndRval.matrix;
   int n = lvalAndRval.numRows;
   ChangeDimensions(n,n,result,1);
   D **s = result->matrix;
   for(int i=1;i <= n;i++) {
      for(int j=1;j <= i;j++) {
         D sum = 0.;
         for(int k=i;k <= n && k>=j;k++)
            sum += r[i][k]*r[j][k];
         s[i][j] = Control(sum);
      }
   }
}


//   ===========================================================
//   **********************   Division   ***********************
//   ===========================================================

//   *************************< / >*****************************
//   * Purpose: Dividing by a T                                *
//   * Example: C = B/3.;                                      *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> operator / (const Te_MatrixRight<D> &lval,D rval) {
   Te_MatrixRight<D> result('*',lval.numRows,lval.numColumns);
   Division(lval.size - 1,lval.matrix[1]+1,rval, result.matrix[1]+1);
   return result;
}


/*
//   ************************< /= >*****************************
//   * Purpose: Dividing by a T                                *
//   * Example: R /= 3.;                                       *
//   ***********************************************************
template <class D>
Te_MatrixRight<D> &Te_MatrixRight<D>::operator /= (D rval) {
   Division(size - 1,matrix[1] + 1,rval);//in UTILITY.CPP
   return *this;
}
*/


//   ===========================================================
//   *              Non-modifying functions                    *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Printing                                       *
//   ***********************************************************
template <class D>
void Te_MatrixRight<D>::Print(char *msg) {
   fprintf(utility::bzzFileOut,"\nTe_MatrixRight No.%d",whoAmI);
   if(*msg)fprintf(utility::bzzFileOut,"  %s\n",msg);
   fprintf(utility::bzzFileOut,"\nrows %d columns %d\n", numRows,numRows);
   for(int row = 1;row <= numRows;row++) {
      for(int column = 1;column <= numRows;column++)
         if(column >= row)
            fprintf(utility::bzzFileOut,"%12.4e", matrix[row][column]);
         else
            fprintf(utility::bzzFileOut,"%12.4e",0.);
      fprintf(utility::bzzFileOut,"\n");
   }
}


//   ************************< Save >***************************
//   * Purpose: Saving  a Te_MatrixRight<D> on formatted file        *
//   * Description: Can be read.                               *
//   *              See TempFile in utility                    *
//   * Example: A.Save("Mat.Dat");                             *
//   ***********************************************************
template <class D>
void Te_MatrixRight<D>::Save(char *filematrix) // formatted
   {
   FILE *bzz;
   if((bzz = fopen(filematrix,"w")) == NULL)
       utility::Error("%s%s%s%sSave",
       Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);

   fprintf(bzz," %d %d\n",numRows,numColumns);
   for(int row = 1;row <= numRows;row++)
      {
      for(int column = row;column <= numColumns;column++)
        fprintf(bzz," %15.7e",matrix[row][column]);
      fprintf(bzz,"\n");
      }
   fclose(bzz);
   }

//   ************************< Save >***************************
//   * Purpose: Saving a Te_MatrixRight<D> on an unformatted file    *
//   * Description: Can be read                                *
//   * Example: A.Save('*',MAT.BIN");                          *
//   *          See also TempFile from utility.cpp             *
//   ***********************************************************
template <class D>
void Te_MatrixRight<D>::Save(char,char *filematrix)
   {
   FILE *bzz;
   if((bzz = fopen(filematrix,"wb")) == NULL)
       utility::Error("%s%s%s%sSave",
       Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   if(fwrite(&numRows,sizeof(int),1,bzz) != 1)
       utility::Error("%s%s%sSave",
       Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   if(fwrite(&numColumns,sizeof(int),1,bzz) != 1)
       utility::Error("%s%s%sSave",
       Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   if(fwrite(matrix[1],sizeof(D)*size,1,bzz) != 1)
       utility::Error("%s%s%sSave",
       Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   fclose(bzz);
   }

//   ===========================================================
//   *********************   Norms   ***************************
//   ===========================================================

#if 0

//   ***********************< NormT >***************************
//   * Purpose: Total matrix norm  (n times the max of aij)    *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::NormT(void)
   {
   D norm=0.,aus;
   D *w=matrix[0];
   for(int i=1;i<size;i++)
      if((aus=Abs(*++w))>norm)norm=aus;
   return (D)numRows*norm;
   }

//   ***********************< NormR >***************************
//   * Purpose: Max of the sums of the abs values              *
//   *          of coeff. rows                                 *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::NormR(void)
   {
   D norm=0.,aus;
   D *w=matrix[0];
   for(int i=1;i<=numRows;i++)
      {
      aus=0.;
      for(int j=i;j<=numRows;j++)aus+=Abs(*++w);
      if(aus>norm)norm=aus;
      }
   return norm;
   }

//   ***********************< NormC >***************************
//   * Purpose: Norm C                                         *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::NormC(void)
   {
   D norm=0.,aus;
   norm=0.;
   for(int j=1;j<=numRows;j++)
      {
      aus=0.;
      for(int i=1;i<=j;i++)aus+=Abs(matrix[i][j]);
      if(aus>norm)norm=aus;
      }
   return norm;
   }

//   ***********************< NormF >***************************
//   * Purpose: Calculating Frobenius' norm                    *
//   * Description: Uses the functions SqrtSumSqr              *
//   * Example: T normF = R.NormF();                        *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::NormF(void)
   {
   return SqrtSumSqr(size-1,matrix[0]+1);
   }

#endif  // eliminadas todas as normas
//   ===========================================================
//   *                 Modifying functions                     *
//   ===========================================================

//   **********************< Delete >***************************
//   * Purpose: Eliminating a matrix of no use                 *
//   *          without it leaving the scope                   *
//   * Example: Delete(&A);                                    *
//   ***********************************************************
template <class D>
void Delete(Te_MatrixRight<D> *result)
   {
   result->Deinitialize();
   result->size = result->numRows = result->numColumns= 0;
   result->matrix = NULL;
   }

//   ***************< ChangeDimensions >************************
//   * Purpose: Changing the dimensions of a Te_MatrixRight<D>       *
//   * Example: ChangeDimensions(row,columns,&A);              *
//   ***********************************************************
template <class D>
void ChangeDimensions(int rows,int columns,
                    Te_MatrixRight<D> *result,char zero)
   {
   if(rows != columns)
       utility::Error("%s%s%sChangeDimensions",
       Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   int who = result->whoAmI;
   if(rows != result->numRows ||
      columns != result->numColumns)
        {
        result->Deinitialize();
        result->Initialize(rows,columns);
        Te_MatrixRight<D>::count--;
        }
   result->whoAmI = who;
   if(zero == 0 && result->size != 0)
      memset(result->matrix[0],0,
            result->size*sizeof(D));
   }

//   **********************< Recover >**************************
//   * Purpose: Recovering a matrix stored with                *
//   *          Save(file name) formatted                      *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved matrix             *
//   * Example: Recover(&x,file name);                         *
//   ***********************************************************
template <class D>
void Recover(Te_MatrixRight<D> *R,char *filematrix) {
   if((utility::bzzFileIn = fopen(filematrix,"r")) == NULL)
      utility::Error("%s%s%s%sRecover",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   int rows,columns;
   fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
   ChangeDimensions(rows,columns,R,1);
   D *w = R->matrix[0];
   for(int i = 1;i < R->size;i++)
      fscanf(utility::bzzFileIn,"%f",++w);
   fclose(utility::bzzFileIn);
}

//   **********************< Recover >**************************
//   * Purpose: Recovering a matrix stored by                  *
//   *          Save('*', file name) unformatted               *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved matrix             *
//   * Example: Recover(&A,'*',file name);                     *
//   ***********************************************************
template <class D>
void Recover(Te_MatrixRight<D> *R,char,char *filematrix) {
   if((utility::bzzFileIn = fopen(filematrix,"rb")) == NULL)
      utility::Error("%s%s%s%sFILE",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_CONSTRUCTOR);
   int rows,columns;
   if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
      utility::Error("%sFILE",Te_MatrixRight<D>::ERRORMESSAGE);
   if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
      utility::Error("%sFILE",Te_MatrixRight<D>::ERRORMESSAGE);
   ChangeDimensions(rows,columns,R,1);
   if(rows < 1 || columns < 1) {
      fclose(utility::bzzFileIn);
      return;
   }
   if(fread(R->matrix[1],sizeof(D)*(R->size),1,utility::bzzFileIn) != 1)
      utility::Error("%s%s%sRecover",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
   fclose(utility::bzzFileIn);
}


//   **********************< Inverse >**************************
//   * Purpose: Executing the inverse of aTe_MatrixRight       *
//   *          The inverse will also be a Te_MatrixRight<D>   *
//   * Example: Inverse(&R);                                   *
//   ***********************************************************
template <class D>
char Inverse(Te_MatrixRight<D> *R) {
   int row,column,k,ok=1;
   D **r = R->matrix;
   D *v = R->matrix[1];
   D *w;
   int n = R->numRows;
   D sum;
   for(row = 1;row <= n;row++) {
      if(r[row][row] == 0.) {
         utility::Message("\nSingular matrix in Te_MatrixRight<D>::Inverse\n");
         ok = 0;
         r[row][row] = 1.;
         for(column = row+1;column <= n;column++)
            r[row][column] = 0.;
      } else r[row][row] = 1./r[row][row];
   }
   for(row = 1;row <= n-1;row++) {
      v++;
      for(column = row+1;column <= n;column++) {
        w = R->matrix[row] + row;
        sum = 0.;
        for(k = row;k <= column-1;k++)
           sum += *w++ * r[k][column];
        *++v = Control(-sum * r[column][column]);
        }
      }
   return ok;
   }

//   ************************< Swap >***************************
//   * Purpose: Swapping the elements of any two Matrices      *
//   * Description: Provides an efficient method of copying    *
//   * Example: Swap(&A,&B);                                   *
//   ***********************************************************
template <class D>
void Swap(Te_MatrixRight<D> *lval,Te_MatrixRight<D> *rval)
   {
   D **temp = lval->matrix;
   lval->matrix = rval->matrix;
   rval->matrix = temp;
   Swap(&lval->numColumns,&rval->numColumns);
   Swap(&lval->numRows,&rval->numRows);
   Swap(&lval->size,&rval->size);
   }

//   ===========================================================
//   ==========   Functions for linear algebra   ===============
//   ===========================================================

//   ***********************< Solve >***************************
//   * Purpose: Solving a Right system R*bx = bx               *
//   * Description: Given the Te_Vector<D> bx it returns             *
//   *              the solution in bx                         *
//   * Example: Solve(R,&bx);                                  *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixRight<D> &R,Te_Vector<D> *bx) {
   if(R.numRows != bx->Size())
      utility::Error("%s%s%sSolve",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   D sum;
   D *r,tmp,*x = &(*bx)[0];
   D **mat = R.matrix;
   for(int i = R.numRows;i >= 1;i--)
     {
     r = mat[i] + i;
     tmp = *r;
     if(tmp == 0.)
       {
       utility::Message("%sSingular Te_Matrix<D>",Te_MatrixRight<D>::utility::ErrorMESSAGE);
       x[i]=0.;
       }
     else
       {
       sum = x[i];
       for(int j = i+1;j <= R.numColumns;j++)
          sum -= *++r * x[j];
       x[i]=Control(sum/tmp);
       }
     }
  }

//   ***********************< Solve >***************************
//   * Purpose: Solving a Right system R*b = x                 *
//   * Description: Given the Vectors b and x  it returns      *
//   *              the solution in x                          *
//   * Example:  Solve(R,b,&x);                                *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixRight<D> &R,const Te_Vector<D> &b,Te_Vector<D> *x) {
   if(b.WhoAmI() == x->WhoAmI())
      Solve(R,x);
   else {
      *x = b;
      Solve(R,x);
      }
   }

//   ***********************< Solve >***************************
//   * Purpose: Solving a Right system for a Te_Matrix<D> BX   *
//   * Description: Substitutes the solution in BX             *
//   * Example: R.Solve(&BX);                                  *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixRight<D> &R,Te_Matrix<D> *BX) {
   if(R.numRows != BX->Rows())
      utility::Error("%s%s%sSolve",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   D sum;
   D tmp,*r,**bx = BX->matrix;
   D **mat = R.matrix;
   for(int k = 1;k <= BX->Columns();k++) {
      for(int i = R.numRows;i >= 1;i--) {
         r = mat[i] + i;
         tmp = *r;
         if(tmp == 0.) {
            utility::Message("%sSingular Matrix",Te_MatrixRight<D>::ERRORMESSAGE);
            bx[i][k] = 0.;
         } else {
            sum = bx[i][k];
            for(int j = i+1;j <= R.numColumns;j++)
            sum -= *++r * bx[j][k];
            bx[i][k] = Control(sum/tmp);
         }
      }
   }
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a Right system for a Te_Matrix<D> B    *
//   * Description: Puts the solution in X                     *
//   * Example: Solve(R,B,&X);                                 *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixRight<D> &R,const Te_Matrix<D> &B,Te_Matrix<D> *X) {
   if(B.WhoAmI() == X->WhoAmI())
      Solve(R,X);
   else {
      *X = B;
      Solve(R,X);
   }
}


//   ******************< TransposeSolve >***********************
//   * Purpose: Solving a Right system RT*bx = bx              *
//   * Description: Given Te_Vector<D> bx it returns           *
//   *              the solution in bx                         *
//   * Example: TransposeSolve(R,&bx);                         *
//   ***********************************************************
template <class D>
void TransposeSolve(const Te_MatrixRight<D> &R,Te_Vector<D> *bx) {
   if(R.numRows != bx->Size())
      utility::Error("%s%s%sTransposeSolve",
         Te_MatrixRight<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   D sum;
   D tmp,*x = &(*bx)[0];
   D **mat = R.matrix;
   int n = R.numRows;
   for(int i = 1;i <= n;i++) {
      tmp = mat[i][i];
      if(tmp == 0.) {
         utility::Message("%sSingular Matrix",Te_MatrixRight<D>::ERRORMESSAGE);
         x[i]=0.;
      } else {
        sum = x[i];
          for(int j = 1;j < i;j++)
            sum -= mat[j][i] * x[j];
         x[i]=Control(sum/tmp);
      }
   }
}


//   ******************< TransposeSolve >***********************
//   * Purpose: Solving a Right system RT*b = x                *
//   * Description: Given the Vectors b and x it returns       *
//   *              the solution in x                          *
//   * Example: TransposeSolve(R,b,&x);                        *
//   ***********************************************************
template <class D>
void TransposeSolve(const Te_MatrixRight<D> &R, const Te_Vector<D> &b, Te_Vector<D> *x)
{
	if(b.WhoAmI() == x->WhoAmI())
		TransposeSolve(R,x);
	else {
		*x = b;
		TransposeSolve(R,x);
	}
}


//   ********************< Determinant >************************
//   * Purpose: Calculating the determinant                    *
//   * Example: R.Determinant();                               *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::Determinant(void)
{
	D determinant = 1.;
	for(int i = 1;i <= numRows;i++)
		determinant *= matrix[i][i];
	return determinant;
}


//   ******************< ConditionNumber >**********************
//   * Purpose: Calculating the condition number               *
//   * Example: R.ConditionNumber();                           *
//   ***********************************************************
template <class D>
D Te_MatrixRight<D>::ConditionNumber(void)
{
	double max = utility::TINY_FLOAT;
	double min = utility::BIG_FLOAT;
	for(int i = 1;i <= numRows;i++) {
		double aux = Abs(matrix[i][i]);
		if(max < aux)
			max = aux;
		if(min > aux)
			min = aux;
	}
	if(min == 0.)
		return utility::BIG_FLOAT;
	else
		return utility::Control(max/min);
}


}  //end lev_math namespace


#endif // RIGHT_HPP
