#ifndef __PROJECTNAMESBUILDER_V1__
#define __PROJECTNAMESBUILDER_V1__


#include "string"
#include "levenum.h"


namespace lev
{


class CProject_Name_Builder
{
protected:
	std::string sProjectName;
	std::string sProjectPath;
	std::string sProjectNameWithPath;

	void ExtractPathAndNameForWindowsOS(std::string namewithpath);
	void ExtractPathAndNameForLinuxOS(std::string namewithpath);

	MESHTYPE mesh_type;


public:
    CProject_Name_Builder(){};
	virtual ~CProject_Name_Builder(){};
	std::string			ExtractString(std::string completestring , std::string what);
	std::string			ReplaceString(std::string completestring , std::string replaced,std::string replacer);

	virtual int			BuildFileNames(std::string namewithpath,bool withPath = true)=0;
	virtual int			BuildFileNamesMeshBased(enum MESHTYPE t)=0;
	virtual std::string GetFileName(enum FILENAME) = 0;


	void SetMeshType(enum MESHTYPE t) {mesh_type = t;}

};//end of class CProject_Name_Builder


class CProject_Name_Builder_v1 : public CProject_Name_Builder
{
protected:
	//attributes
	////////////////////////////////////////////////////////////////////////////////////
	// *NMA* : Vari�veis para armazenar nome dos arquivos de dados no padr�o antigo
	// Arquivos de dados do modelador geom�trico
	std::string mod_conexao,		//(.ell)
				mod_geometria,		//(.ege)
				mod_coordinates,	//(.ept) obs: mod_interrogation
				mod_pre,			//(.epd)
				mod_predis;			//(.pre) //obsoleto

	// Arquivos para os outros m�dulos FEM
	std::string geometria,			//(.n1?)
				segmentos,			//(.g1?)
				elementos,			//(.t1?)
				cc,					//(.c1?)
				meios,				//(.m1?)
				fontes,				//(.f1?)
				potenciais;			//(.p1?)

	//Arquivo para gera��o de malha auto-adaptativa
	std::string	tiposPontos;		//(.gpr)

	// Propfis.mei
	std::string nomePropfis;		//(propfis.mei)
	std::string nomePropfis_v;		//(_v{x}.mei)


	std::string arestas,			//(.ar)
				elementosArestas,	//(.art)
				mod_ell_predis;		//(.mgp) // modelo geom�trico + pre-dis

	std::string auxiliar_primitives; //(.prx) auxiliar primitives

	//gerador de malhas
	std::string element_areas,		//(.areas)
				element_quality;	//(.qlt)

	// Arquivo com os �ndices de primitivas com condi��o de contorno e indice da condi��o de contorno
	std::string boundaryConditionsPrimitivesIndexFile;

	// Arquivo com objetos condi��o de contorno em uso
	std::string boundaryConditionsObjectsInUseFile;

	// Arquivo com objetos source em uso
	std::string sourcesObjectsInUseFile;

	// Arquivo com objetos material em uso
	std::string materialsObjectsInUseFile;

	// Arquivo com as regi�es (inclui indice de material e source, caso existam)
	std::string regionsFile; //.rgn
	std::string regionsFileOld;//.reg (obsoleto)

	// Arquivo com as arestas de geometria
	std::string arestasGeometriaFile;

	//Arquivo com dados para solver de fibras em matlab
	std::string solverOpticalFiberData;

	//Arquivo com dados para solver de guias de onda em matlab
	std::string solverOpticalWaveData;

	//Arquivo para gerador de malha estruturada
	std::string structuredMeshInputData; //(.gme) dados de entrada para o gerador de malha estruturada
	std::string structuredMeshEspecifications; //(.ime) dados de entrada para o gerador de malha estruturada
	std::string structuredMeshPoints; //(.plt) pontos gerador de malha estruturada

	//Arquivo com dados de sub-regi�es e fronteiras
	std::string	subRegionsData; //(.srd)

	// Arquivo com dados de malha de fronteira para identifica��o de regi�es fechadas
	// do modelo geom�trico
	std::string modelMeshFronteira; //(.mmf)

	std::string errorlogfile; //(.errorlog)

	std::string dataspec; //(*.dados)

public:

	virtual int			BuildFileNames(std::string namewithpath,bool withPath = true);
	virtual int			BuildFileNamesMeshBased(enum MESHTYPE t);///<Monta nome dos arquivos dependentes do tipo de malha
	virtual std::string GetFileName(enum FILENAME);


};//end of class CProject_Name_Builder_v1


}//end of namespace lev


#endif
