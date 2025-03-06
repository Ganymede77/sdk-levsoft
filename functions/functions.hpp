#ifndef __FUNCTIONS__
#define __FUNCTIONS__


#include <vector>
#include "levenum.h"


//------------------------------------------------------------------------------------------
//class CFunction
//------------------------------------------------------------------------------------------
class CFunction
{
  protected:
    double   value;
    FUNCTIONTYPE TYPE;

  public:
    CFunction();
    virtual ~CFunction();

    virtual void   Set_Value(double);
    virtual void   Set_Parameters(double *,int )=0;
    virtual void   Set_Parameters(std::vector<double> &)=0;
    virtual double Get_Value(double)=0 ;
    virtual double Get_Parameter(enum FUNCTIONPARAMETERS PAR){return 0;}//*NMA*:24/02/2007: implementa��o vazia para evitar problemas com as derivadas

    virtual FUNCTIONTYPE Get_Type(void){return TYPE;}

    CFunction & operator = (const CFunction &f)
    {
	    value = f.value;
	    return *this;
    }
};


//------------------------------------------------------------------------------------------
//class CFunctionStep
//------------------------------------------------------------------------------------------
class CFunctionStep : public CFunction
{
  protected:
      double k0;
      double k1;
      double kf;

  public:

                      CFunctionStep();
            virtual  ~CFunctionStep();


   virtual void   Set_Parameters(double *, int);
   virtual void   Set_Parameters(std::vector<double> &);
   virtual double Get_Value(double);

   CFunctionStep & operator = (const CFunctionStep &f)
   {
	  value = f.value;
	  k0	= f.k0;
	  k1	= f.k1;
	  kf	= f.kf;
	  return *this;
   }
};


//------------------------------------------------------------------------------------------
//class CFunctionSinusoidal
//------------------------------------------------------------------------------------------
class CFunctionSinusoidal : public CFunction
{
  protected:
      double  w;
      double  alpha;

  public:
                     CFunctionSinusoidal();
            virtual  ~CFunctionSinusoidal();

	virtual void   Set_Parameters(double *, int);
	virtual void   Set_Parameters(std::vector<double> &);
	virtual double Get_Value(double);
	virtual double Get_Parameter(FUNCTIONPARAMETERS par);//*NMA*:24/02/2007 inclus�o para uso em magnetodin�mico

   CFunctionSinusoidal & operator = (const CFunctionSinusoidal &f)
   {
	  value = f.value;
	  w		= f.w;
	  alpha = f.alpha;
	  return *this;
   }
};


//------------------------------------------------------------------------------------------
//class CFunctionTrapezoidal
//------------------------------------------------------------------------------------------
class CFunctionTrapezoidal : public CFunction
{
  protected:
      double k0;
      double k1;
      double k2;
      double kf;

  public:
                   CFunctionTrapezoidal();
         virtual   ~CFunctionTrapezoidal();

   virtual void   Set_Parameters(double *, int);
   virtual void   Set_Parameters(std::vector<double> &);
   virtual double Get_Value(double);

   CFunctionTrapezoidal & operator = (const CFunctionTrapezoidal &f)
   {
	  value	= f.value;
	  k0	= f.k0;
	  k1	= f.k1;
	  k2	= f.k2;
	  kf	= f.kf;
	  return *this;
   }
};


//------------------------------------------------------------------------------------------
//class CFunctionExponential
//------------------------------------------------------------------------------------------
class CFunctionExponential : public CFunction
{
  protected:
      double k0;
      double k1;
      double kf;
      double vcte; // valor constante somado a exponencial
      double T;   //constante de tempo
	  double cex; //coeficiente multiplicativo no argumento da exponencial

  public:
              CFunctionExponential();
     virtual  ~CFunctionExponential();

   virtual void   Set_Parameters(double *, int);
   virtual void   Set_Parameters(std::vector<double> &);
   virtual double Get_Value(double);

   CFunctionExponential & operator = (const CFunctionExponential &f)
   {
	  value = f.value;
	  k0	= f.k0;
	  k1	= f.k1;
	  kf	= f.kf;
	  vcte	= f.vcte;
	  T		= f.T;
	  cex	= f.cex;
	  return *this;
   }
};


//------------------------------------------------------------------------------------------
//class CFunctionExponentialShift
//------------------------------------------------------------------------------------------
class CFunctionExponentialShift : public CFunctionExponential
{
  protected:

  public:
	  CFunctionExponentialShift();
      virtual  ~CFunctionExponentialShift();

   virtual double Get_Value(double);

   CFunctionExponentialShift & operator = (const CFunctionExponentialShift &f)
   {
	  value = f.value;
	  k0	= f.k0;
	  k1	= f.k1;
	  kf	= f.kf;
	  vcte	= f.vcte;
	  T		= f.T;
	  cex	= f.cex;
	  return *this;
   }
};


//------------------------------------------------------------------------------------------
//class CFunctionPulse
//------------------------------------------------------------------------------------------
class CFunctionPulse : public CFunction
{
  protected:
      double k0;
      double pulseWidth;
	  double pulseInterval;
      double kf;

  public:

	  				CFunctionPulse();
       virtual      ~CFunctionPulse();


   virtual void   Set_Parameters(double *, int);
   virtual void   Set_Parameters(std::vector<double> &);
   virtual double Get_Value(double);

   CFunctionPulse & operator = (const CFunctionPulse &f)
   {
	  value				= f.value;
	  k0				= f.k0;
	  pulseWidth		= f.pulseWidth;
	  pulseInterval		= f.pulseInterval;
	  kf				= f.kf;
	  return *this;
   }
};


//------------------------------------------------------------------------------------------
//class CFunctionConstante
//------------------------------------------------------------------------------------------
class CFunctionConstant : public CFunction
{
  public:

	  CFunctionConstant();
       virtual      ~CFunctionConstant();


   virtual void   Set_Parameters(double *, int);
   virtual void   Set_Parameters(std::vector<double> &);
   virtual double Get_Value(double k=0);

   CFunctionConstant & operator = (const CFunctionConstant &f)
   {
	  value = f.value;
	  return *this;
   }
};


#endif
