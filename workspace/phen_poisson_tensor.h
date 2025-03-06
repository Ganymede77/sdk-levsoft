/* 
 * File:   phen_poisson_tensor.h
 * Author: nancy
 *
 * Created on 29 de Agosto de 2012, 11:04
 */

#include "iostream"
#include "phenomenon_poisson.h"
#include "project_poisson.h"

#ifndef CLEV_FEM_POISSON_TENSOR_H
#define	CLEV_FEM_POISSON_TENSOR_H

/**
 * Projeto: SDK-LEVSOFT
 *
 * A classe CLev_FEM_Poisson_Tensor implementa a construção das matrizes locais dos elementos finitos considerando as propriedades dos materiais como tensores,
 * permitindo o tratamento de materiais com propriedades anisotrópicas.
 * 
 *
*/

class CLev_FEM_Poisson_Tensor : public CLev_FEM_Poisson
{
    
protected:
   virtual	void Build_Local_Matrices_Blocks(void); ///< Montagem da matriz local considerando materiais com propriedades anistropicas (tensor). 
                                                    ///< No caso de materias isotrópicos, o tensor tem dimensão 1x1
        
  virtual int  GetProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK, Point &p );  ///< Obtenção da propriedade do material. 
                                                                                                    ///<OBS: em CLev_FEM_Poisson não é virtual
   
   std::string str_phys_property_name;  ///< String que guarda nome da propriedade a ser usada. 
                                        ///< Optou-se pela utilização da propriedade e não do nome do estudo 
                                        ///< considerando casos acoplados, por exemplo, magneto-térmico
   
public:
    CLev_FEM_Poisson_Tensor();    
    CLev_FEM_Poisson_Tensor(std::string str_property){str_phys_property_name = str_property;}    
    virtual ~CLev_FEM_Poisson_Tensor();
    
	void Associate(lev_math::Lo_Templ<Point * , long> *lp) {CLevPhenomenon::Associate(lp);} ///<declarando de novo, pois não está sendo reconhecidas, deveriam ser pois seriam herdadas de CLev_FEM_Poisson
	void Associate(_region::Region *l)  {CLevPhenomenon::Associate(l);} ///<declarando de novo, pois não está sendo reconhecidas, deveriam ser pois seriam herdadas de CLev_FEM_Poisson
	void Associate(lev_math::Lo_Templ<long , long> *l)   {CLevPhenomenon::Associate(l);} ///<declarando de novo, pois não está sendo reconhecidas, deveriam ser pois seriam herdadas de CLev_FEM_Poisson
              
    void Associate(CProject_Solver * pt)
    {
        proj = static_cast<CProject_Poisson *>(pt);
    }///< Associa a classe fenômeno à classe de projeto
  
    void SetPropertyName(std::string name) { str_phys_property_name = name;} ///< Atribui nome da propriedade física a ser utilizada nos calculos
    std::string GetPropertyName(void) { return str_phys_property_name;} ///< Devolve nome da propriedade física a ser utilizada nos calculos
    
private:

};

#endif	/* CLEV_POISSON_H */

