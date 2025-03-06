#ifndef __LEV_NONSTATIONARY_ITERATIVE_METHOD_BICG__
#define __LEV_NONSTATIONARY_ITERATIVE_METHOD_BICG__

/**
*****************************************************************************
* Projeto: LEVSoft                                                          *
*****************************************************************************
* Arquivo: lev_non_sta_ite_met_bicg.hpp                                     *
*---------------------------------------------------------------------------*
* Conteúdo: template <class M,class Q>  CNon_Sta_Ite_Method_BICG                    *
*---------------------------------------------------------------------------*
* Descrição: Implementação do método Gradientes Bi-Conjugados (BiCG)        *
*             para resolução de sistemas de equação lineares               *
*---------------------------------------------------------------------------*
*                                                                           *
* string para verificacao de erros ou modificacoes futuras ou observações: *
*                                                                           *
*                         *NMA*     *AP*     *LAA*                          *
*                                                                           *
*****************************************************************************
*/
#include "lev_input_exception.hpp"
#include "lev_memory_exception.hpp"
#include "lev_non_sta_ite_met.hpp"
namespace lev_math {

class CInput_Exception;


template <class M,class Q>
class CNon_Sta_Ite_Method_BICG : public CNon_Sta_Ite_Method<M,Q>
{
  private:


   typedef typename number_traits<Q>::number_type REALTYPE;

    Ve_Templ<Q, long> r;
    Ve_Templ<Q, long> z;
    Ve_Templ<Q, long> p;
    Ve_Templ<Q, long> q;
    Ve_Templ<Q, long> rt;
    Ve_Templ<Q, long> zt;
    Ve_Templ<Q, long> pt;
    Ve_Templ<Q, long> qt;
    Ve_Templ<Q, long> solucao ;

  protected:

    int validate_matrix(M &);

  public:

    CNon_Sta_Ite_Method_BICG(void);
    ~CNon_Sta_Ite_Method_BICG();

    Ve_Templ<Q, long> solve(M &, Ve_Templ<Q, long> &);
    Ve_Templ<Q, long> solve(M &, Ve_Templ<Q, long> &, const std::string &);
};


/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
* Nome da Funcao:        CNon_Sta_Ite_Method_BICG                          *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    13/10/2003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
* Descricao:                                                               *
*                                                                          *
*            construtor                                                    *
*                                                                          *
*                                                                          *
****************************************************************************
*/
template <class M,class Q>
CNon_Sta_Ite_Method_BICG<M,Q>::CNon_Sta_Ite_Method_BICG(void)
{
}

/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        ~CNon_Sta_Ite_Method_BICG                         *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
****************************************************************************
*                                                                          *
* Descricao:                                                               *
*                                                                          *
*            destrutor                                                     *
*                                                                          *
*                                                                          *
****************************************************************************
*/
template <class M,class Q>
CNon_Sta_Ite_Method_BICG<M,Q>::~CNon_Sta_Ite_Method_BICG()
{
}

/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        validate_matrix                                   *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
****************************************************************************
*                                                                          *
* Descricao:                                                               *
*                                                                          *
*            Função implementada para manter a interface das classes de    *
*    resolução.                                                            *
*                                                                          *
****************************************************************************
*/
template <class M,class Q>
int CNon_Sta_Ite_Method_BICG<M,Q>::validate_matrix(M &A)
{
  return 1;
}

/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        solve                                             *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
* Descricao:    *NMA*                                                      *
*                                                                          *
*    Resolução do sistema pelo método BiCG sem pré-condicionamento         *
*                                                                          *
* Parametros:                                                              *
*             M: matriz esparsa                                            *
*             Ve_Templ<Q, long> b: vetor das ações (righ hand side)    *
*                                                                          *
* Valor de retorno:                                                        *
*             Ve_Templ<Q, long> : vetor com a solução do sistema       *
*                                                                          *
****************************************************************************
*/
template <class M,class Q>
Ve_Templ<Q, long> CNon_Sta_Ite_Method_BICG<M,Q>::solve(M &A, Ve_Templ<Q, long> &b)
{
  std::string null_preconditioner("CInc_Fac_Preconditioner_Null");

  Ve_Templ<Q, long> result;

  result = solve(A, b, null_preconditioner);

  return result;

}

/**
****************************************************************************
* Projeto: LEVSoft                                                         *
****************************************************************************
*                                                                          *
* Nome da Funcao:        solve  (sobrecarga)                               *
* Versao:                1.0                                               *
* Compilador:                                                              *
* Programador:           Nancy Mieko Abe                                   *
*                        Angelo Passaro                                    *
*                        Leandro Augusto de Almeida                        *
* Ultima modificacao:    12/10/2003                                        *
* Modificacao:                                                             *
*                                                                          *
****************************************************************************
* Descricao:    *NMA*                                                      *
*                                                                          *
*    Resoluçãoo do sistema pelo método BiCG com pré-condicionamento        *
*                                                                          *
* Parametros:                                                              *
*             M: matriz esparsa                                            *
*             Ve_Templ<Q, long> b: vetor das ações (righ hand side)       *
*             const std::string &preconditioner_name : tipo de pré-condicionador *
*                                                                          *
* Valor de retorno:                                                        *
*             Ve_Templ<Q, long> : vetor com a solução do sistema          *
*                                                                          *
****************************************************************************
*/
template <class M,class Q>
Ve_Templ<Q, long> CNon_Sta_Ite_Method_BICG<M,Q>::solve(M &A, Ve_Templ<Q, long> &b, const std::string &preconditioner_name)
{
  long i, tamanho;
  REALTYPE zero = 0.0;

  CInc_Fac_Preconditioner<M,Q> &P = *(this->get_preconditioner_instance(preconditioner_name));

  this->set_iteractions(5000);
  this->set_precision(1e-12);

  if (A.prim.Get_Total_Componentes() != b.Get_Total_Componentes()) {
    throw CInput_Exception();
  }

  tamanho = A.prim.Get_Total_Componentes() - 1;

  r.Set_N_Max_Com_Abs(tamanho + 1);
  z.Set_N_Max_Com_Abs(tamanho + 1);
  p.Set_N_Max_Com_Abs(tamanho + 1);
  q.Set_N_Max_Com_Abs(tamanho + 1);
  rt.Set_N_Max_Com_Abs(tamanho + 1);
  zt.Set_N_Max_Com_Abs(tamanho + 1);
  pt.Set_N_Max_Com_Abs(tamanho + 1);
  qt.Set_N_Max_Com_Abs(tamanho + 1);
  solucao.Set_N_Max_Com_Abs(tamanho + 1);

  for(i = 0; i <= tamanho; i++) {
    if (r.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (z.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (p.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (q.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (rt.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (zt.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (pt.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (qt.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (solucao.Inclui(zero)) {
      throw CMemory_Exception();
    }
  }

  //A.Format(SYMMETRIC_SUPERIOR);
	A.Orientation(LINE);

  if (validate_matrix(A) == 0) {
    throw CInput_Exception("Matriz inválida para este solver.");
  }

	r.Zera();
	z.Zera();
	p.Zera();
	q.Zera();
	rt.Zera();
	zt.Zera();
	pt.Zera();
	qt.Zera();
	solucao.Zera();


  REALTYPE Q ro0, ro1, af, bt;

 // std::cout << ".";

  r = b;
  rt = b;

  this->itc = 0;

  P.build(A);

#ifdef __SHOWCONVVALUES__
 std::cout << "TESTING BICG SOLVER" << std::endl;
 std::cout.setf(std::ios::left,std::ios::adjustfield);
 std::cout<<std::setw(15)<<"ITERATION" <<std::setw(15)<< "PRECISION" <<std::setw(15) << "NORMA" << std::endl;
 std::cout << std::endl;
#endif

  do {

    this->itc++;

    P.apply(r, z);
    P.apply(rt, zt);

    ro0 = z * rt; //produto interno
    this->precisao = ro0; //precis�o atual

	if(this->itc ==1)
		if(this->precisao < this->get_precision())
			this->set_precision(this->precisao*1e-10);


    if (ro0 == 0) {
      return solucao; //retorna nulo em caso de erro, verificar depois *LAA*
    }

    if (this->itc == 1) {
      p = z;
      pt = zt;
    } else {
      bt = ro0/ro1; //ro0 - indica o ro atual e ro1 - indica defasado de 1 iteracao
      p = z + (bt * p);
      pt = zt + (bt * pt);
    }

    q = A * p;
    qt = A.Transpose() * pt;

    af = ro0 / (pt * q);

    solucao = solucao + af * p;

    r = r - af * q;
    rt = rt - af * qt;

    ro1 = ro0;



#ifdef __SHOWCONVVALUES__
  std::cout.setf(std::ios::left,std::ios::adjustfield);
   //std::cout.width(10);
  std::cout<<std::setw(15)<<this->itc <<std::setw(15)<< this->precisao <<std::setw(15) << std::setfill(' ') << std::endl;
#endif




  } while ((this->itc <= this->get_iteractions()) && (this->get_precision() < this->precisao));


  return solucao;
}

} //end  lev_math namespace
#endif

