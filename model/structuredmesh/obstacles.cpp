//===================================================================================================
//	Arquivo:			obstacle.cpp
//...................................................................................................
//	Descrição:			Implementação da classe CStructuredMeshObstacle
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CStructuredMeshObstacle
//---------------------------------------------------------------------------------------------------
//
//	Descrição: Armazenamento das informações de pontos inicial e final dos obstaculos definidos
//				para uma malha estruturada
//
//...................................................................................................
//	Data:				02/03/2005
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================

#include "obstacles.h"
	
CStructuredMeshObstacle::CStructuredMeshObstacle()
{
	i_start = 0;
	j_start = 0;
	i_end   = 0;	
	j_end   = 0;	

}

CStructuredMeshObstacle::~CStructuredMeshObstacle(){ };


void CStructuredMeshObstacle::IndexIstart(unsigned int index){i_start = index;}
unsigned int CStructuredMeshObstacle::IndexIstart(void){return i_start;}

void CStructuredMeshObstacle::IndexIend(unsigned int index){i_end = index;}
unsigned int CStructuredMeshObstacle::IndexIend(void){return i_end;}

void CStructuredMeshObstacle::IndexJstart(unsigned int index){j_start = index;}
unsigned int CStructuredMeshObstacle::IndexJstart(void){ return j_start;} 

void CStructuredMeshObstacle::IndexJend(unsigned int index){j_end = index;}
unsigned int CStructuredMeshObstacle::IndexJend(void){return j_end;}

CStructuredMeshObstacle & CStructuredMeshObstacle :: operator = (const CStructuredMeshObstacle & ro_entrada)
{

	if (this != &ro_entrada) 
	{
		i_start = ro_entrada.i_start;
		j_start = ro_entrada.j_start;

		i_end	= ro_entrada.i_end;
		j_end	= ro_entrada.j_end;
	}

	return *this;

}//end of operator =

bool CStructuredMeshObstacle :: operator == (const CStructuredMeshObstacle & ro_entrada)
{
	bool ret = false;

	if(i_start == ro_entrada.i_start)
	 if(i_end == ro_entrada.i_end)
	  if(j_start == ro_entrada.j_start)
		if(j_end == ro_entrada.j_end)
			 {
				 ret = true;
			 }

	return ret;

}//end of operator ==
