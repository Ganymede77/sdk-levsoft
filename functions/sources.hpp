#if !defined  __SOURCES__
#define  __SOURCES__

#include "composedfunctions.hpp"
#include "levenum.h"
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
//	Declara��o de classes de fontes vari�veis no tempo e/ou no espa�o.
//	A classe � composta por quatro objetos da classe de fun��es vari�veis
//	(contante, degrau, senoidal, exponencial, pulso, ...):
//	- varia��o no tempo
//	- varia��o em x;
//	- varia��o em y;
//	- varia��o em z;
//
//
//***************************************************************************
//***************************************************************************
//***************************************************************************
//
//                    C L A S S    C S O U R C E
//
//***************************************************************************
//***************************************************************************

#include "levenum.h"

class CSource
{
  protected:
//	double		amplitude;
	CComposedFunction*	timeFunction;
	CComposedFunction*	xFunction;
	CComposedFunction*	yFunction;
	CComposedFunction*	zFunction;
	CFunctionConstant*	constantFunction;
	CFunction		 *  singleFunction;


	int			n_functions;

	SOURCETYPE				type;
	double					value;

  public:
             CSource();
   virtual  ~CSource();

			void	AssociateTimeFunction(CComposedFunction *);
			void	AssociateXFunction(CComposedFunction *);
			void	AssociateYFunction(CComposedFunction *);
			void	AssociateZFunction(CComposedFunction *);
			void	AssociateConstantFunction(CFunctionConstant *);


	virtual double	Get_Value(void);///fun��o para obter valor constante
	virtual void	Set_Value(double);///fun��o para atriui��o de valor constante

	virtual double	Get_Value(double t, double x, double y, double z );
	virtual double	Get_ValueT(double t);
	virtual double	Get_ValueX(double x);
	virtual double	Get_ValueY(double y);
	virtual double	Get_ValueZ(double z);
	virtual double	Get_Parameter(enum FUNCTIONTYPE stype, enum FUNCTIONPARAMETERS spar);

	int Save(FILE *parq);
	int Read(FILE *parq);

	void Reinitialize (void);

	void		Type(SOURCETYPE t);
	SOURCETYPE	Type(void);

	void		Value(double v);
	double		Value();

	bool operator == (const CSource &a);
	bool operator != (const CSource &a);
	CSource& operator = (const CSource &a);

};
#endif
