#ifndef __PHENOMENON__
#define __PHENOMENON__


#include <fin_elem.hpp>
#include <lo_templ.hpp>
#include <region.hpp>
#include <point.hpp>
#include <sparse_symm_matrix.hpp>


/**
 * Projeto: SDK-LEVSOFT
 *
 * A classe CLevPhenomenon � a classe base da qual s�o derivadas classes para resolu��o de equa��es diferenciais parciais que regem fen�menos f�sicos.
 * As equa��es s�o resolvidas aplicando-se o Metodo dos Elementos Finitos, por�m podem tamb�m incluir outros m�todos num�ricos necess�rios.
 * Como exemplo de EDP: Equa��o de Poisson, Equa��o de Schroedinger
 *
*/

class CProject_Solver;

class CLevPhenomenon
{

friend class  CProject_Solver;

protected:
public:
	lev_math::Lo_Templ<Point * , long> * ptr_lo_points;
	_region::Region	* ptr_regions;
	lev_math::Lo_Templ<long , long>	* ptr_lo_long_cc_dirichlet;

	// integrais para simetria plana
	virtual	int		IntegralNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order = 3);
	virtual	int		IntegralNNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);
	virtual	int		IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order = 3);
	virtual int		IntegralGradNTensorPropertyGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);
	virtual int		IntegralGradNGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);
	virtual int		IntegralGradNMatrixGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat,lev_math::Te_Matrix<double> &MatProd, int integration_order = 3);
	virtual	int		IntegralNdV(Nodal_Element &elem, lev_math::Te_Vector<double> &vect, int integration_order =3);
	virtual int		IntegralNNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);
	virtual int     IntegralNTensorPropertyNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);

	// *AP* 11/10/2007
	// ATENCAO:  REVER ESTA IMPLEMENTA��O PARA O CASO DE REGIOES DE TRANSFORMA��O
	// NAO TESTADA
	virtual int		IntegralNNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat,int region_number, int integration_order = 3);

	virtual int     IntegralNGradN(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order =3);
	virtual int     IntegralGradN(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order =3);

	// integrais para simetria axial
	        virtual double	GetAxialMatrixFactor(Finite_Element &elem);
	        virtual double	GetAxialRHSFactor(Finite_Element &elem);
			virtual	int		IntegralGradNTensorPropertyGradNdVCoordTransfAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order = 3);
			virtual int		IntegralGradNTensorPropertyGradNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);
			virtual int		IntegralGradNMatrixGradNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat,lev_math::Te_Matrix<double> &MatProd, int integration_order = 3);
			virtual	int		IntegralNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Vector<double> &vect, int integration_order =3);
			virtual int		IntegralNNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order = 3);

			inline	virtual int		GetProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK){return 0;};//*NMA*: 24/02/2007: implementa��o vazia para evitar problemas com derivadas j� existentes, ser� substitu�da pois n�o atende a todos os casos

public:
	CLevPhenomenon();
	virtual ~CLevPhenomenon();

			virtual	void	Impose_Boundary_Conditions()	= 0;
			virtual void	Initialize (void)				= 0;
			virtual void	Build (void)					= 0;
			virtual int		Solve(void)						= 0;
			virtual void	Resolve(void)					{}; //*NMA*: 24/02/2007: implementa��o vazia para evitar problemas com derivadas j� existentes

			virtual void	Associate(lev_math::Lo_Templ<Point * , long> *lp) { ptr_lo_points = lp;}
			virtual void	Associate(_region::Region *l) { ptr_regions = l;}
			virtual void	Associate(lev_math::Lo_Templ<long , long> *l) { ptr_lo_long_cc_dirichlet = l;}
			virtual void	Associate(CProject_Solver * pt) 	= 0;

	//fun��es de diagn�stico
    /**
    * Opera��o para gravar o sistema de equa��es.
	* Parametros:
	* std::string: identifica formato de arquivo de gravacao (ex: matlab, superLU, etc)
	* std::string: nome do arquivo com path
    */
	virtual void SaveSystemOfEquations(std::string destino, std::string nome_arquivo){}//*NMA*:18/01/2007:implementa��o vazia para n�o dar problema com as classes derivadas

    /**
    * Opera��o para gravar informa��es do elemento, a matriz local e a matriz global ap�s a inclus�o
	* dos valores da matriz local, as informa��es s�o anexadas (append) ao final do arquivo
	* Parametros:
	* std::string		: texto a ser inclu�do no arquivo antes da grava��o das matrizes
	* Nodal_Element &	:
	* lev_math::Te_Matrix		&	: matriz (buzzi)
	* char *			: nome do arquivo (com path)
    */
	virtual void SaveParcialMatrices(std::string,Nodal_Element &el,lev_math::Te_Matrix<double> &matrix, char* nome_arquivo){}//*NMA*:23/01/2007:implementa��o vazia para n�o dar problema com as classes derivadas
};


#endif //__PHENOMENON__
