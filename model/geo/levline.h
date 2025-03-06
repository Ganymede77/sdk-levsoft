#ifndef __LEVLINE_H_
#define __LEVLINE_H_

//===================================================================================================
//	Arquivo:			levline.h
//...................................................................................................
//	Descri��o:			Interface da classe CLevLine
//...................................................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//---------------------------------------------------------------------------------------------------
//	Classe CLevLine
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associa��es:
//
//	Observa��es:
//...................................................................................................
//	Data:				Junho/2000
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================

#include "levpoint.h"
#include "levprimitive.h"
#include "levdef.h"
#include "levtypedef.h"
#include "lo_templ.hpp"

class CLevLine : public CLevPrimitive
{

protected:
	double	coefficients[3];

	void	Line_Coefficients();
	double	Linear_Coefficient (){Line_Coefficients();return coefficients[2];};
	double	Y_Coefficient      (){Line_Coefficients();return coefficients[0];};

public:

	CLevLine();
	CLevLine(CLevPoint *p1, CLevPoint *p2);
	virtual ~CLevLine();

	CLevPrimitive* Clone() const override
	{
        return new CLevLine(*this); // chama construtor de c�pia de CLevLine
    }

	// *WPG*: Tornou-se public para ser acessado por lineproperties_dlg na funcao UpdateProperties
	double Angular_Coefficient() { Line_Coefficients(); return coefficients[1]; }

	//......................................................................
	//................. fun��es de interface herdadas de CLevGeoEntity..
	//......................................................................
	virtual void Move			(double deltax, double deltay, double deltaz = 0);
	virtual	void Reinitialize   (void);
	virtual	void Rotation		(double angle, CLevPoint &pointc);
	virtual void Scale			(double factor);
	virtual void ReflectionByX		(void);
	virtual void ReflectionByY		(void);
//......................................................................................................
//................. interfaces herdadas de CLevPrimitive......................................
//......................................................................................................
	virtual void SetPoints			(vpCLevPoint &vp);
	virtual bool IsInBoundingBox	(CLevPoint);
	virtual void PreDiscretize		(vCLevPoint &);

	virtual CLevPoint GetMiddlePoint(void);

//......................................................................................................
//......................... Operadores...............................................................
//......................................................................................................
	CLevLine & operator = (const CLevLine &);
	bool operator ==	(const CLevLine &a);
	bool operator !=	(const CLevLine &a);

//......................................................................................................
//.......................Grava��o e Leitura...........................................................
//......................................................................................................
	int	Save(FILE * parq, int unidade, FILENAME tipoArq);
	int Read(FILE * parq, int unidade, char *tp, FILENAME tipoArq);

//......................................................................................................
//................. funções de interface desta classe......................................
//......................................................................................................
	lev_math::USINT CrossingPoint				(CLevLine  otherline, vCLevPoint &vo_crosspoint);
//	USINT CrossingPoint(CLevPrimitive  &otherline, vCLevPoint &vo_crosspoint);


//......................................................................................................
//................... funções friend...................................................................
//......................................................................................................

	friend	unsigned int CrossingPoints( CLevLine  o_line, CLevEllipse  o_ellipse, vCLevPoint &vo_crosspoint);

//......................................................................................................
//................... classes friend...................................................................
//......................................................................................................
	friend class CTabItemPropertiesLine;

};

#endif
