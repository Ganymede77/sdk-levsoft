#ifndef __FEM_POISSON_PHEN__
#define __FEM_POISSON_PHEN__


// *************************************************************
// Classe CLev_FEM_Poisson
// *************************************************************


#include <phenomenon.h>
#include <project_poisson.h>


class CLev_FEM_Poisson : public CLevPhenomenon
{

	friend class CProject_Solver;

	protected:

		long GetNodeRef(long pointIndex, int &cond);
		void Seek_Node_Links(lev_math::NODE *);
		void Mesh_Node_Links(lev_math::NODE *);//*GNM*
		void TransferAll_Node_Links(lev_math::NODE *);//*GNM*
		void Transfer_Node_Links(lev_math::NODE *,long, long);
    	virtual	void Build_Local_Matrices_Blocks(void);
		void Fill_Stiffness_Matrix(Finite_Element & el, lev_math::Te_Matrix<double> &matloc);
		void UpdateVariablesInPoints(void);
		void saida(int a)
		{//*NMA*: verificar possibilidade de implementar throw
			a = 0;
		}

		void SetBoundaryConditions();//atualiza os valores de cc na solu��o ap�s a resolu��o
		void SaveSystemOfEquationsFormatMatlab(std::string nome_arquivo);
        int GetProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK );
        double Get_Property(Finite_Element &elem);

	public:

		lev_math::SparseMatrix<double, long> Gmatrix;
		//lev_math::Sparse_Symm_Matrix<double,long>	A;
		lev_math::Sparse_Matrix<double,long> A_NS;
		lev_math::Ve_Templ<double,long>	rhs;
		lev_math::Ve_Templ<double,long>	solution;
		CProject_Poisson *proj;

        virtual ~ CLev_FEM_Poisson(){};
		void Associate(lev_math::Lo_Templ<Point * , long> *lp) {CLevPhenomenon::Associate(lp);}
		void Associate(_region::Region *l)  {CLevPhenomenon::Associate(l);}
		void Associate(lev_math::Lo_Templ<long , long> *l)   {CLevPhenomenon::Associate(l);}
		void Associate(CProject_Solver * pt) {};

		virtual void Impose_Boundary_Conditions();
		virtual void Initialize(void);	//Build_Skeleton(void);
		virtual void Build (void);		//Build_Global_Matrix(void);
		int  Solve(void);
        void Resolve(void);

        void Fill_RHS_Vector(Finite_Element & el, lev_math::Te_Vector<double> &vecloc);

		//FUN��ES DE DIAGN�STICO

		/**
		* Opera��o para salvar o sistema de equa��es.
		* Parametros:
		* std::string: identifica formato de arquivo de gravacao (ex: matlab, superLU, etc)
		* std::string: nome do arquivo com path
		*/
		virtual void SaveSystemOfEquations(std::string destino, std::string nome_arquivo);

		/**
		* Opera��o para gravar informa��es do elemento, a matriz local e a matriz global ap�s a inclus�o
		* dos valores da matriz local, as informa��es s�o anexadas (append) ao final do arquivo
		* Parametros:
		* std::string		: texto a ser inclu�do no arquivo antes da grava��o das matrizes
		* Nodal_Element &	:
		* lev_math::Te_Matrix		&	: matriz (buzzi)
		* char *			: nome do arquivo (com path)
		*/
		virtual void SaveParcialMatrices(std::string,Nodal_Element &el,lev_math::Te_Matrix<double> &matrix, char* nome_arquivo);
};


#endif // __FEM_POISSON_PHEN__
