#include "project_base.h"
#include <sstream>
#include <iomanip>


namespace lev
{


int CProject_Base::BuildFileNames(void) 
{
	int codeErr=0;
	if(current_phenomenon_index < 0)
		CreateAndIncludePhenSpecification();
	if(dProjectVersion == 1.0) {
		if(current_phenomenon_index >=0 && current_phenomenon_index < (signed)vpPhenSpecificationVector.size())
			o_NameBuilder_v1.SetMeshType(vpPhenSpecificationVector[current_phenomenon_index]->MeshTypeEnum());
		o_NameBuilder_v1.BuildFileNames(sProjectNameWithPath); // with path: valor default
	}
	return codeErr;
}


int CProject_Base::BuildFileNamesMeshBased(void) 
{
	int codeErr = 0;
	if(dProjectVersion == 1.0) {
		if(current_phenomenon_index >=0 && current_phenomenon_index < (signed)vpPhenSpecificationVector.size()) {
			o_NameBuilder_v1.SetMeshType(vpPhenSpecificationVector[current_phenomenon_index]->MeshTypeEnum());
			o_NameBuilder_v1.BuildFileNamesMeshBased(vpPhenSpecificationVector[current_phenomenon_index]->MeshTypeEnum());
		} else
			o_NameBuilder_v1.BuildFileNamesMeshBased(GEOMETRIC); // monta nome default
	}
	return codeErr;
}


std::string CProject_Base::GetFileName(enum FILENAME qual)
{
	std::string sFileName="";
	sFileName = o_NameBuilder_v1.GetFileName(qual);
	return sFileName;
}


void CProject_Base::SetErrorMessage(lev::CLevMessage mdata)
{
	vMessageVector.push_back(mdata);
}


lev::CLevMessage CProject_Base::GetErrorMessageByCode(int codigo)
{
	unsigned int i;
	lev::CLevMessage mdata;
	bool found = false;

	for(i=0; i<vMessageVector.size() && found == false; i++) {
		if(vMessageVector[i].GetMessageCode() == codigo) {
			mdata = vMessageVector[i];
			found = true;
		}
	}

	if(found == false)
		mdata.SetMessageData(codigo,"CProject_Base"," message not found",MSGWARNING);

	return mdata;
}


// Devolve a mensagem armazenada na posição index do vetor
lev::CLevMessage CProject_Base::GetErrorMessageByIndex(int index)
{
	lev::CLevMessage mdata;

	if(index >=0 && index<(signed)vMessageVector.size())
		mdata = vMessageVector[index];
	return mdata;
}


int CProject_Base::SaveErrorLog(void)
{
	int codeErr = 0;
	unsigned int i;

	std::string filename = GetFileName(ERRORLOGFILE);

	if(vMessageVector.size()) {
		std::ofstream fout(filename.c_str(),std::ios::app);

		if(fout.fail())
			codeErr = 1000;
		else
			for(i=0;i<vMessageVector.size();i++)
				fout << vMessageVector[i];
		fout.close();
	}
	return codeErr;
}


///< Apaga arquivo nome_projeto.errorlog para começar de novo
void CProject_Base::EraseErrorLog(void)
{
	FILE		*parqLOG = NULL;
	bool		existe = false;

	std::string filename = GetFileName(ERRORLOGFILE);

	//*NMA*: verifica existência, chama remove somente se existir
	parqLOG = fopen(filename.c_str(),"r");
	if (parqLOG)
	{
		existe = true;
		fclose(parqLOG);
	}

	if(existe) 
		remove(filename.c_str());// apaga arquivo

//	filename = "*.tmp";
//	remove(filename.c_str());//apaga arquivos temporarios

}


/**
Descrição:	Aloca e inclui no vetor de ponteiros para objetos de especificação de fenômeno 
			ponteiro para classe CPhenSpecBase
			Para alocação de ponteiros para classes derivadas de CPhenSpecBase, criar sobrecarga
			em classe de projeto derivada.
			Apesar de o vetor ser público, esta função facilita a alocação.

retorno:	0 - sucesso
			10001 - erro na alocação de memória

Autor:		Nancy Mieko Abe
Data:		04/07/2007
*/
int	CProject_Base::CreateAndIncludePhenSpecification(void)
{
	lev::CLevMessage mdata;
	CPhenSpecBase * pPhenSpec = NULL;
	int				codeErr = 0;

	pPhenSpec = new CPhenSpecBase;

	pPhenSpec->MeshType("geometric");

	if(pPhenSpec)
	{
		vpPhenSpecificationVector.push_back(pPhenSpec);
		current_phenomenon_index = 0;
	}
	else
	{
		codeErr = 1001;
		mdata.SetMessageData(codeErr,"CreateAndIncludePhenSpecification()","Error: insuficient memory.",MSGERROR);
		SetErrorMessage(mdata);
	}

	return codeErr;

}//end of int	CProject_Base::CreateAndIncludePhenSpecification(void)


int	CProject_Base::GetCurrentPhenIndex(void)
{
    if(current_phenomenon_index < 0 ) {
        CreateAndIncludePhenSpecification();
        current_phenomenon_index = 0;
    } else if(current_phenomenon_index >= (signed)vpPhenSpecificationVector.size())
        current_phenomenon_index =0;
    return current_phenomenon_index;
}


///< Leitura do arquivo PRJ em XML: IMPLEMENTAÇÃO PROVISÓRIA
int	CProject_Base::ReadPRJ(std::string name)
{
	int codeErr=0;
	
	XML_PROJECT_STATUS = NODATA;
	return codeErr = 1;// provisório, só para dar erro
}

/**Leitura do arquivo de especificação preliminar (*.dados)
*/
int	CProject_Base:: ReadSpecDataFile(std::string filename)
{
	int codeErr = 0;
	int iaux;
	std::string text;
	std::string string_aux;
	CLevMessage	mensagem;
	bool bcreatespec = false;

	std::ifstream fin(filename.c_str());
	if(fin.fail())
	{
		codeErr = 2;
		text = " Erro openning file  " + filename;
		mensagem.SetMessageData(codeErr,"CProject_Base::ReadSpecDataFile()", text,MSGERROR);
		SetErrorMessage(mensagem);
		return codeErr;
	}
	else
	{
		//considerando estudo com um fenômeno simples
		//verifica se não existe objeto de especificação de fenômeno alocado
		// se não existir, aloca e inclui um
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

			fin>> string_aux; //	dimensão do estudo, ex: two_dimensional
			Dimension(string_aux);// a função atualiza tanto a string quanto a 
									// variável iProjectDimension

		//	fin>> sProjectDimension;//	dimensão do estudo, ex: two_dimensional

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
	}// fecha else
	
	return codeErr;

}//fecha int	CProject_Base:: ReadSpecDataFile(std::string filename)


//=============================================================================
// Funcao....: trim
// Parametros: str - string que sera limpa
// Descricao.: retira caracteres nao alfanumericos do inicio e do final da
//             string
// Data......: 09/Set/2010
// Por  .....: Roberto Yuji Tanaka
//=============================================================================
std::string CProject_Base::trim(std::string& str)
{
	std::string::size_type pos = str.find_last_not_of(' ');
	if(pos != std::string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != std::string::npos)
			str.erase(0, pos);
	} else str.erase(str.begin(), str.end());
	return str;
} // END trim
//=============================================================================

std::string CProject_Base::zeroPadNumber(long int num, int length)
{
    std::ostringstream ss;
    ss << std::setw( length ) << std::setfill( '0' ) << num;
    return ss.str();
}

std::string CProject_Base::zeroPadNumber(double num)
{
	char a[255];
	sprintf(a, "%5.4lf", num);
    return a;
}


}//end of namespace lev
