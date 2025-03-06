#include <project_names_builder_v1.h>


namespace lev
{

//===========================================================================================
// class CProject_Name_Builder
//===========================================================================================

//Extrai uma parte de string
std::string CProject_Name_Builder::ExtractString(std::string completestring, std::string what)
{
	int pos;
	int sizetoreplace;
	std::string rest;

	pos = completestring.find(what.c_str(), what.size());

	sizetoreplace = completestring.size()-pos;

	if(pos != -1)
	{
	 //success
		rest = completestring;
		rest.replace(pos, sizetoreplace,"");
	}
	else 
	{
	  //not found
		rest = completestring;
	}

	return rest;

}//end of std::string CProject_Name_Builder::ExtractString(std::string, std::string)


// Substitui a primeira substring encontrada pela string desejada
std::string CProject_Name_Builder::ReplaceString(std::string completestring, std::string replaced,std::string replacer)
{
	int pos = -1;
	int sizetoreplace;

	std::string rest;				
	
	pos = completestring.find(replaced.c_str(), replaced.size());

	sizetoreplace = completestring.size()-pos;

	if(pos != -1)
	{
	 //success
		rest = completestring;
		rest.replace(pos, sizetoreplace,replacer.c_str());
	}
	else 
	{
	  //not found
		rest = completestring;
	}

	return rest;

}//end of std::string CProject_Name_Builder::ExtractString(std::string, std::string)


void CProject_Name_Builder::ExtractPathAndNameForWindowsOS(std::string namewithpath)
{

	int size_project_name, size_project_name_with_path;
	int pos_aux;
	int i,j;

	size_project_name_with_path = namewithpath.size();
	
	//procura `\` a partir do final da string
	pos_aux = namewithpath.rfind('\\');


	//calcula o tamanho do nome do projeto
	size_project_name = size_project_name_with_path - pos_aux;

	//redimensiona o tamanho da string do nome do projeto
	sProjectName.resize(size_project_name+1); 

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
		sProjectName[j++] = namewithpath[i];

	}

	
}//end of void CProject_Name_Builder::ExtractPathAndName(std::string namewithpath)


//===========================================================================================
// class CProject_Name_Builder_v1
//===========================================================================================
//..............................................................................
//
// BuildFileNames
// path: default value == true 
// returns: 0 - success
//..............................................................................
int CProject_Name_Builder_v1::BuildFileNames(std::string namewithpath, bool withPath)
{
	int codeErr = 0;
	std::string nomeProjeto;
	std::string ext;
	std::string nome_propfis = "propfis.mei";

	if(namewithpath == "")
	{
		codeErr = 1;
	}
	else
	{
		sProjectNameWithPath = namewithpath;
		
#ifdef   __GNUGCC__    
		ExtractPathAndNameForLinuxOS(namewithpath);
#else
		ExtractPathAndNameForWindowsOS(namewithpath);
#endif

		//monta nome dos arquivo: propfis.mei
		nomePropfis = "propfis.mei";
		if(withPath == true)
		{
			nomePropfis = sProjectPath+"propfis.mei";;
		}
		
		//retira extensão .PROJ se existir
		nomeProjeto = ExtractString(sProjectNameWithPath,".proj");

		//retira extensão .PRJ se existir
		nomeProjeto = ExtractString(nomeProjeto,".prj");
		

		//*NMA* 27/07/2007: obsoletos
		mod_conexao								= nomeProjeto + ".ell";
		mod_geometria							= nomeProjeto + ".ege";
		mod_coordinates							= nomeProjeto + ".ept";
		mod_pre									= nomeProjeto + ".epd";

		//*NMA* 27/07/2007: em uso
		mod_ell_predis							= nomeProjeto + ".mgp";	
		modelMeshFronteira						= nomeProjeto + ".mmf";
		auxiliar_primitives						= nomeProjeto + ".prx";
		nomePropfis_v                           = nomeProjeto + "_v1.mei";
		boundaryConditionsPrimitivesIndexFile	= nomeProjeto + ".bcp";
		boundaryConditionsObjectsInUseFile		= nomeProjeto + ".bco";
		sourcesObjectsInUseFile					= nomeProjeto + ".src";
		materialsObjectsInUseFile				= nomeProjeto + ".mts";
		regionsFileOld							= nomeProjeto + ".reg";//OBSOLETO
		regionsFile								= nomeProjeto + ".rgn";
		arestasGeometriaFile                    = nomeProjeto + ".arg";
		solverOpticalFiberData					= nomeProjeto + ".fsd";
		solverOpticalWaveData					= nomeProjeto + ".wsd";

		//*NMA* 27/08/2007: arquivos de dados de malha estruturada
		structuredMeshInputData					= nomeProjeto + ".gme";
		structuredMeshEspecifications			= nomeProjeto + ".ime";
		structuredMeshPoints					= nomeProjeto + ".plt";
		subRegionsData							= nomeProjeto + ".srd";
		errorlogfile							= nomeProjeto + ".errorlog";
		dataspec								= nomeProjeto + ".dados";

		BuildFileNamesMeshBased(mesh_type);
	}

	return codeErr;

}//end of BuildArchivesNames

//===========================================================================================
// class CProject_Name_Builder_v1
//===========================================================================================
//..............................................................................
//
// BuildFileNamesMeshBased
// path: default value == true 
// returns: 0 - success
//..............................................................................
int CProject_Name_Builder_v1::BuildFileNamesMeshBased(enum MESHTYPE t)
{
	int codeErr = 0;
	std::string nomeProjeto;
	std::string ext;

	mesh_type = t;

	nomeProjeto = sProjectNameWithPath;

	if(nomeProjeto == "")
	{
		codeErr = 1;
	}
	else
	{
		switch(mesh_type)
		{
		case ARITHMETIC:		ext = "1a";
			break;
		default:
		case STRUCTURED:		ext = "1g";
		case GEOMETRIC:			ext = "1g";
			break;
		case HARMONIC:			ext = "1h";
			break;
		case SQUARED_HARMONIC:	ext = "1q";
			break;
		case SELF_ADAPTIVE:		ext = "1g";//"g1"; *AP* 03/09/2005 o processo não mais pressupoe a necessidade de existência de uma malha inicial
	//		break;

		}

		
		//*NMA* 27/08/2007: arquivos de dados e propriedades físicas
		// em função da malha de elementos finitos
		geometria	= nomeProjeto + ".n" + ext;
		segmentos	= nomeProjeto + ".g" + ext;
		elementos	= nomeProjeto + ".t" + ext;
		cc			= nomeProjeto + ".c" + ext;
		meios		= nomeProjeto + ".m" + ext;	
		fontes		= nomeProjeto + ".f" + ext;	
		potenciais	= nomeProjeto + ".p" + ext;


		//*NMA* 27/08/2007: arquivos gerados pelo gerador de malhas de delaunay
		tiposPontos				= nomeProjeto + ".gpr";
		element_areas			= nomeProjeto + ".areas";
		element_quality			= nomeProjeto + ".qlt";

		//*NMA* 27/08/2007: arquivos de dados de elementos triangulares gravados por aresta
		arestas					= nomeProjeto + ".ar";
		elementosArestas		= nomeProjeto + ".art";

			
	}



	return codeErr;

}


std::string CProject_Name_Builder_v1::GetFileName(enum FILENAME qual)
{
	std::string name="";

	switch(qual) {
		case MODEL_CONNECTION :
			name = mod_conexao;		                // (.ell) obsoleto
			break;
		case MODEL_GEOMETRY:
			name = mod_geometria;	                // (.ege) obsoleto
			break;
		case MODEL_COORDINATES:
			name = mod_coordinates;	                // (.ept) obsoleto
			break;
		case MODEL_PRE:
			name = mod_pre;			                // (.epd) obsoleto
			break;
		case MODEL_PREDIS:
			name = mod_predis;		                // (.pre) obsoleto
			break;
		case GEOMETRY:
			name = geometria;		                // (.n1?)
			break;
		case SEGMENTS:
			name = segmentos;	                    // (.g1?)
			break;
		case ELEMENTS:
			name = elementos;		                // (.t1?)
			break;
		case BOUNDARYCONDITIONS:
			name = cc;			                    // (.c1?)
			break;
		case MODEL_BOUNDARYCONDITIONS:
			name = boundaryConditionsPrimitivesIndexFile;  // (.bcp)
			break;
		case MODEL_BOUNDARYCONDITIONSINUSE:
			name = boundaryConditionsObjectsInUseFile;     // (.bco)
			break;
		case MODEL_SOURCESINUSE:
			name = sourcesObjectsInUseFile;         // (.src)
			break;
		case MODEL_MATERIALINUSE:
			name = materialsObjectsInUseFile;       // (.mts)
			break;
		case MODEL_REGIONS:
			name = regionsFile;                     // (.rgn)
			break;
		case MODEL_REGIONS_REG:
			name = regionsFileOld;                  // (.reg) obsoleto
			break;
		case ARESTASGEOMETRIA:
			name = arestasGeometriaFile;            // (.arg)
			break;
		case MATERIAL:
			name = meios;		                    // (.m1?)
			break;
		case SOURCE:
			name = fontes;			                // (.f1?)
			break;
		case POTENTIAL:
			name = potenciais;		                // (.p1?)
			break;
		case ARESTASGEO:
			name = arestas;			                // (.ar)
			break;
		case ELEMENTOSARESTAS:
			name = elementosArestas;                // (.art)
			break;
		case PROPFISMEI:
			name = nomePropfis;		                // (propfis.mei)
			break;
		case MODEL_GEOMETRY_PREDIS:
			name = mod_ell_predis;                  // (.mgp)
			break;
		case POINT_TYPE:
			name = tiposPontos;			            // (.gpr)
			break;
		case SOLVER_SCALAR_OPTFIBER:
			name = solverOpticalFiberData;          // (.fsd)
			break;
		case SOLVER_SCALAR_OPTWAVE:
			name = solverOpticalWaveData;           // (.wsd)
			break;
		case MODEL_SUBREGIONS:
			name = subRegionsData;	                // (.srd)
			break;
		case STRUCTUREDMESHINPUTDATA:
			name = structuredMeshInputData;         // (.gme)
			break;
		case STRUCTUREDMESHESPECIFICATIONS:
			name = structuredMeshEspecifications;   // (.ime)
			break;
		case STRUCTUREDMESHPOINTS:
			name = structuredMeshPoints;            // (.plt)
			break;
		case MODELMESHFRONTEIRA:
			name = modelMeshFronteira;              // (.mmf)
			break;
		case ERRORLOGFILE:
			name = errorlogfile;                    // (.errorlog)
			break;
		case ELEMENTAREASFILE:
			name = element_areas;                   // (.areas)
			break;
		case ELEMENTQUALITYFILE:
			name = element_quality;                 // (.qlt)
			break;
		case FILEDATASPEC:
			name = dataspec;                        // (.dados)
			break;
		case MODEL_AUXILIARPRIMITIVES:
			name = auxiliar_primitives; //(.prx)
			break;
		default:
			name = ".lev";
			break;
	}
	return name;
}


} // End of namespace lev
