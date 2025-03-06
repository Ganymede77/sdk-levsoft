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
{ }


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


void CLev_FEM_Poisson_Tensor::Build_Local_Matrices_Blocks(void)
{
    int i, j, matloc_dimension = 3, num_nodes_el=2;
    Nodal_Element *elem;
	Material *mater;
	double property;
    Point p;
    lev_math::Te_Matrix<double> mTensorK;

	std::cout << "\n  - CLev_FEM_Poisson_Tensor::BuildinBlocks()\n";
    
    lev_math::Te_Matrix<double> MatLoc(matloc_dimension,matloc_dimension);
    lev_math::Te_Vector<double> vsource(matloc_dimension), vsourcet(matloc_dimension);
    double source_value = 0.0;

    for(i=0; i<_region::Region::Total_Regions(); i++) {
        for(j=0; j<ptr_regions[i].ptr_lo_elements->Get_Total_Componentes(); j++) {
            elem = ptr_regions[i].ptr_lo_elements[0][j];

            num_nodes_el = elem->Get_Nodes() * proj->vpPhenSpecificationVector[0]->DegreesOfFreedom();
            if(num_nodes_el != matloc_dimension) {
                MatLoc.ChangeDimensions(num_nodes_el,num_nodes_el);
                vsource.ChangeDimension(num_nodes_el);
                vsourcet.ChangeDimension(num_nodes_el);
                matloc_dimension = num_nodes_el;
            }

            // Primeiro verifica se a propriedade e isotropica ou nao!!!
            proj->GetMaterialProperty(*elem, mTensorK, p, str_phys_property_name);
            
            if(mTensorK.Rows() == 1) {
            	if(mTensorK.Columns() == 1) { // isotropico
            		IntegralGradNGradNdV(*elem, MatLoc, elem->integrationOrder);
            		mater = elem->Get_Material();
            		property = mTensorK[1][1]; // Se for "permeability", o projeto ja envia o valor de "relutividade"
            		MatLoc = property * MatLoc;
            	} else
            		IntegralGradNTensorPropertyGradNdV(*elem, MatLoc, elem->integrationOrder);
            }
            
            Fill_Stiffness_Matrix(*elem, MatLoc) ;

            // Tratamento do termo de fonte
            if(elem->Get_Source()) {
            	std::cout << "<.>";
                CSource * source = elem->Get_Source();
                source_value =  source->Get_Value();

                IntegralNdV(*elem, vsource, elem->integrationOrder);
                vsourcet = source_value*vsource;  

                Fill_RHS_Vector(*elem,vsourcet);
            }
        }
    }
} // End of Build_Local_Matrices_Blocks


