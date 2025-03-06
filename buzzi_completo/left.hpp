#ifndef LEFT_HPP
#define LEFT_HPP

//   =======================< LEFT.HPP >========================
//   * Class Te_MatrixLeft                                     *
//   * Description: Chapter 12                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   ****** Constructors for Te_MatrixLeft:                    *
//   * Te_MatrixLeft L; // default                             *
//   * Te_MatrixLeft L = left; // copy-initializer             *
//   * Te_MatrixLeft L(3,3); // sizes and places at 0          *
//   * Te_MatrixLeft L(3,3,                                    *
//   *    1.,                                                  *
//   *    2.,3.,                                               *
//   *    4.,5.,6.);// sizes and initialises                   *
//   * float x[]=                                              *
//   *     {                                                   *
//   *     1.,                                                 *
//   *     2.,3.,                                              *
//   *     4.,5.,6.                                            *
//   *     };                                                  *
//   * Te_MatrixLeft L(3,3,x); // from array                   *
//   * Te_MatrixLeft L("LEFT.DAT"); // Formatted File          *
//   * Te_MatrixLeft L('*',LEFT.BIN"); // Binary File          *
//   ***********************************************************
//   ***** Access functions :                                  *
//   *  i = L.Rows(); // numRows                               *
//   *  i = L.Columns(); // numColumns                         *
//   *  xf = L.GetValue(i);                                    *
//   *  xf = L(i,j);                                           *
//   *  L(i,j) = xf;                                           *
//   *  xf = L[i][j];                                          *
//   *  L[i][j] = xf;                                          *
//   *  L.SetValue(i,j,xf);                                    *
//   ***********************************************************
//   ***** Assignments:                                        *
//   *  L = left; // left Te_MatrixLeft                        *
//   ***********************************************************
//   *****  Operators for tests:                               *
//   * if(L1 == L2)                                            *
//   * if(L1 != L2)                                            *
//   ***********************************************************
//   ***** Implemented operations :                            *
//   * Sum(L1,L2,&L3);        // L3 = L1 + L2;                 *
//   * L3 = L1 + L2;          // L3 = L1 + L2;                 *
//   * Sum(L,R,&A);           // A = L + R;                    *
//   * Sum(R,L,&A);           // A = R + L;                    *
//   * Sum(L1,L2,&L1);        // L1 = L1 + L2;                 *
//   * Sum(&L1,L2);           // L1 = L1 + L2;                 *
//   * L1 += L2;              // L1 = L1 + L2;                 *
//   * Sum(L1,L2,&L2);        // L2 = L1 + L2;                 *
//   * Sum(L1,&L2);           // L2 = L1 + L2;                 *
//   * Sum(L1,L1,&L1);        // L1 = L1 + L1;                 *
//   * Sum(&L1);              // L1 = L1 + L1;                 *
//   * Difference(L1,L2,&L3); // L3 = L1 - L2;                 *
//   * L3 = L1 - L2;          // L3 = L1 - L2;                 *
//   * Difference(L1,L2,&L1); // L1 = L1 - L2;                 *
//   * Difference(&L1,L2);    // L1 = L1 - L2;                 *
//   * L1 -= L2;              // L1 = L1 - L2;                 *
//   * Difference(L1,L2,&L2); // L2 = L1 - L2;                 *
//   * Difference(L1,&L2);    // L2 = L1 - L2;                 *
//   * Difference(L1,L1,&L1); // L1 = L1 - L1;                 *
//   * Difference(&L1);       // L1 = L1 - L1;                 *
//   * Minus(L1,&L2);         // L2 = -L1;                     *
//   * L2 = -L1;              // L2 = -L1;                     *
//   * Minus(&L1);            // L1 = -L1;                     *
//   * Product(L1,L2,&L3);    // L3 = L1*L2;                   *
//   * L3 = L1*L2;                                             *
//   * Product(L1,L2,&L1);    // L1 = L1*L2;                   *
//   * Product(&L1,L2);       // L1 = L1*L2;                   *
//   * L1 *= L2;              // L1 = L1*L2;                   *
//   * Product(L1,L1,&L1);    // L1 = L1*L1;                   *
//   * Product(&L1);          // L1 = L1*L1;                   *
//   * Product(L1,x,&y);      // y = L1*x;                     *
//   * y = L1*x;                                               *
//   * Product(3.,L1,&L2);    // L2 = 3.*L1;                   *
//   * L2 = 3.*L1;                                             *
//   * Product(3.,&L1);       // L1 = 3.*L1;                   *
//   * L1 *= 3.;              // L1 = 3.*L1;                   *
//   ***********************************************************
//   ***** Functions for linear algebra:                       *
//   * Solve(L,b,&x);                                          *
//   * Solve(L,&bx);  else Solve(L,b,&b);                      *
//   * Solve(L,B,&X);                                          *
//   * Solve(l,&BX);  else Solve(L,B,&B);                      *
//   * TranposeSolve(L,b,&x);                                  *
//   * TransposeSolve(L,&bx); else TransposeSolve(L,b,&b);     *
//   * L.Determinant();                                        *
//   * L.ConditionNumber();                                    *
//   ***********************************************************

namespace lev_math {

// preventive declarations
template <class D> class Te_Vector;
template <class D> class Te_Matrix;
template <class D> class Te_MatrixLeft;
template <class D> class Te_MatrixRight;
template <class D> class Te_MatrixSymm;

#if 10
template <class D> char operator == (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);
template <class D> char operator != (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);
template <class D> void Sum(const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval,Te_MatrixLeft<D> *result);
template <class D> Te_MatrixLeft<D> operator + (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);
template <class D> void Sum (Te_MatrixLeft<D> &lval,Te_MatrixRight<D> &rval,Te_Matrix<D> *result);
template <class D> void Sum (Te_MatrixRight<D> &lval,Te_MatrixLeft<D> &rval,Te_Matrix<D> *result);
template <class D> void Sum (Te_MatrixLeft<D> *lvalAndResult,const Te_MatrixLeft<D> &rval);
template <class D> void Sum (const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult);
template <class D> void Sum (Te_MatrixLeft<D> *lvalRvalAndResult);
template <class D> void Difference (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval, Te_MatrixLeft<D> *result);
template <class D> Te_MatrixLeft<D> operator - (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);
template <class D> void Difference (Te_MatrixLeft<D> *lvalAndResult,const Te_MatrixLeft<D> &rval);
template <class D> void Difference (const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult);
template <class D> void Minus (const Te_MatrixLeft<D> &rval,Te_MatrixLeft<D> *result);
template <class D> Te_MatrixLeft<D> operator - (const Te_MatrixLeft<D> &rval);
template <class D> void Minus (Te_MatrixLeft<D> *rvalAndResult);
template <class D> void Product (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval, Te_MatrixLeft<D> *result);
template <class D> Te_MatrixLeft<D> operator *(const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);
template <class D> void Product (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);
template <class D> void Product (const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult);
template <class D> void Product (Te_MatrixLeft<D> *lvalRvalAndResult);
template <class D> void Product (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval, Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval);
template <class D> void Product (double lval,const Te_MatrixLeft<D> &rval, Te_MatrixLeft<D> *result);
template <class D> Te_MatrixLeft<D> operator * (double lval,const Te_MatrixLeft<D> &rval);
template <class D> void Product (double lval,Te_MatrixLeft<D> *rvalAndResult);
template <class D> void TProduct (const Te_MatrixLeft<D> &lvalAndRval,Te_MatrixSymm<D> *result);
template <class D> void ProductT (const Te_MatrixLeft<D> &lvalAndRval,Te_MatrixSymm<D> *result);
template <class D> void Division (const Te_MatrixLeft<D> &lval,double rval, Te_MatrixLeft<D> *result);
template <class D> Te_MatrixLeft<D> operator / (const Te_MatrixLeft<D> &lval,double rval);
template <class D> void Division (Te_MatrixLeft<D> *lvalAndResult,double rval);
//template <class D> void Delete(Te_MatrixLeft<D> *L);
template <class D> void ChangeDimensions(int rows,int columns, Te_MatrixLeft<D> *result,char zero);
template <class D> void Recover(Te_MatrixLeft<D> *A,char *filematrix);
template <class D> void Recover(Te_MatrixLeft<D> *A,char,char *filematrix);
template <class D> Te_MatrixLeft<D> Transpose(const Te_MatrixRight<D> &R);
template <class D> Te_MatrixRight<D> Transpose(const Te_MatrixLeft<D> &L);
template <class D> char Inverse(Te_MatrixLeft<D> *L);
template <class D> void Swap(Te_MatrixLeft<D> *lval,Te_MatrixLeft<D> *rval);
template <class D> void Solve(const Te_MatrixLeft<D> &R,Te_Vector<D> *bx);
template <class D> void Solve(const Te_MatrixLeft<D> &R,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void Solve(const Te_MatrixLeft<D> &R,Te_Matrix<D> *BX);
template <class D> void Solve(const Te_MatrixLeft<D> &R,const Te_Matrix<D> &B,Te_Matrix<D> *X);
template <class D> void TransposeSolve(const Te_MatrixLeft<D> &R,Te_Vector<D> *bx);
template <class D> void TransposeSolve(const Te_MatrixLeft<D> &R,const Te_Vector<D> &b,Te_Vector<D> *x);
#endif

//   ===========================================================
//   =================   class Te_MatrixLeft   ====================
//   ===========================================================
template <class D>
class Te_MatrixLeft {
	friend class Te_Vector<D>;
	friend class Te_Matrix<D>;
	friend class Te_MatrixSymm<D>;
	friend class Te_MatrixRight<D>;

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

	// Te_MatrixLeft A('*',3,3);
	Te_MatrixLeft(char,int rows,int columns);

public:
	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
	//   default // Te_MatrixLeft L
	Te_MatrixLeft(void);

	// copy-initializer // Te_MatrixLeft L = left;
	Te_MatrixLeft(Te_MatrixLeft<D> &rval);

	// sizes and initialises at 0
	// Te_MatrixLeft L(3,3);
	Te_MatrixLeft(int rows,int columns);

	// sizes and initialises
	// Te_MatrixLeft L(2,2,1.,2.,3.);
	Te_MatrixLeft(int rows,int columnsn,D a11,...);

	// from array // Te_MatrixLeft L(3,3,w)
	Te_MatrixLeft(int rows,int columns, D *initvalues);

	// from formatted File // Te_MatrixLeft L("LEFT.DAT");
	Te_MatrixLeft(char *filematrix);

	// from binary File // Te_MatrixLeft L('*',"LEFT.BIN");
	Te_MatrixLeft(char,char *filematrix);

	//   ===========================================================
	//   ********************   destructor   ***********************
	//   ===========================================================
	~Te_MatrixLeft(void);

	//   ===========================================================
	//   **********   Non-modifying access functions   *************
	//   ===========================================================
	//   row number
	int Rows(void) const
	{
		return numRows;
	}

	// column number
	int Columns(void) const
	{
		return numColumns;
	}

	int WhoAmI(void) const { return whoAmI; }

	// receives the values with control
	D GetValue(int row,int col) const;

	//   ===========================================================
	//   *************   Modifying access functions   **************
	//   ===========================================================
	//   assigns values with control
	void SetValue(int row,int col,D val);

	// assigns and receives vector values with control
	D &operator () (int row,int col);

	// assigns and receives vector values without control
	D *operator [](int i)
	{
		return matrix[i];
	}

	//   ===========================================================
	//   ****************  assignment operators ********************
	//   ===========================================================
	Te_MatrixLeft<D> &operator = (const Te_MatrixLeft<D> &rval);

	// A += B; A = A + B;
	Te_MatrixLeft<D> &operator += (const Te_MatrixLeft<D> &rval);

	// A -= B; A = A - B;
	Te_MatrixLeft<D> &operator -= (const Te_MatrixLeft<D> &rval);

	// A *= B; A = A*B
	Te_MatrixLeft<D> &operator *= (const Te_MatrixLeft<D> &rval);

	// A = 3.*A;
	Te_MatrixLeft<D> &operator *= (const double rval);

	// A /= 3.;
	Te_MatrixLeft<D> &operator /= (double rval);

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

	D Determinant(void);
	D ConditionNumber();

	//   ===========================================================
	//   ****************   operators for tests   ******************
	//   ===========================================================
	friend char operator == <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);

	friend char operator != <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);

	//   ===========================================================
	//   ====================   OPERATIONS   =======================
	//   ===========================================================

	//   ===========================================================
	//   ***********************   Sum   ***************************
	//   ===========================================================
	// Sum(A,B,&C); C = A + B;
	friend void Sum <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval, Te_MatrixLeft<D> *result);

	// C = A + B;
	friend Te_MatrixLeft<D> operator + <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);

	// Sum(L,R,&C); C = L + R;
	friend void Sum <> (Te_MatrixLeft<D> &lval,Te_MatrixRight<D> &rval,Te_Matrix<D> *result);

	// Sum(R,L,&C); C = R + L;
	friend void Sum <> (Te_MatrixRight<D> &lval,Te_MatrixLeft<D> &rval,Te_Matrix<D> *result);

	// Sum(&A,B); A = A + B;
	friend void Sum <> (Te_MatrixLeft<D> *lvalAndResult,const Te_MatrixLeft<D> &rval);

	// Sum(B,&A); A = B + A;
	friend void Sum <> (const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult);

	// Sum(&A); A = A + A;
	friend void Sum <> (Te_MatrixLeft<D> *lvalRvalAndResult);

	//   ===========================================================
	//   ********************   Difference   ***********************
	//   ===========================================================
	//   Difference(A,B,&C); C = A - B;
	friend void Difference <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval,Te_MatrixLeft<D> *result);

	// C = A - B;
	friend Te_MatrixLeft<D> operator - <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);

	// Difference(&A,B); A = A - B;
	friend void Difference <>(Te_MatrixLeft<D> *lvalAndResult,const Te_MatrixLeft<D> &rval);

	// Difference(B,&A); A = B - A;
	friend void Difference <> (const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult);

	//   ===========================================================
	//   ***********************   Minus   *************************
	//   ===========================================================
	friend void Minus <> (const Te_MatrixLeft<D> &rval,Te_MatrixLeft<D> *result);

	// unary
	friend Te_MatrixLeft<D> operator - <> (const Te_MatrixLeft<D> &rval);

	friend void Minus <> (Te_MatrixLeft<D> *rvalAndResult);

	//   ===========================================================
	//   **********************   Product   ************************
	//   ===========================================================
	// C=A*B;
	friend void Product <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval,Te_MatrixLeft<D> *result);

	friend Te_MatrixLeft<D> operator * <> (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval);

	// A = A * B;
	friend void Product <> (Te_Matrix<D> *lvalAndResult,const Te_Matrix<D> &rval);

	// B = A * B;
	friend void Product <> (const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult);

	// A = A * A;
	friend void Product <> (Te_MatrixLeft<D> *lvalRvalAndResult);

	// y =A*x;
	friend void Product <> (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval,Te_Vector<D> *result);

	// y = A*x;
	friend Te_Vector<D> operator * <> (const Te_MatrixLeft<D> &lval,const Te_Vector<D> &rval);

	// A = 3.*B;
	friend void Product <> (double lval,const Te_MatrixLeft<D> &rval,Te_MatrixLeft<D> *result);

	// A = 3.*B;
	friend Te_MatrixLeft<D> operator * <> (double lval,const Te_MatrixLeft<D> &rval);

	// A = 3.* A;
	friend void Product <> (double lval,Te_MatrixLeft<D> *rvalAndResult);

	//   ===========================================================
	//   *********************   TProduct   ************************
	//   ===========================================================
	friend void TProduct <> (const Te_MatrixLeft<D> &lvalAndRval,Te_MatrixSymm<D> *result);

	//   ===========================================================
	//   *********************   ProductT   ************************
	//   ===========================================================
	friend void ProductT <> (const Te_MatrixLeft<D> &lvalAndRval,Te_MatrixSymm<D> *result);

	//   ===========================================================
	//   **********************   Division   ***********************
	//   ===========================================================
	// A =B/3.;
	friend void Division <> (const Te_MatrixLeft<D> &lval,double rval,Te_MatrixLeft<D> *result);

	// A = B/3.;
	friend Te_MatrixLeft<D> operator / <> (const Te_MatrixLeft<D> &lval,double rval);

	// A /= 3.;
	friend void Division <> (Te_MatrixLeft<D> *lvalAndResult,double rval);

	//   ===========================================================
	//   =============   Modifying Functions =======================
	//   ===========================================================
	friend void ChangeDimensions <> (int rows,int columns,Te_MatrixLeft<D> *result,char zero);

	// from formatted file
	friend void Recover <> (Te_MatrixLeft<D> *A,char *filematrix);
	// from binary file
	friend void Recover <> (Te_MatrixLeft<D> *A,char,char *filematrix);
	friend Te_MatrixLeft<D> Transpose <> (const Te_MatrixRight<D> &R);
	friend Te_MatrixRight<D> Transpose <> (const Te_MatrixLeft<D> &L);
	friend char Inverse <> (Te_MatrixLeft<D> *L);
	friend void Swap <> (Te_MatrixLeft<D> *lval,Te_MatrixLeft<D> *rval);

	//   ===========================================================
	//   ==========   Functions for linear algebra  ================
	//   ===========================================================
	friend void Solve <> (const Te_MatrixLeft<D> &R,Te_Vector<D> *bx);
	friend void Solve <> (const Te_MatrixLeft<D> &R,const Te_Vector<D> &b, Te_Vector<D> *x);
	friend void Solve <> (const Te_MatrixLeft<D> &R,Te_Matrix<D> *BX);
	friend void Solve <> (const Te_MatrixLeft<D> &R,const Te_Matrix<D> &B, Te_Matrix<D> *X);
	friend void TransposeSolve <>(const Te_MatrixLeft<D> &R, Te_Vector<D> *bx);
	friend void TransposeSolve <>(const Te_MatrixLeft<D> &R, const Te_Vector<D> &b,Te_Vector<D> *x);
};


template <class D>
const char *const Te_MatrixLeft<D>::ERRORMESSAGE = "\n========>>> Te_MatrixLeft error!!!!!!\n";
template <class D>
int Te_MatrixLeft<D>::count = 0;


//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   ********************< Initialize >*************************
//   * Purpose: Common function for initialising constructors  *
//   ***********************************************************
template <class D>
void Te_MatrixLeft<D>::Initialize(int rows,int columns)
{
	count++;
	whoAmI = count;
	if(rows < 1 || columns < 1) {
		numRows = numColumns = size = 0;
		matrix = NULL;
		return;
	}
	if(rows != columns)
		lev_math::utility::Error("%s%s%sInitialize", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
	numRows = numColumns = rows;
	size = numRows*(numRows+1)/2+1;
	matrix = new D *[numRows+1];
	if(!matrix)
		lev_math::utility::Error("%s%s%sInitialize", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
	matrix[0] = new D[size];
	if(!matrix[0])
		lev_math::utility::Error("%s%s%sInitialize", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
	matrix[1] = matrix[0];
	for(int i = 2;i <= numRows;i++)
		matrix[i] = &matrix[i-1][i-1];
}


//   **********************< Deinitialize >*********************
//   * Purpose: Common function for deinitialisation           *
//   ***********************************************************
template <class D>
void Te_MatrixLeft<D>::Deinitialize(void)
{
	if(matrix) {
		delete [] matrix[0]; matrix[0] = NULL;
		delete [] matrix; matrix = NULL;
	}
}


//   ****************< private constructor >*********************
//   * Purpose: Sets size of a Te_MatrixLeft<D> without         *
//   *          initialisation. Serving internally              *
//   * Example: Te_MatrixLeft<D> L('*',4,4);                    *
//   ************************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(char,int rows,int columns)
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
//   * Purpose: Defining a Te_MatrixLeft<D> which can then     *
//   *          receive an assignment                          *
//   * Examples: Te_MatrixLeft<D> L;                           *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(void)
{
	Initialize(0,0);
}


//   ****************< Copy-Initializer >***********************
//   * Purpose: Defining and initialising from                 *
//   *          Te_MatrixLeft<D> Used in returns               *
//   * Examples: Te_MatrixLeft<D> A=B;                         *
//   *           Te_MatrixLeft<D> A=B+2.*C;                    *
//   *           return A; (used implicitly)                   *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(Te_MatrixLeft<D> &rval)
{
	Initialize(rval.numRows,rval.numRows);
	if(numRows != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
}


//   *************< sizing constructor >************************
//   * Purpose: Setting size and initialising to 0 a           *
//   *          Te_MatrixLeft<D> Must then be assigned other   *
//   *          values                                         *
//   * Example: Te_MatrixLeft<D> A(5,5);                       *
//   *         A(1,1)=3.; A(2,1)=7.;....                       *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(int mrows,int ncolumns)
{
	Initialize(mrows,ncolumns);
	if(numRows != 0)
		memset(matrix[0],0,size*sizeof(D));
}


//   *************< sizing and initialising  >******************
//   * Purpose: Initialising a Te_MatrixLeft<D>                *
//   * Example: Te_MatrixLeft<D> A(2,2,1.,2.,3.);              *
//   ***********************************************************
#if 0
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(int rows,int columns,double a11,...)
{
	Initialize(rows,columns);
	D *w=matrix[0] + 1;
	va_list puntaList;
	va_start(puntaList,a11);
	int i;
	*w = Control(a11);
	for(i=2; i<size; i++)
		*++w = Control(va_arg(puntaList,double));
	va_end(puntaList);
}
#endif


//   *********************< from array >************************
//   * Purpose: Initialising a Te_MatrixLeft<D> from an array  *
//   * Example: D w[]={1.,                                     *
//   *                     2.,3.,                              *
//   *                     4.,5.,6.};                          *
//   *          Te_MatrixLeft<D> L(3,3,w);                     *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(int mrows,int ncolumns,D *initvalues)
{
	Initialize(mrows,ncolumns);
	D *w=matrix[0];
	++w;
	if(numRows != 0)
		memcpy(w,initvalues,(size-1)*sizeof(D));
}


//   ****************< from formatted FILE >********************
//   * Purpose: Initialising a Te_MatrixLeft<D> from FILE      *
//   * Example: Te_MatrixLeft<D> A("LEFT.DAT");                *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(char *filematrix)
{
	if((utility::bzzFileIn=fopen(filematrix,"r"))==NULL)
		lev_math::utility::Error("%s%s %s%sConstructor", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
	int rows,columns;
	fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
	if(rows < 1 || columns < 1)
		lev_math::utility::Error("%s%s%sFILE", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
	Initialize(rows,columns);
	D *w = matrix[0];
	for(int i = 1;i < size;i++)
		fscanf(utility::bzzFileIn,"%f",++w);
	fclose(utility::bzzFileIn);
}


//   ****************< from binary FILE >***********************
//   * constructor from FILE obtained by Save('*',file name)   *
//   * Purpose: Initialising a Te_MatrixLeft<D> from           *
//   *                      unformatted FILE                   *
//   * Example: Te_MatrixLeft<D> A('*',"LEFT.DAT");            *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D>::Te_MatrixLeft(char,char *filematrix)
{
	if((utility::bzzFileIn=fopen(filematrix,"rb"))==NULL)
		lev_math::utility::Error("%s%s %s%sConstructor", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
	int rows,columns;
	if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
		lev_math::utility::Error("%sFILE",Te_MatrixLeft<D>::ERRORMESSAGE);
	if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
		lev_math::utility::Error("%sFILE",Te_MatrixLeft<D>::ERRORMESSAGE);
	if(rows < 1 || columns < 1)
		lev_math::utility::Error("%s%s%sFILE", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_CONSTRUCTOR);
	Initialize(rows,columns);
	if(fread(matrix[1],sizeof(D)*size,1,utility::bzzFileIn) != 1)
		lev_math::utility::Error("%sFILE",Te_MatrixLeft<D>::ERRORMESSAGE);
	fclose(utility::bzzFileIn);
}


//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================
template <class D>
Te_MatrixLeft<D>::~Te_MatrixLeft(void)
{
	Deinitialize();
}


//   ===========================================================
//   ************  Non-modifying access functions    ***********
//   ===========================================================

//   *********************< GetValue >**************************
//   * Purpose: Receiving values with control                  *
//   * Example: T xf = L.GetValue(i,j);                        *
//   ***********************************************************
template <class D>
D Te_MatrixLeft<D>::GetValue(int row,int col) const
{
	if(row < 1 || row > numRows)
		lev_math::utility::Error("%s%s%sGetValue", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(col < 1 || col > numRows)
		lev_math::utility::Error("%s%s%sGetValue", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(row < col)
		return 0.;
	return matrix[row][col];
}


//   ===========================================================
//   ************    Modifying access functions ****************
//   ===========================================================

//   *********************< SetValue >**************************
//   * Purpose: Assigning values with control                  *
//   * Example: L.SetValue(i,j,3.1);                           *
//   ***********************************************************
template <class D>
void Te_MatrixLeft<D>::SetValue(int row,int col,D val)
{
	if(row < 1 || row > numRows)
		lev_math::utility::Error("%s%s%sSetValue", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(col < 1 || col > numRows)
		lev_math::utility::Error("%s%s%sSetValue", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(row < col)
		lev_math::utility::Error("%s%s%sSetValue", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	matrix[row][col]=val;
}


//   ********************< operator () >************************
//   * Purpose: Receiving and assigning values with control    *
//   * Example: x = A(1,5); A(3,7) = 5.;                       *
//   ***********************************************************
template <class D>
D &Te_MatrixLeft<D>::operator () (int row,int col)
{
    if( (row < 1 || row > numRows) || (col < 1 || col > row) )
		lev_math::utility::Error("%s%s%s()", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_OPERATOR);
	return matrix[row][col];
}


//   ===========================================================
//   ***************   assignment operators   ******************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment                                     *
//   * Example: A=B;   // simple                               *
//   *          A=C+D;    // with operations                   *
//   *          A=B=C+D;  // multiple                          *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> &Te_MatrixLeft<D>::operator =(const Te_MatrixLeft<D> &rval)
{
	int who = whoAmI;
	if(numRows != rval.numRows) {
		Deinitialize();
		Initialize(rval.numRows,rval.numRows);
		count --;
	}
	whoAmI = who;
	if(numRows != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
	return *this;
}


//   ===========================================================
//   ****************   operators for tests   ******************
//   ===========================================================

//   ************************< == >*****************************
//   * Purpose: Checking if two Te_MatrixLefts are identical   *
//   * Description: Returns 1 if they are identical            *
//   * Example: if(A==B)...                                    *
//   ***********************************************************
template <class D>
char operator == (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval)
{
	if(lval.whoAmI == rval.whoAmI)
		return 1;
	char ch=1;
	if(lval.numRows != rval.numRows)
		ch = 0;
	else {
		if(memcmp(lval.matrix[0]+1,rval.matrix[0]+1,(rval.size-1)*sizeof(D))==0)
			ch=1;
		else
			ch = 0;
	}
	return ch;
}


//   ************************< != >*****************************
//   * Purpose: Checking if two Te_MatrixLefts are different   *
//   * Description: Returns 1 if they are different            *
//   * Example: if(A!=B)...                                    *
//   ***********************************************************
template <class D>
char operator != (const Te_MatrixLeft<D> &lval, const Te_MatrixLeft<D> &rval)
{
	if(lval.whoAmI == rval.whoAmI)
		return 0;
	char ch=0;
	if(lval.numRows!=rval.numRows)
		ch=1;
	else
		ch = memcmp(lval.matrix[0]+1,rval.matrix[0]+1,(rval.size-1)*sizeof(D));
	return ch;
}


//   ===========================================================
//   ====================   OPERATOINS   =======================
//   ===========================================================

//   ===========================================================
//   ***********************   Sum   ***************************
//   ===========================================================

//   ************************< Sum >****************************
//   * Purpose: Addition of Left matrices                      *
//   * Description: Uses Sum from utility.cpp                  *
//   * Example: Sum(A,B,&C); C = A + B;                        *
//   ***********************************************************
template <class D>
void Sum(const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval, Te_MatrixLeft<D> *result)
{
	if(lval.numRows != rval.numRows)
		lev_math::utility::Error("%s%s%s+", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
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
//   * Purpose: Addition of two Left matrices                  *
//   * Description: Uses Sum                                   *
//   * Example: C = A + B;                                     *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> operator + (const Te_MatrixLeft<D> &lval, const Te_MatrixLeft<D> &rval)
{
	Te_MatrixLeft<D> result('*',lval.numRows,lval.numRows);
	Sum(lval,rval,&result);
	return result;
}


//   ************************< Sum >****************************
//   * Purpose: Addition between Left matrices                 *
//   * Description: The result goes in the left matrix         *
//   * Example: Sum(&A,B); A += B;                             *
//   ***********************************************************
template <class D>
void Sum(Te_MatrixLeft<D> *lvalAndResult,const Te_MatrixLeft<D> &rval)
{
	(*lvalAndResult) += rval;
}


//   ************************< += >*****************************
//   * Purpose: Addition between Left matrices                 *
//   * Example: A += B;                                        *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> &Te_MatrixLeft<D>::operator += (const Te_MatrixLeft<D> &rval)
{
	if(numRows != rval.numRows)
		lev_math::utility::Error("%s%s%s+", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
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
void Sum(const Te_MatrixLeft<D> &lval,Te_MatrixLeft<D> *rvalAndResult)
{
	(*rvalAndResult) += lval;
}


//   ************************< Sum >****************************
//   * Purpose: Addition between identical matrices            *
//   * Description: The result replaces the matrix             *
//   * Example: Sum(&A); A = A + A;                            *
//   ***********************************************************
template <class D>
void Sum(Te_MatrixLeft<D> *lvalRvalAndResult)
{
	Sum(lvalRvalAndResult->size-1, lvalRvalAndResult->matrix[1]+1);
}


//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================

//   *************************< - >*****************************
//   * Purpose: Difference between two Left matrices           *
//   * Example: A=B-C;                                         *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> operator - (const Te_MatrixLeft<D> &lval,const Te_MatrixLeft<D> &rval)
{
	if(lval.numRows != rval.numRows)
		lev_math::utility::Error("%s%s%s-", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
	Te_MatrixLeft<D> result('*',lval.numRows,lval.numRows);
	Difference(rval.size,lval.matrix[1],rval.matrix[1],result.matrix[1]);
	return result;
}


//   ************************< -= >*****************************
//   * Purpose: Difference between two Left matrices           *
//   * Example: A -= B;                                        *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> &Te_MatrixLeft<D>::operator -= (const Te_MatrixLeft<D> &rval)
{
	if(numRows != rval.numRows)
		lev_math::utility::Error("%s%s%s-", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
	Difference(rval.size,matrix[1],rval.matrix[1]);
	return *this;
}


//   ===========================================================
//   ***********************   Minus   *************************
//   ===========================================================

//   *************************< - >*****************************
//   * Purpose: Unary minus                                    *
//   * Example: A=-B;                                          *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> operator -(const Te_MatrixLeft<D> &rval)
{
	Te_MatrixLeft<D> result('*',rval.numRows,rval.numRows);
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
//   * Purpose: Product of two Left matrices                   *
//   * Example: C = A*B;                                       *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> operator * (const Te_MatrixLeft<D> &lval, const Te_MatrixLeft<D> &rval)
{
	if(lval.numRows != rval.numRows)
		lev_math::utility::Error("%s%s%s*", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
	Te_MatrixLeft<D> result('*',lval.numRows,lval.numRows);
	D **b=rval.matrix;
	D *w;
	D *v=result.matrix[1];
	for(int row = 1;row <= lval.numRows;row++) {
		w = lval.matrix[row];
		for(int column = 1;column <= row;column++) {
			D sum = 0.0;
			for(int i = column;i <= row;i++)
				sum += *(w + i) * b[i][column];
			*++v = Control(sum);
		}
	}
	return result;
}


//   *************************< * >*****************************
//   * Purpose: Product of a Left matrix with a vector         *
//   * Example: y = L*x;                                       *
//   ***********************************************************
template <class D>
Te_Vector<D> operator * (const Te_MatrixLeft<D> &lval, const Te_Vector<D> &rval)
{
	if(lval.numColumns != rval.dimensions)
		lev_math::utility::Error("%s%s%s*", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
	Te_Vector<D> result('*',lval.numRows);
	D *r = result.vector;
	for(int i = 1;i <= lval.numRows;i++)
		r[i] = Dot(i,lval.matrix[i] +1,rval.vector + 1);
	return result;
}


//   *************************< * >*****************************
//   * Purpose: Product with a T                               *
//   * Example: C = 3.*B;                                      *
//   ***********************************************************
template <class D>
Te_MatrixLeft<D> operator * (D lval,const Te_MatrixLeft<D> &rval)
{
	Te_MatrixLeft<D> result('*',rval.numRows,rval.numColumns);
	Product(rval.size - 1,lval,rval.matrix[1]+1,result.matrix[1]+1);
	return result;
}


//   ************************< *= >*****************************
//   * Purpose: Product with a T                               *
//   * Example: L *= 3.;                                       *
//   ***********************************************************
#if 0
template <class D>
Te_MatrixLeft<D> &Te_MatrixLeft<D>::operator *= (const D rval)
{
	Product(size - 1,rval,matrix[1] + 1);//in UTILITY.CPP
	return *this;
}
#endif


//   ===========================================================
//   *********************   TProduct   ************************
//   ===========================================================

//   **********************< TProduct >*************************
//   * Purpose: Product of a transposed left matrix            *
//   *          multiplied by itself                           *
//   * Description: The result is Te_MatrixSymm                *
//   * Example: TProduct(L,&S);                                *
//   ***********************************************************
template <class D>
void TProduct(const Te_MatrixLeft<D> &lvalAndRval,Te_MatrixSymm<D> *result)
{
	D **r = lvalAndRval.matrix;
	int n = lvalAndRval.numRows;
	int i, j, k;
	ChangeDimensions(n,n,result,1);
	D **s = result->matrix;
	for(i=1; i<=n; i++) {
		for(j=1; j<=i; j++) {
			D sum = 0.;
			for(k=i; k<=n; k++)
				sum += r[k][i]*r[k][j];
			s[i][j] = Control(sum);
		}
	}
}


//   ===========================================================
//   *********************   ProductT   ************************
//   ===========================================================

//   **********************< ProductT >*************************
//   * Purpose: Product of a left matrix                       *
//   *          with its transpose                             *
//   * Description: The result is Te_MatrixSymm                *
//   * Example: ProductT(L,&S);                                *
//   ***********************************************************
template <class D>
void ProductT(const Te_MatrixLeft<D> &lvalAndRval,Te_MatrixSymm<D> *result)
{
	D **r = lvalAndRval.matrix;
	int n = lvalAndRval.numRows;
	int i, j, k;
	ChangeDimensions(n,n,result,1);
	D **s = result->matrix;
	for(i=1; i<=n; i++)
		for(j=1; j<=i; j++) {
			D sum = 0.;
			for(k=1; k<=i && k<=j; k++)
				sum += r[i][k]*r[j][k];
			s[i][j] = Control(sum);
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
Te_MatrixLeft<D> operator / (const Te_MatrixLeft<D> &lval,D rval)
{
	Te_MatrixLeft<D> result('*',lval.numRows,lval.numColumns);
	Division(lval.size - 1,lval.matrix[1]+1,rval,result.matrix[1]+1);
	return result;
}


//   ************************< /= >*****************************
//   * Purpose: Dividing by a T                                *
//   * Example: L /= L;                                        *
//   ***********************************************************
#if 0
template <class D>
Te_MatrixLeft<D> &Te_MatrixLeft<D>::operator /= (D rval)
{
	Division(size - 1,matrix[1] + 1,rval);//in UTILITY.CPP
	return *this;
}
#endif


//   ===========================================================
//   *              Non-modifying functions                    *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Printing                                       *
//   ***********************************************************
template <class D>
void Te_MatrixLeft<D>::Print(char *msg)
{
	fprintf(utility::bzzFileOut,"\nTe_MatrixLeft No.%d",whoAmI);
	if(*msg)
		fprintf(utility::bzzFileOut,"  %s\n",msg);
	fprintf(utility::bzzFileOut,"\nrows %d cols %d\n",numRows,numRows);
	for(int row = 1;row <= numRows;row++) {
		for(int column=1;column <= numRows;column++)
			if(column <= row)
				fprintf(utility::bzzFileOut," %12.4e",matrix[row][column]);
			else
				fprintf(utility::bzzFileOut," %12.4e",0.);
		fprintf(utility::bzzFileOut,"\n");
	}
}


//   ************************< Save >***************************
//   * Purpose: Saving  a Te_MatrixLeft<D> on formatted file   *
//   * Description: Can be read.                               *
//   *              See TempFile in utility                    *
//   * Example: A.Save("Mat.Dat");                             *
//   ***********************************************************
template <class D>
void Te_MatrixLeft<D>::Save(char *filematrix)
{
	FILE *bzz;
	if((bzz = fopen(filematrix,"w")) == NULL)
		lev_math::utility::Error("%s%s %s%sSave", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
	fprintf(bzz," %d %d\n",numRows,numColumns);
	for(int row = 1;row <= numRows;row++) {
		for(int column = 1;column <= row;column++)
			fprintf(bzz," %15.7e",matrix[row][column]);
		fprintf(bzz,"\n");
	}
	fclose(bzz);
}


//   ************************< Save >*****************************
//   * Purpose: Saving a Te_MatrixLeft<D> on an unformatted file *
//   * Description: Can be read                                  *
//   * Example: A.Save('*',MAT.BIN");                            *
//   *          See also TempFile from utility.cpp               *
//   *************************************************************
template <class D>
void Te_MatrixLeft<D>::Save(char,char *filematrix)
{
	FILE *bzz;
	if((bzz = fopen(filematrix,"wb")) == NULL)
		lev_math::utility::Error("%s%s %s%sSave", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
	if(fwrite(&numRows,sizeof(int),1,bzz) != 1)
		lev_math::utility::Error("%s%s%sSave", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
	if(fwrite(&numColumns,sizeof(int),1,bzz) != 1)
		lev_math::utility::Error("%s%s%sSave", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
	if(fwrite(matrix[1],sizeof(D)*size,1,bzz) != 1)
		lev_math::utility::Error("%s%s%sSave", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
	fclose(bzz);
}


//   ===========================================================
//   *********************   Norms   ***************************
//   ===========================================================
//   ***********************< NormT >***************************
//   * Purpose: Total matrix norm  (n times the max of aij)    *
//   ***********************************************************
#if 0
template <class D>
D Te_MatrixLeft<D>::NormD(void)
{
	D norm=0.,aus;
	D *w = matrix[0];
	for(int i = 1;i < size;i++)
		if((aus=Abs(*++w))>norm)
			norm=aus;
	return (D)numRows*norm;
}


//   ***********************< NormR >***************************
//   * Purpose: Max of the sums of the abs values              *
//   *          of coeff. rows                                 *
//   ***********************************************************
template <class D>
D Te_MatrixLeft<D>::NormR(void)
{
	D norm=0.,aus;
	D *w=matrix[0];
	for(int i = 1;i <= numRows;i++) {
		aus=0.;
		for(int j = 1;j <= i;j++)
			aus+=Abs(*++w);
		if(aus>norm)norm=aus;
	}
	return norm;
}


//   ***********************< NormC >***************************
//   * Purpose: Norm C                                         *
//   ***********************************************************
template <class D>
D Te_MatrixLeft<D>::NormC(void)
{
	D norm=0.,aus;
	norm=0.;
	for(int j=1;j<=numRows;j++) {
		aus=0.;
		for(int i=j;i<=numRows;i++)
			aus+=Abs(matrix[i][j]);
		if(aus>norm)
			norm=aus;
	}
	return norm;
}


//   ***********************< NormF >***************************
//   * Purpose: Calculating Frobenius' norm                    *
//   * Description: Uses the functions SqrtSumSqr              *
//   * Example: T normF = L.NormF();                           *
//   ***********************************************************
template <class D>
D Te_MatrixLeft<D>::NormF(void)
{
	return SqrtSumSqr(size-1,matrix[0]+1);
}
#endif


//   ===========================================================
//   *                 Modifying functions                     *
//   ===========================================================

//   **********************< Delete >***************************
//   * Purpose: Eliminating a matrix of no use                 *
//   *          without leaving the scope                      *
//   * Example: Delete(&A);                                    *
//   ***********************************************************
template <class D>
void Delete(Te_MatrixLeft<D> *result)
{
	result->Deinitialize();
	result->size = result->numRows = result->numColumns= 0;
	result->matrix = 0;
}


//   ***************< ChangeDimensions >************************
//   * Purpose: Changing the dimensions of a Te_MatrixLeft<D>  *
//   * Example: ChangeDimensions(row,columns,&A);              *
//   ***********************************************************
template <class D>
void ChangeDimensions(int rows,int columns, Te_MatrixLeft<D> *result,char zero)
{
	if(rows != columns)
		lev_math::utility::Error("%s%s%sChangeDimensions", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
	int who = result->whoAmI;
	if(rows != result->numRows || columns != result->numColumns) {
		result->Deinitialize();
		result->Initialize(rows,columns);
		Te_MatrixLeft<D>::count--;
	}
	result->whoAmI = who;
	if(zero == 0 && result->size != 0)
		memset(result->matrix[0],0,result->size*sizeof(D));
}


//   **********************< Recover >**************************
//   * Purpose: Recovering a matrix stored with                *
//   *          Save(file name) formatted                      *
//   * Description: It is not necessary to have the same       *
//   *              dimensions as the saved matrix             *
//   * Example: Recover(&x,file name);                         *
//   ***********************************************************
template <class D>
void Recover(Te_MatrixLeft<D> *L,char *filematrix)
{
	if((utility::bzzFileIn = fopen(filematrix,"r")) == NULL)
		lev_math::utility::Error("%s%s %s%sRecover", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
	int rows,columns;
	fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
	ChangeDimensions(rows,columns,L,1);
	D *w = L->matrix[0];
	for(int i = 1;i < L->size;i++)
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
void Recover(Te_MatrixLeft<D> *L,char,char *filematrix)
{
	if((utility::bzzFileIn=fopen(filematrix,"rb"))==NULL)
		lev_math::utility::Error("%s%s %s%sConstructor", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
	int rows,columns;
	if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
		lev_math::utility::Error("%sFILE",Te_MatrixLeft<D>::ERRORMESSAGE);
	if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
		lev_math::utility::Error("%sFILE",Te_MatrixLeft<D>::ERRORMESSAGE);
	ChangeDimensions(rows,columns,L,1);
	if(rows < 1 || columns < 1) {
		fclose(utility::bzzFileIn);
		return;
	}
	if(fread(L->matrix[1],sizeof(D)*(L->size),1,utility::bzzFileIn) != 1)
		lev_math::utility::Error("%s%s%sRecover", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
	fclose(utility::bzzFileIn);
}


//   **********************< Inverse >**************************
//   * Purpose: Executing the inverse of a Te_MatrixLeft<D>    *
//   *          The inverse will also be a Te_MatrixLeft<D>    *
//   * Example: Inverse(&L);                                   *
//   ***********************************************************
template <class D>
char Inverse(Te_MatrixLeft<D> *L)
{
	int row,column,k,ok=1;
	D **r = L->matrix;
	D *v = L->matrix[1];
	D *w;
	int n = L->numRows;
	D sum;
	for(row = 1;row <= n;row++) {
		if(r[row][row] == 0.) {
			utility::Message("\nSingular matrix in Te_MatrixLeft<D>::Inverse\n");
			ok = 0;
			r[row][row] = 1.;
			for(column = 1;column < row;column++)
				r[row][column] = 0.;
		} else
			r[row][row] = 1./r[row][row];
	}
	for(row = 2;row <= n;row++) {
		v++;
		w = L->matrix[row];
		for(column = 1;column < row;column++) {
			sum = 0.;
			for(k = column;k < row;k++)
				sum += *(w + k) * r[k][column];
			*++v = Control(-sum * r[row][row]);
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
void Swap(Te_MatrixLeft<D> *lval,Te_MatrixLeft<D> *rval)
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
//   * Purpose: Solving a Left system L*bx = bx                *
//   * Description: Given Te_Vector<D> bx returns              *
//   *              the solution in bx                         *
//   * Example: L.Solve(&bx);                                  *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixLeft<D> &L,Te_Vector<D> *bx)
{
	if(L.numRows != bx->Size())
		lev_math::utility::Error("%s%s%sSolve", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
	D sum;
	D *r,tmp,*x = &(*bx)[0];
	D **mat = L.matrix;
	for(int i = 1;i <= L.numRows;i++) {
		r = mat[i];
		tmp = *(r + i);
		if(tmp == 0.) {
			Message("%sSingular Matrix",Te_MatrixLeft<D>::ERRORMESSAGE);
			x[i]=0.;
		} else {
			sum = x[i];
			for(int j = 1;j < i;j++)
				sum -= *++r * x[j];
			x[i]=Control(sum/tmp);
		}
	}
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a Left system L*b = x                  *
//   * Description: Given the Te_Vector<D> b and x return      *
//   *              the solution in x                          *
//   * Example: Solve(L,b,&x);                                 *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixLeft<D> &L,const Te_Vector<D> &b,Te_Vector<D> *x)
{
	if(b.WhoAmI() == x->WhoAmI())
		Solve(L,x);
	else {
		*x = b;
		Solve(L,x);
	}
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a Left system for a Te_Matrix<D> BX    *
//   * Description: Substitutes the solution in BX             *
//   * Example: Solve(L,&BX);                                  *
//   ***********************************************************
template <class D>
void Solve(const Te_MatrixLeft<D> &L,Te_Matrix<D> *BX)
{
	if(L.numRows != BX->numRows)
		lev_math::utility::Error("%s%s%sSolve", Te_MatrixLeft<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
	int i, j, k;
	D sum;
	D tmp,*r,**bx = BX->matrix;
	D **mat = L.matrix;
	for(k=1; k<=BX->numColumns; k++) {
		for(i=1; i<=L.numRows; i++) {
			r = mat[i];
			tmp = *(r + i);
			if(tmp == 0.) {
				Message("%sSingular Te_Matrix<D>",Te_MatrixLeft<D>::ERRORMESSAGE);
				bx[i][k] = 0.;
			} else {
				sum = bx[i][k];
				for(j=1; j<i; j++)
					sum -= *++r * bx[j][k];
				bx[i][k] = Control(sum/tmp);
			}
		}
	}
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a Left system for a Te_Matrix<D> B     *
//   * Description: Puts the solution in X                     *
//   * Example: Solve(L,B,&X);                                 *
//   *********************************************************
template <class D>
void Solve(const Te_MatrixLeft<D> &L,const Te_Matrix<D> &B,Te_Matrix<D> *X)
{
	if(B.WhoAmI() == X->WhoAmI())
		Solve(L,X);
	else {
		*X = B;
		Solve(L,X);
	}
}


//   ********************< Determinant >************************
//   * Purpose: Calculating the determinant                    *
//   * Example: L.Determinant();                               *
//   ***********************************************************
template <class D>
D Te_MatrixLeft<D>::Determinant(void)
{
	D determinant = 1.;
	for(int i = 1;i <= numRows;i++)
		determinant *= matrix[i][i];
	return determinant;
}


//   ******************< ConditionNumber >**********************
//   * Purpose: Calculating the condition number               *
//   * Example: L.ConditionNumber();                           *
//   ***********************************************************
template <class D>
D Te_MatrixLeft<D>::ConditionNumber(void)
{
	double max = utility::TINY_FLOAT;
	double min = utility::BIG_FLOAT;
	for(int i = 1;i <= numRows;i++) {
		double aux = Abs(matrix[i][i]);
		if(max < aux) max = aux;
		if(min > aux) min = aux;
	}
	if(min == 0.)
		return utility::BIG_FLOAT;
	else
		return utility::Control(max/min);
}


} // End lev_math namespace

#endif // LEFT_HPP
