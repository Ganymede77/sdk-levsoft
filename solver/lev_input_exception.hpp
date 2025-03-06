#ifndef __LEV_INPUT_EXCEPTION__
#define __LEV_INPUT_EXCEPTION__

#include <iostream>
#include <string>



/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Arquivo: lev_input_exception.hpp                                          */
/*---------------------------------------------------------------------------*/
/* Conte�do: CInput_Exception                                                */
/*---------------------------------------------------------------------------*/
/* Descri��o: Classe para tratamento de erro de dado de entrada (solver)     */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* string para verificacao de erros ou modificacoes futuras ou observa�o�es: */
/*                                                                           */
/*                         *NMA*     *AP*     *LAA*                          */
/*                                                                           */
/****************************************************************************/

namespace lev_math {


class CInput_Exception;
std::ostream & operator << (std::ostream & out, CInput_Exception & ex);



class CInput_Exception
{
  private:
    std::string msg;

  protected:

  public:
    CInput_Exception(void) {};
    CInput_Exception(const std::string &text) { msg=text; };
    ~CInput_Exception() {};

    friend std::ostream & operator << (std::ostream & out, CInput_Exception & ex)
	{
      out << std::endl << " Input Error : " << ex << std::endl;
      return out;
    }
};

} //end namespace levMath

#endif
