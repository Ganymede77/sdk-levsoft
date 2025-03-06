#ifndef __COMPOSEDFUNCTIONS__
#define __COMPOSEDFUNCTIONS__


#include "functions.hpp"
#include <vector>

//------------------------------------------------------------------------------------------
//class CComposedFunction
//------------------------------------------------------------------------------------------

class CComposedFunction
{
protected:

	std::vector < CFunction * > functions;
	int			num_functions;

public:
             		CComposedFunction();
         virtual   ~CComposedFunction();

  virtual void		Associate(CFunction *);
  virtual double	Get_Value(double) ;
};


#endif
