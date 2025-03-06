/* 
 * File:   CLev_Poisson.cpp
 * Author: nancy
 * 
 * Created on 29 de Agosto de 2012, 11:04
 */

#include "phen_poisson_tensor.h"

CLev_FEM_Poisson_Tensor::CLev_FEM_Poisson_Tensor()
{
    str_phys_property_name = "permittivity";//valor default
}



CLev_FEM_Poisson_Tensor::~CLev_FEM_Poisson_Tensor()
{
}

int CLev_FEM_Poisson_Tensor::GetProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK, Point &p  )
{
    int codeReturn = 0;

/*    
    double            property;//propriedade do material;
    Material        *mater;
    Point            p;//somente para passar para o material devolver o valor da propriedade

    // *NMA*:03/02/2006: talvez o Get_Property deva ficar no projeto, assim o explorador tamb???m poder??? obter
    //a informa??????o desta forma

    mater  = elem.Get_Material();
    property = mater->Property_Value(p);
    //property = mater->permittivity;
*/
    
    codeReturn = proj->GetMaterialProperty(elem, TensorK, p,str_phys_property_name ); 
    

    return  codeReturn;
}


// ==========================================================================

void CLev_FEM_Poisson_Tensor::Build_Local_Matrices_Blocks(void)
{
    int i,j,matloc_dimension = 3,num_nodes_el=2;
    Nodal_Element *elem;
	Material		*mater;
	double			property;
    
    Point           p;//para adquirir a propriedade no ponto P
    
    lev_math::Te_Matrix<double> mTensorK;

    // Gauss quadrature order for each region
    //int ord_integ[21] = {3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
    //int ord_integ[21] = {1,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

	long order_elem = ptr_regions[0].ptr_lo_elements[0][0]->Order();
  // obtem a ordem do elemento.
    int ord_integ[21] = {order_elem,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

    lev_math::Te_Matrix<double> MatLoc(matloc_dimension,matloc_dimension);
    lev_math::Te_Vector<double> vsource(matloc_dimension), vsourcet(matloc_dimension);
    double            source_value = 0.;

//#define __POISSON_TENSOR_DEBUG__    
#ifdef __POISSON_TENSOR_DEBUG__
    std::cout << "\n\nCLev_FEM_Poisson_Tensor::Build_Local_Matrices_Blocks: inicio" <<  std::endl;                        
    std::cout.flush();
#endif                                                    
    
    
    for (i = 0; i < _region::Region :: Total_Regions(); i++)
    {
        for ( j = 0; j < ptr_regions[i].ptr_lo_elements->Get_Total_Componentes(); j++)
        {
            
        //std::cout << "\n =======\nCLev_FEM_Poisson_Tensor, elemento j= " << j << std::endl;                        
            
            elem   = ptr_regions[i].ptr_lo_elements[0][j];

            num_nodes_el = elem->Get_Nodes();
            if(num_nodes_el != matloc_dimension)
            {
                MatLoc.ChangeDimensions(num_nodes_el,num_nodes_el);
                vsource.ChangeDimension(num_nodes_el);
                vsourcet.ChangeDimension(num_nodes_el);
                matloc_dimension = num_nodes_el;
            }

            
#ifdef __POISSON_TENSOR_DEBUG__
    std::cout << "\nCLev_FEM_Poisson_Tensor::Build_Local_Matrices_Blocks: ANTES de proj->GetMaterialProperty(*elem,mTensorK,p,str_phys_property_name)" <<  std::endl;                        
    std::cout.flush();
#endif                                                    
            
            //primeiro verifica se a propriedade e isotropica ou nao!!!                
            proj->GetMaterialProperty(*elem,mTensorK,p,str_phys_property_name);
            
            if(mTensorK.Rows() == 1)
               if(mTensorK.Columns() ==1)//isotropico
                {
                        //std::cout <<"\n elemento "<< j << ":  material isotropico \n";
                       // std::cout << "\npressione ENTER para continuar " << std::endl; 
                        //getchar();
                       
                        IntegralGradNGradNdV(*elem,MatLoc,ord_integ[i]);
                        mater  = elem->Get_Material();
                        property = mTensorK[1][1]; //se for "permeability", o projeto ja envia o valor de "relutividade"                                                
                        MatLoc = property * MatLoc; 
                     
                        

//                std::cout << "\nCLev_FEM_Poisson_Tensor::Build_Local_Matrices_Blocks: property " << property << std::endl;
//                std::cout << "\npress ENTER to continue " << std::endl;
//                std::cout.flush();
//                getchar();
                        
                        
                 }
                 else
                 {
                     IntegralGradNTensorPropertyGradNdV(*elem, MatLoc,ord_integ[i]);
                 }                        
            
#ifdef __POISSON_TENSOR_DEBUG__
    std::cout << "\nCLev_FEM_Poisson_Tensor::Build_Local_Matrices_Blocks: APOS de proj->GetMaterialProperty(*elem,mTensorK,p,str_phys_property_name)" <<  std::endl;                        
//    std::cout << "\npressione ENTER para continuar " << std::endl; 
  //  getchar();
    
    std::cout.flush();
#endif                                                    

#if 0
            if(pTensorK)
            {
                if(pTensorK->Rows() == 1)
                    if(pTensorK->Columns() ==1)//isotropico
                    {
                        std::cout <<"\n elemento "<< j << ":  material isotropico \n";
                        IntegralGradNGradNdV(*elem,MatLoc,ord_integ[i]);
                        mater  = elem->Get_Material();
                        property = *pTensorK[1][1];
                        MatLoc = property * MatLoc;                        
                    }
                    else
                    {
                        IntegralGradNTensorPropertyGradNdV(*elem, MatLoc,ord_integ[i]);
                    }                        
            }
#endif            

            Fill_Stiffness_Matrix(*elem, MatLoc) ;

            //tratamento do termo de fonte
            if(elem->Get_Source())
            {
                CSource * source = elem->Get_Source();
                source_value =  source->Get_Value();

                //std::cout << "\nCLev_FEM_Poisson_Tensor: source_value = " << source_value << std::endl;
//                std::cout << "\npress ENTER to continue " << source_value << std::endl;
//                std::cout.flush();
//                getchar();
                
                //source_value = Get_Source_Value(i,j);//i:region j:indice do elemento
                // (mesmo no caso est�tico a fonte pode variar em fun??????o da posi??????o)
                IntegralNdV(*elem,vsource,ord_integ[i]);
                vsourcet = source_value*vsource;  
                
//                std::cout << "\nCLev_FEM_Poisson_Tensor: vsource(1) " << vsource(1) << std::endl;
//                std::cout << "\nCLev_FEM_Poisson_Tensor: vsource(2) " << vsource(2) << std::endl;
//                std::cout << "\nCLev_FEM_Poisson_Tensor: vsource(3) " << vsource(3) << std::endl;
//                std::cout << "\nCLev_FEM_Poisson_Tensor: vsourcet(1) " << vsourcet(1) << std::endl;
//                std::cout << "\nCLev_FEM_Poisson_Tensor: vsourcet(2) " << vsourcet(2) << std::endl;
//                std::cout << "\nCLev_FEM_Poisson_Tensor: vsourcet(3) " << vsourcet(3) << std::endl;
//                std::cout << "\npress ENTER to continue " << std::endl;
//                std::cout.flush();
//                getchar();
                
                Fill_RHS_Vector(*elem,vsourcet);
            }//fecha if(elem->Get_Source())
            /* fim tratamento do termo de fonte */
        } // end of for (j< regioes[i].elementos_3D.....
    } // end of for (i<regioes :: Total_Regions)
}  // end of Build_Local_Matrices_Blocks


// ==========================================================================
