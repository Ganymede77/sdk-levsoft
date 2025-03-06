#ifndef __PROPERTY_TENSOR_H_
#define __PROPERTY_TENSOR_H_

//=============================================================================
//	Arquivo:			PropertyTensor.h
//.............................................................................
//	Descricao:			Interface da classe CPropertyTensor
//.............................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//-----------------------------------------------------------------------------
//	Classe CPropertyTensor:
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes:
//
//.............................................................................
//	Data:				Julho/2001
//	Modificado em:
//	Modificado por:
//	Motivo:
//=============================================================================

#include "propertytype.h"

class CPropertyTensor
{
//attibutes
protected:

public:


//implementations
protected:

public:
	CPropertyTensor(); //Constructor
	virtual ~CPropertyTensor(); //Destructor

	virtual double	Get_Value(void) = 0;
	virtual void	Set_Value(double)=0;


};// end of class CPropertyTensor

//===================================================================================================
//	Arquivo:			PropertyTensor.h
//...................................................................................................
//	Descri��o:			Interface da classe CLevPropertyType
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//---------------------------------------------------------------------------------------------------
//	Class CCtePropertyTensor:
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associa��es:
//
//	Observa��es:
//
//...................................................................................................
//	Data:				Julho/2001
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================
class CCtePropertyTensor : public CPropertyTensor
{
//attibutes
protected:
	CCteProperty property;

public:


//implementations
protected:

public:
	CCtePropertyTensor(); //Constructor
	virtual ~CCtePropertyTensor(); //Destructor

	virtual double	Get_Value(void);
	virtual void	Set_Value(double);

};// end of class CCtePropertyTensor



#endif
