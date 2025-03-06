#ifndef SPARSE_HPP
#define SPARSE_HPP

//   ======================< SPARSE.HPP >=======================
//   * Class MatrixSparse                                      *
//   * Description: Chapter 13                                 *
//   * Scientific C++ Building Numerical Libraries             *
//   *                the Object-Oriented Way                  *
//   * by G. Buzzi-Ferraris                                    *
//   * Addison-Wesley (1993)                                   *
//   * Ultima Atualizacao: 29/04/97 por MCC                    *
//   ===========================================================


#include <utility.hpp>
#include <vector.hpp>


namespace lev_math {

template <class D> Te_Vector<D> operator % (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval);
template <class D> Te_MatrixSparse<D> operator + (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);
template <class D> Te_MatrixSparse<D> operator - (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);
template <class D> Te_MatrixSparse<D> operator - (const Te_MatrixSparse<D> &rval);
template <class D> Te_MatrixSparse<D> Minus (const Te_MatrixSparse<D> &rval);
template <class D> void Product (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval, Te_Vector<D> *result);
template <class D> Te_Vector<D> operator * (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval);
template <class D> void ProductSymm (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval, Te_Vector<D> *result);
template <class D> void Product (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval, Te_MatrixSparse<D> *result);
template <class D> Te_MatrixSparse<D> operator * (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);
template <class D> Te_MatrixSparse<D> operator * (const D rval, const Te_MatrixSparse<D> &lval);
template <class D> Te_MatrixSparse<D> operator / (const Te_MatrixSparse<D> &lval,const D rval);
template <class D> Te_MatrixSparse<D> operator && (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);
template <class D> Te_MatrixSparse<D> operator || (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);
template <class D> Te_MatrixSparse<D> Transpose(Te_MatrixSparse<D> &A);
template <class D> Te_MatrixSparse<D> MakeRight(Te_MatrixSparse<D> &A);
template <class D> Te_MatrixSparse<D> MakeLeft(Te_MatrixSparse<D> &A);
template <class D> void Cholesky (Te_MatrixSparse<D> &MC, Te_MatrixSparse<D> &U, Te_MatrixSparse<D> &UT);
template <class D> void CholeskySRF (Te_MatrixSparse<D> &MC, Te_MatrixSparse<D> &U, Te_MatrixSparse<D> &UT);
template <class D> Te_MatrixSparse<D> DiagonalScaling(Te_MatrixSparse<D> &MC);
template <class D> Te_MatrixSparse<D> InverseDiag(Te_MatrixSparse<D> &A);
template <class D> void Solve(Te_MatrixSparse<D> *A,Te_Vector<D> *bx);
//template <class D1, class D2> Te_MatrixSparse<D2> operator * (D1 lval,const Te_MatrixSparse<D2> &rval);
//template <class D> Te_MatrixSparse<D> operator * (const double rval, const Te_MatrixSparse<D> &lval);



//   ===========================================================
//   =================   class MatrixSparse   ==================
//   ===========================================================


template <class T>
class MSElement {
public:
   int column;
   T value;
   MSElement<T> *next;
   MSElement() { next = 0;}
   MSElement(const MSElement<T> &A) {
      column = A.column;
      value = A.value;
      next = A.next;
   }
};


template <typename D, typename Q> class Sparse_Matrix;


template <class D> class Te_MatrixSparse {
private:
   static const char *const ERRORMESSAGE;
   int numRows,numColumns;

   D &InsertElement(MSElement<D> *elem,int row,int column,int first);
   void Initialize(int rows,int columns);
   void Copy(const Te_MatrixSparse<D> &rval);

public:
	//   ===========================================================
	//   ******************   constructors   ***********************
	//   ===========================================================
    // default constructor
    Te_MatrixSparse(void);

    // copy constructor
    Te_MatrixSparse(const Te_MatrixSparse<D> &rval);

    // copy LEV constructor
    template <typename Q>
    Te_MatrixSparse(const Sparse_Matrix<D,Q> &rval);

    // sizing constructor
    Te_MatrixSparse(int rows,int columns);

	//   ===========================================================
	//   ********************   destructor   ***********************
	//   ===========================================================
    ~Te_MatrixSparse(void);

	//   ===========================================================
	//   ******************   Access functions   *******************
	//   ===========================================================
    // number of rows
    template <typename Q>
    void ImportLEV(const Sparse_Matrix<D,Q> &lval);

    MSElement<D> **elementRow;
    int Rows(void) const { return numRows; }

    // number of columns
    int Columns(void) const { return numColumns; }

	// numero de elementos nao nulos de uma linha
	int GetRowElements (int row);

	// colunas nao nulas de uma determinada linha
	int	GetRowColumn (int row, int n);

	//numero total de elementos nao nulos
	int GetTotalElements(void){
		int total=0;
		for (int i=1;i<=numRows;i++)
			total+=GetRowElements(i);
		return total;
	}


   // assigns and receives vector values with control
   D &operator ()
      (int row,int column);

D GetValue ( int row, int column)
{
//   if(row < 1 || row > numRows || column < 1 || column > numColumns)	printf ("Erro na Matriz Esparsa \n");
	MSElement<D> *elem = elementRow[row];
	if(elem == 0 || column < elem->column)	return 0;
	if(column == elem->column)				return elem->value;
	for(;elem->next != 0;elem = elem->next)
	{
      if(column == elem->next->column)	return elem->next->value;
      else if(column < elem->next->column)break;
	}
	return 0;
}

	//   ***********************< Norms >***************************
   	D NormI(void);
	D NormIL(int);

	#if 10   // LUDecomposition
	// decomposicao LU de matriz esparsa

	// decomposicao LU de matriz esparsa
	int	LUDecomposition (Te_MatrixSparse<D> &result, long int *indx, Te_MatrixSparse<D> &L, Te_MatrixSparse<D> &U )
	{
		MSElement<D>	*elem, *elem2;
		int				i, j, k, imax;
		D				big, dum, temp, sum;
		Te_Vector<D>	vv(numRows);
		Te_Vector<D>	Col(numRows);

		// Calcula fatores de escala
		for( i = 1 ; i <= numRows ; i++ ) {
			big = 0.0;
			elem = elementRow[i];
			while ( elem ) {
				temp = Abs ( elem->value );
				if ( Abs(temp) > Abs(big) )
					big = temp;
				elem = elem->next;
			}
			if ( big == 0.0 ) {
				// mensagem de erro padrao da class
				return 0;
			}
			vv[i] = 1.0 / big;
		}

		// Varredura das colunas (algoritmo de Crout)
		for ( j = 1 ; j <= numColumns ; j++ ) {
			// Monta vetor com elementos da coluna em questao
			for ( i = 1 ; i <= numRows ; i++ ) {
				Col(i) = 0;
				elem = elementRow[i];
				while ( elem ) {
					if ( elem->column > j )
						break;
					if ( elem->column == j )
						Col(i) = elem->value;
					elem = elem->next;
				}
			}

			// Varredura das linhas ate j (coeficientes alfa da matriz L)
			for ( i = 1 ; i < j ; i++ ) {
				if ( GetValue(i,j) == 0.0 )
					continue;
				sum = Col(i);
				elem = elementRow[i];
				while ( elem ) {
					if ( elem->column >= i )
						break;
					if ( Col(elem->column) != 0)
						sum -= elem->value * Col(elem->column);
					elem = elem->next;
				}
				if ( Abs(sum) < 1.0e-10 ) {
					result.DeleteElement(i,j);
					continue;
				}
				result(i,j) = sum;
				Col(i) = sum;
			}

			big = 0.0;

			// Varredura das linhas ate o fim (coeficiente beta da matriz U)
			for ( i = j ; i <= numRows ; i++ ) {
				if ( GetValue(i,j) == 0.0 )
					continue;
				sum = Col(i);
				elem = elementRow[i];
				while ( elem ) {
					if ( elem->column >= j )
						break;
					if ( Col(elem->column) != 0 )
						sum -= elem->value * Col(elem->column);
					elem = elem->next;
				}
				if ( Abs(sum) < 1.0e-10 ) {
					result.DeleteElement(i,j);
					continue;
				}
				result(i,j)=sum;
				Col(i) = sum;

				dum = vv[i] * Abs(sum);
				if ( Abs(dum) >= Abs(big) ) {
					big = dum;
					imax = i;
				}
			}

			indx[j] = imax;

			// Divide pelo elemento de pivotamento
			if ( j != numColumns ) {
				dum = 1.0 / result(j,j);
				for ( i = j + 1 ; i <= numRows ; i++ ) {
					elem = elementRow[i];
					while ( elem ) {
						if(elem->column > j)
							break;
						if ( elem->column == j ) {
							result(i,j) *= dum;
							Col(i) *= dum;
							break;
						}
						elem = elem->next;
					}
				}
			}
		} // for j

		// Gera as matrizes L e U
		for ( i = 1 ; i <= numRows ; i++ ) {
			L(i,i) = 1.0;
			elem = result.elementRow[i];
			while ( elem ) {
				if ( elem->column < i )
					L(i,elem->column) = elem->value;
				else
					U(i,elem->column) = elem->value;
				elem = elem->next;
			}
		}

		return 1;
	}
	#endif  // LUDecomposition

	void LUDecomposition(Te_MatrixSparse<D> &L, Te_MatrixSparse<D> &U, D t)
	{
		//Declara��o de vari�veis
		L.Initialize(numRows,numColumns);
		U.Initialize(numRows,numColumns);

		int k;
		D tik,tfk,aux,aux2;
		Te_MatrixSparse<D> Ut;
		MSElement<D> *elemL, *elemU;
		D soma;

		// Primeira linha de U = Primeira Linha de A
		elemU= elementRow[1];
		aux2=elemU->value;
		while (elemU) {
			U(elemU->column,1)=elemU->value;
			elemU=elemU->next;
		}

		// Primeira coluna de L = (Primeira coluna de A)/U(1,1)
		L(1,1)=1;

		for(int i=2; i <= numRows;i++) {
			aux = GetValue(i,1);
			if(aux != 0)
				L(i,1) = aux/aux2;
		}

		// Executa restante da decomposicao
		for(int i=2;i <= numRows;i++) {
			aux2=t*NormIL(i);
			L(i,i)=1;
			for(int j=2;j<i;j++) {
				soma=0.0;
				elemL=L.elementRow[i];
				elemU=U.elementRow[j];
				while (((elemL)&&(elemU))&&(elemL->column <= (j-1))) {
					if(elemU->column==elemL->column) {
						soma+=elemU->value*elemL->value;
						elemU=elemU->next;
						elemL=elemL->next;
					} else if (elemU->column > elemL->column)
						elemL=elemL->next;
					else if (elemU->column < elemL->column)
						elemU=elemU->next;
				} // while
				aux=(GetValue(i,j)-soma)/U.GetValue(j,j);
				if (((aux>0)?aux:-aux) > aux2)
					L(i,j)=aux;
			} // for j
			for(int j=i ; j<= numRows;j++) {
				soma=0.0;
				elemL=L.elementRow[i];
				elemU=U.elementRow[j];
				while (((elemL)&&(elemU))&&(elemL->column <= (i-1))) {
					if(elemU->column==elemL->column) {
						soma+=elemU->value*elemL->value;
						elemU=elemU->next;
						elemL=elemL->next;
					} else if (elemU->column>elemL->column)
						elemL=elemL->next;
					else if (elemU->column<elemL->column)
						elemU=elemU->next;
				} // while
				aux=GetValue(i,j)-soma;
				if (((aux>0)?aux:-aux) > aux2)
					U(j,i)=aux;
			} // for j
		} // for i
		U=Transpose(U); //realiza a transposta de U (que estava sendo armazenada com os �ndices invertidos)
	}

	//   ===========================================================
	//   ******************  assignment operators ******************
	//   ===========================================================
	Te_MatrixSparse<D> &operator = (const Te_MatrixSparse<D> &rval);

	//   ===========================================================
	//   ====================   OPERATIONS   =======================
	//   ===========================================================

	//   ===========================================================
	//   ***********************   Sum   ***************************
	//   ===========================================================

	friend Te_MatrixSparse<D> operator + <> (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);

	Te_MatrixSparse<D> &operator += (const Te_MatrixSparse<D> &rval);

	//   ===========================================================
	//   ********************   Difference   ***********************
	//   ===========================================================

	friend Te_MatrixSparse<D> operator - <> (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);

	Te_MatrixSparse<D> &operator -= (const Te_MatrixSparse<D> &rval);

	//   ===========================================================
	//   ********************     Minus      ***********************
	//   ===========================================================

	friend Te_MatrixSparse<D> operator - <> (const Te_MatrixSparse<D> &rval);

	friend Te_MatrixSparse<D> Minus <> (const Te_MatrixSparse<D> &rval);

	//   ===========================================================
	//   **********************   Product   ************************
	//   ===========================================================


	friend void Product <> (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval, Te_Vector<D> *result);

	friend Te_Vector<D> operator * <> (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval);

	friend void ProductSymm <> (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval, Te_Vector<D> *result);

	friend Te_Vector<D> operator % <> (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval);

	// C=A*B;
	friend void Product <> (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval, Te_MatrixSparse<D> *result);

	// C=A*B;
	friend Te_MatrixSparse<D> operator * <> (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);

	// C = 3.*A;
	friend Te_MatrixSparse<D> operator * <> (const D rval, const Te_MatrixSparse<D> &lval);

	//   ===========================================================
	//   **********************   Division   ***********************
	//   ===========================================================
	// C = A / 3.;
	friend Te_MatrixSparse<D> operator / <> (const Te_MatrixSparse<D> &lval,const D rval);

	//   ===========================================================
	//   ***********   operators for composing matrices   **********
	//   ===========================================================

	// adds a matrix beneath another
	friend Te_MatrixSparse<D> operator && <> (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);

	// add a matrix to the side of another
	friend Te_MatrixSparse<D> operator || <> (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval);

   // calcula a transposta de uma matriz dada
   friend Te_MatrixSparse<D> Transpose <> (Te_MatrixSparse<D> &A);

   // gera uma matriz com a parte superior de uma matriz dada
   friend Te_MatrixSparse<D> MakeRight <> (Te_MatrixSparse<D> &A);

   // gera uma matriz com a parte inferior de uma matriz dada
   friend Te_MatrixSparse<D> MakeLeft <> (Te_MatrixSparse<D> &A);

   // gera as matrizes provenientes da decomposicao de Cholesky
   friend void Cholesky <> (Te_MatrixSparse<D> &MC, Te_MatrixSparse<D> &U,
      Te_MatrixSparse<D> &UT);

   // gera as matrizes provenientes da decomposicao de Cholesky
   friend void CholeskySRF <> (Te_MatrixSparse<D> &MC,
      Te_MatrixSparse<D> &U, Te_MatrixSparse<D> &UT);

   // faz o escalonamento da diagonal principal de uma matriz
   friend Te_MatrixSparse<D> DiagonalScaling <> (Te_MatrixSparse<D> &MC);

	//   ===========================================================
	//   ===========   Non-modifying functions   ===================
	//   ===========================================================

    //   ***********************< Print >***************************
    void Print(char *msg="");
	void Save(FILE *fp);


	//   ===========================================================
	//   =============   Modifying Functions =======================
	//   ===========================================================
   void DeleteElement(int row,int column);
   void DeleteRow(int row);
   void DeleteColumn(int column);
   void DeleteMatrix(void);
   void CleanMatrix(void);// eliminates all elements
   void CleanZeroMatrix(void);// eliminates all elements
   void CleanMatrix(double eps); // eliminates those <= eps

//   ===========================================================
//   =========   System solution functions   ===================
//   ===========================================================
   void SolveRight(Te_Vector<D> *bx);
   void SolveLeft(Te_Vector<D> *bx);

   // calcula a inversa de uma matriz simetrica
   friend Te_MatrixSparse<D> InverseDiag <> (Te_MatrixSparse<D> &A);

   friend void Solve <> (Te_MatrixSparse<D> *A,Te_Vector<D> *bx);
};



//   =====================< SPARSE.CPP >========================
//   * Class MatrixSparse                                      *
//   * Description: Chapter 13                                 *
//   ===========================================================
template <class D>
const char *const Te_MatrixSparse<D>::ERRORMESSAGE = "\n========>>> Te_MatrixSparse error!!!!!!\n";


//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   *************************< Initialize >********************
//   * Purpose: Common function for initialisation             *
//   * Description: Chapter 13                                 *
//   * Example: Initialize(rows,columns)                       *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::Initialize(int rows,int columns)
{
	if(rows < 1 || columns < 1) {
		numRows = numColumns = 0;
		elementRow = NULL;
		return;
	}
	numRows = rows;
	numColumns = columns;
	elementRow = new MSElement<D> *[numRows + 1];
	if(!elementRow)
		lev_math::utility::Error("%s%s%sInitialize", Te_MatrixSparse<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
	for(int row = 0;row <= numRows;row++)
		elementRow[row] = NULL;
	return;
}


//   ===========================================================
//   ================= Private functions =======================
//   ===========================================================

//   *************************< Initialize >********************
//   * Purpose: Function used to import an sparse matrix from  *
//   * Description:                                      LEV   *
//   * Example: ImportLEV(A)                                   *
//   ***********************************************************
template <class D>
template <typename Q>
void Te_MatrixSparse<D>::ImportLEV(const Sparse_Matrix<D,Q> &lval)
{
	// Importacao de matriz da classe LEV
	long prim_ant,prim,pos;
	int dim;
	D val;
	dim=lval.prim.Get_Total_Componentes();
	Te_MatrixSparse<D> A;
	this->Initialize(dim,dim);
	A.Initialize(dim,dim);

	for(int i=0; i < dim ; i++){
		prim_ant=lval.prim[i];
		prim = ((i==dim-1)?lval.position.Get_Total_Componentes():lval.prim[i+1]);
		for(int j=prim_ant; j < prim; j++){
			pos = lval.position[j];
			val = lval.value[j];
			if (val != 0) A(i+1,pos+1)=val;
		}
	}

	Copy(A);
}


//   *************************< Copy >**************************
//   * Purpose: Copying a sparse matrix                        *
//   * Description: Used in copy-initializer                   *
//   *              and in assignments                         *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::Copy(const Te_MatrixSparse<D> &rval)
{
	for(int row = 1;row <= numRows;row++) {
	   MSElement<D> *elem,*elemRval;
	   int first = 1;
	   for(elemRval = rval.elementRow[row]; elemRval != 0;elemRval = elemRval->next) {
		   MSElement<D> *newElement = new MSElement<D>;
		   if(!newElement)
			   lev_math::utility::Error("%s%s%sCopy", Te_MatrixSparse<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
		   newElement->column = elemRval->column;
		   newElement->value = elemRval->value;
		   newElement->next = 0;
		   if(first == 1) {
			   elem = elementRow[row] = newElement;
			   first = 0;
		   } else {
			   elem->next = newElement;
			   elem = elem->next;
		   }
	   }
	}
}


//   *******************< InsertElement >***********************
//   * Purpose: Inserting an element                           *
//   * Description: Used internally                            *
//   ***********************************************************
template <class D>
D &Te_MatrixSparse<D>::InsertElement(MSElement<D> *elem,int row,int column,int first)
{
	MSElement<D> *newElement = new MSElement<D>;
	if(!newElement)
		lev_math::utility::Error("%s%s%sInsertElement", Te_MatrixSparse<D>::ERRORMESSAGE,utility::ERR_SPACE,utility::ERR_FUNCTION);
	newElement->column = column;
	newElement->value = 0;
	if(first == 1) {
		newElement->next = elem;
		elementRow[row] = newElement;
	} else {
		newElement->next = elem->next;  // inserted next
		elem->next = newElement;
	}
	return newElement->value;
}


//   ===========================================================
//   ================== Public functions =======================
//   ===========================================================

//   ===========================================================
//   ******************   constructors   ***********************
//   ===========================================================

// ***********************< default >****************************
// * Purpose: Defining a Te_MatrixSparse which can then receive *
// *          an assignment                                     *
// * Examples: Te_MatrixSparse A;                               *
// *           A = B;                                           *
// **************************************************************
template <class D>
Te_MatrixSparse<D>::Te_MatrixSparse(void)
{
	Initialize(0,0);
}


//   ******************< Copy-Initialize >**********************
//   * Purpose: For definition and initialisation              *
//   *          of Te_MatrixSparse                             *
//   * Examples: Te_MatrixSparse A=B;                          *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D>::Te_MatrixSparse(const Te_MatrixSparse<D> &rval)
{
	Initialize(rval.numRows,rval.numColumns);
	Copy(rval);
}


//   ********************< dimensions >*************************
//   * Purpose: Providing the dimensions of a matrix           *
//   * Examples: Te_MatrixSparse A(120,350);                   *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D>::Te_MatrixSparse(int rows,int columns)
{
	Initialize(rows,columns);
}


//   **********************< Copy LEV>**************************
//   * Purpose: Copying a sparse matrix from LEV               *
//   * Description: Used in copy-initializer                   *
//   *              and in assignments                         *
//   ***********************************************************
template <class D>
template <typename Q>
Te_MatrixSparse<D>::Te_MatrixSparse(const Sparse_Matrix<D,Q> &rval)
{
	ImportLEV(rval);
}


//   ===========================================================
//   ********************   destructor   ***********************
//   ===========================================================
template <class D>
Te_MatrixSparse<D>::~Te_MatrixSparse(void)
{
	DeleteMatrix();
}


//   ===========================================================
//   *****************    Access functions *********************
//   ===========================================================
//   ********************< GetRowElements >*********************
//   * Purpose: Retorna o numero de elementos nao nulos 	   *
//				de uma linha  								   *
//   * Example: A.GetRowElements(5);	                       *
//   ***********************************************************
template <class D>
int	Te_MatrixSparse<D>::GetRowElements(int row)
{
	if(row < 1 || row > numRows)return 0;
	if(elementRow == 0)return 0;
	int n = 0;
	MSElement<D> *elem;

	elem = elementRow[row];
	while (elem!=0)
	 {
		elem = elem->next;
		n++;
	 }
	return n;
}

//   ********************< GetRowColumn >***********************
//   * Purpose: Retorna a enesima coluna nao nula de uma 	   *
//				determinada linha 							   *
//   * Example: A.GetRowColumn(5,n);	n = 6	               *
//   ***********************************************************
template <class D>
int	Te_MatrixSparse<D>::GetRowColumn(int row, int n)
{
	if(row < 1 || row > numRows)return 0;
	if(elementRow == 0)return 0;
	if (n > GetRowElements(row)) return 0;
	int	i;
	MSElement<D> *elem;

	elem = elementRow[row];
	for (i=1; i<n; i++)
	 {
		elem = elem->next;
	 }
	return elem->column;
}


//   ********************< operator () >************************
//   * Purpose: Receiving and assigning values with control    *
//   * Example: x = A(1,5); A(3,7) = 5.;                       *
//   ***********************************************************
template <class D>
D &Te_MatrixSparse<D>::operator () (int row,int column)
   {
   if(row < 1 || row > numRows ||
      column < 1 || column > numColumns)
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Error("%s%s%s()",
        Te_MatrixSparse<D>::ERRORMESSAGE,
        utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   MSElement<D> *elem = elementRow[row];
   if(elem == 0 || column < elem->column)
      return InsertElement(elem,row,column,1);
   if(column == elem->column)
      return elem->value;
   for(;elem->next != 0;elem = elem->next)
      {
      if(column == elem->next->column)
        return elem->next->value;
      else if(column < elem->next->column)break;
      }
   return InsertElement(elem,row,column,0);
   }

//   ===========================================================
//   ******************   assignment operators *****************
//   ===========================================================

//   *************************< = >*****************************
//   * Purpose: Assignment of a Te_MatrixSparse                   *
//   * Example: A = B;                                         *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> &Te_MatrixSparse<D>::operator = (const Te_MatrixSparse<D> &rval)
{
	CleanMatrix();
	if(numColumns != rval.numColumns)
		numColumns = rval.numColumns;
	if(numRows != rval.numRows) {
		delete [] elementRow; elementRow = NULL;
		Initialize(rval.numRows,rval.numColumns);
	}
	Copy(rval);
	return *this;
}


//   ===========================================================
//   ====================   OPERATIONS   =======================
//   ===========================================================

//   ===========================================================
//   ***********************   Sum   ***************************
//   ===========================================================

//   *************************< + >*****************************
//   * Purpose: Addition between matrices                      *
//   * Example: A = B + C;                                     *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> operator +
      (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval)
   {
   if(lval.numRows != rval.numRows ||
      lval.numColumns != rval.numColumns)
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Error("%s%s%s+",
        Te_MatrixSparse<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,
        utility::ERR_OPERATOR);
   Te_MatrixSparse<D> result = lval;
   result += rval;
   return result;
   }

//   *************************< += >****************************
//   * Purpose: Adding a matrix which is also on the right     *
//   * Example: A += B; // A = A + B;                          *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> &Te_MatrixSparse<D>::operator +=
             (const Te_MatrixSparse<D> &rval)
   {
   if(numRows != rval.numRows
             ||
   numColumns != rval.numColumns)
//		printf ("Erro na Matriz Esparsa \n");
       lev_math::utility::Error("%s%s%s+=",Te_MatrixSparse<D>::ERRORMESSAGE,
       utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   MSElement<D> *elem;
   for(int row = 1;row <= numRows;row++)
      {
      	elem = rval.elementRow[row];
		while (elem)
		 {
			(*this)(row,elem->column) += elem->value;
			elem = elem->next;
		 }
      }
   return *this;
   }

//   ===========================================================
//   ********************   Difference   ***********************
//   ===========================================================

//   *************************< - >*****************************
//   * Purpose: Difference between matrices                    *
//   * Example: A = B - C;                                     *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> operator -
       (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval)
   {
   if(lval.numRows != rval.numRows ||
      lval.numColumns != rval.numColumns)
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Error("%s%s%s+",
        Te_MatrixSparse<D>::ERRORMESSAGE,
        utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_MatrixSparse<D> result = lval;
   result -= rval;
   return result;
   }

//   *************************< -= >****************************
//   * Purpose: Difference from a matrix also on the right     *
//   * Example: A -= B; // A = A - B;                          *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> &Te_MatrixSparse<D>::operator -=
                 (const Te_MatrixSparse<D> &rval)
   {
   if(numRows != rval.numRows
             ||
   numColumns != rval.numColumns)
//		printf ("Erro na Matriz Esparsa \n");
       lev_math::utility::Error("%s%s%s+=",Te_MatrixSparse<D>::ERRORMESSAGE,
       utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   MSElement<D> *elem;
   for(int row = 1;row <= numRows;row++)
      {
	  	elem = rval.elementRow[row];
		while (elem)
		 {
			(*this)(row,elem->column) -= elem->value;
			elem = elem->next;
		 }
      }
   return *this;
   }

//   ===========================================================
//   ********************   	Minus 	 ***********************
//   ===========================================================
//   ************************< Minus >**************************
//   * Purpose: Unary minus                                    *
//   * Example: B = Minus(A); B = -A;                           *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> Minus (const Te_MatrixSparse<D> &rval)
   {
	int 	row;
	Te_MatrixSparse<D> result = rval;

      MSElement<D> *elem;
      for(row = 1;row <= rval.numRows;row++)
        {
	        elem = rval.elementRow[row];
			while (elem!=0)
			 {
				result(row,elem->column) = - elem->value;
				elem = elem->next;
			 }
 		}
	return 	result;
   }

//   *************************< - >*****************************
//   * Purpose: Unary minus                                    *
//   * Example: A = -B;                                        *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> operator -(const Te_MatrixSparse<D> &rval) //unary minus
   {
   Te_MatrixSparse<D> result;
   result = Minus(rval);
   return result;
   }


//   ===========================================================
//   **********************   Product   ************************
//   ===========================================================
//   ***********************< Product >*************************
//   * Purpose: Product between sparse matrices and vectors    *
//   * Example: Product(A,x,&y); y = A*x;                      *
//   ***********************************************************
template <class D>
void Product(const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval,
            Te_Vector<D> *result)
   {
   if(lval.numColumns != rval.Size())
//		printf ("Erro na Matriz Esparsa \n");
       lev_math::utility::Error("%s%s%s*",
       Te_MatrixSparse<D>::ERRORMESSAGE,
       utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(rval.WhoAmI() == result->WhoAmI())
      {
      Te_Vector<D> aux;
      Product(lval,rval,&aux); // call recursive
      Swap(&aux,result); // avoid copying
      }
   else
      {
      ChangeDimensions(lval.numRows,result,0);
      for(int i=1;i <= lval.numRows;i++)
        (*result)[i] = 0; // initialise
      MSElement<D> *elem;
      for(int row = 1;row <= lval.numRows;row++)
        {
        elem = lval.elementRow[row];
        for(int column = 1;
               column <= lval.numColumns;column++)
           {
           if(elem == 0)break;
           if(elem->column == column)
              {
              D *temp = &(*result)[row];
              *temp = *temp + elem->value * rval.GetValue(column);
///						Control(*temp +
///                  elem->value * rval.GetValue(column));
              elem = elem->next;
              }
           }
        }
      }
   }

//   ***********************< ProductSymm >*********************
//   * Purpose: Product between symmetric sparse matrices 	   *
//	 *			and vectors									   *
//   * Example: ProductSymm(A,x,&y); y = A % x;                 *
//   ***********************************************************
template <class D>
void ProductSymm(const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval,
            Te_Vector<D> *result)
   {
	int	n;
   if(lval.numColumns != rval.Size())
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Error("%s%s%s*",
        Te_MatrixSparse<D>::ERRORMESSAGE,
        utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   if(rval.WhoAmI() == result->WhoAmI())
      {
      Te_Vector<D> aux;
      Product(lval,rval,&aux); // call recursive
      Swap(&aux,result); // avoid copying
      }
   else
      {
      ChangeDimensions(lval.numRows,result,0);
      for(int i=1;i <= lval.numRows;i++)
        (*result)[i] = 0; // initialise
      MSElement<D> *elem;
      for(int row = 1;row <= lval.numRows;row++)
        {
		n = 0;
        elem = lval.elementRow[row];
		while (elem!=0)
		 {
			elem = elem->next;
			n++;
		 }
		elem = lval.elementRow[row];

       for(int column = 1; column <= n; column++)
           {
              D *temp = &(*result)[row];
              *temp = *temp + elem->value * rval.GetValue(elem->column);
			  if (elem->column > row)
				(*result)[elem->column] = (*result)[elem->column] + elem->value * rval.GetValue(row);
              elem = elem->next;
           }
        }
      }
   }

//   *************************< * >*****************************
//   * Purpose: Product between sparse matrices and vectors    *
//   * Example: y = A * x;                                     *
//   ***********************************************************
template <class D>
Te_Vector<D> operator *
       (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval)
   {
   Te_Vector<D> result;
   Product(lval,rval,&result);
   return result;
   }


//   *************************< % >*****************************
//   * Purpose: Product between symmetric sparse matrices 	   *
//	 *			and vectors 								   *
//   * Example: y = A  % x;                                     *
//   ***********************************************************
template <class D>
Te_Vector<D> operator %
       (const Te_MatrixSparse<D> &lval,const Te_Vector<D> &rval)
   {
   Te_Vector<D> result;
   ProductSymm(lval,rval,&result);
   return result;
   }

//   ***********************< Product >*************************
//   * Purpose: Product between matrices                       *
//   * Description: Pointers are used to eliminate the         *
//                  use of double indices                      *
//   * Example: Product(A,B,&C); C = A * B;                    *
//   ***********************************************************
template <class D>
void Product(const Te_MatrixSparse<D> &lval, const Te_MatrixSparse<D> &rval, Te_MatrixSparse<D> *result) {
   if(lval.numColumns != rval.numRows)
      lev_math::utility::Error("%s%s%sProduct", Te_MatrixSparse<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
      // printf ("Erro na multiplicacao de matrizes esparsas\n");
   MSElement<D> *eleml, *elemr;
   int row;

   for (row = 1; row <= lval.numRows; row++) {
      eleml = lval.elementRow[row];
      while (eleml) {
         elemr = rval.elementRow[eleml->column];
         while (elemr) {
            (*result)(row,elemr->column) +=  eleml->value * elemr->value;
            elemr = elemr->next;
         }
         eleml = eleml->next;
      }
   }
}


//   *************************< * >*****************************
//   * Purpose: Product between matrices                       *
//   * Description: Uses Product(A,B,&C)                       *
//   * Example: C = A * B;                                     *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> operator * (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval) {
   Te_MatrixSparse<D> result(lval.numRows,rval.numColumns);
   Product(lval,rval,&result);
   return result;
}


//   *************************< * >*****************************
//   * Purpose: Product between a matrice and a value          *
//   * Description:Devolve a matriz resultante de uma multipl. *
//   * Example: C = 3.* A;                                     *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> operator * (const D rval, const Te_MatrixSparse<D> &lval)
   {
   Te_MatrixSparse<D> result(lval.numRows,lval.numColumns);

	MSElement<D>	*elem;
	int			row;

	for (row = 1; row <= lval.numRows; row++)
	 {
		elem = lval.elementRow[row];
		while (elem)
		 {
			result(row,elem->column) =  elem->value * rval;
			elem = elem->next;
		 }
	 }

   return result;
   }

//   friend Te_MatrixSparse<D> operator *<>     // C = 3.*A;
//    (double rval, const Te_MatrixSparse<D> &lval);
//   *************************< * >*****************************
//   * Purpose: Product between a matrice and a value          *
//   * Description:Devolve a matriz resultante de uma multipl. *
//   * Example: C = 3.* A;                                     *
//   ***********************************************************
/***********/
//template <class D>
//Te_MatrixSparse<D> operator * (const double rval, const Te_MatrixSparse<D> &lval)
#if 0
template <class D1, class D2>
Te_MatrixSparse<D2> operator * (D1 rval,const Te_MatrixSparse<D2> &lval)
   {
   Te_MatrixSparse<D2> result(lval.numRows,lval.numColumns);

	MSElement<D2>	*elem;
	int			row;

	for (row = 1; row <= lval.numRows; row++)
	 {
		elem = lval.elementRow[row];
		while (elem)
		 {
			result(row,elem->column) =  elem->value * rval;
			elem = elem->next;
		 }
	 }

   return result;
   }
#endif

/*******************/

//   ===========================================================
//   **********************   Division  ************************
//   ===========================================================
//   *************************< / >*****************************
//   * Purpose: Division between a matrice and a value         *
//   * Description:Devolve a matriz resultante de uma divisao  *
//   * Example: C = A / 3.;                                    *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> operator / (const Te_MatrixSparse<D> &lval,const D rval) {
   Te_MatrixSparse<D> result(lval.numRows,lval.numColumns);
   if( !Abs(rval) )
      // printf ("Erro na divisao de matrizes esparsas\n");
      lev_math::utility::Error("%s%s%sProduct", Te_Matrix<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   MSElement<D> *elem;
   int row;

   for (row = 1; row <= lval.numRows; row++) {
      elem = lval.elementRow[row];
      while (elem) {
         result(row,elem->column) =  elem->value / rval;
         elem = elem->next;
      }
   }
   return result;
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
Te_MatrixSparse<D> operator && (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval) {
   if(lval.numColumns != rval.numColumns)
      // printf ("Erro na matriz esparsa\n");
      lev_math::utility::Error("%s%s%s||", Te_Matrix<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_MatrixSparse<D> result(lval.numRows + rval.numRows, lval.numColumns);
   int row;
   MSElement<D> *elem;

   for (row = 1; row <= lval.numRows; row++) {
      elem = lval.elementRow[row];
      while (elem) {
         result(row,elem->column) = elem->value;
         elem = elem->next;
      }
   }

   for (row = 1; row <= rval.numRows; row++) {
      elem = rval.elementRow[row];
      while (elem) {
         result(lval.numRows + row, elem->column) = elem->value;
         elem = elem->next;
      }
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
Te_MatrixSparse<D> operator || (const Te_MatrixSparse<D> &lval,const Te_MatrixSparse<D> &rval) {
   if(lval.numRows != rval.numRows)
   // printf ("Erro na matriz esparsa ||\n");
   lev_math::utility::Error("%s%s%s&&", Te_Matrix<D>::ERRORMESSAGE,utility::ERR_CHECK_DIMENSION,utility::ERR_OPERATOR);
   Te_MatrixSparse<D> result(lval.numRows, lval.numColumns + rval.numColumns);
   int row;
   MSElement<D> *elem;

   for (row = 1; row <= lval.numRows; row++) {
      elem = lval.elementRow[row];
      while (elem) {
         result(row,elem->column) = elem->value;
         elem = elem->next;
      }
      elem = rval.elementRow[row];
      while (elem) {
         result(row, lval.numColumns + elem->column) = elem->value;
         elem = elem->next;
      }
   }
   return result;
}


//   *********************< Transpose >*************************
//   * Purpose: Modifying a matrix into its transpose          *
//   * Description: Gera uma nova matriz que seja a transposta *
//   * Example: B = Transpose(A);                              *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> Transpose(Te_MatrixSparse<D> &A) {
   Te_MatrixSparse<D> result(A.numColumns, A.numRows);
	MSElement<D>		*elem;
	int 		row;

	for (row = 1; row <= A.numRows; row++)
	 {
		elem = A.elementRow[row];
		while (elem)
		 {
			result(elem->column, row) = elem->value;
			elem = elem->next;
		 }
	 }
	return result;

   }


//   ===========================================================
//   *********************   Norms   ***************************
//   ===========================================================

//   ***********************< NormI >***************************
//   * Purpose: Max of the sums of the abs values              *
//   *          of coeff. rows                                 *
//   * Example: D normR = A.NormR ();
//   ***********************************************************

template <class D>
D Te_MatrixSparse<D>::NormI(void) {
   MSElement<D> *elem;
   D norm = 0;

   for(int i = 1;i <= numRows;i++) {
       D sum = 0;
	   elem=elementRow[i];
	   while(elem){
		   sum+=Abs(elem->value);
		   elem=elem->next;
	   }
	   if (sum>norm) norm=sum;
   }
   return norm;
}

//   ***********************< NormIL >***************************
//   * Purpose: Max of the sums of the abs values	            *
//   *          of the i-th row 	                            *
//   * Example: double normR = A.NormIL(int row);     	        *
//   ***********************************************************

template <class D>
D Te_MatrixSparse<D>::NormIL(int row) {
	MSElement<D> *elem;
	D norm=0;
	elem=elementRow[row];
	while(elem){
		if (Abs(elem->value) > norm)
			norm = Abs(elem->value);
		elem=elem->next;
	}
	return norm;
}

//   *********************< MakeRight >*************************
//   * Purpose: Modifica uma matriz simetrica, gerando apenas  *
//				sua parte superior				   		       *
//   * Description: Gera uma matriz simetrica superior		   *
//   * Example: B = MakeRight(A);                              *
 //   ***********************************************************
template <class D>
Te_MatrixSparse<D> MakeRight(Te_MatrixSparse<D> &A)
   {
	Te_MatrixSparse<D>	result(A.numRows, A.numRows);
	MSElement<D>		*elem;
	int 		row;

	if (A.numColumns != A.numRows)
		printf("Erro na matriz esparsa: Matriz nao e' quadrada\n");

	for (row = 1; row <= A.numRows; row++)
	 {
		elem = A.elementRow[row];
		while (elem)
		 {
			if (elem->column >= row)
				result(row, elem->column) = elem->value;
			elem = elem->next;
		 }
	 }

	return result;

   }

//   *********************< MakeLeft >*************************
//   * Purpose: Modifica uma matriz simetrica, gerando apenas  *
//				sua parte inferior				   		       *
//   * Description: Gera uma matriz simetrica inferior		   *
//   * Example: B = MakeLeft(A);                               *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> MakeLeft(Te_MatrixSparse<D> &A)
   {
	Te_MatrixSparse<D>		result(A.numRows, A.numRows);
	MSElement<D>		*elem;
	int 		row;

	if (A.numColumns != A.numRows)
		printf("Erro na matriz esparsa: Matriz nao e' quadrada\n");

	for (row = 1; row <= A.numRows; row++)
	 {
		elem = A.elementRow[row];
		while (elem)
		 {
			if (elem->column <= row)
				result(row, elem->column) = elem->value;
			elem = elem->next;
		 }
	 }
	return result;

   }

//   ===========================================================
//   *              Non-modifying functions                     *
//   ===========================================================

//   ***********************< Print >***************************
//   * Purpose: Printing out the elements of a matrix          *
//   * Description: Uses the file utility::bzzFileOut global            *
//   *              in utility.hpp                             *
//   * Example: A.Print("Matrix A");                           *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::Print(char *msg)
   {
   MSElement<D> *elem;
   if(*msg)fprintf(utility::bzzFileOut,"\n%s",msg);
   fprintf(utility::bzzFileOut,"\nrows %d columns %d\n",
          numRows,numColumns);
   for(int row = 1;row <= numRows;row++)
      {
      elem = elementRow[row];
      for(int column = 1;column <= numColumns;column++)
        {
        if(elem != 0 && elem->column == column)
           {
           fprintf(utility::bzzFileOut," %12.2e",elem->value);
           elem=elem->next;
           }
        else
           fprintf(utility::bzzFileOut,"      *     ");
        }
      fprintf(utility::bzzFileOut,"\n");
      }
   }


//   ***********************< Save >***************************
//   * Purpose:  Salva matriz no formato da SparseLib++        *
//   * Example: A.Save(fp);                           *
//   ***********************************************************
template <class D> void Te_MatrixSparse<D>::Save(FILE *fp)
{
	MSElement<D> *elem;
	int			 flag=0;
	double		 zero = 0.0;

	for(int row = 1 ; row <= numRows ; row++)
	{
		elem = elementRow[row];
		for(int column = 1;column <= numColumns;column++)
		{
			if(elem != 0 && elem->column == column)
			{
				fprintf(fp,"%d\t%d\t%e\n",row,column,elem->value);
				if ( row == numRows && column == numColumns )	flag = 1;
				elem=elem->next;
			}
		}
	}
	if ( flag == 0 )
		fprintf(fp,"%14d\t%14d\t%g\n",numRows,numColumns,zero);
}

//   ===========================================================
//   *                 Modifying functions                      *
//   ===========================================================
//   ******************< DeleteElement >************************
//   * Purpose: Eliminating an element from a matrix           *
//   * Example: A.DeleteElement(i,j);                          *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::DeleteElement(int row,int column)
{
	if(elementRow == 0)
		return;
	if(row < 1 || row > numRows)
		return;
	if(column < 1 || column > numColumns)
		return;
	MSElement<D> *elem,*temp;
	elem = elementRow[row];
	if(elem == NULL)
		return;
	if(column == elem->column) {
		elementRow[row] = elem->next;
		delete elem; elem = NULL;
		return;
	}
	for(;elem->next != 0; elem = elem->next) {
		if(column == elem->next->column) {
			temp = elem->next;
			elem->next = elem->next->next;
			if (temp) {
				delete temp; temp = NULL;
			}
			return;
		}
	}
}


//   ********************< DeleteRow >**************************
//   * Purpose: Eliminating a row from a matrix                *
//   * Example: A.DeleteRow(i);                                *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::DeleteRow(int row)
{
	if(row < 1 || row > numRows)
		return;
	if(elementRow == NULL)
		return;
	MSElement<D> *elem, *temp;
	elem = elementRow[row];
	while(elem != NULL) {
		temp = elem;
		elem = elem->next;
		delete temp; temp = NULL;
	}
	elementRow[row] = NULL;
}


//   *******************< DeleteMatrix >************************
//   * Purpose: Eliminating a matrix of no use                 *
//   * Example: A.DeleteMatrix();                              *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::DeleteMatrix(void)
{
	if(elementRow == NULL)
		return;
	CleanMatrix();
	delete [] elementRow;
	elementRow = NULL;
	numRows = numColumns = 0;
}


//   *******************< CleanMatrix >*************************
//   * Purpose: Eliminating all coefficients of a matrix       *
//   * Example: A.CleanMatrix();                               *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::CleanMatrix(void)
{
	if(elementRow == NULL)
		return;
	MSElement<D> *elem, *temp;
	for(int row =1; row <= numRows; row++) {
		elem = elementRow[row];
		while(elem != NULL) {
			temp = elem;
			elem = elem->next;
			delete temp; temp = NULL;
		}
		elementRow[row] = NULL;
	}
}


//   *******************< CleanMatrix >*************************
//   * Purpose: Eliminating the coefficients of a matrix which *
//   *          are smaller than eps                           *
//   * Example: A.CleanMatrix(1.e-20);                         *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::CleanMatrix(double eps)
   {
   if(elementRow == 0)return;
   eps = utility::Abs(eps);
   MSElement<D> *elem,*temp;
   for(int row =1; row <= numRows; row++)
      {
      elem = elementRow[row];
      for(int column = 1;elem != 0;column++)
        {
        if(elem->column == column)
           {
           if(Abs(elem->value) < eps)
              DeleteElement(row,column);
           elem=elem->next;
           }
        }
      }
   }

//   *******************< CleanZeroMatrix >*************************
//   * Purpose: Eliminating the coefficients of a matrix which *
//   *          are equal to zero                           *
//   * Example: A.CleanZeroMatrix();                         *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::CleanZeroMatrix(void)
{
   if(elementRow == 0)return;
   MSElement<D> *elem;
   for(int row =1; row <= numRows; row++)
      {
      elem = elementRow[row];
      for(int column = 1;elem != NULL /*&& elem != (MSElement<D> *)0xdddddddd*/;column++)
        {
        if(elem->column == column)
           {
           if(Abs(elem->value) == 0.0)
			{
        	   elem=elem->next;
        	   DeleteElement(row,column);
			} else
				elem=elem->next;
           }
        }
      }
}


//   ===========================================================
//   ==========   System solving functions =====================
//   ===========================================================
//   ********************< SolveRight >*************************
//   * Purpose: Solving a system with a Right Te_MatrixSparse     *
//   * Example: A.SolveRight(&bx);                             *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::SolveRight(Te_Vector<D> *bx)
   {
	int n;
   if(numRows != bx->dimensions)
//		printf ("Erro na Matriz Esparsa \n");
      lev_math::utility::Error("%s%s%sSolve",
      Te_MatrixSparse<D>::ERRORMESSAGE,
      utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   MSElement<D> *elem;
   D sum;
   D temp,*x = bx->vector;
   for(int row = numRows;row >= 1;row--)
      {
      elem = elementRow[row];
      if(elem != 0 && elem->column < row)
        {
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Error("%sMatrix non Right",
        Te_MatrixSparse<D>::ERRORMESSAGE);
        }
      if(elem == 0 || (temp = elem->value) == 0
                 || elem->column != row)
        {
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Message("%sSingular Matrix",
        Te_MatrixSparse<D>::ERRORMESSAGE);
        x[row]=0;
        }
      else
        {
        sum = x[row];
		n = 0;
		while (elem!=0)
		 {
			elem = elem->next;
			n++;
		 }
		elem = elementRow[row];
        elem = elem->next;
        for(int column = 1; column < n; column++)
           {
              sum -= elem->value * x[elem->column];
              elem = elem->next;
           }
        x[row] = sum/temp;
        }
      }
   }

//   ********************< SolveLeft >**************************
//   * Purpose: Solving a system with a Left Te_MatrixSparse      *
//   * Example: A.SolveLeft(&bx);                              *
//   ***********************************************************
template <class D>
void Te_MatrixSparse<D>::SolveLeft(Te_Vector<D> *bx)
   {
	int	n;
   if(numRows != bx->dimensions)
//		printf ("Erro na Matriz Esparsa \n");
       lev_math::utility::Error("%s%s%sSolve",
       Te_MatrixSparse<D>::ERRORMESSAGE,
       utility::ERR_CHECK_DIMENSION,utility::ERR_FUNCTION);
   MSElement<D> *elem;
   D sum;
   D temp,*x = bx->vector;
   for(int row = 1;row <= numRows;row++)
      {
      elem = elementRow[row];
      if(elem == 0)
        {
//		printf ("Erro na Matriz Esparsa \n");
        lev_math::utility::Message("%sSingular Matrix",
            Te_MatrixSparse<D>::ERRORMESSAGE);
        x[row]=0;
        }
      else
        {
        sum = x[row];
		n = 0;
		while (elem!=0)
		 {
			elem = elem->next;
			n++;
		 }
		elem = elementRow[row];
        for(int column = 1; column < n; column++)
         	{
              sum -= elem->value * x[elem->column];
              elem = elem->next;
          	}
        if(elem->value == 0 || elem->column != row)
           {
//			printf ("Erro na Matriz Esparsa \n");
           lev_math::utility::Message("%sSingular Matrix",
           Te_MatrixSparse<D>::ERRORMESSAGE);
           x[row]=0;
           }
         else
           x[row] = sum/elem->value;
        }
      }
   }

//   *********************< InverseDiag >***********************
//   * Purpose: Calcula a matriz inversa de uma matriz  	   *
//	 *			diagonal 		  	*
//   * Description: Gera a inversa de uma matriz diagonal	*
//   * Example: B = InverseDiag(A);                            *
//   ***********************************************************
template <class D>
Te_MatrixSparse<D> InverseDiag(Te_MatrixSparse<D> &C)
   {
int		i;
int		NN = C.Rows();
Te_MatrixSparse<D> Caux(NN,NN);


	for (i = 1; i <= NN; i++ )
		Caux(i,i) = 1/C(i,i);


   }

//   ***********************< Cholesky >************************
//   * Purpose: Faz a decomposicao imcompleta de Cholesky      *
//   * Description: Gera as matrizes U e UT provenientes da    *
//   *        decomposicao de Cholesky                         *
//   * Example: Cholesky (MC, U, UT);                          *
//   ***********************************************************
template <class D>
void 	Cholesky ( Te_MatrixSparse<D> &MC, Te_MatrixSparse<D> &U, Te_MatrixSparse<D> &UT ) {
   int i,j,k,l,m;
   int NN;
   int s;

   // FENode* Node;
   // Te_Vector<double> V;
   // Pix p;

   NN = MC.Rows();

   // calcula 1o. elemento u11 = sqrt(h11)
   U(1,1) = sqrt(MC(1,1));
   UT(1,1) = U(1,1);

   // calcula 1a. linha u1j = h1j/u11
   for(l=2;l<=MC.GetRowElements(1);l++) {
      j = MC.GetRowColumn(1,l);
      U(1,j) = MC(1,j)/U(1,1);
      UT(j,1) = U(1,j);
   }

   // para todas as diagonais a partir da 2a. atribui uii = hii
   for(i=2;i<=NN;i++) {
      UT(i,i) = MC(i,i);

      // calcula uii e uik
      for(l=1; l<=UT.GetRowElements(i)-1; l++) {
         j = UT.GetRowColumn(i,l);
         UT(i,i) = UT(i,i) - UT(i,j)*UT(i,j);
      }

      // Importante verificar isso para o caso real (mcc) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // if (real(UT(i,i)) < 0)
      /* {
         printf("diagonal negativa: %d\n", i);
         p = Projeto->FENodeList.first();
         for (s = 1; s<i; s++)
         Projeto->FENodeList.next(p);
         Node = Projeto->FENodeList(p);
         Node->GetCoord(V);
         printf ("coord 1, 2, 3: %g, %g\n", V(1), V(2));
      } */
      // else
      UT(i,i) = sqrt(UT(i,i));

      U(i,i) = UT(i,i);

      for(l=2; l<=MC.GetRowElements(i); l++) {
         k = MC.GetRowColumn(i,l);
         UT(k,i) = MC(i,k);
         for (m=1; m<=UT.GetRowElements(i)-1; m++) {
            j = UT.GetRowColumn(i,m);
            if ( Abs(UT(k,j)) )
               UT(k,i) = UT(k,i) - UT(i,j)*UT(k,j);
            else
               UT.DeleteElement(k,j);
         }

         UT(k,i) = UT(k,i)/UT(i,i);
         U(i,k) = UT(k,i);
      }
   }
}


//   ***********************< CholeskySRF >*********************
//   * Purpose: Faz a decomposicao imcompleta de Cholesky      *
//   * Description: Gera as matrizes U e UT provenientes da    *
//   *		decomposicao de Cholesky atraves do metodo       *
//   *		Square Root Free                                 *
//   * Example: CholeskySRF (MC, U, UT);                       *
//   ***********************************************************
template <class D>
void 	CholeskySRF ( Te_MatrixSparse<D> &MC, Te_MatrixSparse<D> &U, Te_MatrixSparse<D> &UT ) {
   int i,j,k,l,m;
   int NN;
   NN = MC.Rows();
   /*
   // calcula 1o. elemento u11 = sqrt(h11)
   U(1,1) = MC(1,1);
   UT(1,1) = U(1,1);

   // calcula 1a. linha u1j = h1j/u11
   for(l=2;l<=MC.GetRowElements(1);l++) {
      j = MC.GetRowColumn(1,l);
      U(1,j) = MC(1,j)/U(1,1);
      UT(j,1) = U(1,j);
   }
   */
   // para todas as diagonais a partir da 2a. atribui uii = hii
   for(i=1;i<=NN;i++) {
      UT(i,i) = MC(i,i);

      // calcula uii e uik
      for(l=1; l<=UT.GetRowElements(i)-1; l++) {
         j = UT.GetRowColumn(i,l);
         UT(i,i) = UT(i,i) - UT(i,j)*UT(i,j)/UT(j,j);
      }

      // Importante verificar isso para o caso real (mcc) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // if (UT(i,i) < 0)
      //    printf("diagonal negativa: %d\n", i);
      // else
      //    UT(i,i) = sqrt(UT(i,i));
      U(i,i) = UT(i,i);
      for(l=2; l<=MC.GetRowElements(i); l++) {
         k = MC.GetRowColumn(i,l);
         UT(k,i) = MC(i,k);

         for (m=1; m<=UT.GetRowElements(i)-1; m++) {
            j = UT.GetRowColumn(i,m);
            if ( Abs(UT(k,j)) )
               UT(k,i) = UT(k,i) - UT(i,j)*UT(k,j)/UT(j,j);
            else
               UT.DeleteElement(k,j);
         }

         // UT(k,i) = UT(k,i)/UT(i,i);
         U(i,k) = UT(k,i);
      }
   }
}


//   ***********************< DiagonalScaling >**********************
//   * Purpose: Faz o escalonamento da diagonal principal da matriz *
//   *                                                              *
//   * Example: C = DiagonalScaling(MC);                            *
//   ****************************************************************
template <class D>
Te_MatrixSparse<D> DiagonalScaling ( Te_MatrixSparse<D> &MC ) {
   int i;
   int NN = MC.Rows();
   Te_MatrixSparse<D> C(NN,NN);
   for (i = 1; i <= NN; i++ )
      if ( Abs(MC(i,i)) )
         C(i,i) = 1/sqrt(MC(i,i));
      else {
         MC.DeleteElement(i,i);
         C(i,i) = 1;
      }

   return C;
}


template <class D>
Te_Vector<D> ForwardSubstitution(Te_MatrixSparse<D> &L, Te_Vector<D> &b) {

	Te_Vector<D> solucao;
	 solucao = b;

	solucao(1)= solucao(1)/L(1,1);
	for(int i=2; i<= L.Rows();i++){
		for (int k= 1; k <= i-1;k++){

		  solucao(i) = (solucao(i) - L(i,k)*solucao(k))/L(i,i);
		  }
	}


	return solucao;
}


template <class D>
Te_MatrixSparse <D> ForwardSubstitution(Te_MatrixSparse <D> &L, Te_MatrixSparse <D> &b) {

	Te_MatrixSparse<D> y(1,L.Rows());
	int i, j;
	D soma,aux;
	MSElement<D> *elemL, *elemY;

	y(1,1)=b.GetValue(1,1)/L.GetValue(1,1);
	for(i=2;i<=L.Rows();i++){
		elemL=L.elementRow[i];
		elemY=y.elementRow[1];
		soma=0.0;
		while((elemL)&&(elemY)){
			if (elemL->column==elemY->column){
				soma+=elemL->value*elemY->value;
				elemL=elemL->next;
				elemY=elemY->next;
			} else if (elemL->column>elemY->column){
				elemY=elemY->next;
			} else if (elemL->column<elemY->column){
				elemL=elemL->next;
			}
		}
		aux=b.GetValue(1,i)-soma;
		if (aux != 0) y(1,i)=aux;
	}

	return y;
}




template <class D>
Te_Vector <D> BackSubstitution(Te_MatrixSparse <D> &U, Te_Vector<D> &y) {

	Te_Vector <D> solucao;

	 solucao = y;

	 solucao(solucao.Size())=solucao(solucao.Size())/U(U.Rows(),U.Columns());

	for(int i=solucao.Size()-1; i >= 1;i--){

		for (int k= i+1; k <=solucao.Size();k++){

		     solucao(i)= solucao(i)- (U(i,k)*solucao(k));
		  }
	    solucao(i)=solucao(i)/U(i,i);
	}


	return solucao;
}






template <class D>
Te_MatrixSparse <D> BackSubstitution(Te_MatrixSparse<D> &U, Te_MatrixSparse<D> &y) {

	MSElement<D> *elemU, *elemX;
	Te_MatrixSparse <D> x(1,U.Rows());
	int i, j;
	D soma;

	x(1,U.Rows())=y.GetValue(1,U.Rows())/U.GetValue(U.Rows(),U.Rows());

	for (i=U.Rows()-1;i >=1 ;i--) {
		elemU = U.elementRow[i];
		soma=0;
		elemX = x.elementRow[1];
		while ((elemU)&&(elemX)){
			if (elemU->column==elemX->column){
				if (elemU->column>i) soma+= elemU->value*elemX->value;
				elemU = elemU->next;
				elemX = elemX->next;
			} else if (elemU->column>elemX->column){
				elemX=elemX->next;
			} else if (elemU->column<elemX->column){
				elemU=elemU->next;
			}
		}
		x(1,i) = (y(1,i)-soma)/U.GetValue(i,i);
	}
	return x;
}

}  // end namespace lev_math
#endif //SPARSE_HPP
