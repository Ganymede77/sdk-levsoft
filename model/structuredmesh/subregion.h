#ifndef __LEV_SUBREGION_H_
#define __LEV_SUBREGION_H_

//=============================================================================
//	Arquivo:			Subregion.h
//.............................................................................
//	Descricao:			Interface da classe CSubRegion
//.............................................................................
//	Autores:			Nancy Mieko Abe
//						
//-----------------------------------------------------------------------------
//	Classe CSubRegion
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes: Derivada de CLevGroup
//
//.............................................................................
//	Data:				Marco/2004
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//=============================================================================


#include "levfrontier.h"


class CSubRegion 
{
protected:
	int					number;
	int					modelRegionIndex;
	int					localSouthFrontierIndex;
	CLevPoint			oBaricentro;
	LEV_OBJECTVALIDITY	status;

	void		ComputeBaricentro(void);
	int			CCW(CLevPoint &, CLevPoint &, CLevPoint &);

public:
	CSubRegion(); //Constructor
	virtual ~CSubRegion(); //Destructor

	vpCLevFrontier vpFrontiers;
	vInt			vIndexCCWSortedFrontiers;

	int			IncludeFrontier(CLevFrontier *);
	int			IsAFrontier(CLevFrontier *);
	int			SortFrontiers(void);
	void		SetAssociatedModelRegionIndex(int);
	int			GetAssociatedModelRegionIndex(void);
	void		Reinitialize(void);
	int			GetNumFrontiers(void);
	bool		operator == ( CSubRegion &s);
	void		SetLocalSouthFrontierIndex(int);
	int			GetLocalSouthFrontierIndex(void);
	int			GetLocalEastFrontierIndex(void);
	int			GetLocalNorthFrontierIndex(void);
	int			GetLocalWestFrontierIndex(void);
	CLevFrontier *GetLocalSouthFrontier(void);
	int			CheckFrontiersPredisConsistency(void);
	int			GetNumOfPredisSegmentsSouthFrontier(void);
	int			GetNumOfPredisSegmentsEastFrontier(void);
	void		BuildFrontiersPrediscretizationVectors(void);
	LEV_OBJECTVALIDITY Status(void){return status;}
	void		Status(LEV_OBJECTVALIDITY st){status = st;}
	
};
#endif
