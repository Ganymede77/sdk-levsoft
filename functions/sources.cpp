#include <iostream>
#include <cmath>
#include <cstdio>
#include "sources.hpp"

//***************************************************************************
// Projeto: LEVSOFT
//***************************************************************************
//
// Nome: sources.hpp
//
// Versao				:		1
// Compilador			:		Visual C++ 6.0
// Programador			:		Nancy Mieko Abe
// Data cria��o			:		01/12/2003
// Ultima modificacao	:
//
//***************************************************************************
//
// Descricao:
//
//	Implementa��o de classes de fontes vari�veis no tempo e/ou no espa�o.
//	A classe � composta por quatro objetos da classe de fun��es vari�veis
//	(contante, degrau, senoidal, exponencial, pulso, ...):
//	- varia��o no tempo
//	- varia��o em x;
//	- varia��o em y;
//	- varia��o em z;
//
//	- O metodo Get_Value(double t, double x, double y, double z)
//	  retorna o produto das quatro varia��es. Caso uma delas n�o exista,
//	  o valor assumido � um. A amplitude da fonte deve ser inserida somente
//	  em uma das fun��es
//
//  em 23/11/2005
//	- inclu�da associa��o com fun��o constante, ou seja, um valor contante
//		para fonte. Possibilita a economia de tempo nos casos est�ticos e nos
//		estudos com evolu��o no tempo, cuja fonte atribu�da � constante no tempo e no
//		espa�o.
//	- a associa��o a uma fonte constante tem preced�ncia em rela��o aas outras
//		associa��es, ou seja, se a uma fun��o constante for associada,
//		� esta que ser� considerada
//
//***************************************************************************
//***************************************************************************
//***************************************************************************
//
//                    C L A S S    C S O U R C E
//
//***************************************************************************
//***************************************************************************

CSource :: CSource()
{
	timeFunction	= NULL;
	xFunction		= NULL;
	yFunction		= NULL;
	zFunction		= NULL;
	constantFunction= NULL;

}//fim do construtor de CFunction
//___________________________________________________________________________

CSource :: ~CSource()
{
	constantFunction= NULL;
	timeFunction	= NULL;
	xFunction		= NULL;
	yFunction		= NULL;
	zFunction		= NULL;

} // fim do destruidor de CFunction
//___________________________________________________________________________

//===========================================================================
//               class CSource :: AssociateConstantFunction
//===========================================================================
void CSource :: AssociateConstantFunction(CFunctionConstant *f)
{
	constantFunction = f;

}//fim de AssociateTimeFunction

//===========================================================================
//               class CSource :: AssociateTimeFunction
//===========================================================================
void CSource :: AssociateTimeFunction(CComposedFunction *f)
{
	timeFunction = f;

}//fim de AssociateTimeFunction

//===========================================================================
//               class CSource :: AssociateXFunction
//===========================================================================
void CSource :: AssociateXFunction(CComposedFunction * f)
{
	xFunction = f;

}//fim de AssociateXFunction

//===========================================================================
//               class CSource :: AssociateYFunction
//===========================================================================
void CSource :: AssociateYFunction(CComposedFunction * f)
{
	yFunction = f;

}//fim de AssociateYFunction

//===========================================================================
//               class CSource :: AssociateZFunction
//===========================================================================
void CSource :: AssociateZFunction(CComposedFunction * f)
{
	zFunction = f;

}//fim de AssociateZFunction

//===========================================================================
//               class CSource :: Get_ValueT
//===========================================================================
double CSource :: Get_ValueT(double t)
{
	double valor = 0.0;

	if(timeFunction)
	{
		valor = timeFunction->Get_Value(t);
	}

	return valor;

}//fim de Get_ValueT


//===========================================================================
//               class CSource :: Get_ValueX
//===========================================================================
double CSource :: Get_ValueX(double x)
{
	double valor = 0.0;

	if(xFunction)
	{
		valor = xFunction->Get_Value(x);
	}

	return valor;

}//fim de Get_ValueX

//===========================================================================
//               class CSource :: Get_ValueY
//===========================================================================
double CSource :: Get_ValueY(double y)
{
	double valor = 0.0;

	if(yFunction)
	{
		valor = yFunction->Get_Value(y);
	}

	return valor;

}//fim de Get_ValueY

//===========================================================================
//               class CSource :: Get_ValueZ
//===========================================================================
double CSource :: Get_ValueZ(double z)
{
	double valor = 0.0;

	if(zFunction)
	{
		valor = zFunction->Get_Value(z);
	}

	return valor;

}//fim de Get_ValueZ

//===========================================================================
//               class CSource :: Get_Value(void)
//===========================================================================
// Descri��o:
//		Retorna o valor de fonte constante (fun��o constante)
//
//-----------------------------------------------------------------------------
double CSource :: Get_Value(void)
{
	double valor = 0.0;

	if(constantFunction)
	{
		valor = constantFunction->Get_Value();
	}

	return valor;

}

//===========================================================================
//               class CSource :: Set_Value(double)
//===========================================================================
// Descri��o:
//		Atribui o valor de fonte constante (fun��o constante)
//
//-----------------------------------------------------------------------------
void CSource :: Set_Value(double val)
{

	if(constantFunction)
	{
		constantFunction->Set_Value(val);
	}


}



//===========================================================================
//               class CSource :: Get_Value - sobrecarga
//===========================================================================
// Descri��o:
// se constantFunction == NULL (n�o existe fun��o constante associada)
//		Retorna o produto entre as quatro (4) varia��es poss�veis. Caso uma
//	  delas n�o exista, o valor assumido � um. A amplitude da fonte deve ser
//	  inserida somente em uma das fun��es.
// se constantFunction != NULL
//		Retorna o valor da fun��o constante associada. Isto permite uma economia
// de tempo, mesmo nos estudos com evolu��o no tempo, mas com fonte constante aplicada
//-----------------------------------------------------------------------------
double CSource :: Get_Value(double t, double x, double y, double z)
{
	double valor	= 0.0;
	double timeVal	= 1.;
	double xVal		= 1.;
	double yVal		= 1.;
	double zVal		= 1.;

	if(constantFunction)
	{
		valor = constantFunction->Get_Value();
	}
	else
	{
		if(timeFunction)
		{
			timeVal = timeFunction->Get_Value(t);
		}

		if(xFunction)
		{
			xVal = xFunction->Get_Value(x);
		}

		if(yFunction)
		{
			yVal = yFunction->Get_Value(y);
		}

		if(zFunction)
		{
			zVal = zFunction->Get_Value(z);
		}

		valor = timeVal*xVal*yVal*zVal;
	}

	return valor;

}//fim de Get_Value


//*NMA*
/**
Descri��o: \n
	Retorna valor de parametro de fun��o que define uma fonte, desde que a fonte seja
uma fontes simples (n�o composta). \n

\param FUNCTIONTYPE stype: enum tipo de fun��o (definidos no arquivo  levenum.h)
\param FUNCTIONPARAMETER spar: enum tipo de parametro (definidos no arquivo levenum.h)

**/
double	CSource :: Get_Parameter(enum FUNCTIONTYPE stype, enum FUNCTIONPARAMETERS spar)
{
	double param = 0;

	if(singleFunction->Get_Type() == stype)
	{
		param = singleFunction->Get_Parameter(spar);
	}

	return param;
}

//...................... Gravação e Leitura ............................................
int CSource::Save(FILE *parq)
{
	int retorno = 0;
	int tipo;

	tipo = (enum SOURCETYPE) type;

	fprintf(parq,"\n\n%d",tipo);
	fprintf(parq,"\n%lf",value);

	return retorno;
}

int CSource::Read(FILE *parq)
{
	int retorno = 0;
	int tipo;

	fscanf(parq,"\n\n%d",&tipo);
	fscanf(parq,"\n%lf",&value);

	type = (enum SOURCETYPE) tipo;

	return retorno;
}

void CSource::Reinitialize (void)
{
	type			=  INVALIDSOURCE;
	value			=  0;
}

void CSource::Type(SOURCETYPE t)
{
	type = t;
}

SOURCETYPE CSource::Type(void)
{
	return type;
}

void CSource::Value(double v)
{
	value = v;
}

double CSource::Value()
{
	return value;
}

//............ operadores .............................................
bool CSource::operator == (const CSource &a)
{
	bool ret = false;

	if(type == a.type)
		if(value == a.value)
		{
			ret = true;
		}

	return ret;
}

bool CSource::operator != (const CSource &a)
{
	bool ret = true;

	if(type == a.type)
		if(value == a.value)
		{
			ret = false;
		}
	return ret;
}

CSource& CSource::operator = (const CSource &a)
{
	type		= a.type;
	value		= a.value;
	return *this;
}
