//===================================================================================================
//	Arquivo:			recteangle.cpp
//...................................................................................................
//	Descrição:			Implementação da classe CRectangle
//...................................................................................................
//	Autores:			
//						Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CRectangle
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:			
//
//	Observações: 
//
//...................................................................................................
//	Data:				Junho/2005
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================
#include "rectangularelement.h"


////////////////////////
//The constructor function
////////////////////////
CRectangularElement::CRectangularElement()
{
}

CRectangularElement::~CRectangularElement()
{
}

void CRectangularElement::GetVertices(std::vector<int> &vert)
{
	int i;
	int	num_vert;

	//limpa o vetor
	vert.clear();

	num_vert = vertices.size();

	for(i=0;i<num_vert;i++)
	{
		vert.push_back(vertices[i]);
	}
}

void CRectangularElement::SetVertices(std::vector<int> &vert)
{
	int i;
	int	num_vert;

	//limpa o vetor
	vertices.clear();

	num_vert = vert.size();

	for(i=0;i<num_vert;i++)
	{
		vertices.push_back(vert[i]);
	}
}

void CRectangularElement::GetNeighbors(std::vector<int> &neig)
{
	int i;
	int	num_neig;

	//limpa o vetor
	neig.clear();

	num_neig = neighbors.size();

	for(i=0;i<num_neig;i++)
	{
		neig.push_back(neighbors[i]);
	}
}

void CRectangularElement::SetNeighbors(std::vector<int> &neig)
{
	int i;
	int	num_neig;

	//limpa o vetor
	neighbors.clear();

	num_neig = neig.size();

	for(i=0;i<num_neig;i++)
	{
		neighbors.push_back(neig[i]);
	}
}

