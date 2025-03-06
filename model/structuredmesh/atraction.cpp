//===================================================================================================
//	Arquivo:			atraction.cpp
//...................................................................................................
//	Descrição:			Implementação da classe CStructuredMeshAtraction
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						
//---------------------------------------------------------------------------------------------------
//	Classe CStructuredMeshAtraction
//---------------------------------------------------------------------------------------------------
//
//	Descrição: Armazenamento das informações de pontos e linhas de atração definidos
//				para uma malha estruturada
//
//...................................................................................................
//	Data:				02/03/2005
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//===================================================================================================

#include "atraction.h"
	
CStructuredMeshAtraction::CStructuredMeshAtraction()
{
	line=0;		// indice de linha de atração
	column=0;			// indice de coluna de atração
	indexI=0;			// indice I de ponto de atração
	indexJ=0;			// indice J de ponto de atração

	//coeficientes de atração
	coefA=0;
	coefB=0;
	coefC=0;
	coefD=0;

}

CStructuredMeshAtraction::~CStructuredMeshAtraction(){ };

void			CStructuredMeshAtraction::Line(unsigned int index) { line = index;}
unsigned int	CStructuredMeshAtraction::Line(void) { return line;}

void			CStructuredMeshAtraction::Column(unsigned int index) { column = index;}	
unsigned int	CStructuredMeshAtraction::Column(void){return column;}

void			CStructuredMeshAtraction::IndexI(unsigned int index) { indexI = index;}
unsigned int	CStructuredMeshAtraction::IndexI(void) { return indexI;}

void			CStructuredMeshAtraction::IndexJ(unsigned int index) { indexJ = index;}
unsigned int	CStructuredMeshAtraction::IndexJ(void) { return indexJ;}

void			CStructuredMeshAtraction::IndexSubRegion(unsigned int index) { subregion = index;}
unsigned int	CStructuredMeshAtraction::IndexSubRegion(void) { return subregion;}

void			CStructuredMeshAtraction::CoefA(double a) {coefA = a;}
double			CStructuredMeshAtraction::CoefA(void) {return coefA;}

void			CStructuredMeshAtraction::CoefB(double b){coefB = b;}
double			CStructuredMeshAtraction::CoefB(void){return coefB;}

void			CStructuredMeshAtraction::CoefC(double c){coefC = c;}
double			CStructuredMeshAtraction::CoefC(void){return coefC;}
	
void			CStructuredMeshAtraction::CoefD(double d){coefD = d;}
double			CStructuredMeshAtraction::CoefD(void){return coefD;}

CStructuredMeshAtraction & CStructuredMeshAtraction :: operator = (const CStructuredMeshAtraction & ro_entrada)
{

	if (this != &ro_entrada) 
	{
		line	= ro_entrada.line;
		column	= ro_entrada.column;

		indexI	= ro_entrada.indexI;
		indexJ	= ro_entrada.indexJ;

		subregion	= ro_entrada.subregion;

		coefA	= ro_entrada.coefA;
		coefB	= ro_entrada.coefB;
		coefC	= ro_entrada.coefC;
		coefD	= ro_entrada.coefD;
	}

	return *this;

}//end of operator =

bool CStructuredMeshAtraction :: operator == (const CStructuredMeshAtraction & ro_entrada)
{
	bool ret = false;

	if (line == ro_entrada.line)
	 if(column == ro_entrada.column)
	  if(indexI	== ro_entrada.indexI)
		if(indexJ == ro_entrada.indexJ)
   		 if(subregion == ro_entrada.subregion)
		  if(coefA == ro_entrada.coefA)
		   if(coefB == ro_entrada.coefB)
	  		if(coefC == ro_entrada.coefC)
			 if(coefD == ro_entrada.coefD)
			 {
				 ret = true;
			 }

	return ret;

}//end of operator ==
