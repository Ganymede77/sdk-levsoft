////////////////////////
//PcfDevice.h
////////////////////////
//Class declaration

#ifndef __LEV_PCF_DEVICE_H__
#define __LEV_PCF_DEVICE_H__

//=============================================================================
//	Arquivo:		PcfDevice.h
//.............................................................................
//	Descricao:
//.............................................................................
//	Autores:			
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes:
//.............................................................................
//	Data:				janeiro/2003
//	Modificado em:	
//	Modificado por:	
//	Motivo:
//=============================================================================


#include "levgroup.h"


class CPcfDevice : public CLevGroup
{
protected:

	std::vector<double> vRadiusLayer;
	std::vector<int> vNSubLayer;
	
	int		NtotalLayers;
	double	lastRadiusLayer;
	double	centerholeEdit;
	double	pit;

public:

	CPcfDevice(); //Constructor

//----------------------------------------------------
	void	Set_Pcf_Last_Radius_Layer(double);
	void	Set_Pcf_N_Total_Layers(int);

	void	Set_Pcf_Pit(double);
	void	Set_Pcf_Radius_Center_Hole(double);

	void	Set_Pcf_Radius_Layer(double, int i = -1);
	void	Set_Pcf_N_Layer(int, int i = -1);
//----------------------------------------------------		
	double	Get_Pcf_Last_Radius_Layer(void);
	int		Get_Pcf_N_Total_Layers(void);
	
	double	Get_Pcf_Radius_Layer(int);
	int		Get_Pcf_N_Layer(int);

	double	Get_Pcf_Pit(void);
	double	Get_Pcf_Radius_Center_Hole(void);
//----------------------------------------------------	

	virtual ~CPcfDevice(); //Destructor

	

};

#endif
