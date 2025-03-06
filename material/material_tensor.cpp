/* 
 * File:   CMaterial_Tensor.cpp
 * Author: nancy
 * 
 * Created on 29 de Agosto de 2012, 17:54
 */


#include "material_tensor.h"


CMaterial_Tensor::CMaterial_Tensor()
{ }


CMaterial_Tensor::CMaterial_Tensor(const CMaterial_Tensor& orig)
{ }


CMaterial_Tensor::~CMaterial_Tensor()
{ }


bool CMaterial_Tensor::Property_Value_Tensor(Point &p, std::string phys_prop_name , lev_math::Te_Matrix<double> &mTensorK)
{
    unsigned int i;
    int j, k;
    bool bool_foundProp = false;
    std::string str_temp;
            
    for(i=0;i<vPhysical_Properties_Name.size()&& bool_foundProp==false;i++) {
        str_temp = vPhysical_Properties_Name[i];
        if(str_temp == phys_prop_name)
           bool_foundProp = true;
    }
    
    if(bool_foundProp) {
        i--; //apos encontrado, ainda incrementa ino laco for
        mTensorK.ChangeDimensions(vPhysical_Properties[i].Rows(),vPhysical_Properties[i].Columns());            
        for(j=1;j<=mTensorK.Rows();j++)
            for(k=1;k<=mTensorK.Columns();k++)
                mTensorK.SetValue(j,k,vPhysical_Properties[i].GetValue(j,k));
        return true;
    }
    return false;
}


bool CMaterial_Tensor::Set_Property_Tensor(lev_math::Te_Matrix<double> *pTensorK, std::string phys_prop_name)
{
    int j, k;
    lev_math::Te_Matrix<double> mTensorK;
    
    mTensorK.ChangeDimensions(pTensorK->Rows(),pTensorK->Columns());
    for(j=1;j<=mTensorK.Rows();j++)
        for(k=1;k <= mTensorK.Columns();k++)
        	mTensorK(j,k)=pTensorK->GetValue(j,k);
    
    vPhysical_Properties.push_back(mTensorK); ///< ponteiros para tensores de propriedades (matrizes)
    vPhysical_Properties_Name.push_back(phys_prop_name); ///< nome da propriedades
    lev_math::Te_Matrix<double> mTensorKTemp = vPhysical_Properties[vPhysical_Properties.size()-1];

    return true;
}


lev_math::Te_Matrix<double> * CMaterial_Tensor::Create_PropertyTensor()
{
    lev_math::Te_Matrix<double> *pTensorK;
    pTensorK = new lev_math::Te_Matrix<double>;
    return pTensorK;
}
