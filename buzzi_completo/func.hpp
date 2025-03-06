#include <cmath>
#include <fstream>
#include <iostream>
#include <lev_buzzi.hpp>
#include <matrix.hpp>
#include <spars.hpp>
#include <sparse_matrix.hpp>
#include <utility.hpp>
#include <vector.hpp>


Te_MatrixSparse<double> rd_mult(Te_MatrixSparse<double>, Te_MatrixSparse<double>);
Te_MatrixSparse<double> rd_imp_rhs();
void rd_salva_sol(Te_MatrixSparse <double> sol,int);
Te_MatrixSparse<double> rd_multLU(Te_MatrixSparse<double> , Te_MatrixSparse<double> );
Te_MatrixSparse<double> rd_importa();
double tempoGasto(double ,double );


int coocsr_(int *nrow, int *nnz, double *a, int *ir, int *jc, double *ao, int *jao, int *iao)
{
    // System generated locals
    int i__1;

    // Local variables
    static int i, j, k;
    static double x;
    static int k0, iad;

/* -----------------------------------------------------------------------*/
/*  Coordinate     to   Compressed Sparse Row */
/* -----------------------------------------------------------------------*/
/* converts a matrix that is stored in coordinate format */
/*  a, ir, jc into a row general sparse ao, jao, iao format. */
/* on entry: */
/* --------- */
/* nrow	= dimension of the matrix */
/* nnz	= number of nonzero elements in matrix */
/* a, */
/* ir, */
/* jc    = matrix in coordinate format. a(k), ir(k), jc(k) store the nnz */
/*         nonzero elements of the matrix with a(k) = actual real value of*/
/* 	  the elements, ir(k) = its row number and jc(k) = its column */
/* 	  number. The order of the elements is arbitrary. */
/* on return: */
/* ----------- */
/* ir 	is destroyed */

/* ao, jao, iao = matrix in general sparse matrix format with ao */
/* 	continung the real values, jao containing the column indices, */
/* 	and iao being the pointer to the beginning of the row, */
/* 	in arrays ao, jao. */

/* Notes: */
/* ------ This routine is NOT in place.  See coicsr */

/*----------------------------------------------------------------------*/
    /* Parameter adjustments */
    --iao;
    --jao;
    --ao;
    --jc;
    --ir;
    --a;

    /* Function Body */
    i__1 = *nrow + 1;
    for (k = 1; k <= i__1; ++k) {
	iao[k] = 0;
/* L1: */
    }
/* determine row-lengths. */
    i__1 = *nnz;
    for (k = 1; k <= i__1; ++k) {
	++iao[ir[k]];
/* L2: */
    }
/* starting position of each row.. */
    k = 1;
    i__1 = *nrow + 1;
    for (j = 1; j <= i__1; ++j) {
	k0 = iao[j];
	iao[j] = k;
	k += k0;
/* L3: */
    }
/* go through the structure  once more. Fill in output matrix. */
    i__1 = *nnz;
    for (k = 1; k <= i__1; ++k) {
	i = ir[k];
	j = jc[k];
	x = a[k];
	iad = iao[i];
	ao[iad] = x;
	jao[iad] = j;
	iao[i] = iad + 1;
/* L4: */
    }
/* shift back iao */
    for (j = *nrow; j >= 1; --j) {
	iao[j + 1] = iao[j];
/* L5: */
    }
    iao[1] = 1;
    return 0;
}


int	ConverteBuzzi_LEV(Te_MatrixSparse<double> &MC, Sparse_Matrix<double> &MLEV)
{
	int	i, j, nk, nrow, ncol;
	int	*row = NULL;
	int	*col = NULL;
	double *val = NULL;
	int nz;
	int	NTOT;
    
	// Numero de linhas da matriz global
	NTOT = MC.Rows();

	// Elimina possiveis zeros
	MC.CleanZeroMatrix();

	//-------------------------------------
	//	Calcula zeros na diagonal
	//-------------------------------------
	int nzdiag=0;
	for(i=1;i<=MC.Rows();i++)
		if( MC.GetValue(i,i) == 0.0 ) {
			MC(i,i) = 0.0;
			++nzdiag;
		}

	//--------------------------------------------------
	// Transforma de Buzzi para Coordinate Format (COO)
	//--------------------------------------------------
	nz = 0;
	for(nrow=1; nrow<=NTOT; nrow++) {
		nk = MC.GetRowElements(nrow);
		row = (int *) realloc ( row, (nz+nk)*sizeof(int) );
		col = (int *) realloc ( col, (nz+nk)*sizeof(int) );
		val = (double *) realloc ( val, (nz+nk)*sizeof(double) );
		for(j=1; j<=nk; j++) {
			ncol = MC.GetRowColumn(nrow,j);
			row[nz+j-1] = nrow;  
			col[nz+j-1] = ncol;
			val[nz+j-1] = MC(nrow,ncol);
		}
		MC.DeleteRow(nrow);
		nz = nz + nk;
	}

	// Elimina matriz global do Buzzi
	MC.CleanMatrix();

	//------------------------------------------------------------
	// Transforma de COO para Compressed Sparse Row (CSR)
	//------------------------------------------------------------
	double	*ao;
	int		*jao;
	int		*iao;
	int ret = 1;  // retorna 1 ser operacao completada com sucesso
	ao = (double *) calloc(nz,sizeof(double));
	iao = (int *) calloc(NTOT+1,sizeof(int));
	jao = (int *) calloc(nz,sizeof(int));
	if (ao && iao && jao) // *AP* tratamento parcial
	{
		nrow = NTOT-1;  // *AP* matrizes do LEV não armazenam o último elemento!!!
 		coocsr_(&nrow, &nz, ((double *)val), ((int *)row), ((int *)col), ao, jao, iao);

		for ( nrow = 0; nrow < NTOT ; nrow++ ) // *AP* indice dos vetores do LEV começam a partir de zero
		{	iao[nrow] = iao [nrow]-1;	}

		for ( j= 0 ; j < nz ; j++ )  // *AP* indice dos vetores do LEV começam a partir de zero
		{	jao[j] = jao [j] - 1;	}


		MLEV.Set((long *)iao, long(NTOT), (long *)jao, ao, long(nz));
	}
	else ret = 0; // *AP* retorna zero em caso de falha de alocacao

	// Libera arrays do COO
	if (row) { free(row);	row=NULL;}
	if (col) { free(col);	col=NULL;}
	if (val) { free(val);	val=NULL;}
	if (ao)  {free(ao);	 ao=NULL;}
	if (iao) {free(iao); iao=NULL;}
	if (jao) {free(jao); jao=NULL;}
	return ret;
}


double tempoGasto(double tempoi,double tempof)
{
	double tempogasto = (tempof-tempoi)/CLOCKS_PER_SEC;
	return tempogasto;
}


Te_MatrixSparse<double> rd_importa()
{
	long aux,prim_ant,prim,pos;
	int matdim;
	double val;

    std::ifstream  fprim,fpos;
    fpos.open("pos_val.dat",ios::in);
    fprim.open("prim.dat",ios::in); 
	fprim >> aux; 
    fprim >> prim_ant;
	
	matdim = aux;
    	Te_MatrixSparse<double> A(matdim,matdim);

	for(int i=0; i <= matdim ; i++){

		fprim >> prim;
		
		for(int j=prim_ant; j < prim; j++){
			fpos >> pos >> val;
			if (val != 0) A(i+1,pos+1)=val;
		}
		prim_ant=prim;
	}
	
	fprim.close();
	fpos.close();
 
	return A;
}


Te_Vector<double> VectorLevToBuzzi()
{
	int i,aux,dimensao;
	double val;

    std::ifstream rhs;
    rhs.open("rhs.dat",ios::in);
	rhs >> aux; 
	
    dimensao = aux;
    
    Te_Vector<double> avector(dimensao);

	for(i=1; i <= dimensao ; i++){
		rhs >> val;
		avector(i)=val;
	}
	
	rhs.close();
	return avector;
}


Te_MatrixSparse<double> rd_imp_rhs()
{
	int i,aux,N=0;
	double val;

    ifstream rhs;
    rhs.open("rhs.dat",ios::in);
	rhs >> aux; 
	
	N=aux;
    Te_MatrixSparse<double> A(1,N);

	for(i=1; i <= N ; i++){
		rhs >> val;
		A(1,i)=val;
	}
	
	rhs.close();
	return A;
}


void rd_salva_sol(Te_Vector<double> sol, int N)
{
	int i;
	
	std::ofstream arq_sol;
        arq_sol.open("rd_sol.dat",ios::out);
	arq_sol << N << std::endl; 
		
	for(i=1; i <= N ; i++) 
		arq_sol << sol(i) << std::endl;
	
	arq_sol.close();
}


void rd_salva_sol(Te_MatrixSparse <double> sol, int N)
{
	int i;
	
	std::ofstream arq_sol;
	arq_sol.open("rd_sol.dat",ios::out);
	arq_sol << N << std::endl; 
		
	for(i=1; i <= N ; i++) 
		arq_sol << sol(1,i) << std::endl;
	
	arq_sol.close();
}


Te_MatrixSparse<double> rd_mult(Te_MatrixSparse<double> L, Te_MatrixSparse<double>R)
{
	int Nl=L.Rows(),k;
	int Nc=R.Columns();
	Te_MatrixSparse<double> A(Nl,Nc);
	double soma;
	MSElement<double> *eleml;
	
	for(int i=1;i<=Nl;i++) {
		soma=0;
		eleml=L.elementRow[i];
		for(int j=1;j<=Nc;j++) {
			while(eleml) {
				k=eleml->column;
				soma+=eleml->value*R.GetValue(k,j);
				eleml=eleml->next;
			}
		A(i,j)=soma;		
		}
	}
	return A;
}


Te_MatrixSparse<double> rd_multLU(Te_MatrixSparse<double> L, Te_MatrixSparse<double> U)
{
	int Nl=L.Rows();
	int Nc=U.Rows();
	Te_MatrixSparse<double> A(Nl,Nc);
	double acc;
	
	for(int i=1;i<=Nl;i++) {
		for(int j=1;j<=Nc;j++) {
			acc=0.;
			for(int k=1;k<=Nl;k++){
				acc+=L(i,k)*U(j,k);
			}
			A(i,j)=acc;
		}
	}
	return A;
}
