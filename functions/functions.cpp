#include <iostream>
#include <cmath>
#include "functions.hpp"

//***************************************************************************
// Projeto: LEVSOFT
//***************************************************************************
//
// Nome: varying_functions.cpp
//
// Versao				:	1
// Compilador			:	Visual C++ 6.0
// Programador			:	Nancy Mieko Abe
// Data criacao			:	01/12/2003
// Ultima modificacao	:	01/11/2005
// Motivo				:	Alteracao de nomes
//
//***************************************************************************
//
// Descricao:
//
//	Implementacao de classes de funcoes (contante, degrau, senoidal, 
//	exponencial, pulso, ...) baseada na implementacao das classes de 
//  fontes de tensao variaveis no tempo do trabalho de tese de Nancy.
//
//  Estas funcoes podem ser usadas para variacoes no tempo e no espaco. As 
//  unidades utilizadas devem ser:
//	- no caso de tempo: segundos
//	- no caso de espaco: metros
//
//
//***************************************************************************
//***************************************************************************
//***************************************************************************
//
//                    C L A S S    C F U N C T I O N S
//
//***************************************************************************
//***************************************************************************

CFunction :: CFunction()
{
	value = 0.0;
	TYPE = STEPFUNCTION;
}


CFunction :: ~CFunction()
{ }


//===========================================================================
//               class CFunction :: Set_Value
//===========================================================================
void CFunction :: Set_Value(double val)
{
	value = val;
}


//***************************************************************************
//***************************************************************************
//
//                    C L A S S     C F U N C T I O N S T E P
//
//***************************************************************************
//***************************************************************************
CFunctionStep :: CFunctionStep()
{
	k0 = 0.0;
	k1 = 0.0;
	kf = 0.0;
}


CFunctionStep :: ~CFunctionStep()
{ }


//===========================================================================
//               class CFunctionStep :: Set_Parameters
//===========================================================================
void CFunctionStep :: Set_Parameters(double *param, int num)
{
	if(num>=3) {
		k0 = param[0]; // tzero
		k1 = param[1]; // thum
		kf = param[2]; // tfinal
	}
}


//===========================================================================
//               class CFunctionStep :: Set_Parameters
//===========================================================================
void CFunctionStep :: Set_Parameters(std::vector<double> &param )
{
	if(param.size()>=3) {
		k0 = param[0]; // tzero
		k1 = param[1]; // thum
		kf = param[2]; // tfinal
	}
}


//===========================================================================
//               class CFunctionStep :: Get_Value
//===========================================================================
double CFunctionStep :: Get_Value(double k)  //k em segundos ou em metros!!!!!!
{
	double val = 0.0;

	if(k <= k0)
		val = 0.0;
	else if(k>k0 && k<k1) {
		if(k1)
			val = value*k/k1;
        else {
        	std::cout << "\n valor de k1 na definicao da funcao degrau igual a 0";
        	val = 0.0;
        }
	} else if(k >= k1)
		val = value;
	return val;
}


//***************************************************************************
//***************************************************************************
//
//                    C L A S S     C F U N C T I O N  S E N O I D A L
//
//***************************************************************************
//***************************************************************************
CFunctionSinusoidal :: CFunctionSinusoidal()
{
	w     = 0.0;
	alpha = 0.0;
	TYPE = SINUSOIDALFUNCTION;
}


CFunctionSinusoidal :: ~CFunctionSinusoidal()
{ }


//===========================================================================
//               class CFunctionSinusoidal :: Set_Parameters
//===========================================================================
void CFunctionSinusoidal :: Set_Parameters(double *param,int num)
{
	if(num >=2) {
		w     = param[0];//w1;
		alpha = param[1];//alpha1;
	}
}


//===========================================================================
//               class CFunctionSinusoidal :: Set_Parameters
//===========================================================================
void CFunctionSinusoidal :: Set_Parameters(std::vector<double> &param )
{
	if(param.size() >=2) {
		w     = param[0];//w1;
		alpha = param[1];//alpha1;
	}
}


//===========================================================================
//               class CFunctionSinusoidal :: Get_Value
//===========================================================================
double CFunctionSinusoidal :: Get_Value(double k)  //k em segundos ou em metros!!!!!!
{
	double val;
	double coswt=0.0;

	coswt = cos((w*k)+alpha);
	val = value*coswt;
	return val;
}


//===========================================================================
//               class CSource :: Get_Parameter - sobrecarga
//===========================================================================
// Descricao:
// retorna valor do parametro solicitado
//-----------------------------------------------------------------------------
double	CFunctionSinusoidal :: Get_Parameter(FUNCTIONPARAMETERS par)//*NMA*:24/02/2007 inclusao para uso em magnetodinamico
{
	double param_value = 0;
	switch(par) {
		case AMPLITUDEFUNCTIONPARAMETER:
			param_value = value;
			break;
		case FREQUENCYFUNCTIONPARAMETER:
			param_value = w;
			break;
		case SHIFTFUNCTIONPARAMETER:
			param_value = alpha;
			break;
		default:
			param_value = 0;
	}
	return param_value;
}


//***************************************************************************
//***************************************************************************
//
//                    C L A S S    C F U N C T I O N  T R A P E Z O I D A L
//
//***************************************************************************
//***************************************************************************
CFunctionTrapezoidal :: CFunctionTrapezoidal()
{
	k0 = 0.0;
	k1 = 0.0;
	k2 = 0.0;
	kf = 0.0;
	TYPE = TRAPEZOIDALFUNCTION;
}


CFunctionTrapezoidal :: ~CFunctionTrapezoidal()
{ }


//===========================================================================
//               class CFunctionTrapezoidal :: Set_Parameters
//===========================================================================
void CFunctionTrapezoidal :: Set_Parameters(double *param, int num)
{
	if(num>=4) {
		k0 = param[0];//tzero;
		k1 = param[1];//thum;
		k2 = param[2];//tdois;
		kf = param[3];//tfinal;
	}
}


//===========================================================================
//               class CFunctionTrapezoidal :: Set_Parameters
//===========================================================================
void CFunctionTrapezoidal :: Set_Parameters(std::vector<double> &param )
{
	if(param.size()>=4) {
		k0 = param[0];//tzero;
		k1 = param[1];//thum;
		k2 = param[2];//tdois;
		kf = param[3];//tfinal;
	}
}


//===========================================================================
//               class CFunctionTrapezoidal :: Get_Value
//===========================================================================
double CFunctionTrapezoidal :: Get_Value(double k)  //tempoT em segundos ou em metros!!!!!!
{
	double val = 0.0;
	if(k <= k0)
		val = 0.0;
	else if(k>k0 && k<k1) {
		if(k1)
			val = value*k/k1;
        else {
        	std::cout << "\n valor de k1 na definicao da funcao trapezoidal igual a 0";
        	val = 0.0;
        }
	} else if(k >= k1 && k <= k2)
		val = value;
	else if(k>k2 && k<kf)
		val = ((value*k)-(value*kf))/(k2-kf);
	else if(k>kf)
		val = 0.0;
	return val;
}


//***************************************************************************
//
//         C L A S S     C F U N C T I O N  E X P O N E N C I A L
//
//***************************************************************************
CFunctionExponential :: CFunctionExponential()
{
	k0  = 0.0;
	k1  = 0.0;
	kf  = 0.0;
	T   = 0.0;
	cex = 0.0;
	TYPE = EXPONENTIALFUNCTION;
}


CFunctionExponential :: ~CFunctionExponential()
{ }


//===========================================================================
//               class CFunctionExponential :: Set_Parameters
//===========================================================================
void CFunctionExponential :: Set_Parameters(double *param, int num)
{
	if(num>=6) {
		k0 = param[0];//tzero;
		k1 = param[1];//thum;
		kf = param[2];//tfinal;
		vcte = param[3];//vcte;
		T   = param[4];//constTempo;
		cex = param[5];//coeficiente multiplicativo do argumento da exponencial
	}
}


//===========================================================================
//               class CFunctionExponential :: Set_Parameters
//===========================================================================
void CFunctionExponential :: Set_Parameters(std::vector<double> &param )
{
	if(param.size()>=6) {
		k0 = param[0];//tzero;
		k1 = param[1];//thum;
		kf = param[2];//tfinal;
		vcte = param[3];//vcte;
		T   = param[4];//constTempo;
		cex = param[5];//coeficiente multiplicativo do argumento da exponencial
	}
}


//===========================================================================
//               class CFunctionExponential :: Get_Value
//===========================================================================
double CFunctionExponential :: Get_Value(double k)  //k em segundos ou em metros!!!!!!
{
	double val = 0.0;
	if(k <= k0)
		val = 0.0;
	else if(k>k0 && k<k1) {
		if(k1)
			val = (vcte+value)*k/k1;
        else {
        	std::cout << "\n valor de k1 na definicao da funcao exponencial igual a 0\n";
        	val = 0.0;
        }
	} else if(k >= k1)
		val = vcte + ( value*exp(-(cex*k)/T));
	return val;
}


//***************************************************************************
//
//  C L A S S     C F U N C T I O N  E X P O N E N C I A L S P A T I A L
//
//***************************************************************************
CFunctionExponentialShift :: CFunctionExponentialShift()
{
	k0  = 0.0;
	k1  = 0.0;
	kf  = 0.0;
	T   = 0.0;
	cex = 0.0;
	TYPE = EXPONENTIALSHIFTFUNCTION;
}


CFunctionExponentialShift :: ~CFunctionExponentialShift()
{ }


//===========================================================================
//               class CFunctionExponentialShift :: Get_Value
// ATENCAO:
//	x: posicao no eixo de coordenadas GLOBAL
//===========================================================================
double CFunctionExponentialShift :: Get_Value(double x)  //em metros!!!!!!
{
	double val = 0.0;
	double k;
	double k1local, kflocal, k0local;

	//calcula posicao no eixo de coordenadas local
	k0local = 0.0;
	k1local = k1 - k0;
	kflocal = kf - k0;
	k = (x - k0);
	if(kf < k0)
		k = -k;
	if(k < k0local)
		val = 0.0;
	else if(k>k0local && k<k1local) {
		if(k1local)
			val = (vcte+value)*k/k1local;
        else
         val = 0.0;
	} else if(k >= k1local)
		val = vcte + ( value*exp(-(cex*k)/T));
	return val;
}


//***************************************************************************
//
//                    C L A S S     C F U N C T I O N P U L S O
//
//***************************************************************************
CFunctionPulse :: CFunctionPulse()
{
	k0 = 0.0;
	kf = 0.0;
	pulseWidth = 0;
	pulseInterval = 0;
	TYPE = PULSEFUNCTION;
}


CFunctionPulse :: ~CFunctionPulse()
{ }


//===========================================================================
//               class CFunctionPulse :: Set_Parameters
//===========================================================================
void CFunctionPulse :: Set_Parameters(double *param, int num)
{
	if(num>=4) {
		k0					= param[0];//tzero;
		kf					= param[1];//tfinal;
		pulseWidth			= param[2];//largura;
		pulseInterval		= param[3];//intervalo;
	}
}


//===========================================================================
//               class CFunctionPulse :: Set_Parameters
//===========================================================================
void CFunctionPulse :: Set_Parameters(std::vector<double> &param )
{
	if(param.size()>=4) {
		k0					= param[0];//tzero;
		kf					= param[1];//tfinal;
		pulseWidth			= param[2];//largura;
		pulseInterval		= param[3];//intervalo;
	}
}


//===========================================================================
//               class CFunctionPulse :: Get_Value
//===========================================================================
double CFunctionPulse :: Get_Value(double k)  //k em segundos ou em metros!!!!!!
{
	double		val;
	double		n;
	double		krelativo;

	val	= 0.0;

	if(k < k0 || k>kf)
		val= 0.0;
	else {
		n = 0.;
		if(k >= pulseInterval)
			n = (double) floor(k/pulseInterval);
		krelativo = k - (n*pulseInterval);

		if(krelativo <= pulseWidth)
			val = value;
		else
			val = 0.0;
	}
	return val;
}


//***************************************************************************
//
//                    C L A S S     C F U N C T I O N C O N S T A N T
//
//***************************************************************************
CFunctionConstant :: CFunctionConstant()
{
	TYPE = CTEFUNCTION;
}


CFunctionConstant :: ~CFunctionConstant()
{ }


//===========================================================================
//               class CFunctionConstante :: Set_Parameters
//===========================================================================
void CFunctionConstant :: Set_Parameters(double *param, int num)
{
	//o valor e atribuido com Set_Value
}


//===========================================================================
//               class CFunctionConstante :: Set_Parameters
//===========================================================================
void CFunctionConstant :: Set_Parameters(std::vector<double> &param )
{
	//o valor e atribuido com Set_Value
}


//===========================================================================
//               class CFunctionConstante :: Get_Value
//===========================================================================
double CFunctionConstant :: Get_Value(double k)
{
	return value;
}


