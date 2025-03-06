#ifndef __LEV_RECTANGLE_H_
#define __LEV_RECTANGLE_H_

//===================================================================================================
//	Arquivo:			rectangle.h
//...................................................................................................
//	Descrição:			Interface da classe CRectangle
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CQuadrilatero
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:			
//
//	Observações: utilizada para armazenar os vertices dos nós de elementos quadriláteros.
//				criada inicialmente para armazenamento dos elementos da malha
//				estruturada até a criação da classe dos elementos finitos quadriláteros
//...................................................................................................
//	Data:				Junho/2005
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================

#include <vector>


class CRectangularElement
{
protected:

	std::vector<int>		vertices;
	std::vector<int>		neighbors;

public:

	CRectangularElement(); //Constructor
	~CRectangularElement(); //Destructor

	void		GetVertices(std::vector<int> &vert);
	void		SetVertices(std::vector<int> &vert);

	void		GetNeighbors(std::vector<int> &vert);
	void		SetNeighbors(std::vector<int> &vert);
//	bool			operator ==	( CRectangle &);
//	CRectangle&	operator  =	(const CRectangle &);

};
#endif