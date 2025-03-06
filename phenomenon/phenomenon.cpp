#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <phenomenon.h>
#include <matrix.hpp>


#define NUM_MEDIO_LIGACOES_POR_NO 5


using namespace std;


//...........I M P L E M E N T A T I O N ......................................
//==============================================================================
CLevPhenomenon::CLevPhenomenon(){}
CLevPhenomenon::~CLevPhenomenon(){}

/*
 Projeto:				SDK_LEVSOFT \n
 Nome do Arquivo:		phenomenon.cpp\n
 Nome da Funcao:		IntegralGradNTensorKGradNdVCoordTransf
 Versao:				1.5
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
........................................................................................... \n
	String que indica possiveis modificaoes futuras: \n
			*AP*  *NMA* \n
 ........................................................................................... \n
 */
/**
 Descricao:                                                      \n
	Calcula a integral

	\e	Integral  (1/jacH) * (1/jacR) * GradN * [Jhom] * [Jr] * K * [Jr] * [Jhom] * GradN dV   \n
\n
	para elementos finitos (1D, 2D, 3D), levando em consideracao uma transfomacao de
	coordenadas para tratamento de dominios abertos. A integral eh realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
	N:		funcoes de base \n
	K:		tensor propriedade do material \n
	jacH:	determinante da matriz jacobiana da transformacao de coordenadas para o sistema homogeneo \n
	jacR:	determinante matriz jacobiana da transformacao de coordenadas do dominio estudo \n
	Jhom:	matriz jacobiana da transformacao de coordenadas para o sistema homogeneo \n
	Jr	:	matriz jacobiana da transformacao de coordenadas do dominio estudo (ex: dominio aberto para dominio fechado \n
\n
  Exemplo de aplicacao: calculo das matrizes locais para resolucacao da equacao de Poisson \n


\param		Nodal_Element &elem: elemento finito
\param		lev_math::Te_Matrix<double> &Mat: resultado da integracao
\param		int				  region_number: n�mero da regi�o de transforma��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss
\n
\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
				homog�neo igual a zero.
\return    2 - jacobiano da transforma��o de coordenadas para o dom�nio
				fechado igual a zero.

\code
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacR;			// determinante da matriz jacobiana da transformacao para dominio fechado
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homogeneo
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
    Gauss_Pt	ptransf;		// auxiliar para receber o ponto de gauss em coordenadas cartesianas
	int			dimension;		// guarda dimensao do elemento
	int			num_nodes;		// numero de nos do elemento, varia com a dimensao, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss

	Mat = Zeronxn; // "zera" as posicoes da matrix

    elem.MatJacobHom(Jhom, jacH);//obtem jacobiano da transformacao para o sistema homogeneo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transformacao para o sistema homogeneo igual a zero

	if(!codeErro)
	{
		 // laco de integracao
		for (k = 0; k < npgauss && !codeErro; k++)
		{
		   elem.HomToCartesian(pt_gauss[k],ptransf); // transforma o ponto de integracao em ponto do dominio transformado
		   ptr_regions[region_number].Mat_Jacobian(ptransf, &Jr, &jacR); //obtem jacobiano no ponto de gauss no dominio transformado
		   if(!jacR) codeErro = 2; //jacobiano (determinante) no ponto de gauss no dominio transformado igual a zero
		   if(!codeErro)
		   {
			   w = pt_gauss[k].Get_Variables(1);	//obtem peso do ponto de gauss
			   elem.GradN(&pt_gauss[k], GradN);	//obtem gradiente da funcao de base no ponto de gauss
			   GetProperty(elem, K);				//obtem tensor de propriedade do material

			   Matparc =  Jr * Jhom * GradN;
			   Kt = K * Matparc;
			   Mataux  = w/(jacH*jacR) * Kt;

			   Transpose(&Matparc);
			   Sum(&Mat, Matparc * Mataux);
		   }
		} // end of for (k)
	}
	return codeErro;

}  // end of int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)

\endcode
**/
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacR;			// determinante da matriz jacobiana da transformacao para dominio fechado
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homogeneo
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
    Gauss_Pt	ptransf;		// auxiliar para receber o ponto de gauss em coordenadas cartesianas
	int			dimension;		// guarda dimensao do elemento
	int			num_nodes;		// numero de nos do elemento, varia com a dimensao, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obtem os pontos de gauss a partir da ordem de integracao definida por integration_order

	dimension = elem.Get_Dimension();					//obtem dimensao do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona matrizes auxiliares
	lev_math::Te_Matrix<double>	Jr(dimension,dimension);	// Jacobian matrix for the coordinate transformation of open boundaries
	lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
	lev_math::Te_Matrix<double>	K(dimension,dimension);		// matrix (tensor) of "properties"
	lev_math::Te_Matrix<double>	Kt(dimension,dimension);	// auxiliar matrix

	lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
	lev_math::Te_Matrix<double>   GradN(dimension,num_nodes);				// matrix of gradients of the base functions
	lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradN.Columns());	// auxiliar matrix
	lev_math::Te_Matrix<double>	Mataux(Kt.Rows(), Matparc.Columns());	// auxiliar matrix
	//................................................................

    Mat = Zeronxn; // "zera" as posicoes da matrix

    elem.MatJacobHom(Jhom, jacH);//obtem jacobiano da transformacao para o sistema homogeneo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transformacao para o sistema homogeneo igual a zero

	if(!codeErro) {
		 // laco de integracao
		for (k = 0; k < npgauss && !codeErro; k++) {
		   elem.HomToCartesian(pt_gauss[k],ptransf); // transforma o ponto de integracao em ponto do dominio transformado
		   ptr_regions[region_number].Mat_Jacobian(ptransf, &Jr, &jacR); //obtem jacobiano no ponto de gauss no dominio transformado
		   if(!jacR) codeErro = 2; //jacobiano (determinante) no ponto de gauss no dominio transformado igual a zero
		   if(!codeErro) {
			   w = pt_gauss[k].Get_Variables(1);	// obtem peso do ponto de gauss
			   elem.GradN(&pt_gauss[k], GradN);	    // obtem gradiente da funcao de base no ponto de gauss
			   GetProperty(elem, K);				// obtem tensor de propriedade do material

			   Matparc =  Jr * GradN;
			   Kt = K * Matparc;
			   Mataux  = w * Kt;

			   lev_math::Transpose(&Matparc);
			   lev_math::Sum(&Mat, Matparc * Mataux);
		   }
		} // end of for (k)
        Mat = elem.JacDet * Mat;
	}
	return codeErro;

}  // end of int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralGradNTensorKGradNdVCoordTransfAxial
 Versao:				1.5
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	12/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
*/
/**
 Descricao:                                                           					\n
	Para formulacao a ser utilizada em problemas com simetria axial, calcula a integral

\e		Integral  (1/jacH) * (1/jacR) * GradN * [Jhom] * [Jr] * K * [Jr] * [Jhom] * GradN dV

	para elementos finitos 2D.\n
\n
	Chama a funcao: IntegralGradNTensorPropertyGradNdVCoordTransf\n
\n
	Exemplo de aplicacao: calculo das matrizes locais para resolucacao da equacao de Poisson 
							para problemas axissimetricos \n

\n
\param		Simplex_Nodal_Element_2D &elem : especifico para elemento Simplex_Nodal_Element_2D
\param		lev_math::Te_Matrix<double> &Mat: resultado da integracao
\param		int				  region_number: n�mero da regi�o de transforma��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss
\n
\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.
\return    2 - jacobiano da transforma��o de coordenadas para o dom�nio
\return        fechado igual a zero.

\code
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransfAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralGradNTensorPropertyGradNdVCoordTransf(elem,MatLoc,region_number,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;

	return codeErro;
}  // end of int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)

\endcode

**/
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransfAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralGradNTensorPropertyGradNdVCoordTransf(elem,MatLoc,region_number,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;

	return codeErro;

}  // end of int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralGradNTensorKGradNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras:
			*AP*  *NMA*
 ...........................................................................................
 */
/**
 Descricao: \n
	Calcula a integral

\e		Integral (1/jacH) * GradN * [Jhom] * K * [Jhom] * GradN dV

	para elementos finitos (1D, 2D, 3D). A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
	N: fun��es de base \n
	K: tensor propriedade do material \n
	Jhom	: matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
\n
  Exemplo de aplica��o: c�lculo das matrizes locais para resoluca��o da equa��o de Poisson   \n

\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.

\param		Nodal_Element &elem: elemento finito
\param		lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss

 \code
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			dimension;		// guarda dimens�o do elemento
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order

	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	//dimensiona matrizes auxiliares
		lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
		lev_math::Te_Matrix<double>	K(dimension,dimension);		// matrix (tensor) of "properties"
		lev_math::Te_Matrix<double>	Kt(dimension,dimension);	// auxiliar matrix

		lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
		lev_math::Te_Matrix<double>   GradN(dimension,num_nodes);				// matrix of gradients of the base functions
		lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradN.Columns());	// auxiliar matrix
		lev_math::Te_Matrix<double>	Mataux(Kt.Rows(), Matparc.Columns());	// auxiliar matrix
	//................................................................

    Mat = Zeronxn; // "zera" as posi��es da matrix

    elem.MatJacobHom(Jhom, jacH);//obt�m jacobiano da transforma��o para o sistema homog�neo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transforma��o para o sistema homog�neo igual a zero

	if(!codeErro)
	{
		 // laco de integracao
		for (k = 0; k < npgauss && !codeErro; k++)
		{
		   w = pt_gauss[k].Get_Variables(1);	//obtem peso do ponto de gauss
		   elem.GradN(&pt_gauss[k], GradN);	//obtem gradiente da fun��o de base no ponto de gauss
		   GetProperty(elem, K);				//obtem tensor de propriedade do material

		   Matparc =  Jhom * GradN;
		   Kt = K * Matparc;
		   Mataux  = w/(jacH) * Kt;

		   Transpose(&Matparc);
		   Sum(&Mat, Matparc * Mataux);
		} // end of for (k)
	}
	return codeErro;
}  // end of IntegralGradNTensorKGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)

\endcode
**/
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			dimension;		// guarda dimens�o do elemento
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *)ptr_lo_points[0][0];
    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order

	dimension = elem.Get_Dimension();
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona matrizes auxiliares
	lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
	lev_math::Te_Matrix<double>	K(dimension,dimension);		// matrix (tensor) of "properties"
	lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);
	lev_math::Te_Matrix<double> GradN(dimension,num_nodes);
	lev_math::Te_Matrix<double>	Matparc(dimension,num_nodes);
	lev_math::Te_Matrix<double>	Mataux(dimension,num_nodes);

    Mat = Zeronxn;

    elem.MatJacobHom(Jhom, jacH);//obt�m jacobiano da transforma��o para o sistema homog�neo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transforma��o para o sistema homog�neo igual a zero

	if(!codeErro)
	{
		 // laco de integracao
		for(k=0; k<npgauss; k++) {
		   w = pt_gauss[k].Get_Variables(1);
		   elem.GradN(&pt_gauss[k], GradN);
		   GetProperty(elem, K);
		   Matparc = GradN;
		   Mataux  = K * Matparc;
		   Transpose(&Matparc);
		   Sum(&Mat, w * Matparc * Mataux);
		} // end of for (k)
        Mat = elem.JacDet * Mat;
	}
	return codeErro;

}  // end of IntegralGradNTensorKGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)


/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralGradNTensorKGradNdVAxial
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	12/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */
/**...........................................................................................
 Descricao:                                                           					\n
	Para formula��o a ser utilizada em problemas com simetria axial
	calcula a integral

\e		Integral (1/jacH) * GradN * [Jhom] * K * [Jhom] * GradN dV \n

	para elementos finitos 2D.
\n
	Chama a fun��o: IntegralGradNTensorPropertyGradNdV \n

	Exemplo de aplica��o: c�lculo das matrizes locais para resoluca��o da equa��o de Poisson
							para problemas axissim�tricos\n

\param		Simplex_Nodal_Element_2D &elem : espec�fico para elemento Simplex_Nodal_Element_2D
\param		lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss


/return    0 - sucesso
/return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int		codeErro = 0;	// retorno
	double	symmfac = 0.0;
	int		num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)

	num_nodes = elem.Get_Nodes();								//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralGradNTensorPropertyGradNdV(elem,MatLoc,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;

	return codeErro;

}  // end of int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVAxial(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
\endcode
**/
int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int		codeErro = 0;	// retorno
	double	symmfac = 0.0;
	int		num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)

	num_nodes = elem.Get_Nodes();								//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralGradNTensorPropertyGradNdV(elem,MatLoc,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;

	return codeErro;

}  // end of int CLevPhenomenon::IntegralGradNTensorPropertyGradNdVAxial(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)




/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralGradNGradNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */
/**
 Descricao:                                                           			\n
	Calcula a integral

\e		Integral (1/jacH) * (Jhom * GradN)T * Jhom * GradN dV

	para elementos finitos (1D, 2D, 3D). A integral � realizada no sistema
	de coordenadas homogeneas usando a quadratura de Gauss. \n
\n
	N: fun��es de base \n
	Jhom	: matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
 \n

\param		Nodal_Element &elem: elemento finito
\param		lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralGradNGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			dimension;		// guarda dimens�o do elemento
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order

	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	//dimensiona matrizes auxiliares
		lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation

		lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
		lev_math::Te_Matrix<double>   GradN(dimension,num_nodes);				// matrix of gradients of the base functions
		lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradN.Columns());	// auxiliar matrix
		lev_math::Te_Matrix<double>	Mataux(Matparc.Rows(), Matparc.Columns());	// auxiliar matrix
	//................................................................

    Mat = Zeronxn; // "zera" as posi��es da matrix

    elem.MatJacobHom(Jhom, jacH);//obt�m jacobiano da transforma��o para o sistema homog�neo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transforma��o para o sistema homog�neo igual a zero

	if(!codeErro)
	{
		 // laco de integracao
		for (k = 0; k < npgauss && !codeErro; k++)
		{
		   w = pt_gauss[k].Get_Variables(1);	//obtem peso do ponto de gauss
		   elem.GradN(&pt_gauss[k], GradN);		//obtem gradiente da fun��o de base no ponto de gauss

		   Matparc =  Jhom * GradN;
		   Mataux  = Matparc;

		   Transpose(&Matparc);
		   Sum(&Mat, w*Matparc * Mataux);
		} // end of for (k)
        Mat = (1./jacH) * Mat;
	}
	return codeErro;

}  // end of IntegralGradNGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)

\endcode
**/
int CLevPhenomenon::IntegralGradNGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			dimension;		// guarda dimens�o do elemento
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order

	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();					//obt�m n�mero de n�s do elemento

	//dimensiona matrizes auxiliares
	lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
	lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
	lev_math::Te_Matrix<double> GradN(dimension,num_nodes);				// matrix of gradients of the base functions
	lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradN.Columns());	// auxiliar matrix
	lev_math::Te_Matrix<double>	Mataux(Matparc.Rows(), Matparc.Columns());	// auxiliar matrix
	//................................................................

    Mat = Zeronxn; // "zera" as posicoes da matrix

    elem.MatJacobHom(Jhom, jacH); // Obtem jacobiano da transformacao para o sistema homogeneo e o determinante

	if(!jacH)
		codeErro = 1; // Jacobiano (determinante) da transformacao para o sistema homogeneo igual a zero

	if(!codeErro) {
		// laco de integracao
		for(k=0; k<npgauss; k++) {
			w = pt_gauss[k].Get_Variables(1);	// Obtem peso do ponto de gauss
			elem.GradN(&pt_gauss[k], GradN);	// Obtem gradiente da funcao de base no ponto de gauss
			Matparc = GradN;
			Mataux  = Matparc;
			Transpose(&Matparc);
			Sum(&Mat, w * Matparc * Mataux);
		}
        Mat = elem.JacDet * Mat;
	}
	return codeErro;
} // End of IntegralGradNGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)


/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralGradNMatrixGradNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */
/**
 Descricao:                                                           			\n
	Calcula a integral

\e		Integral (1/jacH) * GradN * Jhom * GenericMatrix * Jhom * GradN dV

	para elementos finitos (1D, 2D, 3D). A integral � realizada no sistema
	de coordenadas homogeneas usando a quadratura de Gauss. \n
\n
	N: fun��es de base \n
	GenericMatrix: matriz que sera multiplicada \n
	Jhom	: matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
 \n

\param		Nodal_Element &elem: elemento finito
\param		lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param		lev_math::Te_Matrix<double> &MatProd: matriz a ser multiplicada (GenericMatrix)
\param		int				  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralGradNMatrixGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat,lev_math::Te_Matrix<double> &MatProd, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			dimension;		// guarda dimens�o do elemento
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order

	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	//dimensiona matrizes auxiliares
		lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation

		lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
		lev_math::Te_Matrix<double>   GradN(dimension,num_nodes);				// matrix of gradients of the base functions
		lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradN.Columns());	// auxiliar matrix
		lev_math::Te_Matrix<double>	Mataux(MatProd.Rows(), Matparc.Columns());	// auxiliar matrix
	//................................................................

    Mat = Zeronxn; // "zera" as posi��es da matrix

    elem.MatJacobHom(Jhom, jacH);//obt�m jacobiano da transforma��o para o sistema homog�neo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transforma��o para o sistema homog�neo igual a zero

	if(!codeErro)
	{
		 // laco de integracao
		for (k = 0; k < npgauss && !codeErro; k++)
		{
		   w = pt_gauss[k].Get_Variables(1);	//obtem peso do ponto de gauss
		   elem.GradN(&pt_gauss[k], GradN);		//obtem gradiente da fun��o de base no ponto de gauss

		   Matparc =  Jhom * GradN;
		   Mataux  = w/(jacH) * MatProd * Matparc;

		   Transpose(&Matparc);
		   Sum(&Mat, Matparc * Mataux);
		} // end of for (k)
	}
	return codeErro;

}  // end of IntegralGradNTensorKGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)

\endcode
**/
int CLevPhenomenon::IntegralGradNMatrixGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat,lev_math::Te_Matrix<double> &MatProd, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			dimension;		// guarda dimens�o do elemento
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order

	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona matrizes auxiliares
	lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation

	lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
	lev_math::Te_Matrix<double> GradN(dimension,num_nodes);				// matrix of gradients of the base functions
	lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradN.Columns());	// auxiliar matrix
	lev_math::Te_Matrix<double>	Mataux(MatProd.Rows(), Matparc.Columns());	// auxiliar matrix
	//................................................................

    Mat = Zeronxn; // "zera" as posi��es da matrix

    elem.MatJacobHom(Jhom, jacH);//obt�m jacobiano da transforma��o para o sistema homog�neo e o determinante

	if(!jacH)
		codeErro = 1; //jacobiano (determinante) da transforma��o para o sistema homog�neo igual a zero

	if(!codeErro)
	{
		 // laco de integracao
		for (k = 0; k < npgauss && !codeErro; k++)
		{
		   w = pt_gauss[k].Get_Variables(1);	//obtem peso do ponto de gauss
		   elem.GradN(&pt_gauss[k], GradN);		//obtem gradiente da fun��o de base no ponto de gauss

		   Matparc = GradN;
		   Mataux  = w * MatProd * Matparc;

		   Transpose(&Matparc);
		   Sum(&Mat, Matparc * Mataux);
		} // end of for (k)
        Mat = elem.JacDet * Mat;
	}
	return codeErro;

}  // end of IntegralGradNMatrixGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralGradNMatrixGradNdVAxial
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	12/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
*/
/**
 Descricao:                                                           				\n
	Para formula��o a ser utilizada em problemas com simetria axial.\n
	Calcula a integral

\e		Integral (1/jacH) * GradN * Jhom * GenericMatrix * Jhom * GradN dV \n

	para elementos finitos 2D.
 \n
	Chama a fun��o: IntegralGradNMatrixGradNdV  \n
\n

\param		Simplex_Nodal_Element_2D &elem : espec�fico para elemento Simplex_Nodal_Element_2D
\param		lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param		lev_math::Te_Matrix<double> &MatProd: matriz a ser multiplicada (GenericMatrix)
\param		int				  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralGradNMatrixGradNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat,lev_math::Te_Matrix<double> &MatProd, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralGradNMatrixGradNdV(elem,MatLoc,MatProd,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;

	return codeErro;

}  // end of IntegralGradNTensorKGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)

\endcode
**/
int CLevPhenomenon::IntegralGradNMatrixGradNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat,lev_math::Te_Matrix<double> &MatProd, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralGradNMatrixGradNdV(elem,MatLoc,MatProd,integration_order);

	if(!codeErro)
			Mat = symmfac*MatLoc;

	return codeErro;

}  // end of IntegralGradNTensorKGradNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */
/**
 Descricao:                                                           					\n
	Calcula a integral

\e		Integral (1/jacH) * N dV

	para elementos finitos (1D, 2D, 3D). A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
	N: fun��es de base \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
\n
  Exemplo de aplica��o: c�lculo do termo de fonte da equa��o de Poisson \n
\n
\param  Nodal_Element &elem    : elemento finito
\param		Te_Vector<double> &vect: resultado da integra��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso
\return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.                                          					\n

\code
int CLevPhenomenon::IntegralNdV(Nodal_Element &elem, Te_Vector<double> &vect, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	//dimensiona vetores auxiliares
		Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar

	vect = vectparc;//para zerar o conte�do de vect

	jacH = elem.MatJacobHomDet();
	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
  		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = (w*jacH) * vectN;
			Sum(&vect, vectparc);
		} // end of for (k)
	}

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNdV


FALTA REALIZAR TESTE

\endcode

**/
int CLevPhenomenon::IntegralNdV(Nodal_Element &elem, lev_math::Te_Vector<double> &vect, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar

	vect = vectparc;//para zerar o conte�do de vect

	jacH = elem.MatJacobHomDet();
	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
  		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			//vectparc = (w/jacH) * vectN;    // *AP* 11/10/2007 corrigido na linha seguinte
			vectparc = w * vectN;
			Sum(&vect, vectparc);
		} // end of for (k)
        vect = elem.JacDet * vect;
	}

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNdV

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
*/

/**
 Descricao:                                                           					\n
		Para formula��o a ser utilizada em problemas com simetria axial.	\n
	  Calcula a integral

\e		Integral (jacH) * N dV

	para elementos finitos 2D.
\n
  	Chama a fun��o: IntegralNdV \n
\n

\param  Simplex_Nodal_Element_2D &elem: espec�fico para Simplex_Nodal_Element_2D
\param	lev_math::Te_Vector<double> &vect: resultado da integra��o
\param	int				  integration_order: ordem de integra��o da quadratura de gauss

/return    0 - sucesso
/return    1 - jacobiano da transforma��o de coordenadas para o sistema
        homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Vector<double> &vect, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Vector<double>	vetloc(num_nodes);			// auxiliar vector

	symmfac = GetAxialRHSFactor(elem);

	codeErro = IntegralNdV(elem,vetloc,integration_order);

	if(!codeErro) vect = symmfac*vetloc;

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNdVAxial

\endcode

**/
int CLevPhenomenon::IntegralNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Vector<double> &vect, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Vector<double>	vetloc(num_nodes);			// auxiliar vector

	symmfac = GetAxialRHSFactor(elem);

	codeErro = IntegralNdV(elem,vetloc,integration_order);

	if(!codeErro) vect = symmfac*vetloc;

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNdVAxial

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNdl
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */
/**
 Descricao:                                                           					\n
	Calcula a integral

\e		Integral N dl

	para elementos finitos unidimensionais. A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss \n
\n
  N: fun��es de base \n
\n
  Exemplo de aplica��o: \n
	c�lculo de termo de condi��o de condi��o de contorno em arestas (Neumann, Convec��o) \n

\param Simplex_Nodal_Element_1D &elem : espec�fico para Simplex_Nodal_Element_1D
\param lev_math::Te_Vector<double> &vect: resultado da integra��o
\param	int				  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso

\code
int CLevPhenomenon::IntegralNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar

	vect = vectparc;//para zerar o conte�do de vect

	jacH = elem.MatJacobHomDet();
	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = w * jacH * vectN;   // *AP* 11/10/2007 incluido produto por jacH
			Sum(&vect, vectparc);
		} // end of for (k)
	}

	return codeErro;
}//end of CLevPhenomenon::IntegralNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order = 3)

\endcode
**/
int CLevPhenomenon::IntegralNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	double      jacH;           // jacobiano da transforma��o para o sistema homog�neo
	int			dimension;
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar

	vect = vectparc;//para zerar o conte�do de vect

	jacH = elem.MatJacobHomDet();
	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = w * vectN;   // *AP* 11/10/2007 incluido produto por jacH
			Sum(&vect, vectparc);
		} // end of for (k)
        vect = elem.JacDet * vect;
	}


	return codeErro;
}//end of CLevPhenomenon::IntegralNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order = 3)

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNNdl
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */
/**
 Descricao:                                                           					\n
	Calcula a integral

\e		Integral N * N dl

	para elementos finitos unidimensionais. A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
  N: fun��es de base \n
\n
  Exemplo de aplica��o: \n
	c�lculo de termo de condi��o de condi��o de contorno em arestas (Convec��o) \n
\n
\param		Simplex_Nodal_Element_1D &elem: espec�rfico para Simplex_Nodal_Element_1D
\param		lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param		int				  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso

\code
int CLevPhenomenon::IntegralNNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
		lev_math::Te_Matrix<double>	Matparc(num_nodes,num_nodes);		// matrix auxiliar

	Mat = Matparc;// para zerar o conte�do de Mat
	jacH = elem.MatJacobHomDet();
	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = w * jacH * vectN;
			Matparc = vectN ->* vectparc;
			Sum(&Mat, Matparc);
		} // end of for (k)
	}


	return codeErro;
}//end of CLevPhenomenon::IntegralNNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order = 3)

\endcode

**/
int CLevPhenomenon::IntegralNNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	double      jacH;           // jacobiano da transforma��o para o sistema homog�neo
	int			dimension;
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
		lev_math::Te_Matrix<double>	Matparc(num_nodes,num_nodes);		// matrix auxiliar

	Mat = Matparc;// para zerar o conte�do de Mat

	jacH = elem.MatJacobHomDet();
	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = w * vectN;
			Matparc = vectN ->* vectparc;
			Sum(&Mat, Matparc);
		} // end of for (k)
        Mat = elem.JacDet * Mat;
	}


	return codeErro;
}//end of CLevPhenomenon::IntegralNNdl(Simplex_Nodal_Element_1D &elem, lev_math::Te_Vector<double> &vect, int integration_order = 3)


// *AP* 11/10/2007
// ATENCAO:  REVER ESTA IMPLEMENTA��O PARA O CASO DE REGIOES DE TRANSFORMA��O
// NAO TESTADA

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
 */

/**
 Descricao:                                                           					\n
	Calcula a integral

\e		Integral (jacH) * N * N dV

	para elementos finitos (1D, 2D, 3D), levando em considera��o uma transfoma��o de
	coordenadas para tratamento de dom�nios abertos. A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
	N:		fun��es de base \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
	jacR:	determinante matriz jacobiana da transforma��o de coordenadas do dom�nio estudo \n
\n
  Exemplo de aplica��o: \n
\n
\param	Nodal_Element &elem: elemento finito
\param	lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param	int				  integration_order: ordem de integra��o da quadratura de gauss

\return 0 - sucesso
\return	1 - jacobiano da transforma��o de coordenadas para o sistema homog�neo igual a zero.
\return	2 - jacobiano da transofrma��o de coordenadas para a transforma��o de coordenadas igual a zero

\code
int CLevPhenomenon::IntegralNNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	double		jacR;			// determinante da matriz jacobiana da transformacao para dominio fechado
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
    Gauss_Pt	ptransf;		// auxiliar para receber o ponto de gauss em coordenadas cartesianas
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
		lev_math::Te_Matrix<double>	Matparc(num_nodes,num_nodes);		// matrix auxiliar

	Mat = Matparc;// para zerar o conte�do de Mat;

	jacH = elem.MatJacobHomDet();

	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
  		for (k = 0; k < npgauss; k++)
		{
		   elem.HomToCartesian(pt_gauss[k],ptransf); // transforma o ponto de integracao em ponto do dominio transformado
		   jacR = ptr_regions[region_number].Jacobian(ptransf); //obtem jacobiano no ponto de gauss no dominio transformado
		   if(!jacR) codeErro = 2; //jacobiano (determinante) no ponto de gauss no dominio transformado igual a zero
		   if(!codeErro)
		   {
				w = pt_gauss[k].Get_Variables(1);
				elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
				vectparc = (w/(jacH*jacR)) * vectN;
				Matparc = vectN->*vectparc;
				Sum(&Mat, Matparc);
		   }
		} // end of for (k)
	}

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNNdVCoordTransf

\endcode

**/
int CLevPhenomenon::IntegralNNdVCoordTransf(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int region_number, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	double		jacR;			// determinante da matriz jacobiana da transformacao para dominio fechado
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
    Gauss_Pt	ptransf;		// auxiliar para receber o ponto de gauss em coordenadas cartesianas
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
		lev_math::Te_Matrix<double>	Matparc(num_nodes,num_nodes);		// matrix auxiliar

	Mat = Matparc;// para zerar o conte�do de Mat;

	jacH = elem.MatJacobHomDet();

	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
  		for (k = 0; k < npgauss; k++)
		{
		   elem.HomToCartesian(pt_gauss[k],ptransf); // transforma o ponto de integracao em ponto do dominio transformado
		   jacR = ptr_regions[region_number].Jacobian(ptransf); //obtem jacobiano no ponto de gauss no dominio transformado
		   if(!jacR) codeErro = 2; //jacobiano (determinante) no ponto de gauss no dominio transformado igual a zero
		   if(!codeErro)
		   {
				w = pt_gauss[k].Get_Variables(1);
				elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
				vectparc = (w/jacR) * vectN;
				Matparc = vectN->*vectparc;
				Sum(&Mat, Matparc);
		   }
		} // end of for (k)
        Mat = elem.JacDet * Mat;
	}

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNNdVCoordTransf

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNNdV
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	06/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*  *NMA*
 ...........................................................................................
*/
/**
 Descricao:                                                           					\n
	Calcula a integral

\e		Integral (1/jacH) * N * N dV

	para elementos finitos (1D, 2D, 3D). A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
	N:		fun��es de base \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
\n
  Exemplo de aplica��o: \n
\n
\param Nodal_Element &elem	: elemento
\param lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param int  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso
\return	1 - jacobiano da transforma��o de coordenadas para o sistema homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralNNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[10];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
		lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
		lev_math::Te_Matrix<double>	Matparc(num_nodes,num_nodes);		// matrix auxiliar

	Mat = Matparc;// para zerar o conte�do de Mat;

	elem.MatJacobHom(Jhom, jacH);

	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
  		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = (w/jacH) * vectN;
			Matparc = vectN->*vectparc;
			Sum(&Mat, Matparc);
		} // end of for (k)
	}

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNNdV

\endcode

**/
int CLevPhenomenon::IntegralNNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			npgauss;		// number of Gauss points
	double		jacH;			// determinante da matriz jacobiana da transformacao para o sistema homog�neo
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			dimension;
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension = elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

	//dimensiona vetores auxiliares
		lev_math::Te_Vector<double>	vectN(num_nodes);		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
		lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
		lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
		lev_math::Te_Matrix<double>	Matparc(num_nodes,num_nodes);		// matrix auxiliar

	Mat = Matparc;// para zerar o conte�do de Mat;

	elem.MatJacobHom(Jhom, jacH);

	if(!jacH) codeErro = 1;
		//throw CError_Exception("Homogeneous system jacobian matrix null.");

	if(!codeErro)
	{
  		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);
			elem.Base_Function(&pt_gauss[k], vectN);//fun��es de base
			vectparc = w * vectN;
			Matparc = vectN->*vectparc;
			Sum(&Mat, Matparc);
		} // end of for (k)
        Mat = elem.JacDet * Mat;
	}

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNNdV

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		IntegralNNdVAxial
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	12/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras:*AP*  *NMA*
 ...........................................................................................
*/
/**
 Descricao:                                                           					\n
	Para formula��o a ser utilizada em problemas com simetria axial.\n
	Calcula a integral

\e		Integral (1/jacH) * N * N dV

	para elementos finitos 2D. \n

\param  Simplex_Nodal_Element_2D &elem: espec�fico para Simplex_Nodal_Element_2D
\param	lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param	int				  integration_order: ordem de integra��o da quadratura de gauss

/return 0 - sucesso
/return	1 - jacobiano da transforma��o de coordenadas para o sistema homog�neo igual a zero.

\code
int CLevPhenomenon::IntegralNNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralNNdV(elem,MatLoc,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;


	return codeErro;

}  // end of void CLevPhenomenon::IntegralNNdVAxial

\endcode

**/
int CLevPhenomenon::IntegralNNdVAxial(Simplex_Nodal_Element_2D &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int		codeErro=0;
	int		num_nodes;
	double	symmfac = 0.0;

	num_nodes = elem.Get_Nodes();					//obt�m n�mero de n�s do elemento

	lev_math::Te_Matrix<double>	MatLoc(num_nodes,num_nodes);			// auxiliar matrix

	symmfac = GetAxialMatrixFactor(elem);

	codeErro = IntegralNNdV(elem,MatLoc,integration_order);

	if(!codeErro) Mat = symmfac*MatLoc;

	return codeErro;

}  // end of void CLevPhenomenon::IntegralNNdVAxial


int CLevPhenomenon::IntegralNTensorPropertyNdV(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
    int       codeErro = 0;  // retorno
    int       k;             // contador
    int       npgauss;       // number of Gauss points
    double    jacH;          // determinante da matriz jacobiana da transformacao para o sistema homog�neo
    Gauss_Pt  pt_gauss[20];  // recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
    int       num_nodes;     // numero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)
    double    w;             // auxiliar que guarda peso do ponto de gauss
    int       dimension;
	Te_Nodal_Point<double> *point = (Te_Nodal_Point<double> *) ptr_lo_points[0][0];

    elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obtem os pontos de gauss a partir da ordem de integracao definida por integration_order
    dimension = elem.Get_Dimension();                          //obtem dimensao do elemento (1D, 2D, 3D)
    num_nodes = elem.Get_Nodes() * point->Get_N_Variables();

    //dimensiona vetores auxiliares
	lev_math::Te_Vector<double> vectN(num_nodes);             // auxiliar para armazenar as funcoes de base calculadas no ponto de gauss
	lev_math::Te_Vector<double> vectparc(num_nodes);          // vetor auxiliar
	lev_math::Te_Matrix<double> Jhom(dimension,dimension);    // Jacobian matrix for the homogeneous space transformation
	lev_math::Te_Matrix<double> Matparc(num_nodes,num_nodes); // matrix auxiliar
    lev_math::Te_Matrix<double> MatN(dimension,num_nodes);    // auxiliar matrix
    lev_math::Te_Matrix<double> MatNT(dimension,num_nodes);   // auxiliar matrix
    lev_math::Te_Matrix<double> MatNp(dimension,num_nodes);   // auxiliar matrix
	lev_math::Te_Matrix<double> K(dimension,dimension);       // matrix (tensor) of "properties"

    Mat = Matparc;// para zerar o conteudo de Mat;

    elem.MatJacobHom(Jhom, jacH);

    if(!jacH)
    	codeErro = 1;

    if(!codeErro)
    {
        GetProperty(elem, K); // obtem tensor de propriedade do material

        for (k = 0; k < npgauss; k++)
        {
            w = pt_gauss[k].Get_Variables(1);
            elem.Base_Function(&pt_gauss[k], vectN); //funcoes de base
            vectparc = vectN;
            for(int y=1;y<=dimension;y++) {
            	MatN.SetRow(y, vectN);
            	MatNp.SetRow(y, vectparc);
            }
            Matparc = K * MatNp;
    		MatNT = MatN;
            Transpose(&MatNT);
            Sum(&Mat, w * 0.5*(MatNT * Matparc));
        } // end of for (k)
        Mat = elem.JacDet * Mat;
    }
    return codeErro;
}  // end of void CLevPhenomenon::IntegralNTensorPropertyNdV


/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		GetAxialMatrixFactor
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	12/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras:*AP*  *NMA*
 ...........................................................................................
*/
/**
Descri��o: \n
	Retorna um fator para considera��o da simetria axial para altera��o dos elementos da matriz. \n

\param Finite_Element &elem

\return double factor: valor do fator

\code
double CLevPhenomenon :: GetAxialMatrixFactor(Finite_Element &elem)
{
	double factor;
	Point			o_centroid;
	double			r_baricentro;

	elem.Get_Centroid(o_centroid);
	r_baricentro = o_centroid.X();

	factor = 2*PI*r_baricentro;

	return factor;
}
\endcode

**/
double CLevPhenomenon :: GetAxialMatrixFactor(Finite_Element &elem)
{
	double factor;
	Point			o_centroid;
	double			r_baricentro;

	elem.Get_Centroid(o_centroid);
	r_baricentro = o_centroid.X();

	factor = 2.0*M_PI*r_baricentro;

	return factor;
}

/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		GetAxialMatrixFactor
 Versao:				1.0
 Compilador:			C++
 Programador:			Nancy Mieko Abe, Angelo Passaro
 Ultima modificacao:	12/01/06
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras:*AP*  *NMA*
 ...........................................................................................
*/
/**
Descri��o: \n
	Retorna um fator para considera��o da simetria axial para altera��o dos elementos do rhs. \n

\param Finite_Element &elem

\return double factor: valor do fator

\code
double CLevPhenomenon :: GetAxialRHSFactor(Finite_Element &elem)
{
	double factor;
	Point			o_centroid;
	double			r_baricentro;

	elem.Get_Centroid(o_centroid);
	r_baricentro = o_centroid.X();

	factor = 2*PI*r_baricentro;

	return factor;
}
\endcode

**/

double CLevPhenomenon :: GetAxialRHSFactor(Finite_Element &elem)
{
	double factor;
	Point			o_centroid;
	double			r_baricentro;

	elem.Get_Centroid(o_centroid);
	r_baricentro = o_centroid.X();

	factor = 2.0*M_PI*r_baricentro;

	return factor;
}




/*
 Projeto:				SDK_LEVSOFT
 Nome do Arquivo:		phenomenon.cpp
 Nome da Funcao:		Integral N gradN dV
 Versao:				1.0
 Compilador:			C++
 Programador:			Cristiano Maciel
 Ultima modificacao:	11/10/07
 Modificado por:
 Modificacao:
 ...........................................................................................
	String que indica possiveis modificaoes futuras: *AP*
 ...........................................................................................
*/
/**
 Descricao:                                                           					\n
	Calcula a integral

\e		Integral N * gradN dV

	para elementos finitos (1D, 2D, 3D). A integral � realizada no sistema
	de coordenadas homogeneo usando a quadratura de Gauss. \n
\n
	N:		fun��es de base \n
	jacH:	determinante da matriz jacobiana da transforma��o de coordenadas para o sistema homogeneo \n
\n
  Exemplo de aplica��o: \n
\n
\param Nodal_Element &elem	: elemento
\param lev_math::Te_Matrix<double> &Mat: resultado da integra��o
\param int  integration_order: ordem de integra��o da quadratura de gauss

\return    0 - sucesso
\return	1 - jacobiano da transforma��o de coordenadas para o sistema homog�neo igual a zero.

\code


\endcode

**/
int CLevPhenomenon :: IntegralNGradN(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			dimension;
	int			npgauss;		// number of Gauss points
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)

	elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	lev_math::Te_Vector<double>	vectN(num_nodes),		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
						vectNaux(num_nodes);
	lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
	lev_math::Te_Vector<double>	vect;
	lev_math::Te_Vector<double>	vectGradN;

	lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
	lev_math::Te_Matrix<double>	Zeronxn(num_nodes,num_nodes);			// auxiliar matrix to reset contents of other matrix
	lev_math::Te_Matrix<double> GradNaux(dimension,num_nodes);				// matrix of gradients of the base functions
	lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradNaux.Columns());	// auxiliar matrix
	lev_math::Te_Matrix<double> Mataux(dimension,num_nodes);		// matriz que recebe o resultado da multiplicação entre N e GradN

	Mat = Zeronxn; // "zera" as posi��es da matrix

	if(!codeErro)
	{

		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);

			elem.Base_Function(&pt_gauss[k], vectNaux);//fun��es de base
			vectN = w * vectNaux;     //*AP* 11/10/2007 retirado o produto por jacH

			elem.GradN(&pt_gauss[k], GradNaux);		//obtem gradiente da fun��o de base no ponto de gauss
			Matparc = GradNaux;

			vectGradN = Matparc.GetRow(1);

			//////////////////////////////////////////////////////////////////////////
			// *CM* (04/09/2008)
			//	As duas linhas de código abaixo apresentavam erro para elementos de segunda ordem.
			//  Depois da multiplicação entre o vetor N e o vetor GradN, a matriz GradNaux assumia
			//  a dimensão de 3x3. No segundo ponto de gauss (k=1), não era possível calcular:
			//  Matparc = Jhom * GradNaux;
			//  pois Jhom era 1x1 e GradNaux era 3x3.
			//  Para consertar foi necessário criar a variável auxiliar "Mataux".
			//////////////////////////////////////////////////////////////////////////
			//GradNaux = vectN ->* vectGradN;
			//Sum(&Mat, GradNaux);

			Mataux = vectN ->* vectGradN;
			Sum(&Mat, Mataux);
		}
        Mat = elem.JacDet * Mat;
	}

	return codeErro;
}

int CLevPhenomenon :: IntegralGradN(Nodal_Element &elem, lev_math::Te_Matrix<double> &Mat, int integration_order)
{
	int			codeErro = 0;	// retorno
	int			k;				// contador
	int			dimension;
	int			npgauss;		// number of Gauss points
	Gauss_Pt	pt_gauss[20];	// recebe pontos de gauss dos elementos: dimensionado com a maior ordem de quadratura
	double		w;				// auxiliar que guarda peso do ponto de gauss
	int			num_nodes;		// n�mero de nos do elemento, varia com a dimens�o, o tipo e a ordem dos elementos(*NMA*: a ordem dos elementos deve ser definida para a classe ( Nodal_Element :: Set_Order(ordem_el);)

	elem.Gauss_Points(integration_order,npgauss,&pt_gauss[0]); //obt�m os pontos de gauss a partir da ordem de integra��o definida por integration_order
	dimension	= elem.Get_Dimension();					//obt�m dimens�o do elemento (1D, 2D, 3D)
	num_nodes	= elem.Get_Nodes();						//obt�m n�mero de n�s do elemento

	lev_math::Te_Vector<double>	vectN(num_nodes),		// auxiliar para armazenar as func��es de base calculadas no ponto de gauss
						vectNaux(num_nodes);
	lev_math::Te_Vector<double>	vectparc(num_nodes);	// vetor auxiliar
	lev_math::Te_Vector<double>	vect;
	lev_math::Te_Vector<double>	vectGradN;

	lev_math::Te_Matrix<double>	Jhom(dimension,dimension);	// Jacobian matrix for the homogeneous space transformation
	lev_math::Te_Matrix<double>	Zeronxn(dimension,num_nodes);			// auxiliar matrix to reset contents of other matrix
	lev_math::Te_Matrix<double> GradNaux(dimension,num_nodes);				// matrix of gradients of the base functions
	lev_math::Te_Matrix<double>	Matparc(Jhom.Rows(),GradNaux.Columns());	// auxiliar matrix
	lev_math::Te_Matrix<double> Mataux(dimension,num_nodes);		// matriz que recebe o resultado da multiplicação entre N e GradN

	Mat = Zeronxn; // "zera" as posicoes da matrix

	if(!codeErro)
	{

		for (k = 0; k < npgauss; k++)
		{
			w = pt_gauss[k].Get_Variables(1);

			elem.GradN(&pt_gauss[k], GradNaux);		// obtem gradiente da funcao de base no ponto de gauss

			Sum(&Mat, GradNaux);
		}
        Mat = elem.JacDet * Mat;
	}

	return codeErro;
}
