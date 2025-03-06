#ifndef FACTORED_HPP
#define FACTORED_HPP

//   =====================< FACTORED.HPP >======================
//   * basic class for factorisations                          *
//   * Description: Chapter 15                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   *                 factorizationStatus     matrixStatus    *
//   * initially             UNFACTORED          AVAILABLE     *
//   *                                                         *
//   * OnlySolve              FACTORED           DESTROYED     *
//   * Solve                  FACTORED           AVAILABLE     *
//   * TRansposeSolve         FACTORED           AVAILABLE     *
//   * HyperSolve             FACTORED           AVAILABLE     *
//   ***********************************************************
//   * The matrix is used for solving linear systems           *
//   * and connected problems                                  *
//   * Valid functions for all factorisations:                 *
//   * Solve(A,b,&x);                                          *
//   * Solve(A,&bx);  else  Solve(A,b,&b);                     *
//   * Solve(&A,b,&x);                                         *
//   * Solve(&A,&bx); else  Solve(&A,b,&b);                    *
//   * Solve(A,B,&X);                                          *
//   * Solve(A,&BX);  else  Solve(A,B,&B);                     *
//   * Solve(&A,B,&X);                                         *
//   * Solve(&A,&BX); else  Solve(&A,B,&B);                    *
//   * HyperSolve(A,b,&x);                                     *
//   * HyperSolve(A,&bx); or HyperSolve(A,b,&b);               *
//   * double det = A.Determinant(void)                        *
//   * double cond = A.ConditionNumber(void)                   *
//   * char s = A.Singular(void)                               *
//   * Matrix inv; A.Inverse(&inv)                             *
//   * Matrix inv = A.Inverse(void)                            *
//   ***********************************************************
//   * Valid functions for PLR factorisations:                 *
//   * TransposeSolve(A,b,&x);                                 *
//   * TransposeSolve(A,&bx);  or TransposeSolve(A,b,&b);      *
//   * TransposeSolve(&A,b,&x);                                *
//   * TransposeSolve(&A,&bx); or TransposeSolve(&A,b,&b);     *
//   ***********************************************************
//   * Can be defined as objects of the classes:               *
//   * FactoredGauss, FactoredCrout                            *
//   * FactoredQR, FactoredLQ                                  *
//   * See ESGAUSS.CPP for initialising                        *
//   ***********************************************************
//   * The coefficients of the matrix are accessible if        *
//   * matrixStatus != DESTROYED                               *
//   * double val = A(i,j);                                    *
//   * val =  A.GetValue(i,j);                                 *
//   * Vector v = A.GetRow(i);                                 *
//   * v =  A.GetColumn(j);                                    *
//   ***********************************************************
//   *The matrix can be modified                               *
//   *     only if matrixStatus != DESTROYED                   *
//   * A.SetValue(i,j,val);                                    *
//   * A(i,j) = val;                                           *
//   * A.SetRow(i,v);                                          *
//   * A.SetColumn(j,v);                                       *
//   *     if factoredStatus = FACTORED                        *
//   *     puts matrixStatus = MODIFIED                        *
//   *     and refactorised when necessary                     *
//   ***********************************************************
//   * The derived classes must have:                          *
//   * FurtherInit                                             *
//   *    initialises to 0 specific vectors (es indx)          *
//   * SpecificInitialize(void)                                *
//   *    initialises specific vectors                         *
//   * SpecificDeinitialize(void)                              *
//   *    deinitialise specific vectors                        *
//   * Factorization(void)                                     *
//   *    for factorising the factored matrix                  *
//   * Solution(Vector *bx)                                    *
//   *    solving the system and putting the solution in bx    *
//   * Solution(const Vector &b,Vector *x)                     *
//   *    solving the system and putting the solution in x     *
//   * TransposeSolution(Vector *bx)                           *
//   *    solving the system with AT                           *
//   * TransposeSolution(const Vector &b,Vector *x)            *
//   *    solving the system with AT                           *
//   * Solution(Matrix *BX)                                    *
//   *    solving the system for a matrix                      *
//   * Solution(const Matrix &B,Matrix *X)                     *
//   *    solving the system for a matrix                      *
//   * Condition(void)                                         *
//   *    calculating the condition number                     *
//   * DeterminantEvaluation(void)                             *
//   *    calculating the determinant                          *
//   ***********************************************************


#include <malloc.h>
#include <matrix.hpp>
#include <utility.hpp>
#include <vector.hpp>


template <class D> void Solve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void Solve(Factored<D> &A,Te_Vector<D> *bx);
template <class D> void Solve(Factored<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void Solve(Factored<D> *A,Te_Vector<D> *bx);
template <class D> void Solve(Factored<D> &A,const Te_Matrix<D> &B,Te_Matrix<D> *X);
template <class D> void Solve(Factored<D> &A,Te_Matrix<D> *BX);
template <class D> void Solve(Factored<D> *A,const Te_Matrix<D> &B,Te_Matrix<D> *X);
template <class D> void Solve(Factored<D> *A,Te_Matrix<D> *BX);
template <class D> void Solve(FactoredSVD<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x, int numD);
template <class D> void Solve(FactoredSVD<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x, int numD);
template <class D> void TransposeSolve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void TransposeSolve(Factored<D> &A,Te_Vector<D> *bx);
template <class D> void TransposeSolve(Factored<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void TransposeSolve(Factored<D> *A,Te_Vector<D> *bx);
template <class D> void HyperSolve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x);
template <class D> void HyperSolve(Factored<D> &A,Te_Vector<D> *bx);


//   ===========================================================
//   ==================   class Factored   =====================
//   ===========================================================

extern int fcount;

template <class D>
class Factored
{

	//   ===========================================================
	//   ==========   Functions for linear algebra   ===============
	//   ===========================================================
	friend void Solve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x);
	friend void Solve(Factored<D> &A,Te_Vector<D> *bx);
	friend void Solve(Factored<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x);
	friend void Solve(Factored<D> *A,Te_Vector<D> *bx);
	friend void Solve(Factored<D> &A,const Te_Matrix<D> &B,Te_Matrix<D> *X);
	friend void Solve(Factored<D> &A,Te_Matrix<D> *BX);
	friend void Solve(Factored<D> *A,const Te_Matrix<D> &B,Te_Matrix<D> *X);
	friend void Solve(Factored<D> *A,Te_Matrix<D> *BX);
	friend void Solve(FactoredSVD<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x, int numD);
	friend void Solve(FactoredSVD<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x, int numD);
	friend void TransposeSolve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x);
	friend void TransposeSolve(Factored<D> &A,Te_Vector<D> *bx);
	friend void TransposeSolve(Factored<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x);
	friend void TransposeSolve(Factored<D> *A,Te_Vector<D> *bx);
	friend void HyperSolve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x);
	friend void HyperSolve(Factored<D> &A,Te_Vector<D> *bx);

protected:

	enum FactorizationStatus {
		UNFACTORED,
		FACTORED
	} factorizationStatus;

	enum MatrixStatus {
		AVAILABLE,
		DESTROYED,
		MODIFIED
	} matrixStatus;

	static const char *const ERROR;
	static const char *const ERR_DESTROYED;

	D **matrix;
	D **factored;
	int numRows, numColumns;
	int size;
	int whoAmI;
	char singular;  // singular = 1 and singular
	double norm;

	// initialising constructors
	void Initialize(int mrows,int ncolumns);

	// initialise factored if requested
	void FactoredInitialize(void);

	// deinitialisation
	void Deinitialize(void);

	// deinitialise factored if requested
	void FactoredDeinitialize(void);

	// preparing assignments
	void PrepCopy(int rRows,int rColumns);

	// protected constructor type Factored A('*',3,5);
	Factored(char,int rows,int columns);

	// preparing the OnlySolve
	void PrepOnlySolve(void);

	// preparing the Solve
	void PrepSolve(void);

	// calculating the Condition norm
	void Norm(void);

	//   ===========================================================
	//   ==========   Functions for linear algebra   ===============
	//   ===========================================================
	// For each PLR factorisation, Gauss, Qr, SVD
	virtual void Factorization(void) = 0;
	virtual void SpecificInitialize(void) = 0;
	virtual void SpecificDeinitialize(void) = 0;
	virtual void Solution(const Te_Vector<D> &b,Te_Vector<D> *x) = 0;
	virtual void Solution(Te_Vector<D> *bx) = 0;
	virtual void TransposeSolution(const Te_Vector<D> &b,Te_Vector<D> *x) = 0;
	virtual void TransposeSolution(Te_Vector<D> *bx) = 0;
	virtual void Solution(const Te_Matrix<D> &B,Te_Matrix<D> *X) = 0;
	virtual void Solution(Te_Matrix<D> *BX) = 0;
	virtual D Condition(void) = 0;
	virtual D DeterminantEvaluation(void) = 0;

public:

	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
	// default constructor type Factored A;
	Factored(void);

	// copy constructor
	Factored(const Factored<D> &rval);

	// constructor A(3,5);
	Factored(int rows,int columns);

	// constructor A(3,5,w);
	Factored(int rows,int columns,D *initvalues);

	// constructor from Matrix
	Factored(const Te_Matrix<D> &rval);

	// make a submatrix with rows,columns
	Factored(int rows,int columns,const Te_Matrix<D> &rval);

	// as above, starting from irow,jcol
	Factored(int rows,int columns,int irow,int jcol,const Te_Matrix<D> &rval);

	//   ===========================================================
	//   ********************   destructor   ***********************
	//   ===========================================================
	virtual ~Factored(void) {
		if(factored != NULL) {
			delete [] factored[0]; factored[0] = NULL;
			delete [] factored; factored = NULL;

		}
		if(matrix != NULL) {
			delete [] matrix[0]; matrix[0] = NULL;
			delete [] matrix; matrix = NULL;
		}
	}

	//   ===========================================================
	//   ************   Non-modifying access functions   ***********
	//   ===========================================================
	// number of rows
	int Rows(void) const {return numRows;}

	// number of columns
	int Columns(void) const {return numColumns;}

	int WhoAmI(void) const {return whoAmI;}

	// receives values with control
	D GetValue(int row,int col) const;
	Te_Vector<D> GetRow(int i) const;  // row i of matrix
	Te_Vector<D> GetColumn(int j) const; // column j of matrix

	//   ===========================================================
	//   ************   Modifying access functions   ***************
	//   ===========================================================
	//assigning values with control
	void SetValue(int row,int column,D val);

	// assigning and receiving values of vectors with control
	D &operator () (int row,int col);

	// row i = rval
	void SetRow(int i,const Te_Vector<D> &rval);

	// column j = rval
	void SetColumn(int j,const Te_Vector<D> &rval);

	//   ===========================================================
	//   ***************  assignment operators   *******************
	//   ===========================================================
	// transforms a Matrix in Factored
	// creates an independent matrix
	virtual void CopyFromMatrix(const Te_Matrix<D> &rval) {
		PrepCopy(rval.numRows,rval.numColumns);
		if(numRows != 0)
			memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
	}

	// transforms a Matrix in Factored
	// Matrix is destroyed
	friend void CommuteMatrixToFactored(Te_Matrix<D> *lval,Factored<D> *rval);

	//   ===========================================================
	//   ===========   Non-modifying functions   ===================
	//   ===========================================================

	//   ***********************< Print >***************************
	void Print(char *msg="");

	//   ===========================================================
	//   ===========   Modifying functions   =======================
	//   ===========================================================
	friend void Delete(Factored<D> *result);
	double Determinant(void);
	double ConditionNumber(void);
	char Singular(void); // 1 if singular 0 if not singular
	void Inverse(Te_Matrix<D> *inv);
	Te_Matrix<D> Inverse(void);
};


//   ========================< FACTORED.CPP >===================
//   * basic class for factorisations                          *
//   * Description: Chapter 15                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================


template <class D>
const char *const Factored<D>::ERROR = "\n========>>> Factored error!!!!!!\n";
template <class D>
const char *const Factored<D>::ERR_DESTROYED = "Te_Matrix Destroyed";


//   ===========================================================
//   ================   protected functions   ==================
//   ===========================================================

//   *********************< Initialize >************************
//   * Purpose: Common function for initialisation             *
//   ***********************************************************
template <class D>
void Factored<D>::Initialize(int rows,int columns)
{
	fcount++;
	whoAmI = fcount;
	singular = 0;
	norm = 0.;
	factorizationStatus = UNFACTORED;
	matrixStatus = AVAILABLE;
	factored = NULL;
	if(rows < 1 || columns < 1) {
		numRows = numColumns = size = 0;
		matrix = NULL;
		return;
	}
	numRows = rows;
	numColumns = columns;
	size = numRows * numColumns + 1;
	matrix = new D *[numRows + 1];
	if(!matrix)
		Error("%s%s%sInitialize", Factored<D>::ERROR,ERR_SPACE,ERR_FUNCTION);
	matrix[0]=new D[size];
	if(!matrix[0])
		Error("%s%s%sInitialize", Factored<D>::ERROR,ERR_SPACE,ERR_FUNCTION);
	matrix[1] = matrix[0];
	for(int i=2; i<=numRows; i++)
		matrix[i] = matrix[i-1]+numColumns;
}


//   ****************< FactoredInitialize >*********************
//   * Purpose: Factored initialisation                        *
//   ***********************************************************
template <class D>
void Factored<D>::FactoredInitialize(void)
{
	factored = new D *[numRows + 1];
	if(!factored)
		Error("%s%s%sFactoredInizialization", Factored<D>::ERROR,ERR_SPACE,ERR_FUNCTION);
	factored[0] = new D[size];
	if(!factored[0])
		Error("%s%s%sFactoredInizialization", Factored<D>::ERROR,ERR_SPACE,ERR_FUNCTION);
	factored[1] = factored[0];
	for(int i=2; i<=numRows; i++)
		factored[i] = factored[i-1] + numColumns;
	memcpy(factored[0],matrix[0],size*sizeof(D));
}


//   ********************< Deinitialize >***********************
//   * Purpose: Common function for deinitialisation           *
//   ***********************************************************
template <class D>
void Factored<D>::Deinitialize(void)
{
	FactoredDeinitialize();
	if(matrix != NULL) {
		delete [] matrix[0]; matrix[0] = NULL;
		delete [] matrix; matrix = NULL;
	}
}


//   ***************< FactoredDeinitialize >*********************
//   * Purpose: Common function for factored                    *
//   *          deinitialisation                                *
//   ************************************************************
template <class D>
void Factored<D>::FactoredDeinitialize(void)
{
	if(factored != NULL) {
		delete [] factored[0]; factored[0] = NULL;
		delete [] factored; factored = NULL;
	}
}


//   ***********************< PrepCopy >************************
//   * Purpose: Preparing an assignment                        *
//   ***********************************************************
template <class D>
void Factored<D>::PrepCopy(int rRows,int rColumns)
{
	int who = whoAmI;
	SpecificDeinitialize();
	FactoredDeinitialize();
	singular = 0;
	norm = 0.;
	factorizationStatus = UNFACTORED;
	matrixStatus = AVAILABLE;
	factored = NULL;
	if(numRows != rRows || numColumns != rColumns || matrix == 0) {
		Deinitialize();
		Initialize(rRows,rColumns);
		fcount--;
	}
	whoAmI = who;
}


//   ****************< private constructor >********************
//   * Purpose: Sizing a Factored without initialisation       *
//   *          Serves internally                              *
//   * Example: Factored A('*',4,5);                           *
//   ***********************************************************
template <class D>
Factored<D>::Factored(char,int rows,int columns)
{
	Initialize(rows,columns);
}


//   *******************< PrepOnlySolve >***********************
//   * Purpose: Preparing the factorisation                    *
//   * Description: The factored matrix is superimposed on a   *
//   *              matrix                                     *
//   ***********************************************************
template <class D>
void Factored<D>::PrepOnlySolve(void)
{
	if(factorizationStatus == UNFACTORED && matrixStatus == DESTROYED)
		Error("%s%s%sPrepOnlySolve", Factored<D>::ERROR, Factored<D>::ERR_DESTROYED,ERR_FUNCTION);
	else if(factorizationStatus == UNFACTORED && matrixStatus == AVAILABLE) {
		Norm();
		factored = matrix;
		matrix = NULL;
		factorizationStatus = FACTORED;
		matrixStatus = DESTROYED;
		SpecificInitialize();
		Factorization();
	} else if(factorizationStatus == FACTORED && matrixStatus == MODIFIED) { // factorised and modified
		Norm();
		FactoredDeinitialize();
		factored = matrix;
		matrix = NULL;
		matrixStatus = DESTROYED;
		Factorization();
	} else if(factorizationStatus == FACTORED && matrixStatus == AVAILABLE) { // factorised with PrepSolve
		if(matrix != NULL) {
			delete [] matrix[0]; matrix[0] = NULL;
			delete [] matrix; matrix = NULL;
		}
		matrixStatus = DESTROYED;
	}
}


//   *********************< PrepSolve >*************************
//   * Purpose: Preparing the factorisation                    *
//   * Description: Factored and matrix are independent        *
//   ***********************************************************
template <class D>
void Factored<D>::PrepSolve(void)
{
	if(factorizationStatus == UNFACTORED && matrixStatus == DESTROYED)
		Error("%s%s%sPrepOnlySolve", Factored<D>::ERROR, Factored<D>::ERR_DESTROYED,ERR_FUNCTION);
	else if(factorizationStatus == UNFACTORED && matrixStatus == AVAILABLE) {
		FactoredInitialize();
		SpecificInitialize();
		Factorization();
		factorizationStatus = FACTORED;
	} else if(factorizationStatus == FACTORED && matrixStatus == MODIFIED) {
		memcpy(factored[0],matrix[0],size*sizeof(D));
		Factorization();
		matrixStatus = AVAILABLE;
	}
}


//   ************************< Norm >***************************
//   * Purpose: Calculating the Condition Number norm          *
//   * Description: NormR of the matrix from the beginning     *
//   ***********************************************************
template <class D>
void Factored<D>::Norm(void)
{
	double aus;
	norm = 0;
	D *w = matrix[0];
	for(int i=1; i<=numRows; i++) {
		aus=0;
		for(int j=1; j<=numColumns; j++)
			aus += Abs(*++w);
		if(aus > norm)
			norm = aus;
	}
}


//   ===========================================================
//   =================   public functions   ====================
//   ===========================================================

//   ===========================================================
//   ******************   constructors   ***********************
//   ===========================================================

//   **********************< default >**************************
//   * Purpose: To define a Factored which can then            *
//   *          receive an assignment                          *
//   * Examples: Factored A;                                   *
//   *           A = B;                                        *
//   ***********************************************************
template <class D>
Factored<D>::Factored(void)
{
	Initialize(0,0);
}


//   ****************< Copy-Initializer >***********************
//   * Description: Objects of the Factored or derived type    *
//   *              must not be copied or initialised          *
//   ***********************************************************
template <class D>
Factored<D>::Factored(const Factored<D> &rval)
{
	Error("\n%sUses copy-initializer constructor%s\n con Factored N. %d",Factored::ERROR,rval.whoAmI);
}


//   *************< sizing constructor >************************
//   * Purpose: Sizing a Factored and initialising it to 0     *
//   *          They must then be assigned to other values     *
//   * Example: Factored A(4,5);                               *
//   *          A(1,1)=3.; A(1,2)=7.;....                      *
//   ***********************************************************
template <class D>
Factored<D>::Factored(int rows,int columns)
{
	Initialize(rows,columns);
	if(size != 0)
		memset(matrix[0],0,size*sizeof(D));
}


//   ********************< constructor >************************
//   * Purpose: Initialising a Factored from array             *
//   * Example: double w[]={1.,2.,3.,4.,5.,6.};                *
//   *          Factored A(2,3,w);                             *
//   ***********************************************************
template <class D>
Factored<D>::Factored(int rows,int columns,D *initvalues)
{
	Initialize(rows,columns);
	D *w=matrix[0];
	++w;
	if(numRows != 0)
		memcpy(w,initvalues,(size-1)*sizeof(D));
}


//   *******************< from Matrix >*************************
//   * Purpose: Defining and initialising from Matrix          *
//   * Examples: Factored A=B;  // B Matrix                    *
//   *           Factored A=B+2.*C; // B and C Matrix          *
//   ***********************************************************
template <class D>
Factored<D>::Factored(const Te_Matrix<D> &rval)
{
	Initialize(rval.numRows,rval.numColumns);
	if(numRows != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
}


//   ********************< from submatrix >*********************
//   * Purpose: From a matrix with an identical start          *
//   * Example: Matrix B(4,5,A);                               *
//   ***********************************************************
template <class D>
Factored<D>::Factored(int rows,int columns,const Te_Matrix<D> &rval)
{
	if(rows < 1 || rows > rval.Rows())
		Error("%s%s%sSubFactored", Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_CONSTRUCTOR);
	if(columns < 1 || columns > rval.Columns())
		Error("%s%s%sSubFactored", Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_CONSTRUCTOR);
	Initialize(rows,columns);
	for(int row=1; row<=numRows; row++)
		memcpy(matrix[row]+1,rval.matrix[row]+1,numColumns*sizeof(D));
}


//   *****************< from internal submatrix >***************
//   * Purpose: From internal submatrix                        *
//   * Example: Matrix B(m,n,i,j,A);      B(m,n) from i,j      *
//   ***********************************************************
template <class D>
Factored<D>::Factored(int rows,int columns,int irow,int jcol,const Te_Matrix<D> &rval)
{
	if(rows < 1 || irow < 1 || irow > rval.Rows() || rows > (rval.Rows()-irow+1))
		Error("%s%s%sSubFactored",Factored<D>::ERROR,ERR_RANGE,ERR_CONSTRUCTOR);
	if(columns < 1 || jcol < 1 || jcol > rval.Columns() || columns>(rval.Columns()-jcol+1))
		Error("%s%s%sSubFactored",Factored<D>::ERROR,ERR_RANGE,ERR_CONSTRUCTOR);
	Initialize(rows,columns);
	for(int row=1;row<=numRows;row++)
		memcpy(matrix[row]+1,rval.matrix[row+irow-1]+jcol,numColumns*sizeof(D));
}


//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================

template <class D>
Factored<D>::~Factored()
{
	if(factored != NULL) {
		delete [] factored[0];
		delete [] factored;
	}
	if(matrix != NULL) {
		delete [] matrix[0];
		delete [] matrix;
	}
}


//   ===========================================================
//   ***********   Non-modifying access functions   ************
//   ===========================================================

//   *********************< GetValue >**************************
//   * Purpose: Receiving values with control                  *
//   * Example: double xf = A.GetValue(2,3);                   *
//   ***********************************************************
template <class D>
D Factored<D>::GetValue(int row,int col) const {
	if(matrixStatus == DESTROYED)
		Error("%s%s",Factored::ERROR,ERR_DESTROYED);
	if( (row < 1 || row > numRows) || (col < 1 || col > numColumns) )
		Error("%s%s%sGetValue",Factored<D>::ERROR,ERR_RANGE,ERR_FUNCTION);
	return matrix[row][col];
}


//   **********************< GetRow >***************************
//   * Purpose: Receiving the values of a row                  *
//   * Example: Vector v = A.GetRow(3);                        *
//   ***********************************************************
template <class D>
Te_Vector<D> Factored<D>::GetRow(int i) const
{
	if(matrixStatus == DESTROYED)
		Error("%s%s",Factored::ERROR,ERR_DESTROYED);
	if(i < 1 || i > numRows)
		Error("%s%s%sGetRow",Factored<D>::ERROR,ERR_RANGE,ERR_FUNCTION);
	Vector<D> result('*',numColumns);
	D *w=matrix[i];
	D *r=result.vector;
	memcpy(r,w,(numColumns+1)*sizeof(D));
	return result;
}


//   *********************< GetColumn >*************************
//   * Purpose: Receiving the values of a column               *
//   * Example: Vector v = A.GetVector(2);                     *
//   ***********************************************************
template <class D>
Te_Vector<D> Factored<D>::GetColumn(int j) const
{
	if(matrixStatus == DESTROYED)
		Error("%s%s",Factored::ERROR,ERR_DESTROYED);
	if(j < 1 || j > numColumns)
		Error("%s%s%sGetColumn",Factored<D>::ERROR,ERR_RANGE,ERR_FUNCTION);
	Vector<D> result('*',numRows);
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
//   * Example: x = A(1,5); A(3,7) = 5.;                       *
//   ***********************************************************
template <class D>
D &Factored<D>::operator () (int row,int col)
{
	if(matrixStatus == DESTROYED)
		Error("%s%s",Factored::ERROR,ERR_DESTROYED);
	if(factorizationStatus == FACTORED)
		matrixStatus = MODIFIED;
	if( (row < 1 || row > numRows) || (col < 1 || col > numColumns) )
		Error("%s",Factored::ERROR);
	return matrix[row][col];
}


//   *********************< SetValue >**************************
//   * Purpose: Assigning values with control                  *
//   ***********************************************************
template <class D>
void Factored<D>::SetValue(int row,int col,D val)
{
	(*this)(row,col) = val;
}


//   **********************< SetRow >***************************
//   * Purpose: Assigning a row with a Vector                  *
//   * Example: A.SetRow(3,v);                                 *
//   ***********************************************************
template <class D>
void Factored<D>::SetRow(int i,const Te_Vector<D> &rval)
{
	if(matrixStatus == DESTROYED)
		Error("%s%s",Factored::ERROR,ERR_DESTROYED);
	if(factorizationStatus == FACTORED)
		matrixStatus = MODIFIED;
	if( (i < 1 || i > numRows) || (rval.dimensions != numColumns ) )
		Error("%s%s%sSetRow",Factored<D>::ERROR,ERR_RANGE,ERR_FUNCTION);
	memcpy(matrix[i]+1,rval.vector+1,numColumns*sizeof(D));
}


//   *********************< SetColumn >*************************
//   * Purpose: Assigning a column with a Vector               *
//   * Example: A.SetColumn(3,v);                              *
//   ***********************************************************
template <class D>
void Factored<D>::SetColumn(int j,const Te_Vector<D> &rval)
{
	if(matrixStatus == DESTROYED)
		Error("%s%s",Factored::ERROR,ERR_DESTROYED);
	if(factorizationStatus == FACTORED)
		matrixStatus = MODIFIED;
	if( (j < 1 || j > numColumns) || (rval.dimensions != numRows) )
		Error("%s%s%sSetColumn",Factored<D>::ERROR,ERR_RANGE,ERR_FUNCTION);
	D *r = rval.vector;
	for(int k = 1;k <= numRows;k++)
		matrix[k][j] = *++r;
}


//   ===========================================================
//   ***************   assignment operators   ******************
//   ===========================================================

//   ******************< CopyFromMatrix >***********************
//   * Purpose: Assignment of a Matrix                         *
//   * Description: Used internally                            *
//   ***********************************************************
template <class D>
void Factored<D>::CopyFromMatrix(const Te_Matrix<D> &rval)
{
	PrepCopy(rval.numRows,rval.numColumns);
	if(numRows != 0)
		memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
}


//   ***************< CommuteMatrixToFactored >*****************
//   * Purpose: Transforming a Matrix in Factored              *
//   * Description: Destroys the Matrix                        *
//   ***********************************************************
template <class D>
void CommuteMatrixToFactored(Te_Matrix<D> *lval,Factored<D> *rval)
{
	Delete(rval);
	rval->numRows = lval->numRows;
	rval->numColumns = lval->numColumns;
	rval->size = lval->size;
	rval->matrix = lval->matrix;
	lval->numRows = 0;
	lval->numColumns = 0;
	lval->size = 0;
	lval->matrix = 0;
}


//   ===========================================================
//   *              Non-modifying functions                    *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Printing                                       *
//   ***********************************************************
template <class D>
void Factored<D>::Print(char *msg)
{
	fprintf(bzzFileOut,"\nFactored No.%d",whoAmI);
	if(*msg)
		fprintf(bzzFileOut,"  %s",msg);
	if(matrixStatus == DESTROYED)
		return;
	fprintf(bzzFileOut,"\nrows %d columns %d\n",numRows,numColumns);
	for(int row = 1;row <= numRows;row++) {
		for(int column = 1;column <= numColumns;column++)
			fprintf(bzzFileOut," %12.4e",matrix[row][column]);
		fprintf(bzzFileOut,"\n");
	}
}

//   ===========================================================
//   *                 Modifying functions                     *
//   ===========================================================

//   **********************< Delete >***************************
//   * Purpose: Eliminating a factored of no use               *
//   *          without it leaving the purpose                 *
//   * Example: Delete(&A);                                    *
//   ***********************************************************
template <class D>
void Delete(Factored<D> *result)
{
	int who = result->whoAmI;
	result->SpecificDeinitialize();
	result->Deinitialize();
	result->Initialize(0,0);
	Factored<D>::fcount--;
	result->whoAmI = who;
}


//   ===========================================================
//   ==========   Functions for linear algebra   ===============
//   ===========================================================

//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a vector           *
//   * Description: The matrix A is factorised                 *
//   *              the solution replaces bx                   *
//   * Example: Solve(&A,&bx);                                 *
//   ***********************************************************
template <class D>
void Solve(Factored<D> *A,Te_Vector<D> *bx)
{
	if(bx->Size() != A->numRows)
		Error("%s%s%sOnlySolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A->PrepOnlySolve();
	A->Solution(bx);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a vector           *
//   * Description: The matrix A is factorised                 *
//   *              The solution replaces x                    *
//   * Example: Solve(&A,b,&x);                                *
//   ***********************************************************
template <class D>
void Solve(Factored<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x)
{
	if(b.Size() != A->numRows)
		Error("%s%s%sOnlySolve",Factored::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A->PrepOnlySolve();
	if(b.WhoAmI() == x->WhoAmI())
		A->Solution(x);
	else
		A->Solution(b,x);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a matrix           *
//   * Description: The matrix A is factorised                 *
//   *              The solution replaces BX                   *
//   * Example: Solve(&A,&BX);                                 *
//   ***********************************************************
template <class D>
void Solve(Factored<D> *A,Te_Matrix<D> *BX)
{
	if(BX->Rows() != A->numRows)
		Error("%s%s%sOnlySolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A->PrepOnlySolve();
	A->Solution(BX);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a matrix           *
//   * Description: The matrix A is factorised                 *
//   *              The solution replaces X                    *
//   * Example: Solve(&A,B,&X);                                *
//   ***********************************************************
template <class D>
void Solve(Factored<D> *A,const Te_Matrix<D> &B,Te_Matrix<D> *X)
{
	if(B.Rows() != A->numRows)
		Error("%s%s%sOnlySolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A->PrepOnlySolve();
	if(B.WhoAmI() == X->WhoAmI())
		A->Solution(X);
	else
		A->Solution(B,X);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a vector           *
//   * Description: The matrix A is factorised                 *
//   *              The solution replaces bx                   *
//   * Example: Solve(A,&bx);                                  *
//   ***********************************************************
template <class D>
void Solve(Factored<D> &A,Te_Vector<D> *bx)
{
	if(bx->Size() != A.numRows)
		Error("%s%s%sSolve",Factored::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A.PrepSolve();
	A.Solution(bx);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a vector           *
//   * Description: The matrix A is saved                      *
//   *              The solution replaces x                    *
//   * Example: Solve(A,b,&x);                                 *
//   ***********************************************************
template <class D>
void Solve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x)
{
	if(b.Size() != A.numRows)
		Error("%s%s%sSolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A.PrepSolve();
	if(b.WhoAmI() == x->WhoAmI())
		A.Solution(x);
	else
		A.Solution(b,x);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a matrix           *
//   * Description: The matrix A is saved                      *
//   *              The solution replaces BX                   *
//   * Example: Solve(A,&BX);                                  *
//   ***********************************************************
template <class D>
void Solve(Factored<D> &A,Te_Matrix<D> *BX)
{
	if(BX->Rows() != A.numRows)
		Error("%s%s%sSolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A.PrepSolve();
	A.Solution(BX);
}


//   ***********************< Solve >***************************
//   * Purpose: Solving a linear system for a matrix           *
//   * Description: The matrix A is saved                      *
//   *              The solution replaces X                    *
//   * Example: Solve(A,,B,&X);                                *
//   ***********************************************************
template <class D>
void Solve(Factored<D> &A,const Te_Matrix<D> &B,Te_Matrix<D> *X)
{
	if(B.Rows() != A.numRows)
		Error("%s%s%sSolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A.PrepSolve();
	if(B.WhoAmI() == X->WhoAmI())
		A.Solution(X);
	else
		A.Solution(B,X);
}


//   **************< TransposeOnlySolve >***********************
//   * Purpose: Solving a linear system for a vector           *
//   *          with A transposed                              *
//   * Description: The matrix A is factorised                 *
//   *              The solution replaces bx                   *
//   * Example: TransposeSolve(&A,&bx);                        *
//   ***********************************************************
template <class D>
void TransposeSolve(Factored<D> *A,Te_Vector<D> *bx)
{
	if(bx->Size() != A->numColumns)
		Error("%s%s%sTransposeOnlySolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A->PrepOnlySolve();
	A->TransposeSolution(bx);
}


//   **************< TransposeOnlySolve >***********************
//   * Purpose: Solving a linear system for a vector           *
//   *        with A transposed                                *
//   * Description: The matrix A is factorised                 *
//   *              The solution replaces x                    *
//   * Example: TransposeSolve(&A,b,&x);                       *
//   ***********************************************************
template <class D>
void TransposeSolve(Factored<D> *A,const Te_Vector<D> &b,Te_Vector<D> *x)
{
	if(b.Size() != A->numColumns)
		Error("%s%s%sTransposeOnlySolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A->PrepOnlySolve();
	if(b.WhoAmI() == x->WhoAmI())
		A->TransposeSolution(x);
	else
		A->TransposeSolution(b,x);
}


//   ******************< TransposeSolve >***********************
//   * Purpose: Solving a linear system for a vector           *
//   *          with A transposed                              *
//   * Description: The matrix A is saved                      *
//   *              the solution replaces bx                   *
//   * Example: TransposeSolve(A,&bx);                         *
//   ***********************************************************
template <class D>
void TransposeSolve(Factored<D> &A,Te_Vector<D> *bx)
{
	if(bx->Size() != A.numColumns)
		Error("%s%s%sTransposeSolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A.PrepSolve();
	A.TransposeSolution(bx);
}


//   ******************< TransposeSolve >***********************
//   * Purpose: Solving a linear system for a vector           *
//   *          with A transposed                              *
//   * Description: The matrix A is saved                      *
//   *              The solution replaces x                    *
//   * Example: TransposeSolve(A,b,&x);                        *
//   ***********************************************************
template <class D>
void TransposeSolve(Factored<D> &A,const Te_Vector<D> &b,Te_Vector<D> *x)
{
	if(b.Size() != A.numColumns)
		Error("%s%s%sTransposeSolve",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	A.PrepSolve();
	if(b.WhoAmI() == x->WhoAmI())
		A.TransposeSolution(x);
	else
		A.TransposeSolution(b,x);
}


//   *********************< HyperSolve >************************
//   * Purpose: Solving a linear system for a vector           *
//   *          iteratively to improve the solution            *
//   * Description: The matrix A is saved                      *
//   *              The solution replaces bx                   *
//   * Example: HyperSolveSolve(A,&bx);                        *
//   ***********************************************************
template <class D>
void HyperSolve(Factored<D> &A,Te_Vector<D> *bx)
{
	int i, j, iter;
	const int NITER = 10;
	D **mat = A.matrix;
	Vector<D> b = *bx;
	Vector<D> x(A.numColumns);
	Vector<D> r(A.numRows);
	Solve(A,bx);
	x = *bx;
	double delx;
	double normdxOld;
	for(iter=1; iter<=NITER; iter++) {
		for(i=1;i<=A.numRows;i++) {
			delx = b[i];
			for(j = 1;j <= A.numColumns;j++)
				delx -= mat[i][j] * (*bx)[j];
			r[i] = Control(delx);
		}
		*bx = r;
		Solve(A,bx);
		double normdx = bx->NormI();
		double normx = x.NormI();
		if(iter == 1) {
			normdxOld = normx;
			if(normdx > .5*normx)
				break;
        } else if(normdx >= normdxOld)
        	break;
		normdxOld = normdx;
		x += *bx;
		*bx = x;
		if(normdx < 10.*MACH_EPS*normx)
			break;
	}
}


//   *****************< ConditionNumber >***********************
//   * Purpose: Calculating the condition number               *
//   * Example: double condition = A.ConditionNumber();        *
//   ***********************************************************
template <class D>
double Factored<D>::ConditionNumber(void)
{
	if(numRows != numColumns)
		Error("%s%s%sConditionNumber",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	PrepSolve();
	if(matrixStatus == AVAILABLE)
		Norm();
	return Condition();
}


//   *******************< Determinant >*************************
//   * Purpose: Calculating the determinant of a matrix        *
//   * Example: Double determinant = A.Determinant();          *
//   ***********************************************************
template <class D>
double Factored<D>::Determinant(void)
{
	if(numRows != numColumns)
		Error("%s%s%sDeterminant",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	PrepSolve();
	return DeterminantEvaluation();
}


//   **********************< Singular >*************************
//   * Purpose: Checking if the resulting matrix is singular   *
//   * Description: If 1 the matrix is singular, if 0 it is not*
//   * Example: char singular = A.Singular();                  *
//   ***********************************************************
template <class D>
char Factored<D>::Singular(void)
{
	PrepSolve();
	return singular;
}


//   **********************< Inverse >**************************
//   * Purpose: Calculating the inverse of a matrix            *
//   * Example: Matrix inv;                                    *
//   *          A.Inverse(&inv);                               *
//   ***********************************************************
template <class D>
void Factored<D>::Inverse(Te_Matrix<D> *inv)
{
	int i, j;
	if(numRows != numColumns)
		Error("%s%s%sDeterminant",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	PrepSolve();
	ChangeDimensions(numRows,numColumns,inv,0);
	for(i=1; i<=numRows; i++) {
		for(j=1; j<=numColumns; j++)
			(*inv)[i][j] = 0;
		(*inv)[i][i] = 1;
	}
	Solve(*this,inv);
}


//   **********************< Inverse >**************************
//   * Purpose: Calculating the inverse of a matrix            *
//   * Example: Matrix inv = A.Inverse();                      *
//   ***********************************************************
template <class D>
Te_Matrix<D> Factored<D>::Inverse(void)
{
	if(numRows != numColumns)
		Error("%s%s%sDeterminant",Factored<D>::ERROR,ERR_CHECK_DIMENSION,ERR_FUNCTION);
	PrepSolve();
	Matrix<D> inv(numRows,numColumns);
	for(int i=1; i<=numRows; i++)
		inv[i][i] = 1;
	Solve(*this,&inv);
	return inv;
}


#endif // FACTORED_HPP
