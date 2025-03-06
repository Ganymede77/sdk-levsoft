#ifndef __LEV_NONSTATIONARY_ITERATIVE_METHOD_BICGSTAB__
#define __LEV_NONSTATIONARY_ITERATIVE_METHOD_BICGSTAB__


// *************************************************************************************
//  Projeto: LEVSoft                                                                   *
// *************************************************************************************
//  Arquivo: lev_non_sta_ite_met_bicgstab.hpp                                          *
// ------------------------------------------------------------------------------------*
//  Conteudo: template <class M,class T, class Q>  CNon_Sta_Ite_Method_BICGSTAB        *
// ------------------------------------------------------------------------------------*
//  Descricao: Implementacao do metodo Gradientes Bi-Conjugados  estabilizado          *
//              BiCGStab para resolucao de sistemas de equacoes lineares               *
// ------------------------------------------------------------------------------------*
//                                                                                     *
//  string para verificacao de erros ou modificacoes futuras ou observacoes:           *
//                                                                                     *
//                          *NMA*     *AP*     *LAA*                                   *
//                                                                                     *
// *************************************************************************************


#include "lev_memory_exception.hpp"
#include "lev_non_sta_ite_met.hpp"


namespace lev_math {


class CInput_Exception;


template <class M,class T, class Q>
class CNon_Sta_Ite_Method_BICGSTAB : public CNon_Sta_Ite_Method<M,T,Q>
{
private:

    typedef typename number_traits<T>::number_type REALTYPE;

    Ve_Templ<T, Q> r;
    Ve_Templ<T, Q> p;
    Ve_Templ<T, Q> s;
    Ve_Templ<T, Q> rt;
    Ve_Templ<T, Q> pt;
    Ve_Templ<T, Q> st;
    Ve_Templ<T, Q> v;
    Ve_Templ<T, Q> t;
    Ve_Templ<T, Q> solucao ;

protected:

    Q validate_matrix(M &);

public:

    CNon_Sta_Ite_Method_BICGSTAB(void);
    ~CNon_Sta_Ite_Method_BICGSTAB();

    Ve_Templ<T, Q> solve(M &, Ve_Templ<T, Q> &);
    Ve_Templ<T, Q> solve(M &, Ve_Templ<T, Q> &, const std::string &);
};


/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome do Arquivo:       LEV_pre_ilu.cpp                                   */
/* Nome da Funcao:        CNon_Sta_Ite_Method_BICGSTAB                      */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    13/10/2003                                        */
/* Modificacao:                                                             */
/****************************************************************************/
/* Descricao:                                                               */
/*                                                                          */
/*            construtor                                                    */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
template <class M, class T,class Q>
CNon_Sta_Ite_Method_BICGSTAB<M,T,Q>::CNon_Sta_Ite_Method_BICGSTAB(void)
{ }


/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        ~CNon_Sta_Ite_Method_BICGSTAB                     */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/****************************************************************************/
/*                                                                          */
/* Descricao:                                                               */
/*                                                                          */
/*            destrutor                                                     */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
CNon_Sta_Ite_Method_BICGSTAB<M,T, Q>::~CNon_Sta_Ite_Method_BICGSTAB()
{ }


/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        validate_matrix                                   */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/****************************************************************************/
/*                                                                          */
/* Descricao:                                                               */
/*                                                                          */
/*            Fun��o implementada para manter a interface das classes de    */
/*    resolu��o.                                                            */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
Q CNon_Sta_Ite_Method_BICGSTAB<M,T,Q>::validate_matrix(M &A)
{
  return 1;
}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        solve                                             */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/****************************************************************************/
/* Descricao:    *NMA*                                                      */
/*                                                                          */
/*    Resolusão do sistema pelo metodo BiCGSTAB sem pré-condicionamento     */
/*                                                                          */
/* Parametros:                                                              */
/*             M: matriz esparsa                                            */
/*             Q b: vetor das ações (righ hand side)    */
/*                                                                          */
/* Valor de retorno:                                                        */
/*             Q : vetor com a solução do sistema       */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
Ve_Templ<T, Q> CNon_Sta_Ite_Method_BICGSTAB<M,T, Q>::solve(M &A, Ve_Templ<T, Q> &b)
{
  std::string null_preconditioner("CInc_Fac_Preconditioner_Null");

  Ve_Templ<T, Q> result;

  result = solve(A, b, null_preconditioner);

  return result;

}

/****************************************************************************/
/* Projeto: LEVSoft                                                         */
/****************************************************************************/
/*                                                                          */
/* Nome da Funcao:        solve  (sobrecarga)                               */
/* Versao:                1.0                                               */
/* Compilador:                                                              */
/* Programador:           Nancy Mieko Abe                                   */
/*                        Angelo Passaro                                    */
/*                        Leandro Augusto de Almeida                        */
/* Ultima modificacao:    12/10/2003                                        */
/* Modificacao:                                                             */
/*                                                                          */
/****************************************************************************/
/* Descricao:    *NMA*                                                      */
/*                                                                          */
/*    Resolução do sistema pelo metodo BiCGSTAB com pré-condicionamento     */
/*                                                                          */
/* Parametros:                                                              */
/*             M: matriz esparsa                                            */
/*             Q b: vetor das ações (righ hand side)    */
/*             const std::string &preconditioner_name : tipo de pré-condicionador */
/*                                                                          */
/* Valor de retorno:                                                        */
/*             Q : vetor com a solução do sistema       */
/*                                                                          */
/****************************************************************************/
template <class M,class T, class Q>
Ve_Templ<T, Q> CNon_Sta_Ite_Method_BICGSTAB<M,T, Q>::solve(M &A, Ve_Templ<T, Q> &b, const std::string &preconditioner_name)
{
  long i, tamanho;
  REALTYPE  zero = 0.0;

  CInc_Fac_Preconditioner<M,T, Q> &P = *(this->get_preconditioner_instance(preconditioner_name));

  this->set_iteractions(5000);
  this->set_precision(1e-12);

	if (A.prim.Get_Total_Componentes() != b.Get_Total_Componentes()) {
    throw CInput_Exception();
  }

  tamanho = A.prim.Get_Total_Componentes() - 1;

  r.Set_N_Max_Com_Abs(tamanho + 1);
  p.Set_N_Max_Com_Abs(tamanho + 1);
  s.Set_N_Max_Com_Abs(tamanho + 1);
  rt.Set_N_Max_Com_Abs(tamanho + 1);
  pt.Set_N_Max_Com_Abs(tamanho + 1);
  st.Set_N_Max_Com_Abs(tamanho + 1);
  v.Set_N_Max_Com_Abs(tamanho + 1);
  t.Set_N_Max_Com_Abs(tamanho + 1);
  solucao.Set_N_Max_Com_Abs(tamanho + 1);

  for(i = 0; i <= tamanho; i++) {
    if (r.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (p.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (s.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (rt.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (pt.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (st.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (v.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (t.Inclui(zero)) {
      throw CMemory_Exception();
    }

    if (solucao.Inclui(zero)) {
      throw CMemory_Exception();
    }
  }

  A.Format(SYMMETRIC_SUPERIOR);
	A.Orientation(LINE);

  if (validate_matrix(A) == 0) {
    throw CInput_Exception(" Invalid Matrix for this Solver.");
  }

	r.Zera();
	p.Zera();
	s.Zera();
	rt.Zera();
	pt.Zera();
	st.Zera();
	v.Zera();
	t.Zera();
	solucao.Zera();

  REALTYPE ro0, ro1 = 0.0, af = 0.0, w = 0.0, bt;
  const double NORM_LIMIT = 1e-30;

  //std::cout << ".";

  r = b;
  rt = b;

  this->itc = 0;


  P.build(A);


#ifdef __SHOWCONVVALUES__
  std::cout << "TESTING BICGSTAB SOLVER" << std::endl;
  std::cout.setf(std::ios::left,std::ios::adjustfield);
  std::cout<<std::setw(15)<<"ITERATION" <<std::setw(15)<< "PRECISION" <<std::setw(15) << "NORMA" << std::endl;
  std::cout << std::endl;
#endif

  do {

    this->itc++;

    ro0 = rt * r; //produto interno

    this->precisao = ro0; //precis�o atual
    if (this->precisao < 0) this->precisao = -this->precisao; //talvez seja necess�rio adicionar esta linha em todos os solvers *LAA*

	if(this->itc ==1)
		if(this->precisao < this->get_precision())
			this->set_precision(this->precisao*1e-10);

    if (ro0 == 0) {
      return solucao; //retorna nulo em caso de erro, verificar depois *LAA*
    }

    if (this->itc == 1) {
      p = r;
    } else {
      bt = (ro0/ro1)*(af/w); //ro0 - indica o ro atual e ro1 - indica defasado de 1 iteracao
      p = r + bt * (p - w * v);
    }

    P.apply(p, pt);

    v = A * pt;

    af = ro0 / (rt * v);

    s = r - af * v;

    if (s.Norma(NORM_INFINITE) < NORM_LIMIT) {
      solucao = solucao + af * pt;
      return solucao;
    }

    P.apply(s, st);

    t = A * st;

    w = (t * s) / (t * t);

    solucao = solucao + af * pt + w * st;

    r = s - w * t;

    ro1 = ro0;



#ifdef __SHOWCONVVALUES__
    std::cout.setf(std::ios::left,std::ios::adjustfield);
   //std::cout.width(10);
  std::cout<<std::setw(15)<<this->itc <<std::setw(15)<< this->precisao <<std::setw(15) << std::setfill(' ') << std::endl;
#endif

  } while ((this->itc <= this->get_iteractions()) && (this->get_precision() < this->precisao) && (w != 0));

  return solucao;
}

} //end lev_math namespace

#endif

