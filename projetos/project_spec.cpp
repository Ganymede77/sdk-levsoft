/* 
 * File:   CProject_Solver_Temporario.cpp
 * Author: nancy
 * 
 * Created on 14 de Agosto de 2012, 15:55
 */
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "project_spec.h"

/*
 obs:
 * Nancy 15/08/2012
 * 
 * incluir uma função em CProjectBase que retorna somente o nome do projeto e somente o path do projeto
 * 
 */
CProject_Spec::CProject_Spec()
{
}

CProject_Spec::CProject_Spec(const CProject_Spec& orig)
{
}

CProject_Spec::~CProject_Spec()
{
}


std::vector<std::string> CProject_Spec::ExtractPathAndNameForWindowsOS(std::string namewithpath)
{

    
    std::vector<std::string> vNames;
	int size_project_name, size_project_name_with_path;
	int pos_aux;
	int i,j;

    sProjectNameOnly.clear();
    sProjectPath.clear();
    
	size_project_name_with_path = namewithpath.size();
	
	//procura `\` a partir do final da string
	pos_aux = namewithpath.rfind('\\');


	//calcula o tamanho do nome do projeto
	size_project_name = size_project_name_with_path - pos_aux;

	//redimensiona o tamanho da string do nome do projeto
	sProjectNameOnly.resize(size_project_name+1); 

	//redimensiona o tamanho da string do path do projeto
	sProjectPath. resize(pos_aux+1);


	//copia path do projeto para a string do nome do projeto
	for(i=0;i<=pos_aux;i++)
	{
		sProjectPath[i] = namewithpath[i];
	}


	j=0; 
	//copia nome do projeto para a string do nome do projeto
	for(i=pos_aux+1;i<size_project_name_with_path;i++)
	{
		sProjectNameOnly[j++] = namewithpath[i];

	}

    vNames.push_back(sProjectNameOnly);
    vNames.push_back(sProjectPath);
    
//    std::cout << "\nCProject_Spec::ExtractPathAndNameForWindowsOS sProjectNameOnly: " << sProjectNameOnly;
//    std::cout << "\nCProject_Spec::ExtractPathAndNameForWindowsOS sProjectPath: " << sProjectPath;
    
    return vNames;
	
}//end of void CProject_Name_Builder::ExtractPathAndName(std::string namewithpath)

/**Leitura do arquivo de especificaçăo preliminar (*.dados)
 * OBS: esta implementado em CProject_Base, porém não é possível instaciar somente o CProjec_base
 * talvez fosse interessante fazer isso para poder usar as funções que estão implementadas lá, como a fução acima ExtractPathAndNameForWindowsOS
*/
int	CProject_Spec:: ReadSpecDataFile(std::string filename)
{
	int codeErr = 0;
	int iaux;
	std::string text;
	std::string string_aux;
//	CLevMessage	mensagem;
	bool bcreatespec = false;

	std::ifstream fin(filename.c_str());
	if(fin.fail())
	{
		codeErr = 2;
		text = " Erro openning file  " + filename;
		//mensagem.SetMessageData(codeErr,"CProject_Solver_Temporario::ReadSpecDataFile()", text,MSGERROR);
		//SetErrorMessage(mensagem);
		return codeErr;
	}
	else
	{
			fin>> string_aux; //	nome do fenômeno, ex: electrostatic 
				o_phen_specbase.PhenomenonName(string_aux);

			fin>> string_aux; //	dimensăo do estudo, ex: two_dimensional
			Dimension(string_aux);// a funçăo atualiza tanto a string quanto a 
									// variável iProjectDimension

		//	fin>> sProjectDimension;//	dimensăo do estudo, ex: two_dimensional

			fin>> string_aux;//	tipo de elemento, ex: simplex_2D
				o_phen_specbase.ElementType(string_aux);

//			fin>> string_aux;//	ordem do elemento, ex: first_order
//				vpPhenSpecificationVector[0]->ElementOrder(string_aux);
			fin>> iaux;//	ordem do elemento, numérico
				o_phen_specbase.ElementOrder(iaux);

//			fin>> string_aux;//	graus de liberdade, exe: one_dof
//				vpPhenSpecificationVector[0]->DegreesOfFreedom(string_aux);
			fin>> iaux;//	graus de liberdade, numerico
				o_phen_specbase.DegreesOfFreedom(iaux);


			fin.close();
        
        
#if 0      //implementação original   
		//considerando estudo com um fenômeno simples
		//verifica se năo existe objeto de especificaçăo de fenômeno alocado
		// se năo existir, aloca e inclui um
		if(!vpPhenSpecificationVector.size()) bcreatespec = true;
		else if(vpPhenSpecificationVector[0]==NULL)
		{
			vpPhenSpecificationVector.clear();
			bcreatespec = true;
		}

		if(bcreatespec == true)
		codeErr = CreateAndIncludePhenSpecification();

		if(!codeErr)
		{
			fin>> string_aux; //	nome do fenômeno, ex: electrostatic 
				vpPhenSpecificationVector[0]->PhenomenonName(string_aux);

			fin>> string_aux; //	dimensăo do estudo, ex: two_dimensional
			Dimension(string_aux);// a funçăo atualiza tanto a string quanto a 
									// variável iProjectDimension

		//	fin>> sProjectDimension;//	dimensăo do estudo, ex: two_dimensional

			fin>> string_aux;//	tipo de elemento, ex: simplex_2D
				vpPhenSpecificationVector[0]->ElementType(string_aux);

//			fin>> string_aux;//	ordem do elemento, ex: first_order
//				vpPhenSpecificationVector[0]->ElementOrder(string_aux);
			fin>> iaux;//	ordem do elemento, numérico
				vpPhenSpecificationVector[0]->ElementOrder(iaux);

//			fin>> string_aux;//	graus de liberdade, exe: one_dof
//				vpPhenSpecificationVector[0]->DegreesOfFreedom(string_aux);
			fin>> iaux;//	graus de liberdade, numerico
				vpPhenSpecificationVector[0]->DegreesOfFreedom(iaux);


			fin.close();

		}//fecha if(!codeErr)
#endif
        
	}// fecha else
	
	return codeErr;

}//fecha int	CProject_Base:: ReadSpecDataFile(std::string filename)
