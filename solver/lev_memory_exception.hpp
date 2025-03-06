#ifndef __LEV_MEMORY_EXCEPTION__
#define __LEV_MEMORY_EXCEPTION__

#include <iostream>
#include <string>

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/* Arquivo: lev_memory_exception.hpp                                         */
/*---------------------------------------------------------------------------*/
/* Conteúdo: CMemory_Exception                                               */
/*---------------------------------------------------------------------------*/
/* Descrição: Classe para tratamento de erro alocação de memória(solver)     */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* string para verificacao de erros ou modificacoes futuras ou observaçoões: */
/*                                                                           */
/*                         *NMA*     *AP*     *LAA*                          */
/*                                                                           */
/****************************************************************************/

namespace lev_math {

class CMemory_Exception;
 std::ostream & operator << (std::ostream & out, CMemory_Exception & ex);


class CMemory_Exception
{
  private:
    std::string msg;

  protected:

  public:
    CMemory_Exception(void) {};
    CMemory_Exception(const std::string & text) { msg=text; };
    ~CMemory_Exception() {};

    //friend std::ostream & lev_math::operator << (std::ostream & out, CMemory_Exception & ex) {
    friend std::ostream & operator << (std::ostream & out, CMemory_Exception & ex) {
      out << std::endl << "Memory allocation fault: " << ex << std::endl;
      return out;
    }
};

} //end lev_math namespace
#endif
