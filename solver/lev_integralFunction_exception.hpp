#ifndef __LEV_INTEGRALFUNCTION_EXCEPTION__
#define __LEV_INTEGRALFUNCTION_EXCEPTION__

#include <iostream>
#include <string>



/*********************************************************************************************/
/* Projeto: LEVSoft                                                                          */
/*********************************************************************************************/
/* Arquivo: lev_IntegralFunction_exception.hpp                                               */
/*-------------------------------------------------------------------------------------------*/
/* Conte�do: CIntegralFunction_Exception                                                     */
/*-------------------------------------------------------------------------------------------*/
/* Descri��o: Classe para tratamento de erro da fun��o de integral da classe phenomenon      */
/*-------------------------------------------------------------------------------------------*/
/*                                                                                           */
/* string para verificacao de erros ou modificacoes futuras ou observa�o�es:                 */
/*                                                                                           */
/*                         *NMA*     *AP*     *LAA*                                          */
/*                                                                                           */
/*********************************************************************************************/

namespace lev_math {
class CIntegralFunction_Exception
{
  private:
    std::string  msg;

  protected:

  public:
    CIntegralFunction_Exception(void) {};
    CIntegralFunction_Exception(std::string text) { msg = text; };
    ~CIntegralFunction_Exception() {};

    friend std::ostream & operator << (std::ostream & out, CIntegralFunction_Exception & ex)
	{
		out << std::endl << "Erro da funcao de integracao da classe Phenomenon: " << ex.msg << std::endl;
      return out;
    }
};

} // namespace lev_math

#endif

