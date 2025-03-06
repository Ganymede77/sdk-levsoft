#ifndef __LEV_REGION_H_
#define __LEV_REGION_H_

//=============================================================================
//	Arquivo:			LevRegion.h
//.............................................................................
//	Descricao:			Interface da classe CLevRegion
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//						
//-----------------------------------------------------------------------------
//	Classe CLevRegion
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes: Derivada de CLevGroup
//
//.............................................................................
//	Data:				Setembro/2002
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//=============================================================================


#include "levgroup.h"


class CLevRegion : public CLevGroup
{

protected:

	vInt		vElementsRegion;
	vInt		vElementsRegionRefined;
	CLevPoint   oPointClicked;
	double		permanentMagnetAngle;

public:

	CLevRegion();
	virtual ~CLevRegion();

	vCLevPoint	vPoints;

	virtual void Reinitialize	(void);

	void		IncludeElementsRegion(vInt &);
	void		IncludeElementsRegionRefined(vInt &);
	void		GetElementsRegion(vInt &);
	void		GetElementsRegionRefined(vInt &);
	void		SortPoints(void);
	int			IsAPointRegion(CLevPoint&);
	bool		IsInBoundingBox(CLevPoint &);
	void		SetIdentificationPoint(CLevPoint & );
	void		GetIdentificationPoint(CLevPoint & );
	double		GetPermanentMagnetAngle(void);
	void		SetPermanentMagnetAngle(double);

	//......................................................................................................
	//......................... Operadores...............................................................
	//......................................................................................................
	bool		operator ==	( CLevRegion &);
	CLevRegion&	operator  =	(const CLevRegion &);
};


#endif
