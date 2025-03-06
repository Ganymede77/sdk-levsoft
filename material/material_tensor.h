#ifndef CMATERIAL_TENSOR_H
#define	CMATERIAL_TENSOR_H


/* 
 * File:   CMaterial_Tensor.h
 * Author: nancy
 *
 * Created on 29 de Agosto de 2012, 17:54
 */


#include <vector>
#include "matrix.hpp"
#include "material.hpp"


class CMaterial_Tensor : public Material
{
protected:
    std::vector<lev_math::Te_Matrix<double>  > vPhysical_Properties; ///< tensores de propriedades (matrizes)
    std::vector<std::string > vPhysical_Properties_Name; ///< nome da propriedades

public:
    CMaterial_Tensor();
    CMaterial_Tensor(const CMaterial_Tensor& orig);
    virtual ~CMaterial_Tensor();
    
	bool Property_Value_Tensor(Point &p, std::string phys_prop_name , lev_math::Te_Matrix<double> &mTensorK);///< Devolve os valores da propriedade tensorial identificada pelo nome
	bool Set_Property_Tensor(lev_math::Te_Matrix<double> *pTensorK, std::string phys_prop_name);///<Inclui propriedade tensorial identificada pelo nome
    
    lev_math::Te_Matrix<double> * Create_PropertyTensor();///< Cria um ponteiro para um tensor (matriz)
    
	short int Recover(FILE *fp, short int a, short int = 0) { a = 0; return 0; } ///< Implementacao vazia, somente para poder instanciar um objeto    
};


#endif	/* CMATERIAL_TENSOR_H */
