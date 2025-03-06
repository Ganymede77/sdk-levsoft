//===================================================================================================
//	Arquivo:			LevGeoPhysData.h
//...................................................................................................
//	Descrição:			Implementação da classe CLevPhysData
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//...................................................................................................
//	Observações:
//
//===================================================================================================

#include "levcolorsRGB.h"
#include "levcolorsRGB_pallete.h"
#include "levphysdata.h"
//#include <boost/interprocess/sync/file_lock.hpp>



CLevPhysData::CLevPhysData()
{
	oGroupBoundaryConditions. Reinitialize();

	isBoundaryConditionModified = lev_enum::NO;
	isMaterialModified			= lev_enum::NO;
	isSourceModified			= lev_enum::NO;

	pModel = NULL;
	readpropfisOk = false;

    NELT = 1;
    NCCOR =1 ;
    NMAG = 5;

	codigos = new int [10000];
	if(!codigos) {
		std::cout << "\nDelaunay2D_AutoAdaptativo::Le_BD_Materiais(): memory allocation error for codigos\n";
		exit(0);
	}
	materiais = new double* [10000];
	if(!materiais) {
		std::cout << "\nDelaunay2D_AutoAdaptativo::Le_BD_Materiais(): memory allocation error for materiais\n";
		exit(0);
	}
	for(int i=0; i<10000; i++) {
		materiais[i] = new double [7];
		if(!materiais[i]) {
			std::cout << "\nDelaunay2D_AutoAdaptativo::Le_BD_Materiais(): memory allocation error for materiais[i]\n";
			exit(0);
		}
		memset(&materiais[i][0], 0, sizeof(double)*7);
	}
}


CLevPhysData::~CLevPhysData()
{
	FreeMemory();
	if(codigos) {
		delete [] codigos; codigos = NULL;
	}
	for(int i=0; i<10000; i++) {
		if(materiais[i]) {
			delete [] materiais[i]; materiais[i] = NULL;
		}
	}
	if(materiais) {
		delete [] materiais; materiais = NULL;
	}
}


void CLevPhysData::AssociateGeoModel(CLevGeoData * pm)
{
	pModel = pm;
}


void CLevPhysData::FreeMemory(void)
{
	vBoundaryConditions.clear();
	oGroupBoundaryConditions.Reinitialize();
	vMaterialDB.clear();
	vMaterialInUse.clear();
	vSourcesInUse.clear();
}


void CLevPhysData::Reinitialize(void)
{ }


CLevPhysData & CLevPhysData :: operator = (const CLevPhysData & d)
{
	int i,j;

	if (this != &d)
	{
		NCCOR				= d.NCCOR;
		NELT				= d.NELT;
		NMAG				= d.NMAG;

		for(i=0;i<40;i++) codigos[i] = d.codigos[i]	;

		isBoundaryConditionModified = d.isBoundaryConditionModified;
		isMaterialModified			= d.isMaterialModified;
		isSourceModified			= d.isSourceModified;

		for(i=0;i<40;i++)
		{
			for(j=0;j<7;j++)
				materiais[i][j] = d.materiais[i][j];
		}

		vBoundaryConditions		= d.vBoundaryConditions;
		vMaterialDB				= d.vMaterialDB;
		vMaterialInUse			= d.vMaterialInUse;
		vSourcesInUse			= d.vSourcesInUse;

		oGroupBoundaryConditions = d.oGroupBoundaryConditions;
	}

	return *this;
}

#if 0 //*NMA: não parece ter sentido um operator de comparação
bool CLevPhysData :: operator == ( CLevPhysData & d)
{
	int i,j;
	bool ret = true;

//	if (status && d.status)
	{
		if(NCCOR != d.NCCOR) ret = false;

		if(ret == true) if(NELT != d.NELT) ret = false;
		if(ret == true) if(NMAG	!= d.NMAG) ret = false;

		if(ret == true)
		{
			for(i=0;i<40 && ret == true;i++)
			{
				if(codigos[i] != d.codigos[i]) ret = false;
			}
		}

		for(i=0;i<40 && ret == true;i++)
		{
			for(j=0;j<7 && ret == true;j++)
			{
				if(materiais[i][j] != d.materiais[i][j]) ret = false;
			}
		}
#if 0
		if(ret == true) if(vBoundaryConditions	!= d.vBoundaryConditions) ret = false;
		if(ret == true) if(vMaterialDB			!= d.vMaterialDB) ret = false;
		if(ret == true) if(vMaterialInUse		!= d.vMaterialInUse) ret = false;
		if(ret == true) if(vSourcesInUse		!= d.vSourcesInUse) ret = false;

		if(ret == true) if(oGroupBoundaryConditions == d.oGroupBoundaryConditions) ret = true;
						else ret = false;
#endif
	}

	return ret;
}
#endif

//-----------------------------------------------------------------------------------------------------
//	GetBoundaryConditions()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Devolve cópia de vetor de objetos CBoundCond (condições de contorno)
//
//	parâmetros:	vCLevBoundCond & (definido em LevTypeDe.h)
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevPhysData::GetBoundaryConditions(vCLevBoundCond & vBC)
{
	vBC = vBoundaryConditions;
}
////// Devolve o numero total de componentes do vetor de condições de contorno
int CLevPhysData::GetNumBoundaryConditions(void)
{
	return vBoundaryConditions.size();
}


////// Devolve o numero de condições de contorno válidas
int CLevPhysData::GetNumValidBoundaryConditions(void)
{
	unsigned int i;
	int pValid = 0;
	for(i=0;i<vBoundaryConditions.size();i++)
		if(vBoundaryConditions[i].Type() != INVALIDBC) pValid++;
	return pValid;
}



//-----------------------------------------------------------------------------------------------------
//	IncludeBoundaryCondition()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Inclui condição de contorno nas primitivas do grupo passado e
//				atualiza a lista de condições de contorno em uso
//
//	parâmetros:	CLevGroup &oGroupBC : grupo com as primitivas que devem receber a condição de contorno
//										(índice no vetor das condições de contorno em uso)
//
//				CBoundCond & oBoundCond: a condição de contorno que deve ser incluída no vetor
//											vBoundaryConditions e cujo índice neste vetor será atribuido
//											à primitiva.
//
//	retorno:	  -1 : o modelo não foi associado
//				>= 0 : o índice da condição de contorno no vetor vBoundaryConditions
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
long CLevPhysData::IncludeBoundaryCondition(CLevGroup &oGroupBC, CBoundCond & oBoundCond)
{
	unsigned int i, j;
	long		indexBC = -1;
	int			pos, indexbond;
	bool		existecond = false, incluicond = true;

	if(!pModel) //o modelo não foi associado
		return -1;

	if(oGroupBC.vpPrimitives.size() || oGroupBC.vpPoints.size())
	{
		 // o grupo so tem uma primitiva: quando a atribuição é feita pelo modelador com interface
		 //*NMA*:14/04/2005:	para o caso da otimização, existe mais de uma primitiva
		 //						(todas as primitivas que fecham uma região)
		 // as condições de Neumann, convection e PEC não são atribuídas a pontos (são atribuídas a arestas dos elementos finitos)
		 // *NMA*: não consegui enxergar outra maneira de evitar a inclusão,
		 //			 com a estrutura atual de inclusão de cc
		 if(oGroupBC.vpPoints.size())
		 {
			 if(oBoundCond.Type() == NEUMANNBC || oBoundCond.Type() == CONVECTIONBC || oBoundCond.Type() == PECBC)
			 {
				 incluicond = false;
			 }

		 }//fecha if(oGroupBC.vpPoints.size())

		if(incluicond)
		{
			indexBC = IncludeInBoundaryConditionList(oBoundCond);

			if(indexBC >= 0 )
			{
				// inclui condição de contorno nas primitivas do grupo
				for(i=0;i<oGroupBC.vpPrimitives.size(); i++)
				{
					pos = pModel->GetPosition(oGroupBC.vpPrimitives[i]);

					//*NMA*:obtém indice da condição de contorno anterior
					indexbond = pModel->vpPrimitives[pos]->GetBoundaryCondition();
					pModel->vpPrimitives[pos]->SetBoundaryCondition(indexBC);
					oGroupBoundaryConditions.IncludeNextPrimitive(pModel->vpPrimitives[pos]);
					//break;

				  // Verifica se a condição de contorno aplicada já existia
				  // e verifica se ela ainda existe. Se a condição nao existir mais
				  // em nenhuma primitiva o algoritmo invalida a condição de contorno antiga
				//___________________________________________________________________________
					existecond = false;
					if(indexbond!=UNDEFINED)
					{
						for(j=0;j<pModel->vpPrimitives.size();j++)
						{
							if (pModel->vpPrimitives[j]->GetBoundaryCondition()== indexbond)
							{
								existecond = true;
							}
						}


						for(j=0;j<pModel->vpPoints.size();j++)
						{
							if (pModel->vpPoints[j]->GetBoundaryCondition()== indexbond)
							{
								existecond = true;
							}
						}

						if(!existecond)
						{
							vBoundaryConditions[indexbond].Type(INVALIDBC);

						}
					}//fecha if(indexbond!=UNDEFINED)
				}//fecha for(i=0;i<oGroupBC.vpPrimitives.size(); i++)
			//___________________________________________________________________________
			}//fecha if(indexBC >= 0 )
		} //fecha if(incluicond)
	}//fecha if(oGroupBC.vpPrimitives.size() || oGroupBC.vpPoints.size())

	IsBoundaryConditionModified(lev_enum::YES);
	return indexBC;

}//end of IncludeBoundaryCondition

//-----------------------------------------------------------------------------------------------------
//	RemoveBoundaryCondition()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Remove a condição de contorno nas primitivas do grupo passado e
//				atualiza a lista de condições de contorno em uso
//
//	parâmetros:	CLevGroup &oGroupBC : grupo com as primitivas nas quais devem ser
//										removidas a condição de contorno
//
//	retorno:	  -1 : o modelo não foi associado
//				   0 : sucesso
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//---------------------------------------------------------------------------------------------------
int CLevPhysData::RemoveBoundaryCondition(CLevGroup &oGroupBC)
{

	unsigned int i,j;
	long	index = -1;
	bool	mantemCondicaoNaLista = false;
	int		pos;

	// varre as primtivas do grupo (que guarda só uma  primitiva, quando a remoção é feita pelo modelador com interface )
	//  para obter o índice remove a condição da primitiva
	 //*NMA*:14/04/2005:	para o caso da otimização, existe mais de uma primitiva
	 //						(todas as primitivas que fecham uma região)


	if(!pModel) // se o modelo não foi associado
		return -1;

	for(i=0;i<oGroupBC.vpPrimitives.size(); i++)
	{
		pos = pModel->GetPosition(oGroupBC.vpPrimitives[i]);

		// se não existir boundary condition, retorna UNDEFINED
		index = pModel->vpPrimitives[pos]->GetBoundaryCondition();
		pModel->vpPrimitives[pos]->SetBoundaryCondition(UNDEFINED);

		if(index != UNDEFINED && index < (signed)vBoundaryConditions.size())
		{
			for(j=0;j<pModel->vpPoints.size() && mantemCondicaoNaLista == false ;j++)
				if(pModel->vpPoints[j]->GetBoundaryCondition() == index)
					mantemCondicaoNaLista = true;

			for(j=0;j<pModel->vpPrimitives.size() && mantemCondicaoNaLista == false ;j++)
				if(pModel->vpPrimitives[j]->GetBoundaryCondition() == index)
					mantemCondicaoNaLista = true;

			if(mantemCondicaoNaLista == false)
				vBoundaryConditions[index].Type(INVALIDBC);
		}
	}

	for(i=0;i<oGroupBC.vpPoints.size() ; i++)
	{
		pos		= pModel->GetPosition(oGroupBC.vpPoints[i]);
		index	= pModel->vpPoints[pos]->GetBoundaryCondition();
		pModel->vpPoints[pos]->SetBoundaryCondition(UNDEFINED);

		if(index != -1 && index < (signed)vBoundaryConditions.size())
		{
			for(j=0;j<pModel->vpPoints.size() && mantemCondicaoNaLista == false ;j++)
				if(pModel->vpPoints[j]->GetBoundaryCondition() == index)
					mantemCondicaoNaLista = true;

			for(j=0;j<pModel->vpPrimitives.size() && mantemCondicaoNaLista == false ;j++)
				if(pModel->vpPrimitives[j]->GetBoundaryCondition() == index)
					mantemCondicaoNaLista = true;

			if(mantemCondicaoNaLista == false)
				vBoundaryConditions[index].Type(INVALIDBC);
		}
	}

	isBoundaryConditionModified = lev_enum::YES;

	return 0; //sucesso
}



//*NMA*:Inclui condição de contorno na lista de condições de contorno
// retorno : retorno = -1, se não conseguiu incluir, significa uma inconsistência
//			 retorno >= 0: posição da condição de contorno
//
// Obs: verifica se a condição de contorno já estava na lista
long CLevPhysData::IncludeInBoundaryConditionList(CBoundCond & oBoundCond)
{
	unsigned long i;
	long	position = -1;
	bool	isInList = false;

	//verifica se a condição já está na lista de condições de contorno
	for(i=0;i<vBoundaryConditions.size() && isInList==false;i++)
	{
		if(vBoundaryConditions[i] == oBoundCond)
		{
			isInList = true;
			position = i;
		}
	}

	// inclui a condição se não estiver na lista
	if(!isInList)
	{
		vBoundaryConditions.push_back(oBoundCond);
		position = vBoundaryConditions.size()-1;
	}

	return position;
}


#define MAXCOLOR 16 //*NMA* : quantidade de cores definida no vetor colors


static long colors[] = { 12632256 , 12639424 , 128      , 32768    ,
		                 32896    , 8388608  , 8388736  , 8421376  ,
		                 255      , 65280    , 65535    , 16711680 ,
		                 16711935 , 16776960 , 14438415 , 3381659 };


//*NMA* OBSOLETO ********************************************************************************************
// substituida por ReadPropFis01
//*NMA* OBSOLETO ********************************************************************************************
int CLevPhysData::ReadPropFis(std::string nomePropFis)
{
	FILE *parqPROPFIS = NULL;
	int i;
	unsigned int j;
	char nome_temp[250];
	//long matColor;
	CLEV_ColorRGB matColor;
	int	indColor = 0;
	CMaterial materialTemp, materialRead;

	//======================= le arquivo propfis.mei ===========================
	vMaterialDB.clear();

	//boost::interprocess::file_lock flock("propfis.mei");

	//flock.lock();

	parqPROPFIS = fopen(nomePropFis.c_str(),"r");

	fscanf(parqPROPFIS, "%d %d %d %d", &NM, &NELT, &NCCOR, &NMAG);

	for(i=0;i<NM;i++) {
		fscanf(parqPROPFIS,"\n%d %s %lg %lg %lg %lg %lg %lg %lg",
										 &codigos[i],
										 nome_temp,
										 &materiais[i][0],
										 &materiais[i][1],
										 &materiais[i][2],
										 &materiais[i][3],
										 &materiais[i][4],
										 &materiais[i][5],
										 &materiais[i][6]);

		//*NMA*: 19/09/2002
		// Por enquanto só guarda o nome e o código no
		// objeto material armazenado, pois são esses os
		// dados utilizados na modelagem física
		// Substituir a classe CMaterial simplificada pela nova estrutura!

		materialTemp.Set_Name(nome_temp);
		materialTemp.Set_Codigo(codigos[i]);

		materialTemp.Status(LEV_VALID);

		indColor++;
		if(indColor > MAXCOLOR)
			indColor = 0;

		matColor = colors[indColor];

		if(GetNumValidMaterialsInUse())
			for(j=0;j<vMaterialInUse.size();j++)
				if(vMaterialInUse[j].Status())
				{
					materialRead = vMaterialInUse[j];
					if( vMaterialInUse[j] == materialTemp)
						matColor = vMaterialInUse[j].Get_Color();
				}

		materialTemp.Set_Color(matColor);

		// *NMA: para facilitar fravaçao de materiais em uso, coloquei um vetor double materiais em cada material
		materialTemp.material[0] = materiais[i][0];
		materialTemp.material[1] = materiais[i][1];
		materialTemp.material[2] = materiais[i][2];
		materialTemp.material[3] = materiais[i][3];
		materialTemp.material[4] = materiais[i][4];
		materialTemp.material[5] = materiais[i][5];
		materialTemp.material[6] = materiais[i][6];

		vMaterialDB.push_back(materialTemp);
	}
	fclose(parqPROPFIS);

	//flock.unlock();

	return (0);
}

// *NMA*: 2015-3-02: Substitui ReadPropFis e ReadPropFis01
// Alteracoes:
//  - elimina necessidade da matriz dimensionada estaticamente materiais[40][7]
//  - realiza a leitura somente do codigo e do nome do material
//................................................................................................
int CLevPhysData::ReadPropFis02(std::string nomePropFis)
{
//	FILE		*parqPROPFIS = NULL;

	std::cout << "\nint CLevPhysData::ReadPropFis02(std::string nomePropFis)"; std::cout.flush();

	int			i, j, k;
	int			NM; //n�mero de materiais no arquivo propfis.mei
	char		nome_temp[2500];
	std::vector<double> mat_aux; //*NMA* auxilizr para leitura das propriedades
	double		aux=0.;
	int			codigo;
	int			NTOTAL;
	int			indColor =0;
	CLEV_ColorRGB matColor;


	CMaterial	materialTemp, materialRead;
	CMaterial	materialHOLE;


	materialHOLE.Set_Name("hole-buraco");
	materialHOLE.Set_Codigo(-1);
	materialHOLE.Set_Color( CLEV_ColorRGB(0,0, 0) );
	materialHOLE.Status(LEV_VALID);




//======================= le arquivo propfis.mei ===========================

	vMaterialDB.clear();

	vMaterialDB.push_back(materialHOLE);

	//std::cout << "\nint CLevPhysData::ReadPropFis02 nomePropFis =  " << nomePropFis.c_str(); std::cout.flush(); std::cout.flush();

	propfismei_lines.clear();

	std::ifstream f(nomePropFis.c_str());
	std::string line;
	std::string col1, col2;


	std::getline(f, line);

	//guarda linha no vetor de strings
	propfismei_lines.push_back(line);

	//obtem valor de NM: numero de materiais do arquivo
	std::istringstream ss(line);
	ss >> col1;

	NM = atoi( col1.c_str() );

#if 0
	std::cout << "\nvCLevPhysData::ReadPropFis02: line = " << line.c_str(); std::cout.flush(); std::cout.flush();
	std::cout << "\nvCLevPhysData::ReadPropFis02: col1 = " << col1.c_str(); std::cout.flush(); std::cout.flush();
	std::cout << "\nvCLevPhysData::ReadPropFis02: NM = " << NM; std::cout.flush(); std::cout.flush();
#endif

	  for(i=0;i<NM;i++)
	  {

		  std::getline(f, line);
 		 //guarda linha no vetor de strings
		 propfismei_lines.push_back(line);

		  std::istringstream ss(line);
		  ss >> col1 >> col2;

			codigo = atoi( col1.c_str() );

#if 10
			std::cout << "\nvCLevPhysData::ReadPropFis02: line = " << line.c_str(); std::cout.flush(); std::cout.flush();
			std::cout << "\nvCLevPhysData::ReadPropFis02: col1 = " << col1.c_str(); std::cout.flush(); std::cout.flush();
			std::cout << "\nvCLevPhysData::ReadPropFis02: col2 = " << col2.c_str(); std::cout.flush(); std::cout.flush();

			std::cout << "\nvCLevPhysData::ReadPropFis02: codigo = " << codigo; std::cout.flush(); std::cout.flush();
#endif
			materialTemp.Set_Name(col2.c_str());
			materialTemp.Set_Codigo(codigo);
			materialTemp.Status(LEV_VALID);

			indColor++;
			if(indColor > MAXCOLOR_LEVRGB)
			{
				indColor = 0;
			}

			matColor = colors_LEVRGB[indColor];

			if(GetNumValidMaterialsInUse())
			{

				for(j=0;j<vMaterialInUse.size();j++)
				{

					if(vMaterialInUse[j].Status())
					{
						materialRead = vMaterialInUse[j];

						//std::cout << "\n\nvCLevPhysData::ReadPropFis02: materialTemp.Get_Name().c_str() " << materialTemp.Get_Name().c_str(); std::cout.flush(); std::cout.flush();
						//std::cout << "\nvCLevPhysData::ReadPropFis02: materialRead.Get_Name().c_str() " << materialRead.Get_Name().c_str(); std::cout.flush(); std::cout.flush();

//						if( materialRead == materialTemp)
						if( materialRead.Get_Codigo() == materialTemp.Get_Codigo())
						{
							//std::cout << "\nvCLevPhysData::ReadPropFis02: achou material igual = " << j; std::cout.flush(); std::cout.flush();
							matColor = vMaterialInUse[j].Get_Color();
						}
					}
				}

			}

			//materialTemp.Set_Color(matColor);
			int cor_red = matColor.getRedCode();
			int cor_green = matColor.getGreenCode();
			int cor_blue = matColor.getBlueCode();

			 materialTemp.Set_Color( CLEV_ColorRGB(cor_red,cor_green, cor_blue) );

			vMaterialDB.push_back(materialTemp);
	  }

	f.close();


 return (0);

}// end of int CLevPhysData::ReadPropFis02(std::string nomePropFis)

//Substituido por ReadPropFis02
// *NMA*: 2015-3-26: Substitui ReadPropFis
// Alteracoes:
//  - elimina necessidade da matriz dimensionada estaticamente materiais[40][7]
//  - realiza a leitura das propriedades, considerando a somatoria de NELT, NCCOR e NMAG (lembrar que se forem colocados valores consistentes, pode se colocar quantas propriedades quiser na linha
//................................................................................................
int CLevPhysData::ReadPropFis01(std::string nomePropFis)
{
	FILE		*parqPROPFIS = NULL;

	int			i, j, k;
	int			NM; //n�mero de materiais no arquivo propfis.mei
	char		nome_temp[250];
	std::vector<double> mat_aux; //*NMA* auxilizr para leitura das propriedades
	double		aux=0.;
	int			codigo;
	int			NTOTAL;
	int			indColor =0;
	CLEV_ColorRGB matColor;


	CMaterial	materialTemp, materialRead;

//======================= le arquivo propfis.mei ===========================


	std::cout << "\nint CLevPhysData::ReadPropFis01 nomePropFis =  " << nomePropFis.c_str(); std::cout.flush(); std::cout.flush();

	parqPROPFIS = fopen(nomePropFis.c_str(),"r");
	if (parqPROPFIS == NULL)
	{
//		Mensagem_erro("erro na abertura do arquivo propfis.mei");
//		Ready = NNAO;
		return 102;
	}


  fscanf(parqPROPFIS, "%d %d %d %d", &NM, &NELT, &NCCOR, &NMAG);

  NTOTAL = NELT+NCCOR+NMAG;

  mat_aux.clear();
  materialTemp.material.clear();

 //std::cout << "\nvCLevPhysData::ReadPropFis01: GetNumValidMaterialsInUse() = " << GetNumValidMaterialsInUse(); std::cout.flush(); std::cout.flush();

  for(i=0;i<NM;i++)
  {

		fscanf(parqPROPFIS,"\n%d %s",&codigo, nome_temp);

		materialTemp.Set_Name(nome_temp);
		materialTemp.Set_Codigo(codigo);
		materialTemp.Status(LEV_VALID);

		mat_aux.clear();
		for(j=0;j<NTOTAL;j++)
		{
			fscanf(parqPROPFIS,"%lg", &aux);
			mat_aux.push_back(aux);
		}

		materialTemp.material.clear();
		for(k=0;k<mat_aux.size();k++)
		{
			materialTemp.material.push_back(mat_aux[k]);
		}

		indColor++;
		if(indColor > MAXCOLOR_LEVRGB)
		{
			indColor = 0;
		}

		matColor = colors_LEVRGB[indColor];

		if(GetNumValidMaterialsInUse())
		{

			for(j=0;j<vMaterialInUse.size();j++)
			{

				if(vMaterialInUse[j].Status())
				{
					materialRead = vMaterialInUse[j];

					//std::cout << "\n\nvCLevPhysData::ReadPropFis01: materialTemp.Get_Name().c_str() " << materialTemp.Get_Name().c_str(); std::cout.flush(); std::cout.flush();
					//std::cout << "\nvCLevPhysData::ReadPropFis01: materialRead.Get_Name().c_str() " << materialRead.Get_Name().c_str(); std::cout.flush(); std::cout.flush();

					if( materialRead == materialTemp)
					{
						//std::cout << "\nvCLevPhysData::ReadPropFis01: achou material igual = " << j; std::cout.flush(); std::cout.flush();
						matColor = vMaterialInUse[j].Get_Color();
					}
				}
			}

		}

		//materialTemp.Set_Color(matColor);
		int cor_red = matColor.getRedCode();
		int cor_green = matColor.getGreenCode();
		int cor_blue = matColor.getBlueCode();

		 materialTemp.Set_Color( CLEV_ColorRGB(cor_red,cor_green, cor_blue) );

		vMaterialDB.push_back(materialTemp);
  }

  fclose(parqPROPFIS);

 return (0);

}// end of int CLevPhysData::ReadPropFis01(std::string nomePropFis)


//*NMA* OBSOLETO ********************************************************************************************
// substituida por SavePropFis01
//*NMA* OBSOLETO ********************************************************************************************
int CLevPhysData::SavePropFis(std::string nomePropFis)
{
	FILE		*parqPROPFIS = NULL;

	int			i;
	int			NM; //número de materiais no arquivo propfis.mei
	std::string     nome_temp;

	CMaterial	materialTemp;

//======================= grava arquivo propfis.mei ===========================

  std::cout << "\nem int CLevPhysData::SavePropFis(std::string nomePropFis): nomePropFis.c_str()= " << nomePropFis.c_str(); std::cout.flush();
  std::cout.flush();

	do {
		parqPROPFIS = fopen(nomePropFis.c_str(),"w");
		//		if (parqPROPFIS == NULL)
		//		{
		//			//		Mensagem_erro("erro na abertura do arquivo propfis.mei");
		//			//		Ready = NNAO;
		//			return 103;
		//		}
	} while(parqPROPFIS == NULL);

  NM = vMaterialDB.size();

  std::cout << "\nem int CLevPhysData::SavePropFis(std::string nomePropFis): NM= " << NM; std::cout.flush();
  std::cout.flush();
  fprintf(parqPROPFIS, "%d %d %d %d", NM, NELT, NCCOR, NMAG);

  for(i=0;i<NM;i++)
  {

	  nome_temp = vMaterialDB[i].Get_Name();

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

// *NMA*: 2015-3-26: Substitui SavePropFis
// Alteracoes:

int CLevPhysData::SavePropFis01(std::string nomePropFis)
{
	FILE		*parqPROPFIS = NULL;

	int				i, j;
	int				NM; //n�mero de materiais no arquivo propfis.mei
	int				codigo;
	std::string     nome_temp;
	CMaterial		materialTemp;
	double			prop;

//======================= grava arquivo propfis.mei ===========================

  std::cout << "\nem int CLevPhysData::SavePropFis01(std::string nomePropFis): nomePropFis.c_str()= " << nomePropFis.c_str(); std::cout.flush();
  std::cout.flush();

	parqPROPFIS = fopen(nomePropFis.c_str(),"w");
	if (parqPROPFIS == NULL)
	{
		return 103;
	}

  NM = vMaterialDB.size();

  std::cout << "\nem int CLevPhysData::SavePropFis01(std::string nomePropFis): NM= " << NM; std::cout.flush();
  std::cout.flush();
  fprintf(parqPROPFIS, "%d %d %d %d", NM, NELT, NCCOR, NMAG);

  for(i=0;i<NM;i++)
  {

	  nome_temp = 	vMaterialDB[i].Get_Name();
	  codigo = 		vMaterialDB[i].Get_Codigo();

	  fprintf(parqPROPFIS,"\n%d %s", codigo, nome_temp.c_str());

	  for(j=0;j<vMaterialDB[i].material.size();j++)
	  {
		  prop = vMaterialDB[i].material[j];
		  fprintf(parqPROPFIS,"\t%lg", prop);
	  }

  }

  fclose(parqPROPFIS);

 return (0);

}//end of CLevPhysData::SavePropFis01(std::string nomePropFis)

//=================================================================================================
/// Grava o arquivo propfis.mei a partir do vetor de strings que armazena as linhas do arquivo lido
//
//  Substitui SavePropFis e SavePropFis01
//=================================================================================================
int CLevPhysData::SavePropFis02(std::string nomePropFis)
{

	std::cout << "\nint CLevPhysData::SavePropFis02(std::string nomePropFis)"; std::cout.flush();

	int i;

	std::ofstream fout(nomePropFis.c_str());

	for(i=0;i< propfismei_lines.size();i++)
	{
		fout << propfismei_lines[i].c_str() << '\n';
	}

	fout.close();


}//fim de int CLevPhysData::SavePropFis02(std::string nomePropFis)


int CLevPhysData::Get_Num_Materials()
{

	return vMaterialDB.size();
}

#if 0
//retorna nome de material
// indice: posiçao no vetor materials
std::string CLevPhysData::Get_Material_Name_Color(int indice, long &cor)
{
	std::string nome;

	if(indice >= (signed)vMaterialDB.size() || indice <0)
	{
		nome = "";
		rgb2int(255, 255, 255, cor);
	} else {
		nome = vMaterialDB[indice].Get_Name();
		cor = vMaterialDB[indice].Get_Color();;
	}
	return nome;
}
#endif

//retorna nome de material
// indice: posi�ao no vetor materials
std::string CLevPhysData::Get_Material_Name_Color(int indice, CLEV_ColorRGB &cor)
{
	CLEV_ColorRGB WHITE = CLEV_ColorRGB(255, 255,255);

	std::string nome;

#ifdef __NMA_debug_ASSOCIATE_MATERIAL__ //definido em levmainwin_qt.h
    std::cout << "\n em CLevPhysData:::Get_Material_Name_Color: indice = " << indice;   std::cout.flush();   std::cout.flush();
    std::cout << "\n em CLevPhysData:::Get_Material_Name_Color:  vMaterialDB.size() = " <<  vMaterialDB.size();   std::cout.flush();   std::cout.flush();
#endif

	if(indice >= vMaterialDB.size() || indice <0)
	{
		nome = "";
		cor = WHITE;
	}
	else
	{
		nome = vMaterialDB[indice].Get_Name();

		cor = vMaterialDB[indice].Get_Color();;
	}

   //std::cout << "\n em CLevPhysData:::Get_Material_Name_Color: nome = " << nome;   std::cout.flush();   std::cout.flush();
   //std::cout << "\n em CLevPhysData:::Get_Material_Name_Color:  red = " <<  cor.getRedCode() << "  green=" << cor.getGreenCode() << " blue=" << cor.getBlueCode();   std::cout.flush();   std::cout.flush();

	return nome;

}//end of std::string CLevPhysData::Get_Material_Name_Color(int indice, QColor &cor)


void CLevPhysData::GetMaterialsInUse(vCLevMaterial & v)
{
	v = vMaterialInUse;
}


void CLevPhysData::GetMaterialsDB(vCLevMaterial & v)
{
	v = vMaterialDB;
}

//-----------------------------------------------------------------------------------------------------
//	AssociateMaterialToRegion()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Associa material a uma região
//				Atualiza a lista de materiais em uso
//
//	parâmetros:
//		int		regionIndex : índice da região
//		long	  matIndex  : índice do material no vetor vMaterialDB (mesmo índice (posição) no propfis.mei)
//		COLORREF  matCor	: cor do material
//		CLevPoint &o_point	: ponto dentro da região, que a identificou
//		double    angle     : angulo no caso de ímãs permanentes
//
//	retorno:	  -1 : o modelo não foi associado
//				   0 : sucesso
//
//	Observações:
//		Chama a sobrecarga que não recebe a cor do material.
//		Geralmente, só será usada pelo modelador gráfico.
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//---------------------------------------------------------------------------------------------------
#if 0
int CLevPhysData::AssociateMaterialToRegion(int regionIndex,long matIndex, long matCor, CLevPoint &o_point, double angle)
{

	if(!pModel) // o modelo não foi associado
		return -1;

	if(regionIndex != -1) //  Região existe
	{
		vMaterialDB[matIndex].Set_Color(matCor);

		AssociateMaterialToRegion(regionIndex,matIndex, o_point,angle);

	}

	return 0;
}//end of AssociateMaterialToRegion
#endif
int CLevPhysData::AssociateMaterialToRegion(int regionIndex,long matIndex, CLEV_ColorRGB matCor, CLevPoint &o_point, double angle)
{

	if(!pModel) // o modelo n�o foi associado
		return -1;

	std::cout << "\nCLevPhysData::AssociateMaterialToRegion(int regionIndex,long matIndex, CLEV_ColorRGB matCor, CLevPoint &o_point, double angle)";
	std::cout << "\nCLevPhysData::AssociateMaterialToRegion regionIndex,  matIndex  = " << regionIndex << ", " << matIndex ;

	if(regionIndex != -1) //  Regi�o existe
	{
		if(matIndex != -1)
		vMaterialDB[matIndex].Set_Color(matCor);

		AssociateMaterialToRegion(regionIndex,matIndex, o_point,angle);

	}

	return 0;


}//end of AssociateMaterialToRegion


//-----------------------------------------------------------------------------------------------------
//	AssociateMaterialToRegion() - sobrecarga
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Associa material a uma região
//				Atualiza a lista de materiais em uso
//
//	parâmetros:
//		int		regionIndex : índice da região
//		long	  matIndex  : índice do material no vetor vMaterialDB (mesmo índice (posição) no propfis.mei)
//		CLevPoint &o_point	: ponto dentro da região, que a identificou
//		double    angle     : angulo no caso de ímãs permanentes
//
//	retorno:	  -1 : o modelo não foi associado
//				   0 : sucesso
//
//	Observações:
//				Retirada do parâmetro  matCor	(cor do material) para compatibilidade com os
// programas de otimização que necessitam alterar o material da região sem passar a cor.
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//---------------------------------------------------------------------------------------------------
int CLevPhysData::AssociateMaterialToRegion(int regionIndex,long matIndex, CLevPoint &o_point, double angle)
{

	unsigned int i;
	long	matInUseIndex;
	long	matIndexOld;
	bool	keepMaterialInList = false;

	if(!pModel) // o modelo não foi associado
		return -1;

	if(regionIndex != -1) //  Região existe
	{
		matInUseIndex = IncludeInMaterialInUseList(matIndex);

		matIndexOld = pModel->vpModelRegions[regionIndex]->GetMaterial();

		pModel->vpModelRegions[regionIndex]->SetMaterial( matInUseIndex);
		pModel->vpModelRegions[regionIndex]->SetIdentificationPoint(o_point);
		pModel->vpModelRegions[regionIndex]->SetPermanentMagnetAngle(angle);


		if(matIndexOld != UNDEFINED)
		{
			for(i=0;i<pModel->vpModelRegions.size() && !keepMaterialInList;i++)
			{
				if(pModel->vpModelRegions[i]->Status())
				{
					if(pModel->vpModelRegions[i]->GetMaterial() == matIndexOld)
						keepMaterialInList = true;
				}
			}

			if(!keepMaterialInList )
				vMaterialInUse[matIndexOld].Status(LEV_INVALID);
		}

		isMaterialModified	=	lev_enum::YES;
	}

	return 0;
}


int	CLevPhysData::GetNumValidMaterialsInUse(void)
{
	unsigned int i;
	int num =0;
	for(i=0;i<vMaterialInUse.size();i++)
		if(vMaterialInUse[i].Status()) num++;
	return num;
}


int CLevPhysData::GetNumMaterialsInUse(void)
{
	return vMaterialInUse.size();
}


long CLevPhysData::IncludeInMaterialInUseList( long matIndex)
{
	unsigned int i;
	int		insertionposition = -1;
	bool	found = false;

	for (i = 0; i < vMaterialInUse.size() && (found == false); i++)
	{
		if (vMaterialDB[matIndex] == vMaterialInUse[i])
		{
			insertionposition = i;
			vMaterialInUse[i].Set_Color(vMaterialDB[matIndex].Get_Color());
            vMaterialInUse[i].Status(LEV_VALID);
			found = true;   // já existe material na lista
		}
	}

	if(!found)
	{
		vMaterialInUse.push_back(vMaterialDB[matIndex]);
		vMaterialInUse.back().Status(LEV_VALID);

		insertionposition = vMaterialInUse.size()-1;
	}


	return insertionposition;
}


long CLevPhysData::IncludeInMaterialList(CMaterial & mat)
{
	unsigned int i;
	int insertionposition = -1;
	bool found = false;

	for(i=0; i<vMaterialDB.size() && (found == false); i++) {
		if(vMaterialDB[i] == mat) {
			insertionposition = i;
			// vMaterialInUse[i].Set_Color(vMaterialDB[matIndex].Get_Color());
			found = true;   // já existe material na lista
		}
	}

	if(!found) {
		vMaterialDB.push_back(mat);
		vMaterialDB.back().Status(LEV_VALID);

		insertionposition = vMaterialDB.size()-1;
	}

	return insertionposition;
}


void CLevPhysData::GetSourcesInUse(vCLevSource & v)
{
	v = vSourcesInUse;
}

int	CLevPhysData::GetNumValidSourcesInUse(void)
{
	unsigned int i;
	int numValid=0;
	for(i=0;i<vSourcesInUse.size();i++)
		if(vSourcesInUse[i].Type() != INVALIDSOURCE)
			numValid++;
	return numValid;
}

int CLevPhysData::GetNumSourcesInUse(void)
{
	return vSourcesInUse.size();
}


//*NMA*:Inclui fonte na lista de fontes em uso
// retorno : retorno = -1, se não conseguiu incluir, significa uma inconsistência
//			 retorno >= 0: posição da fonte na lista
//
// Obs: verifica se a fonte já estava na lista
long CLevPhysData::IncludeInSourceInUseList(CSource & oSource)
{
	unsigned long i;
	long	position = -1;
	bool	isInList = false;

	//verifica se a condição já está na lista de condições de contorno
	for(i=0;i<vSourcesInUse.size() && isInList==false;i++)
	{
		if(vSourcesInUse[i] == oSource)
		{
			isInList = true;
			position = i;
			if(vSourcesInUse[i].Type() == INVALIDSOURCE)
				vSourcesInUse[i].Type(CTESOURCE);
		}
	}

	// inclui a fonte se não estiver na lista
	if(!isInList)
	{
		vSourcesInUse.push_back(oSource);
		position = vSourcesInUse.size()-1;
	}

	return position;
}

//-----------------------------------------------------------------------------------------------------
//	RemoveSourceInUse()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Remove source da lista em uso se não tiver mais nenhuma região com esta fonte
//
//	parâmetros:
//		int			index : índice da fonte
//
//	retorno:	  -1 : o modelo não foi associado
//				   0 : sucesso
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//---------------------------------------------------------------------------------------------------
int CLevPhysData::RemoveSourceInUse(int index)
{
	unsigned int i;
	bool	keepSourceInList = false;

	if(!pModel) // o modelo não foi associado
		return -1;

	// varre as regiões e verifica se em algumas delas a fonte está atribuída
	// neste ponto, o valor da fonte na região a qual ela era atribuída já foi alterado

	if(index != UNDEFINED)
	{
		for(i=0;i<pModel->vpModelRegions.size() && !keepSourceInList;i++)
		{
			if(pModel->vpModelRegions[i]->Status())
			{
				if(pModel->vpModelRegions[i]->GetSource() == index)
					keepSourceInList = true;
			}
		}

		if(!keepSourceInList)
			vSourcesInUse[index].Type(INVALIDSOURCE);

		isSourceModified = lev_enum::YES;
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------
//	RemoveMaterialInUse()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Remove material da lista em uso se não tiver mais nenhuma região com esta fonte
//
//	parâmetros:
//		int			index : índice da fonte
//
//	retorno:	  -1 : o modelo não foi associado
//				   0 : sucesso
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//---------------------------------------------------------------------------------------------------
int CLevPhysData::RemoveMaterialInUse(int index)
{
	unsigned int i;
	bool	keepMaterialInList = false;

	if(!pModel) // o modelo não foi associado
		return -1;

	// varre as regiões e verifica se em algumas delas o material está atribuído
	// neste ponto, o valor do material na região a qual ela era atribuída já foi alterado

	if(index != UNDEFINED)
	{
		for(i=0;i<pModel->vpModelRegions.size() && !keepMaterialInList;i++)
		{
			if(pModel->vpModelRegions[i]->Status())
			{
				if(pModel->vpModelRegions[i]->GetMaterial() == index)
					keepMaterialInList = true;
			}
		}

		if(!keepMaterialInList)
			vMaterialInUse[index].Status(LEV_INVALID);
		else vMaterialInUse[index].Status(LEV_VALID);
	}

	return 0;

} // end of RemoveMaterialInUse

//-----------------------------------------------------------------------------------------------------
//	AssociateSourceToRegion()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Associa source a uma região
//				Atualiza a lista de source em uso
//
//	parâmetros:
//		int		regionIndex : índice da região
//		CSource &oSource	: fonte
//
//	retorno:	  -1 : o modelo não foi associado
//				   0 : sucesso
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por:
//	Modificado em:
//	Motivo:
//---------------------------------------------------------------------------------------------------
int CLevPhysData::AssociateSourceToRegion(int regionIndex,CSource &oSource)
{

	long	sourceInUseIndex;
	long	sourceIndexOld;

	if(!pModel)
		return -1;


	if(regionIndex != -1) //  Região existe
	{
		sourceInUseIndex = IncludeInSourceInUseList(oSource);

		sourceIndexOld = pModel->vpModelRegions[regionIndex]->GetSource();

		pModel->vpModelRegions[regionIndex]->SetSource( sourceInUseIndex);

		RemoveSourceInUse(sourceIndexOld);

		isSourceModified = lev_enum::YES;

	}

	return 0;

}//end of AssociateSourceToRegion


//................................................................................
//......................... Gravação e Leitura............................................
//................................................................................


int CLevPhysData::SaveBoundaryConditionsInUse(std::string fileName)
{
	int			num,i;
	FILE  *		parq = NULL;
	int			ref_node_id=-1,ref_node_local_id=-1, ref_prim_id=-1;
	int			ref_node=-1,ref_node_local=-1, ref_prim=-1;
	double		value;

	num = vBoundaryConditions.size();

	parq = fopen(fileName.c_str(),"w");

	if (!parq)
	{
		if(parq) fclose(parq);
		return 103;
	}
	else
	{
		fprintf(parq,"%d",num);

		// *NMA*: Por enquanto grava também os objetos inválidos que por ventura existirem
		for(i=0;i<num;i++)
		{

			//*NMA*: 25/07/2005: a gravação terá que ser feita sem uso da função, pois
			// os índices armazenados para o nó de referencia de cc float e primitivas
			// de referencia de cc periódicas são as posições nos vetores. Estes índices
			// podem não ser mais válidos quando da leitura do projeto gravado (lembrar que
			// os pontos e primitivas deletadas são eliminados dos vetores e que as elipses
			// são gravadas depois das retas). Usar o Id() também não é adequado pois também
			// podem ser alterados durante a edição. Só é válido quando da gravação para leitura
			// posterior.
			int tipo;

			tipo = (enum BOUNDARYCONDITIONTYPE) vBoundaryConditions[i].Type();

			ref_node = vBoundaryConditions[i].RefNode();
			if( ref_node != -1 && ref_node < (signed)pModel->vpPoints.size() )
				ref_node_id = pModel->vpPoints[ref_node]->Id();
			ref_node_local = vBoundaryConditions[i].RefNodeLocal();
			if( ref_node_local != -1 && ref_node_local < (signed)pModel->vpPoints.size() )
				ref_node_local_id = pModel->vpPoints[ref_node_local]->Id();

			ref_prim = vBoundaryConditions[i].RefPrim();
			if( ref_prim != -1 && ref_prim < (signed)pModel->vpPrimitives.size() )
				ref_prim_id = pModel->vpPrimitives[ref_prim]->Id();

			value = vBoundaryConditions[i].Value();

			fprintf(parq,"\n\n%d",tipo);
			fprintf(parq,"\n%d",ref_node_id);
			fprintf(parq,"\n%d",ref_node_local_id);
			fprintf(parq,"\n%d",ref_prim_id);
			fprintf(parq,"\n%lf",value);


		}
	}

	fclose(parq);

	return 0;
}


//-----------------------------------------------------------------------------------------------------
//	ReadBoundaryConditionsInUse(std::string fileName)
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Leitura das condições de contorno em uso no modelo. Armazena as informações em um vetor
//				de objetos da classe CBoundCond. Nas primitivas e pontos, a associação com a condição
//				de contorno é realizada por um índice a este vetor.
//
//	parâmetros:	nome do arquivo com extensão (.bco) com path completo
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por: Nancy
//	Modificado em:  Novembro/2004
//	Motivo:			Migração para CLevGeoData
//-----------------------------------------------------------------------------------------------------
int CLevPhysData::ReadBoundaryConditionsInUse(std::string fileName)
{
	int			num,i;
	FILE  *		parq = NULL;
	CBoundCond	oBoundCond;

	parq = fopen(fileName.c_str(),"r");

	if (!parq)
	{
		if(parq) fclose(parq);
		return 103;
	}
	else
	{
		fscanf(parq,"%d",&num);

		for(i=0;i<num;i++)
		{
			oBoundCond.Read(parq);

			vBoundaryConditions.push_back(oBoundCond);
		}
	}

	fclose(parq);
	return 0;

}//end of ReadBoundaryConditionsInUse


//-----------------------------------------------------------------------------------------------------
//	ReadMaterialsInUse(std::string fileName)
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Leitura dos materiais em uso no modelo. Armazena as informações em um vetor
//				de objetos da classe CMaterial. Nas regiões fechadas do modelo geomérico,
//				a associação com o material é realizada por um índice a este vetor.
//
//	parâmetros:	nome do arquivo com extensão (.mat) com path completo
//
//	retorno:
//
//	Observações:
//				O status de validade ou não é mantido para facilitar o tratamento dos índices.
//				Os materiais são sempre inseridos na lista. Na gravação da modelagem
//				não é feita uma "compactação" para eliminar os materiais atribuídos "por engano" e
//				mais usados.
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por: Nancy
//	Modificado em:  Novembro/2004
//	Motivo:			Migração para CLevGeoData
//-----------------------------------------------------------------------------------------------------
#if 0
int CLevPhysData::ReadMaterialsInUse(std::string fileName)
{
	FILE		*parq = NULL;
	int			i;
	int			num_mat; //número de materiais no vetor de materiais em uso
	char		nome_temp[50]= "";
	double		material[7];
	int			codigo, statusInt;
	LEV_OBJECTVALIDITY status;
	CMaterial	materialTemp;
	long cor;

	parq = fopen(fileName.c_str(),"r");
	if (parq == NULL)
		return 107;

	fscanf(parq, "%d %d %d %d", &num_mat, &NELT, &NCCOR, &NMAG);

	for(i=0;i<num_mat;i++) {

		fscanf(parq,"\n%ld", &cor);

		//materialTemp.Set_Color( CLEV_ColorRGB(cor_red,cor_green, cor_blue) ); // verificar a possibilidade de converter cor (int) para qcolor

		materialTemp.Set_Color( cor );

		fscanf(parq,"\n%d",  &statusInt);

		status = (enum LEV_OBJECTVALIDITY) statusInt;

		materialTemp.Status(status);

		fscanf(parq,"\n%d %s %lg %lg %lg %lg %lg %lg %lg",
						 &codigo,
						 nome_temp,
						 &material[0],
						 &material[1],
						 &material[2],
						 &material[3],
						 &material[4],
						 &material[5],
						 &material[6]);

			materialTemp.Set_Codigo(codigo);
			materialTemp.Set_Name(nome_temp);
			materialTemp.material[0] = material[0];
			materialTemp.material[1] = material[1];
			materialTemp.material[2] = material[2];
			materialTemp.material[3] = material[3];
			materialTemp.material[4] = material[4];
			materialTemp.material[5] = material[5];
			materialTemp.material[6] = material[6];

			vMaterialInUse.push_back(materialTemp);
	  }

	  fclose(parq);

 return (0);

}//end of ReadMaterialsInUse
#endif

int CLevPhysData::ReadMaterialsInUse(std::string fileName)
{
	FILE		*parq = NULL;
	int			i, j, k;
	int			num_mat; //n�mero de materiais no vetor de materiais em uso
//	CString     nome_temp;
	char		nome_temp[50]= "";
	double		material[7];
	int			codigo, statusInt;
	LEV_OBJECTVALIDITY status;
	int			cor_red, cor_green, cor_blue;
	double		cor_red_aux_a, cor_green_aux_a, cor_blue_aux_a;
	int			cor;
	char		cor_char[50];
	std::string cor_str;
	int			indColor =0;
	CLEV_ColorRGB matColor;
	std::vector<double> mat_aux; //*NMA* auxilizr para leitura das propriedades
	double		aux=0.;

	CMaterial	materialTemp;
	bool		corOK = false;

  	int mm;
  	char cor_red_aux[4], cor_green_aux[4], cor_blue_aux[4];

	parq = fopen(fileName.c_str(),"r");
	if (parq == NULL)
	{
		return 107;
	}

	fscanf(parq, "%d %d %d %d", &num_mat, &NELT, &NCCOR, &NMAG);

	int NTOTAL = NELT+NCCOR+NMAG;

	materialTemp.material.clear();

	for(i=0;i<num_mat;i++)
	{
		fscanf(parq,"\n%s", &cor_char);

  		cor_str = cor_char;

  		//materialTemp.Set_Color( QColor(cor) ); // verificar a possibilidade de converter cor (int) para qcolor

  	    if(cor_str.size() == 9)
  	    {
  	    	for(mm=0;mm<3; mm++)
  	    	{
  	    		cor_red_aux[mm] = cor_str[mm];
  	    	}
  	    	cor_red_aux[mm] = '\0';

  	    	cor_red = atoi(cor_red_aux) ;

  	    	if(cor_red <= 256 && cor_red >=0)
  	    	{
  	  	    	for(mm=3;mm<6; mm++)
  	  	    	{
  	  	    		cor_green_aux[mm-3] = cor_str[mm];
  	  	    	}
  	  	    	cor_green_aux[mm-3] = '\0';

  	  	    	cor_green = atoi(cor_green_aux) ;

  	  	    	if(cor_green <= 256 && cor_green >=0)
  	  	    	{
  	  	  	    	for(mm=6;mm<9; mm++)
  	  	  	    	{
  	  	  	    		cor_blue_aux[mm-6] = cor_str[mm];
  	  	  	    	}
  	  	  	    	cor_blue_aux[mm-6] = '\0';

  	  	  	    	cor_blue = atoi(cor_blue_aux) ;

  	  	  	    	if(cor_blue <= 256 && cor_blue >=0)
  	  	  	    	{
  	  	  	    		corOK = true;
  	  	  	    	}
  	  	    	}
  	    	}
  	    }

  	    if(corOK == false)
  	    {
  			indColor++;
  			if(indColor > MAXCOLOR_LEVRGB)
  			{
  				indColor = 0;
  			}

  			matColor = colors_LEVRGB[indColor];

  			cor_red = matColor.getRedCode();
  			cor_green = matColor.getGreenCode();
  			cor_blue = matColor.getBlueCode();

  	    }

  	    materialTemp.Set_Color( CLEV_ColorRGB(cor_red,cor_green, cor_blue) );

		fscanf(parq,"\n%d",  &statusInt);

		status = (enum LEV_OBJECTVALIDITY) statusInt;

		materialTemp.Status(status);

		fscanf(parq,"\n%d %s",&codigo, nome_temp);

		materialTemp.Set_Name(nome_temp);
		materialTemp.Set_Codigo(codigo);

		mat_aux.clear();
		for(j=0;j<NTOTAL;j++)
		{
			fscanf(parq,"%lg", &aux);
			mat_aux.push_back(aux);
		}

		materialTemp.material.clear();

		for(k=0;k<mat_aux.size();k++)
		{
			materialTemp.material.push_back(mat_aux[k]);
		}

		#if 0
		fscanf(parq,"\n%d %s %lg %lg %lg %lg %lg %lg %lg",
					 &codigo,
					 nome_temp,
					 &material[0],
					 &material[1],
					 &material[2],
					 &material[3],
					 &material[4],
					 &material[5],
					 &material[6]);

		materialTemp.Set_Codigo(codigo);
		materialTemp.Set_Name(nome_temp);
		materialTemp.material.push_back(material[0]) ;
		materialTemp.material.push_back(material[1]) ;
		materialTemp.material.push_back(material[2]) ;
		materialTemp.material.push_back(material[3]) ;
		materialTemp.material.push_back(material[4]) ;
		materialTemp.material.push_back(material[5]) ;
		materialTemp.material.push_back(material[6]) ;
		#endif

		vMaterialInUse.push_back(materialTemp);

		materialTemp.material.clear();
	}
	fclose(parq);

	return (0);
}

int CLevPhysData::ReadMaterialsInUse01(std::string fileName)
{
	int			i, j, k;
	int			num_mat; //n�mero de materiais no vetor de materiais em uso
	char		nome_temp[50]= "";
	double		material[7];
	int			codigo, statusInt;
	LEV_OBJECTVALIDITY status;
	int			cor_red, cor_green, cor_blue;
	double		cor_red_aux_a, cor_green_aux_a, cor_blue_aux_a;
	int			cor;
	char		cor_char[50];
	std::string cor_str;
	int			indColor =0;
	CLEV_ColorRGB matColor;
	//std::vector<double> mat_aux; //*NMA* auxilizr para leitura das propriedades
	double		aux=0.;

	CMaterial	materialTemp;
	bool		corOK = false;

  	int mm;
  	char cor_red_aux[4], cor_green_aux[4], cor_blue_aux[4];

	std::ifstream fin(fileName.c_str());
	std::string line;

	std::getline(fin, line);//le linha 01
   std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 line =" << line.c_str(); std::cout.flush(); std::cout.flush();

	//obtem valor de NM: numero de materiais do arquivo
	std::istringstream ss(line);
	ss >> num_mat;

#if 0
	std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 " ; std::cout.flush(); std::cout.flush();
	std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 num_mat = " << num_mat; std::cout.flush(); std::cout.flush();
#endif

	 materialTemp.material.clear();

	  for(i=0;i<num_mat;i++)
	  {
		//   std::cout << "\n\nint CLevPhysData::ReadMaterialsInUse01 i =" << i; std::cout.flush(); std::cout.flush();
		std::getline(fin, line); //le cor
		//   std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 line =" << line.c_str(); std::cout.flush(); std::cout.flush();
		std::istringstream ss01(line);
 		 ss01 >> cor_char;

#if 0
 		 std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 cor_char = " << cor_char; std::cout.flush(); std::cout.flush();
#endif

  		cor_str = cor_char;

  	    if(cor_str.size() == 9)
  	    {

  	    	for(mm=0;mm<3; mm++)
  	    	{
  	    		cor_red_aux[mm] = cor_str[mm];
  	    	}
  	    	cor_red_aux[mm] = '\0';

  	  	    //std::cout << "\nint CLevPhysData::ReadMaterialsInUse cor_red_aux =" << cor_red_aux; std::cout.flush(); std::cout.flush();

  	    	cor_red = atoi(cor_red_aux) ;
  	  	    //std::cout << "\nint CLevPhysData::ReadMaterialsInUse cor_red =" << cor_red; std::cout.flush(); std::cout.flush();

  	    	if(cor_red <= 256 && cor_red >=0)
  	    	{
  	  	    	for(mm=3;mm<6; mm++)
  	  	    	{
  	  	    		cor_green_aux[mm-3] = cor_str[mm];
  	  	    	}
  	  	    	cor_green_aux[mm-3] = '\0';

  	  	    	cor_green = atoi(cor_green_aux) ;

  	  	    	if(cor_green <= 256 && cor_green >=0)
  	  	    	{
  	  	  	    	for(mm=6;mm<9; mm++)
  	  	  	    	{
  	  	  	    		cor_blue_aux[mm-6] = cor_str[mm];
  	  	  	    	}
  	  	  	    	cor_blue_aux[mm-6] = '\0';

  	  	  	    	cor_blue = atoi(cor_blue_aux) ;

  	  	  	    	if(cor_blue <= 256 && cor_blue >=0)
  	  	  	    	{
  	  	  	    		corOK = true;
  	  	  	    	}
  	  	    	}
  	    	}
  	    }//fecha if(cor_str.size() == 9)


  	    if(corOK == false)
  	    {
  			indColor++;
  			if(indColor > MAXCOLOR_LEVRGB)
  			{
  				indColor = 0;
  			}

  			matColor = colors_LEVRGB[indColor];

  			cor_red = matColor.getRedCode();
  			cor_green = matColor.getGreenCode();
  			cor_blue = matColor.getBlueCode();

  	    }

	  	  //std::cout << "\nint CLevPhysData::ReadMaterialsInUse cor_red =" << cor_red; std::cout.flush(); std::cout.flush();
	  	  //std::cout << "\nint CLevPhysData::ReadMaterialsInUse cor_green =" << cor_green; std::cout.flush(); std::cout.flush();
	  	  //std::cout << "\nint CLevPhysData::ReadMaterialsInUse cor_blue =" << cor_blue; std::cout.flush(); std::cout.flush();


		  materialTemp.Set_Color( CLEV_ColorRGB(cor_red,cor_green, cor_blue) );

			std::getline(fin, line); //le status
	   //std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 line =" << line.c_str(); std::cout.flush(); std::cout.flush();
			std::istringstream ss02(line);
	 		 ss02 >> statusInt;

		  status = (enum LEV_OBJECTVALIDITY) statusInt;

	  	 // std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 statusInt =" << statusInt; std::cout.flush(); std::cout.flush();
	  	 // std::cout << "\nint CLevPhysData::ReadMaterialsInUse status =" << status; std::cout.flush(); std::cout.flush();

		  materialTemp.Status(status);

		 //fin >> codigo >> nome_temp;// fscanf(parq,"\n%d %s",&codigo, nome_temp);
			std::getline(fin, line); //le codigo e nome
  // std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 line =" << line.c_str(); std::cout.flush(); std::cout.flush();
			std::istringstream ss03(line);
	 		 ss03 >> codigo >> nome_temp;//fin >> cor_char; //fscanf(parq,"\n%s", &cor_char);

	  	  std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 codigo =" << codigo; std::cout.flush(); std::cout.flush();
	  	  std::cout << "\nint CLevPhysData::ReadMaterialsInUse01 nome_temp =" << nome_temp; std::cout.flush(); std::cout.flush();

		  materialTemp.Set_Name(nome_temp);
		  materialTemp.Set_Codigo(codigo);


			vMaterialInUse.push_back(materialTemp);

			materialTemp.material.clear();
	  }
	   //std::cout << "\nint CLevPhysData::ReadMaterialsInUse vMaterialInUse = " << vMaterialInUse.size(); std::cout.flush(); std::cout.flush();

	  fin.close();//fclose(parq);

 return (0);

}//end of ReadMaterialsInUse01

//-----------------------------------------------------------------------------------------------------
//	ReadSourcesInUse(std::string fileName)
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Leitura das fontes em uso no modelo. Armazena as informações em um vetor
//				de objetos da classe CSource. Nas regiões fechadas do modelo geomérico,
//				a associação com a fonte é realizada por um índice a este vetor.
//
//	parâmetros:	nome do arquivo com extensão (.src) com path completo
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Setembro/2002
//	Modificado por: Nancy
//	Modificado em:  Novembro/2004
//	Motivo:			Migração para CLevGeoData
//-----------------------------------------------------------------------------------------------------
int CLevPhysData::ReadSourcesInUse(std::string fileName)
{
	int			num, i;
	FILE  *		parq = NULL;
	CSource		oSource;

	parq = fopen(fileName.c_str(),"r");

	if (!parq)
	{
		if(parq) fclose(parq);
		return 103;
	}
	else
	{
		fscanf(parq,"%d",&num);

		for(i=0;i<num;i++)
		{
			oSource.Read(parq);
			vSourcesInUse.push_back(oSource);
		}
	}

	fclose(parq);

	return 0;

}//end of ReadSourcesInUse

//............................................................................................
//............................................................................................
int	CLevPhysData::SaveBoundaryConditionsPrimitives(std::string fileName)
{
	unsigned int i, num_prim=0;
	int		prim, primType;
	long	index;
	vInt	vPrim;
	vLong	vIndex;
	vInt	vPrimType;
	FILE *  parq;

	for(i=0;i<oGroupBoundaryConditions.vpPrimitives.size();i++)
	{
		index = oGroupBoundaryConditions.vpPrimitives[i]->GetBoundaryCondition();

		if(oGroupBoundaryConditions.vpPrimitives[i]->Status() &&  index != UNDEFINED)
		{
			//as primitivas inválidas não são gravadas
			vPrim.push_back(oGroupBoundaryConditions.vpPrimitives[i]->Id());
			vIndex.push_back(index);
			switch(oGroupBoundaryConditions.vpPrimitives[i]->GeoEntityType()) //WPG: de GeoEntity para GeoEntityType
			{
			case LINELEV: vPrimType.push_back(LINELEV);
				break;
			case ELLIPSELEV: vPrimType.push_back(ELLIPSELEV);
				break;
			}
		}

	}

	for(i=0;i<oGroupBoundaryConditions.vpPoints.size();i++)
	{
		index = oGroupBoundaryConditions.vpPoints[i]->GetBoundaryCondition();

		if(oGroupBoundaryConditions.vpPoints[i]->Status() &&  index != UNDEFINED)
		{
			vPrim.push_back(oGroupBoundaryConditions.vpPoints[i]->Id());
			vIndex.push_back(index);
			vPrimType.push_back(POINTLEV);
		}
	}

	num_prim = vPrim.size();

	if(num_prim)
	{
		parq = fopen(fileName.c_str(),"w");

		if (!parq)
		{
			if(parq) fclose(parq);
			return 105;
		} else {
			fprintf(parq,"%d",num_prim);

			for(i=0;i<num_prim;i++)
			{
				prim	= vPrim[i];
				index	= vIndex[i];
				primType = vPrimType[i];

				fprintf(parq,"\n%d %ld %d", prim, index, primType);
			}
			fclose(parq);
		}
	}
	return 0;
}


int	CLevPhysData::ReadBoundaryConditionsPrimitives(std::string fileName)
{
	int		i;
	int		num_prim=0;
	int		prim, primType;
	long	index;
	FILE	*parq=NULL;

	parq = fopen(fileName.c_str(),"r");

	if (!parq)
	{
		if(parq) fclose(parq);
		return 105;
	}
	else
	{
		fscanf(parq,"%d",&num_prim);

		for(i=0;i<num_prim;i++)
		{
			fscanf(parq,"\n%d %ld %d", &prim, &index, &primType);

			if(prim >= 0 && index >= 0)
			{
				switch(primType)
				{
				case LINELEV:
				case ELLIPSELEV:
					pModel->vpPrimitives[prim]->SetBoundaryCondition(index);
					oGroupBoundaryConditions.IncludeNextPrimitive(pModel->vpPrimitives[prim]);
					break;
				case POINTLEV:
					pModel->vpPoints[prim]->SetBoundaryCondition(index);
					oGroupBoundaryConditions.IncludeNextPoint(pModel->vpPoints[prim]);
					break;
				}
			}
		}
		fclose(parq);
	}
	return 0;
}


int CLevPhysData::SaveSourcesInUse(std::string fileName)
{
	int			num, i;
	FILE  *		parq = NULL;

	num = vSourcesInUse.size();

	parq = fopen(fileName.c_str(),"w");

	if (!parq)
	{
		if(parq) fclose(parq);
		return 104;
	} else {
		fprintf(parq,"%d",num);

		// *NMA*: Por enquanto grava também os objetos inválidos que por ventura existirem
		for(i=0;i<num;i++)
			vSourcesInUse[i].Save(parq);
	}

	fclose(parq);
	return 0;
}

#if 0
int CLevPhysData::SaveMaterialsInUse(std::string fileName)
{
	FILE		*parq = NULL;

	int			i;
	int			num_mat; //número de materiais no vetor de materiais em uso
	std::string     nome_temp;
	long cor;
	CMaterial	materialTemp;

	parq = fopen(fileName.c_str(),"w");
	if (parq == NULL)
		return 106;

	num_mat = vMaterialInUse.size();

	fprintf(parq, "%d %d %d %d", num_mat, NELT, NCCOR, NMAG);

	for(i=0;i<num_mat;i++) {
	  materialTemp = vMaterialInUse[i];
	  nome_temp = vMaterialInUse[i].Get_Name();
	  cor = vMaterialInUse[i].Get_Color();
  	  fprintf(parq,"\n%ld", cor);
	  fprintf(parq,"\n%d", materialTemp.Status());
	  fprintf(parq,"\n%d %s %lg %lg %lg %lg %lg %lg %lg",
					 materialTemp.Get_Codigo(),
					 nome_temp.c_str(),
					 materialTemp.material[0],
					 materialTemp.material[1],
					 materialTemp.material[2],
					 materialTemp.material[3],
					 materialTemp.material[4],
					 materialTemp.material[5],
					 materialTemp.material[6]);
	}
	fclose(parq);
	return (0);
}
#endif
int CLevPhysData::SaveMaterialsInUse(std::string fileName)
{
	FILE		*parq = NULL;

	int			i,j;
	int			num_mat; //n�mero de materiais no vetor de materiais em uso
	std::string     nome_temp;
	//QColor		cor;
	CLEV_ColorRGB cor;
	CMaterial	materialTemp;
	int			codigo;
	double		prop;

	parq = fopen(fileName.c_str(),"w");
	if (parq == NULL)
	{
//		Mensagem_erro("erro na abertura do arquivo propfis.mei");
//		Ready = NNAO;
		return 106;
	}

  num_mat = vMaterialInUse.size();

  fprintf(parq, "%d %d %d %d", num_mat, NELT, NCCOR, NMAG);

  for(i=0;i<num_mat;i++)
  {

	  cor = vMaterialInUse[i].Get_Color();

	  // red ================================================
	  if(cor.getRedCode() < 10 )
	  {
		  fprintf(parq,"\n00%d", cor.getRedCode());
	  }
	  else
	  {
		  if(cor.getRedCode() < 100 )
			  fprintf(parq,"\n0%d", cor.getRedCode());
		  else
			  fprintf(parq,"\n%d", cor.getRedCode());
	  }

	  // green  ================================================
	  if(cor.getGreenCode() < 10 )
	  {
		  fprintf(parq,"00%d", cor.getGreenCode());
	  }
	  else
	  {
		  if(cor.getGreenCode() < 100 )
			  fprintf(parq,"0%d", cor.getGreenCode());
		  else
			  fprintf(parq,"%d", cor.getGreenCode());
	  }

	  // blue  ================================================
	  if(cor.getBlueCode() < 10 )
	  {
		  fprintf(parq,"00%d", cor.getBlueCode());
	  }
	  else
	  {
		  if(cor.getBlueCode() < 100 )
			  fprintf(parq,"0%d", cor.getBlueCode());
		  else
			  fprintf(parq,"%d", cor.getBlueCode());
	  }

//  	  fprintf(parq,"\n%d", materialTemp.Status());
  	  fprintf(parq,"\n%d",vMaterialInUse[i].Status());

	  nome_temp = 	vMaterialInUse[i].Get_Name();
	  codigo = 		vMaterialInUse[i].Get_Codigo();

	  fprintf(parq,"\n%d %s", codigo, nome_temp.c_str());

	  for(j=0;j<vMaterialInUse[i].material.size();j++)
	  {
		  prop = vMaterialInUse[i].material[j];
		  fprintf(parq,"\t%lg", prop);
	  }

  }

#if 0
  for(i=0;i<num_mat;i++)
  {
	  materialTemp = vMaterialInUse[i];

	  nome_temp = vMaterialInUse[i].Get_Name();

	  cor = vMaterialInUse[i].Get_Color();

//  	  fprintf(parq,"\n%d%d%d", cor.red(),cor.green(),cor.blue());
  	  fprintf(parq,"\n%d %d %d", cor.getRedCode(),cor.getGreenCode(),cor.getBlueCode());

	  fprintf(parq,"\n%d", materialTemp.Status());

	  fprintf(parq,"\n%d %s %lg %lg %lg %lg %lg %lg %lg",
					 materialTemp.Get_Codigo(),
					 nome_temp.c_str(),
					 materialTemp.material[0],
					 materialTemp.material[1],
					 materialTemp.material[2],
					 materialTemp.material[3],
					 materialTemp.material[4],
					 materialTemp.material[5],
					 materialTemp.material[6]);

  }
#endif

  fclose(parq);

 return (0);

}

//............................................................................................
// grava o arquivo a partir das linhas do arquivo arazenadas no vetor de strings
//............................................................................................
int CLevPhysData::SaveMaterialsInUse01(std::string fileName)
{
	FILE		*parq = NULL;

	int			i,j;
	int			num_mat; //n�mero de materiais no vetor de materiais em uso
	std::string     nome_temp;
	//QColor		cor;
	CLEV_ColorRGB cor;
	CMaterial	materialTemp;
	int			codigo;
	double		prop;

/*
	parq = fopen(fileName.c_str(),"w");
	if (parq == NULL)
	{
//		Mensagem_erro("erro na abertura do arquivo propfis.mei");
//		Ready = NNAO;
		return 106;
	}
*/

	std::ofstream fout(fileName.c_str());

	num_mat = vMaterialInUse.size();

	if(!num_mat)
	{
		std::cout << "\nint CLevPhysData::SaveMaterialsInUse01(std::string fileName): there is no material in use to be salved."; std::cout.flush();  std::cout.flush();
		return 0;
	}

	if(!propfismei_lines.size())
	{
		std::cout << "\nint CLevPhysData::SaveMaterialsInUse01(std::string fileName): there are no material in the vector propfismei_lines."; std::cout.flush();  std::cout.flush();
		return 1;
	}

	std::cout << "\nint CLevPhysData::SaveMaterialsInUse01(std::string fileName)"; std::cout.flush();  std::cout.flush();

	//separa o valor de NM do resto da linha
	std::string line, rest_of_line;

	int col1;

	line = propfismei_lines[0];

	std::istringstream ss(line);
	ss >> col1;
	std::getline(ss,  rest_of_line);;

  //fprintf(parq, "%d %d %d %d", num_mat, NELT, NCCOR, NMAG);

	fout << num_mat << " " << rest_of_line.c_str();// << '\n';

	//std::cout << "\nint CLevPhysData::SaveMaterialsInUse01(std::string fileName)  num_mat = " << num_mat; std::cout.flush();  std::cout.flush();
	//std::cout << "\nint CLevPhysData::SaveMaterialsInUse01(std::string fileName)  rest_of_line.c_str() = " << rest_of_line.c_str(); std::cout.flush();  std::cout.flush();


  for(i=0;i<num_mat;i++)
  {

	//std::cout << "\nint CLevPhysData::SaveMaterialsInUse01  i = " << i; std::cout.flush();  std::cout.flush();
	//std::cout << "\nint CLevPhysData::SaveMaterialsInUse01  vMaterialInUse[i].Get_Codigo() = " << vMaterialInUse[i].Get_Codigo(); std::cout.flush();  std::cout.flush();

	  cor = vMaterialInUse[i].Get_Color();

	  fout << "\n";

	  // red ================================================
	  if(cor.getRedCode() < 10 )
	  {
		 fout << "00" << cor.getRedCode(); // fprintf(parq,"\n00%d", cor.getRedCode());
	  }
	  else
	  {
		  if(cor.getRedCode() < 100 )
			  fout << "0" << cor.getRedCode();//fprintf(parq,"\n0%d", cor.getRedCode());
		  else
			  fout << cor.getRedCode();//fprintf(parq,"\n%d", cor.getRedCode());
	  }

	  // green  ================================================
	  if(cor.getGreenCode() < 10 )
	  {
		  fout << "00" << cor.getGreenCode(); //fprintf(parq,"00%d", cor.getGreenCode());
	  }
	  else
	  {
		  if(cor.getGreenCode() < 100 )
			  fout << "0" << cor.getGreenCode();//fprintf(parq,"0%d", cor.getGreenCode());
		  else
			  fout << cor.getGreenCode();//fprintf(parq,"%d", cor.getGreenCode());
	  }

	  // blue  ================================================
	  if(cor.getBlueCode() < 10 )
	  {
		  fout << "00" << cor.getBlueCode();//fprintf(parq,"00%d", cor.getBlueCode());
	  }
	  else
	  {
		  if(cor.getBlueCode() < 100 )
			  fout << "0" << cor.getBlueCode();//  fprintf(parq,"0%d", cor.getBlueCode());
		  else
			  fout << cor.getBlueCode();//fprintf(parq,"%d", cor.getBlueCode());
	  }

	  fout << "\n";
//  	  fprintf(parq,"\n%d", materialTemp.Status());
  	  //fprintf(parq,"\n%d",vMaterialInUse[i].Status());
	  fout << vMaterialInUse[i].Status();// << '\n';

	  nome_temp = 	vMaterialInUse[i].Get_Name();
	  codigo = 		vMaterialInUse[i].Get_Codigo();

	  if(codigo == -1) //hole-buraco
	  {
		  fout << '\n' << codigo << "\t" << nome_temp.c_str()  << "\t" << 0 << "\t" << 0 <<"\t" << 0 <<"\t" << 0 << "\t" << 0 << "\t" << 0 << "\t" << 0;
	  }
	  else
	  {
		  //procura a linha com o codigo para gravar
			//std::cout << "\nint CLevPhysData::SaveMaterialsInUse01(std::string fileName)  propfismei_lines.size() = " << propfismei_lines.size(); std::cout.flush();  std::cout.flush();
		  for(j=1;j< propfismei_lines.size();j++)
		  {
				line = propfismei_lines[j];
				std::istringstream ss01(line);
				ss01 >> col1;

				if(col1 == codigo)
				{
//					fout << line.c_str() << '\n';
					fout  << '\n' << line.c_str();

					break;
				}

		  }

	  }


  }//fecha for num_mat

  fout.close();//fclose(parq);

 return (0);

}

bool CLevPhysData::IsAPermanentMagnet(int indexMat)
{
	bool retorno = false;
	if(vMaterialDB[indexMat].Get_Codigo() >300 )
		retorno = true;
	return retorno;
}


lev_enum::LEV_BOOL CLevPhysData::IsBoundaryConditionModified(void)
{
	return isBoundaryConditionModified;
}


void CLevPhysData::IsBoundaryConditionModified(lev_enum::LEV_BOOL flag)
{
	isBoundaryConditionModified = flag;
}
lev_enum::LEV_BOOL CLevPhysData::IsMaterialModified(void)
{
	return isMaterialModified;
}
void CLevPhysData::IsMaterialModified(lev_enum::LEV_BOOL flag)
{
	isMaterialModified = flag;
}
lev_enum::LEV_BOOL CLevPhysData::IsSourceModified(void)
{
	return isSourceModified;
}
void CLevPhysData::IsSourceModified(lev_enum::LEV_BOOL flag)
{
	isSourceModified = flag;

}

//-----------------------------------------------------------------------------------------------------
//	Update()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Operação chamada para atualizar consistentemente os dados físicos
//				em função das alterações realizadas no modelo geométrico:
//				 - operação de eliminação de primitivas
//				 - outras operações que resultam na chamada da função para deletar primitivas
//					- inclusão de primitivas
//						- só no caso de interseccção com alguma outra primitiva, neste caso, a operação para deletar é chamada
//					- movimentação de primitivas
//						- só no caso de interseccção com alguma outra primitiva, neste caso, a operação para deletar é chamada
//
//	parâmetros:
//
//	retorno:
//
//	Observações: Deve ser chamada somente pelo objeto que aramazena o modelo geométrico
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Novembro/2004
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
void CLevPhysData::Update(GEOMETRICOPERATION operation,CLevPrimitive * pPrimitive)
{
	long		index = -1;
	CLevGroup	oGroupBCtemp;
	unsigned int i;
	SOURCETYPE  type = INVALIDSOURCE;

	if(operation == DELETEOPERATION && pPrimitive)
	{
		//verificar se a primitiva tinha condição de contorno, se sim retirar de lista de primitivas com cc
		index = pPrimitive->GetBoundaryCondition();
		if(index != UNDEFINED)
		{
			// Remove a condição de contorno da primitiva e
			// da lista de condições em uso, se for o caso
			oGroupBCtemp.IncludeNextPrimitive(pPrimitive);
			RemoveBoundaryCondition(oGroupBCtemp);
		}
	}

	// verificar se todos os materiais  e fontes em uso continuam em uso

	// para todos os materiais
	for(i=0;i<vMaterialInUse.size();i++)
	{
		if(vMaterialInUse[i].Status())
		{
			// se o material for usado, ele é mantido na lista
			// se não, é retirado da lista
			RemoveMaterialInUse(i);
		}

	}//close for for(i=0;i<vMaterialInUse.size();i++)


	//VERIFICAR!!!!!!!!!!!!!!
	// para todas as fontes
	for(i=0;i<vSourcesInUse.size();i++)
	{
		if(type != INVALIDSOURCE)
		{
			// se o material for usado, ele é mantido na lista
			// se não, é retirado da lista
			RemoveSourceInUse(i);
		}
	}
}


//-----------------------------------------------------------------------------
//*NMA*: 01/06/2005: leitura das propriedades físicas
//-----------------------------------------------------------------------------
int CLevPhysData::Read(void)
{
	int codeReturned = 0;

	if(pModel && pModel->project)
	{
		if(pModel->ModelPrimitivesOk())
		{
			codeReturned = ReadBoundaryConditionsPrimitives(pModel->project->GetFileName(MODEL_BOUNDARYCONDITIONS));

			if(!codeReturned)
				codeReturned = ReadBoundaryConditionsInUse(pModel->project->GetFileName(MODEL_BOUNDARYCONDITIONSINUSE));

			//*NMA*: não verifica os retornos anteriores pois a leitura, neste caso,
			//       é independente
			if(pModel->ModelRegionsOk())
			{
				codeReturned = ReadMaterialsInUse(pModel->project->GetFileName(MODEL_MATERIALINUSE));

				codeReturned = ReadSourcesInUse(pModel->project->GetFileName(MODEL_SOURCESINUSE));
			}
		}

		codeReturned = ReadPropFis(pModel->project->GetFileName(PROPFISMEI));
		if(!codeReturned)
			readpropfisOk = true;

	}

	return codeReturned; //não tem muito sentido pois os arquivos podem simplesmente não existir

}

