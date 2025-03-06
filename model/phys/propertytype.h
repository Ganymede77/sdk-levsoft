#ifndef __PROPERTY_TYPE_H_
#define __PROPERTY_TYPE_H_

//===================================================================================================
//	Arquivo:			PropertyType.h
//...................................................................................................
//	Descrição:			Interface da classe CPropertyType
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//---------------------------------------------------------------------------------------------------
//	Class CPropertyType:
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:
//
//	Observações:
//
//...................................................................................................
//	Data:				Julho/2001
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================

class CPropertyType
{
//attibutes
protected:

public:


//implementations
protected:

public:
	CPropertyType(); //Constructor
	virtual ~CPropertyType(); //Destructor

	virtual double Get_Value(void) = 0;

};// end of class CPropertyType

//===================================================================================================
//	Arquivo:			PropertyType.h
//...................................................................................................
//	Descrição:			Interface da classe CPropertyType
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//---------------------------------------------------------------------------------------------------
//	Classe CPropertyType:
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:
//
//	Observações:
//
//...................................................................................................
//	Data:				Julho/2001
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================
class CCteProperty : public CPropertyType
{
//attibutes
protected:
	double value;

public:


//implementations
protected:

public:
	CCteProperty(); //Constructor
	~CCteProperty(); //Destructor

	virtual double	Get_Value(void);
	virtual void	Set_Value(double);

};// end of class CCteProperty



#endif
