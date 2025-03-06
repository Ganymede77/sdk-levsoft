#ifndef SYMM_HPP
#define SYMM_HPP

//   =======================< SYMM.HPP >========================
//   * Class Te_MatrixSymm                                     *
//   * Description: Chapter 12                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   ===========================================================
//   ****** Constructions for Te_MatrixSymm:                   *
//   * Te_MatrixSymm S; // default                             *
//   * Te_MatrixSymm S = symm; // copy-initializer             *
//   * Te_MatrixSymm S(3,3); // sized and placed at 0          *
//   * Te_MatrixSymm S(3,3,                                    *
//   *      1.,                                                *
//   *      2.,3.,                                             *
//   *      4.,5.,6.);// sizes and initialises                 *
//   * double x[]=                                             *
//   *     {                                                   *
//   *     1.,                                                 *
//   *     2.,3.,                                              *
//   *     4.,5.,6.                                            *
//   *     };                                                  *
//   * Te_MatrixSymm S(3,3,x); // from array                   *
//   * Te_MatrixSymm S("MAT.DAT"); // Formatted File           *
//   * Te_MatrixSymm S('*',MAT.BIN"); // Binary File           *
//   ***********************************************************
//   ***** Access functions :                                  *
//   *  i = S.Rows(); // numRows                               *
//   *  i = S.Columns(); // numColumns                         *
//   *  xf = S.GetValue(i);                                    *
//   *  xf = S(i,j);                                           *
//   *  S(i,j) = xf;                                           *
//   *  xf = S[i][j]; // NOT implemented                       *
//   *  S[i][j] = xf; // NOT implemented                       *
//   *  S.SetValue(i,j,xf);                                    *
//   ***********************************************************
//   ***** Assignments:                                        *
//   *  S = symm; // symm Te_MatrixSymm                        *
//   ***********************************************************
//   *****  Operators for tests:                               *
//   * if(S1 == S2)                                            *
//   * if(S1 != S2)                                            *
//   ***********************************************************
//   ***** Implemented operations :                            *
//   * S3 = S1 + S2;          // S3 = S1 + S2;                 *
//   * S3 = S1 - S2;          // S3 = S1 - S2;                 *
//   * TProduct(L,&S);        // S = LTL                       *
//   * ProductT(L,&S);        // S = LLT                       *
//   * TProduct(R,&S);        // S = RTR                       *
//   * ProductT(R,&S);        // S = RRT                       *
//   ***********************************************************
//   *  Other functions:                                       *
//   *  S.Print("comments");                                   *
//   *  S.Save("MAT.DAT");                                     *
//   *  S.Save('*',"MAT.BIN");                                 *
//   *  Delete(&S); // eliminates Te_MatrixSymm                *
//   *  ChangeDimensions(rows,columns,&S);                     *
//   *  Recover(&S,"MAT.DAT");                                 *
//   *  Recover(&S,'*',"MAT.BIB");                             *
//   ***********************************************************


#include <complex>
#include <matrix.hpp>


namespace lev_math {

// preventive declarations
#if 10
template <class D> class Te_Vector;
template <class D> class Te_Matrix;
template <class D> class Te_MatrixLeft;
template <class D> class Te_MatrixRight;
template <class D> class Te_MatrixSymm;
template <class D> class FactoredSymm;
template <class D> void CommuteMatrixToFactored();
template <class D> char operator == (const Te_MatrixSymm<D> &lval, const Te_MatrixSymm<D> &rval);
template <class D> char operator != (const Te_MatrixSymm<D> &lval, const Te_MatrixSymm<D> &rval);
template <class D> Te_MatrixSymm<D> operator + (const Te_MatrixSymm<D> &lval, const Te_MatrixSymm<D> &rval);
template <class D> Te_MatrixSymm<D> operator - (const Te_MatrixSymm<D> &lval, const Te_MatrixSymm<D> &rval);
template <class D> void TProduct(const Te_MatrixRight<D> &lvalAndRval, Te_MatrixSymm<D> *result);
template <class D> void TProduct(const Te_MatrixLeft<D> &lvalAndRval, Te_MatrixSymm<D> *result);
template <class D> void ProductT(const Te_MatrixRight<D> &lvalAndRval, Te_MatrixSymm<D> *result);
template <class D> void ProductT(const Te_MatrixLeft<D> &lvalAndRval, Te_MatrixSymm<D> *result);
template <class D> void Delete(Te_MatrixSymm<D> *S);
template <class D> void ChangeDimensions(int rows,int columns, Te_MatrixSymm<D> *result,char zero);
template <class D> void Recover(Te_MatrixSymm<D> *A,char *filematrix);
template <class D> void Recover(Te_MatrixSymm<D> *A,char,char *filematrix);
template <class D> void Swap(Te_MatrixSymm<D> *lval,Te_MatrixSymm<D> *rval);
#endif

//   ===========================================================
//   =================   class Te_MatrixSymm   =================
//   ===========================================================
template <class D>
class Te_MatrixSymm {
   friend class Te_Vector<D>;
   friend class Te_Matrix<D>;
   friend class Te_MatrixRight<D>;
   friend class Te_MatrixLeft<D>;

private:
   static const char *const ERRORMESSAGE;
   static int count; // for whoAmI

   D **matrix;
   int numRows,numColumns;
   int size;
   int whoAmI;

   void Initialize(int rows,int columns);
   // private constructor R('*',3,3)

   // deinitialisation
   void Deinitialize(void);

   Te_MatrixSymm(char,int rows,int columns); // OK

public:
   //   ===========================================================
   //   ******************   constructors   ***********************
   //   ===========================================================
   //   default // Te_MatrixSymm S
   Te_MatrixSymm(void);

   // copy-initializer // Te_MatrixSymm S = symm;
   Te_MatrixSymm(Te_MatrixSymm<D> &rval);

   // sized and initialised to 0
   // Te_MatrixSymm S(3,3)
   Te_MatrixSymm(int rows,int columns);

   // sized and initialised
   // Te_MatrixSymm S(2,2,1.,2.,3.);
   Te_MatrixSymm(int rows,int columnsn,D a11,...);

   // from array // Te_MatrixSymm S(3,3,w)
   Te_MatrixSymm(int rows,int columns,D *initvalues);

   // from formatted File // Te_MatrixSymm S("SYMM.DAT");
   Te_MatrixSymm(char *filematrix);

   // from binary File // Te_MatrixSymm L('*',"SYMM.BIN");
   Te_MatrixSymm(char,char *filematrix);

   //   ===========================================================
   //   ********************   destructor   ***********************
   //   ===========================================================
   ~Te_MatrixSymm(void);

   //   ===========================================================
   //   ***********   Non-modifying access functions   ************
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

   //   ===========================================================
   //   **************   Modifying access functions   *************
   //   ===========================================================
   //   assigns values with control
   void SetValue(int row,int col,D val);

   // assigns and receives vector values with control
   D &operator () (int row,int col);

   //   ===========================================================
   //   ***************  assignment operators *********************
   //   ===========================================================
   Te_MatrixSymm<D> &operator = (const Te_MatrixSymm<D> &rval);

   //   ===========================================================
   //   ***************   operators for tests   *******************
   //   ===========================================================
   friend char operator == <> (const Te_MatrixSymm<D> &lval,
      const Te_MatrixSymm<D> &rval);

   friend char operator != <> (const Te_MatrixSymm<D> &lval,
      const Te_MatrixSymm<D> &rval);

   //   ===========================================================
   //   ====================   OPERATIONS   =======================
   //   ===========================================================

   //   ===========================================================
   //   ***********************   Sum   ***************************
   //   ===========================================================
   friend Te_MatrixSymm<D> operator + <> (const Te_MatrixSymm<D> &lval,
      const Te_MatrixSymm<D> &rval);

   //   ===========================================================
   //   ********************   Difference   ***********************
   //   ===========================================================
   friend Te_MatrixSymm<D> operator - <> (const Te_MatrixSymm<D> &lval,
      const Te_MatrixSymm<D> &rval);

   //   ===========================================================
   //   *********************   TProduct   ************************
   //   ===========================================================
   friend void TProduct <> (const Te_MatrixRight<D> &lvalAndRval,
      Te_MatrixSymm<D> *result);

   friend void TProduct <> (const Te_MatrixLeft<D> &lvalAndRval,
      Te_MatrixSymm<D> *result);

   //   ===========================================================
   //   *********************   ProductT   ************************
   //   ===========================================================
   friend void ProductT <>(const Te_MatrixRight<D> &lvalAndRval,
      Te_MatrixSymm<D> *result);

   friend void ProductT <> (const Te_MatrixLeft<D> &lvalAndRval,
      Te_MatrixSymm<D> *result);

   //   ===========================================================
   //   ===========   Non-modifying functions   ===================
   //   ===========================================================
   //   ***********************< Print >***************************
   void Print(char *msg="");

   //   ************************< Save >***************************
   void Save(char *filematrix); // formatted
   void Save(char,char *filematrix);// binary


   //   ===========================================================
   //   =============   Modifying Functions =======================
   //   ===========================================================
   friend void Delete <> (Te_MatrixSymm<D> *S); // eliminates Te_MatrixSymm
   friend void ChangeDimensions <> (int rows,int columns,
      Te_MatrixSymm<D> *result,char zero);

   // from formatted file
   friend void Recover <> (Te_MatrixSymm<D> *A,char *filematrix);
   // from binary file
   friend void Recover <> (Te_MatrixSymm<D> *A,char,char *filematrix);
   friend void Swap <> (Te_MatrixSymm<D> *lval,Te_MatrixSymm<D> *rval);

   // transforms a Te_MatrixSymm in FactoredSymm
   // Te_MatrixSymm gets destroyed
   //   friend void CommuteMatrixToFactored<>
   //   (Te_MatrixSymm<D> *lval,FactoredSymm<D> *rval);
};


//============================================================================
//       IMPLEMENTATION
//============================================================================

template <class D> const char *const  Te_MatrixSymm<D>::ERRORMESSAGE =
   "\n========>>>  Te_MatrixSymm<D> error!!!!!!\n";
template <class D> int  Te_MatrixSymm<D>::count = 0;

//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   ********************< Initialize >*************************
//   * Purpose: Common function for initialising constructors  *
//   ***********************************************************
template <class D> void  Te_MatrixSymm<D>::Initialize(int rows,int columns) {
   count++;
   whoAmI = count;
   if(rows < 1 || columns < 1) {
      numRows = numColumns = size = 0;
      matrix = NULL;
      return;
   }
   if(rows != columns)
      lev_math::utility::Error("%s%s%sInitialize",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
      numRows = numColumns = rows;
      size = numRows*(numRows+1)/2+1;
      matrix = new D *[numRows+1];
      if(!matrix)
         lev_math::utility::Error("%s%s%sInitialize",
            Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
      matrix[0] = new D[size];
      if(!matrix[0])
         lev_math::utility::Error("%s%s%sInitialize",
            Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
      matrix[1] = matrix[0];
      for(int i = 2;i <= numRows;i++)
         matrix[i] = &matrix[i-1][i-1];
}


//   **********************< Deinitialize >*********************
//   * Purpose: Common function for deinitialisation           *
//   * Description: Chapter 12                                 *
//   * Example: Deinitialize();                                *
//   ***********************************************************
template <class D>
void Te_MatrixSymm<D>::Deinitialize(void)
{
	if(matrix) {
		delete [] matrix[0]; matrix[0] = NULL;
		delete [] matrix; matrix = NULL;
	}
}


//   ****************< private constructor >********************
//   * Purpose: Sets size of a  Te_MatrixSymm<D> without       *
//   *          initialisation. Serving internally             *
//   ***********************************************************
template <class D>
Te_MatrixSymm<D>::Te_MatrixSymm(char,int rows,int columns)
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
//   * Purpose: Defining a  Te_MatrixSymm<D> which can then    *
//   *          receive an assignment                          *
//   * Examples:  Te_MatrixSymm<D> S;                          *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm(void) {
   Initialize(0,0);
}


//   ****************< Copy-Initializer >***********************
//   * Purpose: Defining and initialising  Te_MatrixSymm<D>    *
//   *          Used in returns                                *
//   * Examples: Te_MatrixSymm A=B;                            *
//   *           Te_MatrixSymm A=B+2.*C;                       *
//   *           return A; (used implicitly)                   *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm( Te_MatrixSymm<D> &rval) {
   Initialize(rval.numRows,rval.numRows);
   if(numRows != 0)
      memcpy(matrix[0],rval.matrix[0],size*sizeof(D));
}


//   *************< sizing constructor >************************
//   * Purpose: Setting size and initialising to 0 a           *
//   *          Te_MatrixSymm Must then be assigned other      *
//   *          values                                         *
//   * Example: Te_MatrixSymm A(5,5);                          *
//   *          A(1,1)=3.; A(2,1)=7.;....                      *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm(int rows,int columns) {
   Initialize(rows,columns);
   if(numRows != 0)
      memset(matrix[0],0,size*sizeof(D));
}


//   *************< sizing and initialising  >******************
//   * Purpose: Initialising a Te_MatrixSymm                   *
//   * Example: Te_MatrixSymm A(2,2,1.,2.,3.);                 *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm(int rows,int columns,
   D a11,...) {
   Initialize(rows,columns);
   D *w=matrix[0] + 1;
   va_list puntaList;
   va_start(puntaList,a11);
   int i;
   *w = Control(a11);
   for(i = 2;i < size;i++)
      *++w = Control(va_arg(puntaList,D));
   va_end(puntaList);
}


//   *********************< from array >************************
//   * Purpose: Initialising a Te_MatrixSymm from an array     *
//   * Example: T w[]={1.,                                     *
//   *                     2.,3.,                              *
//   *                     4.,5.,6.};                          *
//   *          Te_MatrixSymm A(3,3,w);                        *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm(int mrows,int ncolumns,
   D *initvalues) {
   Initialize(mrows,ncolumns);
   D *w = matrix[0];
   ++w;
   if(numRows != 0)
      memcpy(w,initvalues,(size-1)*sizeof(D));
}


//   ****************< from formatted File>*********************
//   * Purpose: Initialising a Te_MatrixSymm from FILE         *
//   * Example: Te_MatrixSymm S("MAT.DAT");                    *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm(char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"r"))==NULL)
      lev_math::utility::Error("%s%s%s%sConstructor",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   int rows,columns;
   fscanf(utility::bzzFileIn,"%d %d",&rows,&columns);
   if(rows < 1 || columns < 1)
      lev_math::utility::Error("%s%s%sFILE",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION, utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
   D *w=matrix[0];
   for(int i=1;i<size;i++) fscanf(utility::bzzFileIn,"%f",++w);
   fclose(utility::bzzFileIn);
}


//   ****************< from binary File >***********************
//   * Purpose: Initialising a Te_MatrixSymm from              *
//   *          unformatted FILE                               *
//   * Example: Te_MatrixSymm A('*',"MAT.DAT");                *
//   ***********************************************************
template <class D> Te_MatrixSymm<D>:: Te_MatrixSymm(char,char *filematrix) {
   if((utility::bzzFileIn=fopen(filematrix,"rb"))==NULL)
      lev_math::utility::Error("%s%s%s%sFILE",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_CONSTRUCTOR);
   int rows,columns;
   if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%sFILE", Te_MatrixSymm<D>::ERRORMESSAGE);
   if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%sFILE", Te_MatrixSymm<D>::ERRORMESSAGE);
   if(rows < 1 || columns < 1)
      lev_math::utility::Error("%s%s%sFILE",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION, utility::ERR_CONSTRUCTOR);
   Initialize(rows,columns);
      if(fread(matrix[1],sizeof(D)*size,1,utility::bzzFileIn) != 1)
         lev_math::utility::Error("%sFILE", Te_MatrixSymm<D>::ERRORMESSAGE);
   fclose(utility::bzzFileIn);
}


//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================
template <class D> Te_MatrixSymm<D>::~Te_MatrixSymm(void)
	{
	Deinitialize();
	}

//   ===========================================================
//   ************  Non-modifying access functions    ***********
//   ===========================================================

//   *********************< GetValue >**************************
//   * Purpose: Receiving values with control                  *
//   * Example: S.GetValue(1,2);                               *
//   ***********************************************************
template <class D>D  Te_MatrixSymm<D>::GetValue(int row,int col) const
	{
	if(row < 1 || row > numRows)
		lev_math::utility::Error("%s%s%sGetValue",
		 Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(col < 1 || col > numRows)
		lev_math::utility::Error("%s%s%sGetValue",
		 Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(row > col) return matrix[row][col];
	else return matrix[col][row];
	}

//   ===========================================================
//   ************    Modifying access functions ****************
//   ===========================================================

//   *********************< SetValue >**************************
//   * Purpose: Assigning values with control                  *
//   * Example: S.SetValue(1,2,5.);                            *
//   ***********************************************************
template <class D>void  Te_MatrixSymm<D>::SetValue(int row,int col,D val)
	{
	if(row < 1 || row > numRows)
		lev_math::utility::Error("%s%s%sSetValue",
		 Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(col < 1 || col > numRows)
		lev_math::utility::Error("%s%s%sSetValue",
		 Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_FUNCTION);
	if(row > col) matrix[row][col]=val;
	else matrix[col][row]=val;
	}

//   ************************< () >*****************************
//   * Purpose: Get and set with control                       *
//   * Example: A(1,2)=3.;//Set                                *
//   *          a=A(4,5); // Get                               *
//   ***********************************************************
template <class D>D & Te_MatrixSymm<D>::operator ()(int row,int col)
	{
	if( (row < 1 || row > numRows)
			||
	  (col < 1 || col > numColumns) )
		 lev_math::utility::Error("%s%s%s()",
		  Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_RANGE,utility::ERR_OPERATOR);
	if(row > col)return matrix[row][col];
	else return matrix[col][row];
}

//   ===========================================================
//   ***************   assignment operator   *******************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment                                     *
//   * Example: A=B;   // simple                               *
//   *          A=C+D;    // with operations                   *
//   *          A=B=C+D;  // multiple                          *
//   ***********************************************************
template <class D> Te_MatrixSymm<D> & Te_MatrixSymm<D>::operator =(const  Te_MatrixSymm<D> &rval)
	{
	int who = whoAmI;
	if(numRows != rval.numRows)
		{
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
//   * Purpose: Checking if two Te_MatrixSymms are identical   *
//   * Description: Returns 1 if they are identical            *
//   * Example: if(A==B)...                                    *
//   ***********************************************************
template <class D>
char operator == (const Te_MatrixSymm<D> &lval, const Te_MatrixSymm<D> &rval) {
   if(lval.whoAmI == rval.whoAmI)
      return 1;
   char ch=1;
   if(lval.numRows != rval.numRows) ch=0;
   else	{
      if(memcmp(lval.matrix[0]+1,rval.matrix[0]+1,
         (rval.size-1)*sizeof(D))==0)ch=1;
      else ch = 0;
   }
   return ch;
}


//   ************************< != >*****************************
//   * Purpose: Check if two Te_MatrixSymms are different      *
//   * Description: Returns 1 if they are different            *
//   * Example: if(A!=B)...                                    *
//   ***********************************************************
template <class D>
char operator != (const Te_MatrixSymm<D> &lval, const Te_MatrixSymm<D> &rval) {
   if(lval.whoAmI == rval.whoAmI) return 0;
   char ch = 0;
   if(lval.numRows != rval.numRows)
      ch = 1;
   else
      ch = memcmp(lval.matrix[0]+1,rval.matrix[0]+1,(rval.size-1)*sizeof(D));
   return ch;
}


//   ===========================================================
//   ====================   OPERATIONS   =======================
//   ===========================================================

//   ===========================================================
//   ***********************   Sum   ***************************
//   ===========================================================

//   *************************< + >*****************************
//   * Purpose: Addition between matrices                      *
//   * Description: Uses Sum from utility.cpp                  *
//   * Example: C = A + B;                                     *
//   ***********************************************************
template <class D> Te_MatrixSymm<D> operator + (const  Te_MatrixSymm<D> &lval,
                     const  Te_MatrixSymm<D> &rval)
   {
   if(lval.numRows != rval.numRows)
      lev_math::utility::Error("%s%s%s+",
       Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
    Te_MatrixSymm<D> result('*',lval.numRows,lval.numRows);
   Sum(rval.size,lval.matrix[1],rval.matrix[1],
            result.matrix[1]);
	return result;
   }

//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================

//   *************************< - >*****************************
//   * Purpose: Difference between matrices                    *
//   * Description: Uses Difference from utility.cpp           *
//   * Example:  A=B-C;                                        *
//   ***********************************************************
template <class D>
Te_MatrixSymm<D> operator - (const  Te_MatrixSymm<D> &lval,
   const  Te_MatrixSymm<D> &rval) {
   if(lval.numRows != rval.numRows)
      lev_math::utility::Error("%s%s%s-",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_MatrixSymm<D> result('*',lval.numRows,lval.numRows);
   Difference(rval.size,lval.matrix[1],rval.matrix[1],result.matrix[1]);
   return result;
}


//   ===========================================================
//   *              Non-modifying functions                    *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Printing                                       *
//   ***********************************************************
template <class D>
void  Te_MatrixSymm<D>::Print(char *msg) {
   fprintf(utility::bzzFileOut,"\nTe_MatrixSymm No.%d",whoAmI);
   if(*msg) fprintf(utility::bzzFileOut,"  %s",msg);
   fprintf(utility::bzzFileOut,"\nrows %d cols %d\n",numRows,numRows);
   for(int row=1;row<=numRows;row++) {
      for(int col=1;col<=numRows;col++) {
         if(row>col)
            fprintf(utility::bzzFileOut," %12.4e",matrix[row][col]);
         else
            fprintf(utility::bzzFileOut," %12.4e",matrix[col][row]);
      }
      fprintf(utility::bzzFileOut,"\n");
   }
}


//   ************************< Save >***************************
//   * Purpose: Saving a Te_MatrixSymm on formatted file       *
//   * Description: Can be read                                *
//   *              See TempFile in utility                    *
//   * Example: A.Save("Mat.Dat");                             *
//   ***********************************************************
template <class D>
void  Te_MatrixSymm<D>::Save(char *filematrix) { // formatted
   FILE *bzz;
   if((bzz = fopen(filematrix,"w")) == NULL)
      lev_math::utility::Error("%s%s%s%sSave",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);

   fprintf(bzz," %d %d\n",numRows,numColumns);
   for(int row = 1;row <= numRows;row++) {
      for(int column = 1;column <= row;column++)
         fprintf(bzz," %15.7e",matrix[row][column]);
      fprintf(bzz,"\n");
   }
   fclose(bzz);
}


//   ************************< Save >***************************
//   * Purpose: Saving a Te_MatrixSymm on an unformatted file  *
//   * Description: Can be read                                *
//   * Example: A.Save('*',MAT.BIN");                          *
//   *          See also TempFile from utility.cpp             *
//   ***********************************************************
template <class D>
void  Te_MatrixSymm<D>::Save(char,char *filematrix) {
   FILE *bzz;
   if((bzz = fopen(filematrix,"wb")) == NULL)
      lev_math::utility::Error("%s%s%s%sSave",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
   if(fwrite(&numRows,sizeof(int),1,bzz) != 1)
      lev_math::utility::Error("%s%s%sSave",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   if(fwrite(&numColumns,sizeof(int),1,bzz) != 1)
      lev_math::utility::Error("%s%s%sSave",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   if(fwrite(matrix[1],sizeof(D)*size,1,bzz) != 1)
      lev_math::utility::Error("%s%s%sSave",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_WRITING_FILE,utility::ERR_FUNCTION);
   fclose(bzz);
}


//   ===========================================================
//   *                 Modifying functions                     *
//   ===========================================================

//   **********************< Delete >***************************
//   * Purpose: Eliminating a matrix of no use                 *
//   *        without it leaving the scope                     *
//   * Example: Delete(&A);                                    *
//   ***********************************************************
template <class D>
void Delete( Te_MatrixSymm<D> *result) {
   result->Deinitialize();
   result->size = result->numRows = result->numColumns= 0;
   result->matrix = 0;
}


//   ***************< ChangeDimensions >************************
//   * Purpose: Change the dimensions of a Te_MatrixSymm       *
//   * Example: ChangeDimensions(row,columns,&A);              *
//   ***********************************************************
template <class D>
void ChangeDimensions(int rows,int columns, Te_MatrixSymm<D> *result,
   char zero) {
   if(rows != columns)
   lev_math::utility::Error("%s%s%sChangeDimensions",
      Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   int who = result->whoAmI;
   if(rows != result->numRows || columns != result->numColumns) {
      result->Deinitialize();
      result->Initialize(rows,columns);
      Te_MatrixSymm<D>::count--;
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
template <class D>void Recover( Te_MatrixSymm<D> *L,char *filematrix)
   {
   if((utility::bzzFileIn = fopen(filematrix,"r")) == NULL)
       lev_math::utility::Error("%s%s%s%sRecover",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_FUNCTION);
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
template <class D>void Recover( Te_MatrixSymm<D> *L,char,char *filematrix)
	{
	if((utility::bzzFileIn=fopen(filematrix,"rb"))==NULL)
		 lev_math::utility::Error("%s%s%s%sFILE",
         Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_OPEN_FILE,filematrix,utility::ERR_CONSTRUCTOR);
   int rows,columns;
   if(fread(&rows,sizeof(int),1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%sFILE",  Te_MatrixSymm<D>::ERRORMESSAGE);
   if(fread(&columns,sizeof(int),1,utility::bzzFileIn) != 1)
      lev_math::utility::Error("%sFILE",  Te_MatrixSymm<D>::ERRORMESSAGE);
   ChangeDimensions(rows,columns,L,1);
   if(rows < 1 || columns < 1)
      {fclose(utility::bzzFileIn);return;}
   if(fread(L->matrix[1],sizeof(D)*(L->size),
              1,utility::bzzFileIn) != 1)
        lev_math::utility::Error("%s%s%sRecover",
          Te_MatrixSymm<D>::ERRORMESSAGE,utility::ERR_READING_FILE,utility::ERR_FUNCTION);
   fclose(utility::bzzFileIn);
   }

//   ************************< Swap >***************************
//   * Purpose: Swapping the elements of any two Matrices      *
//   * Description: Provides an efficient method of copying    *
//   * Example: Swap(&A,&B);                                   *
//   ***********************************************************
template <class D>void Swap( Te_MatrixSymm<D> *lval, Te_MatrixSymm<D> *rval)
	{
	D **temp = lval->matrix;
	lval->matrix = rval->matrix;
	rval->matrix = temp;
	Swap(&lval->numColumns,&rval->numColumns);
	Swap(&lval->numRows,&rval->numRows);
	Swap(&lval->size,&rval->size);
	}


} //end lev_math namespace



#endif //SYMM_HPP
