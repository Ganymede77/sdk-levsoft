#ifndef __LEVPOINT_H_
#define __LEVPOINT_H_

//===================================================================================================
//	Arquivo:			LevPoint.h
//...................................................................................................
//	Descrição:			Interface da classe CLevPoint
//...................................................................................................
//	Autores:			Angelo Passaro
//						Nancy Mieko Abe
//---------------------------------------------------------------------------------------------------
//	Classe CLevPoint
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:
//
//	Observações:
//...................................................................................................
//	Data:				Junho/2000
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================

#include "levgeoentity.h"

class CLevPoint : public CLevGeoEntity
{
protected:
	double	x;
	double	y;
	double  z;

public:

	// incluir um objeto de associação entre ponto e as linhas ligadas a ele.
	int	   num_primitives; // numero de primitivas que tem o ponto como vértice
	int	   num_ellipses; // numero de elipses que tem o ponto como vértice *NMA*: mantido por causa das operações Move

public:

	CLevPoint(); //Constructor
	~CLevPoint(); //Destructor

	double	GetXMax(void){ return x;};
	double	GetXMin(void){ return x;};
	double	GetYMin(void){ return y;};
	double	GetYMax(void){ return y;};

	double	X(void){ return x;};
	double	Y(void){ return y;};
	double	Z(void){ return z;};

	void	X(double a){ x = a;};
	void	Y(double a){ y = a;};
	void	Z(double a){ z = a;};

	void	Set_Coord(double xi,double yi){x=xi; y=yi;};
	void	Modifica(double xi,double yi,double zi){x=xi; y=yi; z=zi;};

	void	ReflectionByX(void);
	void	ReflectionByY(void);
	void	Move        (double a, double b, double c = 0);
	void	Rotation    (double ang, CLevPoint &p);
	void	Scale		(double factor);

	void	Reinitialize(void);
	void	Limits(CLevPoint &, CLevPoint &);
	void	TakeLimits() {};  // no_op

	double  CalcDistance(CLevPoint &);

	CLevPoint& operator =  (const CLevPoint &a);
	bool operator == (const CLevPoint &a);
	bool operator != (const CLevPoint &a);

};

#endif
