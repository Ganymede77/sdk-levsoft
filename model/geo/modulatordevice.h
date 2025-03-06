////////////////////////
//ModulatorDevice.h
////////////////////////
//Class declaration

#ifndef __LEV_MODULATOR_DEVICE_H__
#define __LEV_MODULATOR_DEVICE_H__


//=============================================================================
//	Arquivo:		ModulatorDevice.h
//.............................................................................
//	Descricao:
//.............................................................................
//	Autores:			
//-----------------------------------------------------------------------------
//	Classe CModulatorDevice
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


class CModulatorDevice : public CLevGroup
{

public:

	CModulatorDevice(); //Constructor
	//----------------------------------------------------
#if 10
	void	Set_Modulator_W1(double);
	void	Set_Modulator_W2(double);
	void	Set_Modulator_Wc(double);
	void	Set_Modulator_S1(double);
	void	Set_Modulator_S2(double);
	void	Set_Modulator_E(double);
	void	Set_Modulator_B(double);
	void	Set_Modulator_H(double);
	void	Set_Modulator_L(double);

	
	double	Get_Modulator_W1(void); 
	double	Get_Modulator_W2(void);
	double	Get_Modulator_Wc(void); 
	double	Get_Modulator_S1(void);
	double	Get_Modulator_S2(void); 
	double	Get_Modulator_E(void);
	double	Get_Modulator_B(void); 
	double	Get_Modulator_H(void);
	double	Get_Modulator_L(void);

	
	//----------------------------------------------------
#endif
	
	virtual ~CModulatorDevice(); //Destructor

	
protected:

public:
	//std::vector<double> vW;
	//std::vector<double> vH;
	double w1,w2,wc,s1,s2,e,b,h,l;
};

#endif
