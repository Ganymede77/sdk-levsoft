#ifndef __LEV_GROUP_H_
#define __LEV_GROUP_H_

//=============================================================================
//	Arquivo:			LevGroup.h
//.............................................................................
//	Descricao:			Interface da classe CLevGroup
//.............................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//						
//-----------------------------------------------------------------------------
//	Classe CLevGroup
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes: Baseada em CLevPolygon implementado em SystemPOI
//
//.............................................................................
//	Data:				Abril/2002
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//=============================================================================

#include "levgeoentity.h"
#include "levenum.h"
#include "levtypedef.h"
#include "levpoint.h"
#include "levline.h"
#include "levellipse.h"


class CLevGroup : public CLevGeoEntity
{
protected:

	CLevPoint	pt_minxminy,pt_maxxmaxy;  // armazena os limites da primitiva

public:

	CLevGroup(); //Constructor
	virtual ~CLevGroup(); //Destructor

//......................................................................................................
//................. fun��es de interface herdadas de CLevGeoEntity......................................
//......................................................................................................
	virtual void Limits			(CLevPoint &, CLevPoint &);
	virtual void Move			(double deltax, double deltay, double deltaz=0);
	virtual void Reinitialize	(void);
	virtual void Rotation		(double angle, CLevPoint &pointc);
	virtual void Scale			(double factor);
	virtual void ReflectionByX		(void);
	virtual void ReflectionByY		(void);
	virtual void TakeLimits		(void);  // obtem os pontos v�rtices de um ret�ngulo que engloba o poligono

//......................................................................................................
//................. fun��es de interface desta classe......................................
//......................................................................................................
	//========================================================================
	// os pr�ximos m�todos permitem incluir novas linhas e elipses � lista do grupo
	// Devem ser utilizadas enquanto constroi-se um novo grupo.
	//========================================================================
	int IsAPoint			(CLevPoint     *);    
	int IsALine				(CLevLine      *);    
	int IsAnEllipse			(CLevEllipse   *);    
	int IsAPrimitive		(CLevPrimitive *);    

	int IncludeNextPoint	(CLevPoint	   *p);
	int IncludeNextLine		(CLevLine      *l);     
	int IncludeNextEllipse  (CLevEllipse   *e);
	int IncludeNextPrimitive(CLevPrimitive *p);

	void DeletePoint		(CLevPoint    *p);     
	void DeleteLine			(CLevLine     *l);     
	void DeleteEllipse		(CLevEllipse  *e);
	void DeletePrimitive	(CLevPrimitive  *p);
	
	void SetLines			(vpCLevLine );
	void SetEllipses		(vpCLevEllipse );

	int	 NumValidLines		(void);	
	int	 NumValidCurves		(void);
	int	 NumValidPoints		(void);
	int	 NumValidPrimitives (void);

//  void SetPoints  	(vpCLevPoint);
//	void DeletePoint		(CLevPoint    *p);
//	int IncludeNextPoint	(CLevPoint    *p);

//......................................................................................................
//......................... Operadores...............................................................
//......................................................................................................
	CLevGroup & operator = (const CLevGroup & );
	bool		operator ==	( CLevGroup &a);
//	bool		operator !=	(const CLevGroup &a);

}; // end of CLevGroup
#endif
