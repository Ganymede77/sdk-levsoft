#ifndef __LEV_INPUT_EXCEPTION__
#define __LEV_INPUT_EXCEPTION__

#include <iostream>
#include <string>


using namespace std;
/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Arquivo: lev_error_exception.hpp                                          */
/*---------------------------------------------------------------------------*/
/* Conte�do: CError_Exception                                                */
/*---------------------------------------------------------------------------*/
/* Descrição: Classe para tratamento de erro genérico					     */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* string para verificacao de erros ou modificacoes futuras ou observa�o�es: */
/*                                                                           */
/*                         *NMA*     *AP*			                          */
/*                                                                           */
/****************************************************************************/

class CError_Exception
{
  private:
    char msg[1000];

  protected:

  public:
    CError_Exception(void) {};
    CError_Exception(char* text) { strcpy(msg, text); };
    ~CError_Exception() {};

    friend ostream & operator << (ostream & out, CError_Exception & ex)
	{
      out << endl << "Exception error: " << ex.msg << endl;
      return out;
    }
};

#endif
