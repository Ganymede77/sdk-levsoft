#ifndef __LEV_ARROW_DEVICE_H__
#define __LEV_ARROW_DEVICE_H__


//=============================================================================
//	Arquivo:		ArrowDevice.h
//.............................................................................
//	Descricao:
//.............................................................................
//	Autores:			
//-----------------------------------------------------------------------------
//	Classe CLevEllipse
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes:
//.............................................................................
//	Data:				novembro/2002
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//=============================================================================


#include "levgroup.h"


class CArrowDevice : public CLevGroup
{

public:

	CArrowDevice(); //Constructor
	//----------------------------------------------------
	void	Set_Arrow_H(double, int i = -1);
	double	Get_Arrow_H(int); 

	void	Set_Arrow_W(double,int i = -1);
	double	Get_Arrow_W(int);
	//----------------------------------------------------
	virtual ~CArrowDevice(); //Destructor
	
protected:

public:
	std::vector<double> vW;
	std::vector<double> vH;
};

#endif
