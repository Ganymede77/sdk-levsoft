/* 
 * File:   CProject_Poisson.cpp
 * Author: nancy
 * 
 * Created on 29 de Agosto de 2012, 10:30
 */


#include <stdexcept>
#include "project_poisson.h"
#include <iomanip>
#include <istream>
#include <sstream>
#include "project_poisson.h"
#include "matrix.hpp"
#include "fin_elem.hpp"

CProject_Poisson::CProject_Poisson()
{ }


CProject_Poisson::~CProject_Poisson()
{ }


//============================================================================
// Cria_Material(void)
//============================================================================
Material* CProject_Poisson::Cria_Material(void)
{
	CMaterial_Tensor *m = NULL;
	m = new CMaterial_Tensor;
	return m;
} // end of Cria_Material


int CProject_Poisson::GetMaterialProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK, Point &p, std::string str_phys_property_name )
{
    int codeReturn = 0;
    int i,j;
    double value=0.0;
    
    lev_math::Te_Matrix<double> propertyTensor,propertyTensorAdequado;//propriedade do material;
    CMaterial_Tensor *mater;

    mater  = static_cast<CMaterial_Tensor *>(elem.Get_Material());
    //propertyTensor = mater->Property_Value(p);
    
    mater->Property_Value_Tensor(p, str_phys_property_name.c_str() , propertyTensor);
    //property = mater->permittivity;

    if(str_phys_property_name == "permeability") //no calculo ï¿½ usada direto a relutividade = 1/permeabilidade
    {
        propertyTensorAdequado.ChangeDimensions(propertyTensor.Rows(),propertyTensor.Columns());
            
        for(i=1;i<=propertyTensor.Rows();i++)
          for(j=1;j<=propertyTensor.Columns();j++)
            {
              value = propertyTensor[i][j];
              
                //std::cout << "\n CProject_Poisson::GetMaterialProperty  === permeability = " << value;
                if(value)
                    propertyTensorAdequado.SetValue(i,j,1./value);
                    //propertyTensorAdequado[i][j] = 1./value;
            
            
        
                TensorK = propertyTensorAdequado;

            }
    }
    else
            TensorK = propertyTensor;
    
    
#if 0 
    std::cout << "\nCProject_Poisson::GetMaterialProperty: apos TensorK = propertyTensor " << std::endl;
        std::cout << "\npressione ENTER para continuar " << std::endl;    
        std::cout.flush();
        getchar();
#endif
        return codeReturn;
    
}//end of CProject_Poisson::GetMaterialProperty(Finite_Element &elem, lev_math::Te_Matrix<double> &TensorK )

//=============================================================================
// Segue a implementacao  de short CProject_Schrodinger::ReadBDMaterials(void)
// para permitir considerar versoes do arquivo *.mei
// *NMA* modificacao realizada para incluir a leitura do arquivo propfis.mei,
// se o arquivo nome_projeto.mei ainda nÃ£o existir (para leitura de projeto antigos)
//=============================================================================
short CProject_Poisson::ReadBDMaterials(std::string str_propfis)
{
	short codeErr = 0;
    std::string line;

    //verificacao de nome igual ao implementado em  CProject_Schrodinger
    std::string strMessage;
    if(project_name.empty()) {
    	strMessage = "\nem CProject_Poisson::ReadBDMaterials > Project not defined!";
    	throw std::runtime_error(strMessage.c_str());
    }

    //tenta leitura do arquivo *.mei
    std::string filename = project_name + ".mei";
    std::ifstream fin(filename.c_str());
    if(fin.fail())// se falhou a leitura do *.mei tenta ler propfis.mei
    {
        std::ifstream fin_propfis(str_propfis.c_str());
        
        if(fin_propfis.fail())// se falhou a leitura do propfis.mei
        {
            std::cout << "\nem CProject_Poisson::ReadBDMaterials - Erro na abertura do arquivo de materiais " << filename.c_str() << std::endl;
            std::cout << "\nem CProject_Poisson::ReadBDMaterials - Erro na abertura do arquivo de materiais " << str_propfis.c_str() << std::endl;
            std::cout << "\n Aviso: Um destes arquivos deve existir no mesmo diretorio do projeto. " << std::endl;
            
            std::cout.flush();
            codeErr = 100; 
            return codeErr;        
        }                
    
        std::cout << "\nem CProject_Poisson::ReadBDMaterials -  Lendo arquivo de materiais " << filename.c_str() << std::endl;
        
        fin_propfis.close();
        codeErr = CProject_Solver::ReadBDMaterials(str_propfis);
        
        //grava o arquivo com o nome do projeto
        codeErr = SavePropFis(filename);
              
    }
    std::cout << "\nCProject_Poisson::ReadBDMaterials - Lendo arquivo de materiais " << filename.c_str() << std::endl;
    std::cout.flush();

    fin.close();
    codeErr = CProject_Solver::ReadBDMaterials(filename);
    
    return codeErr;
}


short CProject_Poisson::SavePropFis(std::string nomePropFis)
{
	FILE		*parqPROPFIS = NULL;
	int			i;
	std::string nome_temp;

	
//======================= salva arquivo propfis.mei ===========================

	parqPROPFIS = fopen(nomePropFis.c_str(),"w");
	if (parqPROPFIS == NULL)
	{
//		Mensagem_erro("erro na abertura do arquivo propfis.mei");
//		Ready = NNAO;	
		return 103;
	}

  fprintf(parqPROPFIS, "%d %d %d %d", NM, NELT, NCCOR, NMAG);

  for(i=0;i<NM;i++)
  {

	  nome_temp = vMaterialName[i];
	  fprintf(parqPROPFIS,"\n%d %s %lg %lg %lg %lg %lg %lg %lg",
										 codigos[i],
										 nome_temp.c_str(),
										 materiais[i][0],
										 materiais[i][1],
										 materiais[i][2],
										 materiais[i][3],
										 materiais[i][4],
										 materiais[i][5],
										 materiais[i][6]);

  }

  fclose(parqPROPFIS);

 return (0); 

}


//=============================================================================
//ReadMaterials(std::string filename)
//=============================================================================
short CProject_Poisson::ReadMaterials(std::string filename)
{
	short codeErr = 0, j;
	long num_meios,i;
	int codigo;
	int pos = 0;
	bool achou = false;
	CMaterial_Tensor *mater, *mater_aux;
    lev_math::Te_Matrix<double> * pTensorK = NULL;
	double angulo;
    std::string str_phen;
    std::string str_matname;;
	std::ifstream fin(filename.c_str());

	if(!fin.is_open()) {
		std::cout << "\nem CProject_Poisson::ReadMaterials - Erro na abertura do arquivo de materiais (meios) " << filename.c_str() << std::endl;
		std::cout.flush();
		codeErr = 100;
	} else {
		std::cout << "\nCProject_Poisson::ReadMaterials -Lendo arquivo de materiais (meios) " << filename.c_str() << std::endl;
		std::cout.flush();

		fin >> num_meios;

		for(i=0;i<num_meios;i++) {
			fin >> codigo;
			fin >> angulo;
            
			achou = false;

			for(j=0;j<NM && achou == false;j++) {
				if(codigos[j] == codigo) {
					achou = true;
					pos = j;
				}
			}

			if(achou == true) {
                str_matname = vMaterialName[pos];
                // tipo de estudo
                str_phen = vpPhenSpecificationVector[0]->PhenomenonName();

				mater_aux = static_cast<CMaterial_Tensor *> (ptr_regions->ptr_lo_elements[0][i]->Get_Material());
				if(mater_aux) delete mater_aux;
				                
                mater = static_cast<CMaterial_Tensor *> (Cria_Material());

				if(mater) {
					mater->code	= codigo;
					mater->angle = angulo;
                    strcpy(mater->name, str_matname.c_str());
                    //pTensorK = new lev_math::Te_Matrix<double>;
                    pTensorK            = mater->Create_PropertyTensor();
                    if(pTensorK) {
                        pTensorK->ChangeDimensions(1,1);
                        pTensorK->SetValue(1,1,materiais[pos][0]);
                        
                        if(str_phen == "ThermalStationary" )
                            mater->Set_Property_Tensor(pTensorK, "thermalconductivity");                       
                        else {
                            mater->Set_Property_Tensor(pTensorK, "permittivity");
                            std::cout.flush();
                        }
                        pTensorK->SetValue(1,1,materiais[pos][1]);
                        mater->Set_Property_Tensor(pTensorK, "conductivity");

                        //*NMA* aqui so considerando casos 2D anisotropicos lineares
                        if(materiais[pos][5] == 0.)//isotropico
                        {
                            double propvalue =0.0;
                            pTensorK->ChangeDimensions(1,1);
                            if(materiais[pos][4]) 
                                propvalue = 1./materiais[pos][4];//*NMA* no .mei estï¿½ gravada a relutividade
                            pTensorK->SetValue(1,1,propvalue);
                        } else {
                            double propvalue =0.0;
                            pTensorK->ChangeDimensions(2,2);
                            if(materiais[pos][4]) 
                                propvalue = 1./materiais[pos][4];//*NMA* no .mei estï¿½ gravada a relutividade
                            pTensorK->SetValue(1,1,propvalue);
                            if(materiais[pos][5]) 
                                propvalue = 1./materiais[pos][5];//*NMA* no .mei estï¿½ gravada a relutividade
                            pTensorK->SetValue(2,2,propvalue);
                        }
                        mater->Set_Property_Tensor(pTensorK, "permeability");
                    }
					ptr_regions->ptr_lo_elements[0][i]->Set_Material(mater);
					delete pTensorK;
				} else
					codeErr = 101;
			} else
				codeErr = 102; // cÄ�Å¼Ë�digo de material nÄ�Å¼Ë�o cadastrado
		} // fecha for(i=0;i<num_meios;i++)
	} //fecha else
	return codeErr;
}


short CProject_Poisson::SaveSolution()
{
	short codeErr;
	long num_points, i, j;
	double value;
	Point * p_aux;
	Te_Nodal_Point<double> *ponto;

	for(j=0; j<this->vpPhenSpecificationVector[0]->DegreesOfFreedom(); j++) {
		std::stringstream filename, postfix;
		if(j)
			postfix << "_var" << j;
		else
			postfix << "";
		filename << ProjectNameWithPath() << postfix.str() << ".p1g";

		std::ofstream fout(filename.str().c_str());

		if(fout.fail()) {
			std::cout << "\nem CProject_Poisson::SaveSolution - Erro na abertura do arquivo de solucao: " << filename.str().c_str() << std::endl;
			codeErr = 100;
		} else {
			num_points = ptr_lo_points->Get_Total_Componentes();
			fout << num_points << std::endl;
			for(i=0;i<num_points;i++) {
				p_aux = ptr_lo_points[0][i];
				ponto = (Te_Nodal_Point<double> *) p_aux;
				value = ponto->Get_Variables(j+1);
				fout.setf(std::ios_base::scientific, std::ios_base::floatfield);
				fout << value <<  std::endl;
			}
			fout.close();
		}
	}
	return codeErr;
} // End of SaveSolution

