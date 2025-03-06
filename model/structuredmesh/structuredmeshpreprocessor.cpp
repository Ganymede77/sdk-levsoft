//===================================================================================================
//	Arquivo:			StructuredMeshPreProcessor.cpp
//...................................................................................................
//	Descrição:			Implementação da classe CStructuredMeshPreProcessor
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//...................................................................................................
//	Observações:
//
//===================================================================================================


#include <fstream>
#include <deque>
#include "structuredmeshpreprocessor.h"
#include "levenum.h"


CStructuredMeshPreProcessor::CStructuredMeshPreProcessor()
{
	rotation = false;
	save_go_file =  false;
	max_iterations = 9500;
	precision = 1e-5;
	relaxation_factor= 0.6;
	scale_factor = 1;
	isAxisimetric = false;
	num_I = 0;
	num_J = 0;

}


CStructuredMeshPreProcessor::~CStructuredMeshPreProcessor()
{
	unsigned int i;

	for(i=0;i<vpFrontiers.size();i++)
		if(vpFrontiers[i]) {
			delete [] vpFrontiers[i]; vpFrontiers[i] = NULL;
		}

	vpFrontiers.clear();
	vSubRegions.clear();
	vMeshPoints.clear();
}


//-----------------------------------------------------------------------------------------------------
//	CreateFrontier()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: Cria uma fronteira (CLevFrontier)
//
//	parâmetros:
//
//	retorno:	ponteiro para a fronteira criada
//				(NULL se a fronteira nao tiver sido
//                     criada)
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			22/02/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
CLevFrontier* CStructuredMeshPreProcessor::CreateFrontier(void)
{
	CLevFrontier * f = NULL;

	f = new  CLevFrontier;

	return f;

}//end of CreateFrontier


int CStructuredMeshPreProcessor::IncludeFrontier(CLevFrontier &frontier)
{
	unsigned int i;
	int index = -1;
	bool found = false;
	CLevFrontier *pf;

	for(i=0; i<vpFrontiers.size() && found == false; i++)
		if(*vpFrontiers[i]==frontier) {
			index = i;
			found = true;
		}

	if(!found) {
		pf = CreateFrontier();
		*pf = frontier;
		vpFrontiers.push_back(pf);
		vpFrontiers.back()->Status(LEV_FRONTIER);
		index = vpFrontiers.size()-1;
	}

	return index;
}


int CStructuredMeshPreProcessor::IsAFrontier(CLevPoint &screenPoint, CLevGroup & frontier, int &position, double deltax, double deltay)
{
	unsigned int i;
	CLevPoint pt_minxminy, pt_maxxmaxy;

	for (i = 0; i < vpFrontiers.size(); i++) {
		if (vpFrontiers[i]->Status()) {
			vpFrontiers[i]->Limits(pt_minxminy,pt_maxxmaxy);

			if ((screenPoint.X() >= pt_minxminy.X()- deltax) && (screenPoint.X() <= pt_maxxmaxy.X()+deltax)) {
				if ((screenPoint.Y() >= pt_minxminy.Y()- deltay) && (screenPoint.Y() < pt_maxxmaxy.Y()+ deltay)) {
					frontier = *vpFrontiers[i];
					position = i;
					return 1;
				}
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------------------------------
//	IdentifySubregions()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	identifica subregiões. As subregiões são formadas por fronteiras.
//				Varre a lista de regiões fechadas, verifica se as primitivas que formam a
//				região fechada pertencem a alguma fronteira da lista de fronteiras.
//				Caso não pertença a nenhuma, cria uma nova fronteira, inserindo-se a primitiva.
//				Atualiza o índice da fronteira da sub-região. Se a sub-região apresentar mais
//				de quatro fronteiras, apresenta erro (as primitivas precisar ser agrupadas).
//
//	parâmetros:
//
//	retorno:
//				0: sucesso na identificação
//				1: existem mais do que duas conexões a uma primitiva
//				2: número de fronteiras de uma sub-região maior do que 4
//				3: inconsistencia de dados
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::IdentifySubregions(CLevGeoData *pModel)
{
	int				retorno=0;
	unsigned int    i, k;
	int				j;
	bool			found = false, error = false;
	CLevFrontier	frontierAux;
	int				index = 0, index_a, index_b;
	int				num_frontiers=0;
	CSubRegion		oSubRegionAux;
	vInt			vIsRegionInclude;
	int				num_subregions;
	int				size;


	// Se já tiver sido feita uma identificação anterior, apaga tudo sem verificar
	if(vSubRegions.size())
	{
		ResetSubRegionsData();
	}

	num_subregions = pModel->vpModelRegions.size();
	// para todas as regiões, verifica se todas as primitivas pertencem a algums fronteira, se não cria uma fronteira com a primitiva
	// cria as sub-regiões e inclui na lista
	for (i = 0; i < pModel->vpModelRegions.size() && error == false ; i++)
	{
		if(pModel->vpModelRegions[i]->Status())
		{
			//para as primitivas que formam a região fechada
			for(k=0;k<pModel->vpModelRegions[i]->vpPrimitives.size() && error == false;k++)
			{
				found = false;
				if(pModel->vpModelRegions[i]->vpPrimitives[k]->Status())
				{
					//verifica se a primitiva pertence a uma das fronteiras já definidas
					size = vpFrontiers.size();
					for(j=0;j<size && found == false;j++)
					{
						if(vpFrontiers[j]->IsAPrimitive(pModel->vpModelRegions[i]->vpPrimitives[k]) >= 0)
						{
							found = true;
							index = j;
						}
					}

					// se a primitiva não pertence a nenhuma fronteira, cria-se uma só com essa primitiva
					// e a inclui na lista
					if(found == false)
					{
						frontierAux.IncludeNextPrimitive(pModel->vpModelRegions[i]->vpPrimitives[k]);
						index = IncludeFrontier(frontierAux);
						frontierAux.Reinitialize();
					}

					//inclui a fronteira
					num_frontiers = oSubRegionAux.IncludeFrontier(GetFrontier(index));

					if(num_frontiers > 4)
						retorno = 4;
				}
			}//fecha for todas as linhas
		}// fecha if(pModel->vRegions[i].Status())

		//inclui subregião na lista de subregiões
		if(!retorno) {
			oSubRegionAux.SetAssociatedModelRegionIndex(i);
			IncludeSubRegion(oSubRegionAux);
		}

		oSubRegionAux.Reinitialize();

	} //fecha for (i = 0; i < pModel->vRegions.size() && error == false ; i++)

	//identifica fronteira Sul e ordena as sub-regiões
	if(!retorno) {
		//ordena as fronteiras de todas as sub-regiões no sentido anti-horário
		for(i=0;i<vSubRegions.size() && !retorno;i++)
		{
			retorno = vSubRegions[i].SortFrontiers();
			//0: sucesso
			//1: existem mais do que duas conexões a uma primitiva
			//2: existem mais do que quatro fronteiras
			//3: inconsistencia de dados em alguma fronteira
		}

		if(!retorno) // Ordenação das fronteiras de todas as regiões foram realizadas com sucesso
		{
			num_subregions = vSubRegions.size();
			// se só tiver uma sub-região
			if(vSubRegions.size() == 1)
				vSubRegions[0].SetLocalSouthFrontierIndex(0);
			else {
				// se tiver mais do que uma sub-região
				//Obs: Será necessário também, verificar a ordenação das sub-regiões,
				//		o desenho pode ser feito em qualquer ordem, não garantindo desta
				//		forma que duas regiões da lista vSubRegions seja contíguas ou que
				//		a região I seja a primeira

				//	procura uma sub-região que tenha uma fronteira que pertença a duas
				//  sub-regiões e a fronteira local relacionada só pertença a uma sub-região
				bool		foundFirstSubRegion = false;
				bool		foundFrontier = false;
				int			indexFirstSubRegion = -1;
				vCSubRegion vSubRegionsAux;

				if(vSubRegions.size() == 1) {
					foundFirstSubRegion = true;
					indexFirstSubRegion = 0;
				}

				for(i=0;i<vSubRegions.size() && foundFirstSubRegion == false;i++) {
					index_a = vSubRegions[i].vIndexCCWSortedFrontiers[0];
					index_b = vSubRegions[i].vIndexCCWSortedFrontiers[2];

					if(vSubRegions[i].vpFrontiers[index_a]->GetNumOfSubRegionIndex() == 2 &&
					   vSubRegions[i].vpFrontiers[index_b]->GetNumOfSubRegionIndex() == 1)
					{
						vSubRegions[i].SetLocalSouthFrontierIndex(index_b);
						foundFirstSubRegion = true;
					}

					if(foundFirstSubRegion == false)
					{
						index_a = vSubRegions[i].vIndexCCWSortedFrontiers[2];
						index_b = vSubRegions[i].vIndexCCWSortedFrontiers[0];

						if(vSubRegions[i].vpFrontiers[index_a]->GetNumOfSubRegionIndex() == 2 &&
						   vSubRegions[i].vpFrontiers[index_b]->GetNumOfSubRegionIndex() == 1)
						{
							vSubRegions[i].SetLocalSouthFrontierIndex(index_b);
							foundFirstSubRegion = true;
						}
					}

					if(foundFirstSubRegion == false)
					{
						index_a = vSubRegions[i].vIndexCCWSortedFrontiers[1];
						index_b = vSubRegions[i].vIndexCCWSortedFrontiers[3];

						if(vSubRegions[i].vpFrontiers[index_a]->GetNumOfSubRegionIndex() == 2 &&
						   vSubRegions[i].vpFrontiers[index_b]->GetNumOfSubRegionIndex() == 1)
						{
							vSubRegions[i].SetLocalSouthFrontierIndex(index_b);
							foundFirstSubRegion = true;
						}
					}

					if(foundFirstSubRegion == false) {
						index_a = vSubRegions[i].vIndexCCWSortedFrontiers[3];
						index_b = vSubRegions[i].vIndexCCWSortedFrontiers[1];

						if(vSubRegions[i].vpFrontiers[index_a]->GetNumOfSubRegionIndex() == 2 &&
						   vSubRegions[i].vpFrontiers[index_b]->GetNumOfSubRegionIndex() == 1)
						{
							vSubRegions[i].SetLocalSouthFrontierIndex(index_b);
							foundFirstSubRegion = true;
						}
					}

					if(foundFirstSubRegion == true)
							indexFirstSubRegion = i;

				}//fecha for(i=0;i<pModel->vSubRegions.size() && foundFirstSubRegion == false;i++)

				//ordena as sub-regiões
				if(foundFirstSubRegion == true) {
					//flag auxiliar para indicar se uma sub-região já foi incluída ou não
					for(i=0;i<vSubRegions.size();i++)
						vIsRegionInclude.push_back(0);

					int size, size1, index_south;

				//inclui a sub-região I e atualiza flag de inclusão
					vSubRegionsAux.push_back(vSubRegions[indexFirstSubRegion]);
					vIsRegionInclude[indexFirstSubRegion] = 1;

					do {
						for(i=0;i<vSubRegions.size();i++) {
							size = vSubRegionsAux.size();

							if(!vIsRegionInclude[i]) {
								// verifica se alguma das fronteiras da ultima sub-região incluida
								// é comum a sub-região i
								foundFrontier = false;
								for(j=0;j<4 && foundFrontier == false;j++) {
									CLevFrontier * po_FAux = vSubRegionsAux[size-1].vpFrontiers[j];
									index_south = vSubRegions[i].IsAFrontier(po_FAux);
									if(index_south>=0) {
										foundFrontier = true;
										vSubRegions[i].SetLocalSouthFrontierIndex(index_south);
										vSubRegionsAux.push_back(vSubRegions[i]);
										vIsRegionInclude[i] = 1;
									}
								}
							}
						}

						size = vSubRegionsAux.size();
						size1 = vSubRegions.size();

					} while(vSubRegionsAux.size() != vSubRegions.size() );

					// atualiza o vetor permanente
					vSubRegions.clear();

					for(i=0;i<vSubRegionsAux.size();i++)
						vSubRegions.push_back(vSubRegionsAux[i]);
				} else {
					//inconsistencia de dados: Não encontrou a sub-região 1
					retorno = 3;
				}
			} // fecha else // se tiver mais do que uma sub-região
		}
	} else
		retorno = 2;

	if(retorno) //ocorreu algum erro
	{
		ResetSubRegionsData();

		//elimina as fronteiras com somente uma primitiva
		do {
			found= false;
			num_frontiers = vpFrontiers.size();
			for(i=0;i<vpFrontiers.size();i++) {
				if(vpFrontiers[i]->vpPrimitives.size() == 1) {
					RemoveFrontier(vpFrontiers[i]);
					found = true;
				}
			}
		} while(found == true);
	}

	return retorno;
}


// retorna a posição em que foi incluida ou encontrada a sub-região
int CStructuredMeshPreProcessor::IncludeSubRegion(CSubRegion &subr)
{
	unsigned int i;
	int index = 0;
	bool found = false;

	for(i=0; i<vSubRegions.size() && found == false; i++)
		if(subr == vSubRegions[i]) {
			found = true;
			index = i;
		}

	if(found == false) {
		vSubRegions.push_back(subr);
		index = vSubRegions.size()-1;
	}
	return index;
}


int	CStructuredMeshPreProcessor::CheckSubRegionFrontiersPredisConsistency(int index)
{
	int retorno = 0;

	if(index >=0 && index < (signed)vSubRegions.size()) {
		retorno = vSubRegions[index].CheckFrontiersPredisConsistency();
		//0 : sucesso
		//1 : fronteiras Sul-Norte inconsistentes
		//2 : fronteiras Leste-Oeste inconsistentes
	}

	return retorno;

}


void CStructuredMeshPreProcessor::ResetSubRegionsData(void)
{
 	unsigned int i;
	vSubRegions.clear();
	for(i=0;i<vpFrontiers.size();i++)
		vpFrontiers[i]->ReinitializeAuxiliarData();
}


void CStructuredMeshPreProcessor::RemoveFrontier(int pos)
{
	ResetSubRegionsData();

	delete [] vpFrontiers[pos];
	vpFrontiers[pos] = NULL;

	vpFrontiers.erase(vpFrontiers.begin() + pos);
}


void CStructuredMeshPreProcessor::RemoveFrontier(CLevFrontier *pF)
{
	unsigned int i;
	bool found = false;

	for(i=0; i<vpFrontiers.size() && found == false; i++) {
		if(vpFrontiers[i] == pF) {
			delete [] vpFrontiers[i];

			vpFrontiers[i] = NULL;

			vpFrontiers.erase(vpFrontiers.begin() + i);

			found = true;
		}
	}
}


CLevFrontier* CStructuredMeshPreProcessor::GetFrontier(int ind)
{

	return vpFrontiers[ind];
}

CSubRegion * CStructuredMeshPreProcessor::GetSubRegion(int i)
{
	return &vSubRegions[i];
}

int CStructuredMeshPreProcessor::GetNumFrontiers(void)
{
	return vpFrontiers.size();
}

int CStructuredMeshPreProcessor::GetNumSubRegions(void)
{
	return vSubRegions.size();
}

//-----------------------------------------------------------------------------------
// 	SetIdToValidFrontiers()
//-----------------------------------------------------------------------------------
// *NMA*:
//		Atribui os Id's das Fronteiras. Todas as fronteiras existentes são válidas.
//		Quando é uma delas é removida, é feita uma eliminação da posição do vetor.
//		Para manter uma padronização com SetIdToValidPrimitives e SetIdToValidPoints
//		mantive o Valid no nome .
//-----------------------------------------------------------------------------------
void CStructuredMeshPreProcessor::SetIdToValidFrontiers(void)
{
	unsigned int i;

	// Atribui Id's às fronteiras
	for(i=0;i<vpFrontiers.size();i++)
		vpFrontiers[i]->Id(i);
}


void CStructuredMeshPreProcessor::Update(GEOMETRICOPERATION operation, CLevPrimitive * pPrimitive )
{
	bool found;
	unsigned int i, j, size;
	int  ind;
	CLevFrontier *pFrontierPrim, *pFrontierEast,*pFrontierWest;

	found = false;

	if(operation == DELETEOPERATION) {
		// Se a fronteira for uma fronteira interior (norte ou sul),
		// as fronteiras contíguas (tem um extermo em comum)
		// devem ser deletadas também, pois estas fronteiras ficaram inconsistentes

		for(i=0;i<vpFrontiers.size() && found == false;i++) {
			if(vpFrontiers[i]->IsAPrimitive(pPrimitive) != -1) {
				pFrontierPrim = vpFrontiers[i];

				for(j=1;j<vSubRegions.size();j++) { // se for a fronteria Sul global não precisa deletar
					// se for a sul local, deleta as fronteiras leste e oeste
					ind = vSubRegions[j].GetLocalSouthFrontierIndex();
					if(pFrontierPrim == vSubRegions[j].vpFrontiers[ind]) {
						ind = vSubRegions[j].GetLocalEastFrontierIndex();
						pFrontierEast = vSubRegions[j].vpFrontiers[ind];

						ind = vSubRegions[j].GetLocalWestFrontierIndex();
						pFrontierWest = vSubRegions[j].vpFrontiers[ind];

						RemoveFrontier(pFrontierEast);
						RemoveFrontier(pFrontierWest);
					}
				}

				size = vSubRegions.size();
				//se for a norte local, deleta as fronteiras leste e oeste
				for(j=0;j<(size-1);j++) { // se for a fronteria norte global não precisa deletar
					ind = vSubRegions[j].GetLocalNorthFrontierIndex();
					if(pFrontierPrim == vSubRegions[j].vpFrontiers[ind]) {
						ind = vSubRegions[j].GetLocalEastFrontierIndex();
						pFrontierEast = vSubRegions[j].vpFrontiers[ind];

						ind = vSubRegions[j].GetLocalWestFrontierIndex();
						pFrontierWest = vSubRegions[j].vpFrontiers[ind];

						RemoveFrontier(pFrontierEast);
						RemoveFrontier(pFrontierWest);
					}
				}
				RemoveFrontier(pFrontierPrim);
				found = true;
			}
		}
	}
	ResetSubRegionsData();
}


//-----------------------------------------------------------------------------------------------------
//	SaveSRD()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Grava o arquivo que contém as informações de sub-regiões do modelo, utilizadas para
//				identificação da seqüência das fronteiras para a geração de malha estruturada.
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Maio/2004
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::SaveSRD(std::string fileName)
{
	FILE		*parq = NULL;
	unsigned int i,j;
	int			num_fron, num_subreg=0, num_primitives=0;
	int			prim, prim_type, aux;
	char		msg_open[1000] = " \n\nCould not open file for storing:    ";
	char		msg_writing[1000] = " \n\n Writing file :    ";
	int			codeRet;

	// montagem das mensagens de erro
	strcat(msg_open, fileName.c_str());

	strcat(msg_writing, fileName.c_str());

	num_fron = vpFrontiers.size();

	if(num_fron) {
		parq = fopen(fileName.c_str(),"w");
		if (!parq) {
			throw COutput_Exception(msg_open);
			if(parq)
				fclose(parq);
			return 200;
		} else {
			//gravação das fronteiras .........................................................................

			//atualiza id das fronteiras
			SetIdToValidFrontiers();

			codeRet = fprintf(parq,"%d\n", vpFrontiers.size());

			if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

			for(i=0;i<vpFrontiers.size();i++)
			{
				// gravação do número de primitivas
				num_primitives	= vpFrontiers[i]->NumValidPrimitives();

				codeRet = fprintf(parq,"%d\n",num_primitives);
				if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

				// a gravação tem que seguir a ordem das primitivas, por causa da ordenação por
				// pontos de ligação
				for(j=0;j<(unsigned)num_primitives;j++)
				{
					prim = vpFrontiers[i]->vpPrimitives[j]->Id();

					if(vpFrontiers[i]->vpPrimitives[j]->GeoEntityType() == LINELEV) prim_type = 1;
					else prim_type = 2;

					codeRet = fprintf(parq,"%d %d\n", prim, prim_type);
					if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

				}

				//os outros dados são usados somente no processo construtivo

			}// fecha for(i=0;i<vFrontiers.size();i++)

			num_subreg = vSubRegions.size();

			codeRet = fprintf(parq,"%d\n", num_subreg);
			if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

			if(num_subreg)
			{
				// gravação das sub-regiões .........................................................................

				for(i=0;i<vSubRegions.size();i++)
				{
					// gravação dos índices das fronteiras (tem que existir 4 fronteiras)
					for(j=0;j<vSubRegions[i].vpFrontiers.size();j++)
					{
						aux = vSubRegions[i].vpFrontiers[j]->Id();
						codeRet = fprintf(parq,"\n%d", aux);
						if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

					}

					// gravação da fronteira Sul local
					aux = vSubRegions[i].GetLocalSouthFrontierIndex();
					codeRet = fprintf(parq,"\n%d", aux);
					if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}


					// gravação da região do modelo associada
					aux = vSubRegions[i].GetAssociatedModelRegionIndex();
					codeRet = fprintf(parq,"\n%d", aux);
					if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

					// gravação da sequencia das fronteiras ordenadas
					for(j=0;j<4;j++) // tem que ser 4 fronteiras
					{
						aux = vSubRegions[i].vIndexCCWSortedFrontiers[j];
						codeRet = fprintf(parq,"\n%d", aux);
						if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}
					}
				}// fecha for subregions
			}

			fclose(parq);
		}// fecha else (conseguiu abrir arquivo)
	}// fecha if(num_fron)

	return 0;

}// end of SaveSRD
//-----------------------------------------------------------------------------------------------------
//	SaveGME()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Grava o arquivo que contém as coordenadas dos pontos de discretização das fronteiras
//				para a geração de malha estruturada
//
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Maio/2004
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::SaveGME(std::string fileName)
{
	unsigned int i, j;
	FILE		*parq = NULL;
	int			index;
	int			num_subReg, num_aux;
	CLevPoint	opoint;
	char		msg_open[1000] = " \n\nCould not open file for storing:    ";
	char		msg_writing[1000] = " \n\n Writing file :    ";
	int			codeRet= 0;
	double		x, y;

	num_subReg = vSubRegions.size();

	if(num_subReg) {
		parq = fopen(fileName.c_str(),"w");
		if (!parq) {
			throw COutput_Exception(msg_open);
			if(parq) fclose(parq);
			return 200;
		} else {
			// numero de sub-regiões
			codeRet = fprintf(parq,"%d\n",num_subReg);
			if(codeRet < 0) {
				throw COutput_Exception(msg_writing);
				return 200;
			}

			// numero de segmentos da fronteira Sul
			num_aux = vSubRegions[0].GetNumOfPredisSegmentsSouthFrontier();

			codeRet = fprintf(parq,"%d\n",num_aux);
			if(codeRet < 0) {
				throw COutput_Exception(msg_writing);
				return 200;
			}

			num_aux = 0;
			num_aux = GetNumPointsAllEastFrontier();

			codeRet = fprintf(parq,"%d\n",num_aux);
			if(codeRet < 0) {
				throw COutput_Exception(msg_writing);
				return 200;
			}

			// gravação do pontos
			for(i=0;i<vSubRegions.size();i++) {
				//gravação do número de segmentos da fronteira leste da sub-região i
				num_aux = vSubRegions[i].GetNumOfPredisSegmentsEastFrontier();
				codeRet = fprintf(parq,"%d\n",num_aux);
				if(codeRet < 0) {
					throw COutput_Exception(msg_writing);
					return 200;
				}

				//Constroi vetor de pontos de pré-discretização de todas as fronteiras da sub-região
				vSubRegions[i].BuildFrontiersPrediscretizationVectors();

				//gravação dos pontos das fronteiras com repetição dos pontos extremos

				if(i==0) {
					// gravação de pontos da fronteira sul, somente para a sub-região I

					// coordenadas dos pontos da fronteira sul da sub-região
					index = vSubRegions[i].GetLocalSouthFrontierIndex();

					for(j=0;j<vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size();j++) {
						opoint = vSubRegions[i].vpFrontiers[index]->vPointsFrontier[j];
						x = opoint.X(); y = opoint.Y();
						codeRet = fprintf(parq,"%f %f\n",x,y);
						if(codeRet < 0) {
							throw COutput_Exception(msg_writing) ;
							return 200;
						}
					}
				}

				// coordenadas dos pontos da fronteira leste da sub-região
				index = vSubRegions[i].GetLocalEastFrontierIndex();
				num_aux=vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size();
				for(j=0; j<vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size(); j++) {
					opoint = vSubRegions[i].vpFrontiers[index]->vPointsFrontier[j];
					x = opoint.X(); y = opoint.Y();
					codeRet = fprintf(parq,"%f %f\n",x,y);
					if(codeRet < 0) {
						throw COutput_Exception(msg_writing);
						return 200;
					}
				}

				// coordenadas dos pontos da fronteira norte da sub-região
				index = vSubRegions[i].GetLocalNorthFrontierIndex();
				num_aux = vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size();
				for(j=0;j<vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size();j++) {
					opoint = vSubRegions[i].vpFrontiers[index]->vPointsFrontier[j];
					x = opoint.X(); y = opoint.Y();
					codeRet = fprintf(parq,"%f %f\n",x,y);
					if(codeRet < 0) {
						throw COutput_Exception(msg_writing);
						return 200;
					}
				}

				// coordenadas dos pontos da fronteira oeste da sub-região
				index = vSubRegions[i].GetLocalWestFrontierIndex();
				num_aux = vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size();
				for(j=0;j<vSubRegions[i].vpFrontiers[index]->vPointsFrontier.size();j++) {
					opoint = vSubRegions[i].vpFrontiers[index]->vPointsFrontier[j];
					x = opoint.X(); y = opoint.Y();
					codeRet = fprintf(parq,"%f %f\n",x,y);
					if(codeRet < 0) {
						throw COutput_Exception(msg_writing);
						return 200;
					}
				}
			}
			fclose(parq);
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------------------------------
//	ReadSRD()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Leitura do arquivo que contém as informações de sub-regiões do modelo, utilizadas para
//				identificação da seqüência das fronteiras para a geração de malha estruturada.
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//					Angelo Passaro
//	Data:			Maio/2004
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::ReadSRD(std::string fileName, CLevGeoData * pModel)
{
	FILE			*parq = NULL;
	int				i,j;
	int				num_fron, num_subreg=0, num_primitives=0;
	int				prim, prim_type, aux;
	CLevFrontier	o_frontierAux;
	CSubRegion		o_subRegionAux;

	parq = fopen(fileName.c_str(),"r");
	if(!parq)
		return 200;
	else {
		//leitura das fronteiras .........................................................................

		fscanf(parq,"%d\n", &num_fron);

		for(i=0;i<num_fron;i++) {
			// leitura do número de primitivas
			fscanf(parq,"%d\n",&num_primitives);
			for(j=0;j<num_primitives;j++) {
				fscanf(parq,"%d %d\n", &prim, &prim_type);
				o_frontierAux.IncludeNextPrimitive(pModel->GetPrimitive(prim));
			}

			IncludeFrontier(o_frontierAux);
			o_frontierAux.Reinitialize();
		}

		fscanf(parq,"%d\n", &num_subreg);
		if(num_subreg) {
			// leitura das sub-regiões .........................................................................
			for(i=0;i<num_subreg;i++) {
				// leitura dos índices das fronteiras (tem que existir 4 fronteiras)
				for(j=0;j<4;j++) {
					fscanf(parq,"\n%d", &aux);
					o_subRegionAux.IncludeFrontier(GetFrontier(aux));
				}

				// leitura da fronteira Sul local
				fscanf(parq,"\n%d", &aux);
				o_subRegionAux.SetLocalSouthFrontierIndex(aux);

				// leitura da região do modelo associada
				fscanf(parq,"\n%d", &aux);
				o_subRegionAux.SetAssociatedModelRegionIndex(aux);

				for(j=0;j<4;j++) // inclui zero para atualizar a seguir
					o_subRegionAux.vIndexCCWSortedFrontiers.push_back(0);

				// leitura da sequencia das fronteiras ordenadas
				for(j=0;j<4;j++) // tem que ser 4 fronteiras
				{
					fscanf(parq,"\n%d", &aux);
					o_subRegionAux.vIndexCCWSortedFrontiers[j] = aux;
				}

				IncludeSubRegion(o_subRegionAux);
				o_subRegionAux.Reinitialize();
			} // fecha for subregions
		}
	}
	return 0;
}


void CStructuredMeshPreProcessor::SetRotation(bool r)
{
	rotation = r;
}


void CStructuredMeshPreProcessor::SetSave_go_File(bool s)
{
	save_go_file = s;
}

void CStructuredMeshPreProcessor::SetMaxIterations(int i)
{
	max_iterations = i;
}

void CStructuredMeshPreProcessor::SetPrecision(double p)
{
	precision = p;
}

void CStructuredMeshPreProcessor::SetRelaxationFactor(double r)
{
	relaxation_factor  = r;
}

void CStructuredMeshPreProcessor::SetScaleFactor(double s)
{
	scale_factor = s;
}

void CStructuredMeshPreProcessor::SetIsAxisimetric(bool s)
{
	isAxisimetric = s;
}


bool CStructuredMeshPreProcessor::GetRotation(void)
{
	return rotation;
}

bool CStructuredMeshPreProcessor::GetSave_go_File(void)
{
	return save_go_file;
}

int CStructuredMeshPreProcessor::GetMaxIterations(void)
{
	return max_iterations;
}

double CStructuredMeshPreProcessor::GetPrecision(void)
{
	return precision;
}

double CStructuredMeshPreProcessor::GetRelaxationFactor(void)
{
	return relaxation_factor;
}

double CStructuredMeshPreProcessor::GetScaleFactor(void)
{
	return scale_factor;
}

bool CStructuredMeshPreProcessor::GetIsAxisimetric(void)
{
	return isAxisimetric;
}

bool CStructuredMeshPreProcessor::MeshOk(void)
{
	return meshOk;
}

void CStructuredMeshPreProcessor::MeshOk(bool m)
{
//	if(m == false) vMeshPoints.clear();

	meshOk = m;
}

//-----------------------------------------------------------------------------------------------------
//	SaveIME()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Grava o arquivo que contém as especificações para a geração de malha estruturada
//
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::SaveIME(std::string fileName)
{
	FILE	*parq = NULL;
	int		codeRet = 0;

	char		msg_open[1000] = " \n\nCould not open file for storing:    ";
	char		msg_writing[1000] = " \n\n Writing file :    ";
	char		rot, iprint='T', istore ='T', aksi = 'F';
	char		atrac[7];
	int			num_atrac, num_obstac, num_subregions, num_atrac_sub, index_atrac;
	unsigned int auxLine, auxColumn, auxI, auxJ;
	double		a, b,c, d;

	int			i, j, indexSubregion;

	std::vector<int>  vTemp;
	std::vector<std::vector<int>  >  vdAtractionSubregion;

	parq = fopen(fileName.c_str(),"w");
	if (!parq)
	{
		throw COutput_Exception(msg_open);

		if(parq) fclose(parq);
		return 200;
	}
	else
	{
		if(rotation) rot = 'T';
		else rot = 'F';

		codeRet = fprintf(parq,"%c Rot90\n",rot);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		codeRet = fprintf(parq,"%g PREC maximum erro for grid\n",precision);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		codeRet = fprintf(parq,"%d MAXIT   Maximum IT\n",max_iterations);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		codeRet = fprintf(parq,"%lf  SF (scale factor)\n",scale_factor);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		codeRet = fprintf(parq,"%lf ALFAXY Relaxation parameter for grid generation (lower it if overflow occur)\n",relaxation_factor);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		num_subregions = vSubRegions.size();


		num_atrac = vAtraction.size(); // verifica se existem pontos de atração

		if(num_atrac)
		{

			for(i=0;i<num_subregions;i++)
			{
				// cada posição corresponde a uma sub-região
				vdAtractionSubregion.push_back(vTemp);
			}

			//verifica quais pontos ou linhas de atração são relativas a quais sub-regiões
			for(i=0;i<num_atrac;i++)
			{
				indexSubregion = vAtraction[i].IndexSubRegion(); //o índice começa em zero

				vdAtractionSubregion[indexSubregion-1].push_back(i);
			}

			//grava pontos e/ou linhas de atração de cada sub-região
			for(i=0;i<num_subregions;i++)
			{
				num_atrac_sub = vdAtractionSubregion[i].size();
				if(num_atrac_sub) 	strcpy(atrac,"'CATR'");
				else strcpy(atrac,"'NATR'");

				codeRet = fprintf(parq,"%s \n",atrac);
				if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

				if(num_atrac_sub)
				{
					codeRet = fprintf(parq,"%d \n",num_atrac_sub);
					if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

					for(j=0;j<num_atrac_sub;j++)
					{
						index_atrac =vdAtractionSubregion[i][j];

						auxLine		= vAtraction[index_atrac].Line();
						auxColumn	= vAtraction[index_atrac].Column();
						auxI		= vAtraction[index_atrac].IndexI();
						auxJ		= vAtraction[index_atrac].IndexJ();
						a			= vAtraction[index_atrac].CoefA();
						b			= vAtraction[index_atrac].CoefB();
						c			= vAtraction[index_atrac].CoefC();
						d			= vAtraction[index_atrac].CoefD();

						codeRet = fprintf(parq,"%d %d %d %d %lf %lf %lf %lf\n",auxLine, auxColumn, auxI,auxJ,a,b,c,d);
						if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

					}// close for(j=0;j<num_atrac_sub;j++)
				}
			}//close for(i=0;i<num_subregions;i++)
		}//close if(num_atrac)
		else // não existem pontos ou linhas de
		{
			strcpy(atrac,"'NATR'");

			codeRet = fprintf(parq,"%s \n",atrac);
			if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

			codeRet = fprintf(parq,"%d \n",num_atrac);
			if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		}

//		if(save_go_file) iprint = 'T'; //os dois tem que ser true para gravar
//		else iprint = 'F';
		if(isAxisimetric) aksi = 'T';
		else aksi = 'F';

		num_obstac = vObstacles.size();

		codeRet = fprintf(parq,"%c %c %c %d\n", iprint,istore,aksi,num_obstac);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 200;}

		fclose(parq);

	}

	return 0;

}//end of SaveIME

//-----------------------------------------------------------------------------------------------------
//	ReadIME()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Le o arquivo que contém as especificações para a geração de malha estruturada
//
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::ReadIME(std::string filename)
{
	int		codeErr = 0;

	CStructuredMeshAtraction atraction;
	char    aux[250];
	char    iprint, aksi, istore;
	int		num_atrac, num_obstac, num_subregions;
	unsigned int auxLine, auxColumn, auxI, auxJ;
	double	a, b,c, d;

	int		i,j;

	std::ifstream fin(filename.c_str());

	if(fin.fail())
	{
		codeErr = 100;
	}
	else
	{
		fin >> aux;

		if(aux[0] == 'T') rotation = true;
		else rotation = false;

		fin >> aux; //rot90

		fin >> precision;


		fin >> aux; //PREC
		fin >> aux; //maximum
		fin >> aux; //error
		fin >> aux; //for
		fin >> aux; //grid

		fin >> max_iterations;

		fin >> aux; //MAXIT
		fin >> aux; //Maximum
		fin >> aux; //IT

		fin >> scale_factor;

		fin >> aux; //SF
		fin >> aux; //(scale
		fin >> aux; //factor)

		fin >> relaxation_factor;

		fin >> aux; //ALFAXY
		fin >> aux; //Relaxation
		fin >> aux; //parameter
		fin >> aux; //for
		fin >> aux; //grid
		fin >> aux; //generation
		fin >> aux; //(lower
		fin >> aux; //it
		fin >> aux; //if
		fin >> aux; //overflow
		fin >> aux; //occur)


		num_subregions = vSubRegions.size();

		for(j=0;j<num_subregions;j++)
		{
			fin >> aux; //CATR ou NATR

			if(aux[1] == 'C')
			{
				fin >> num_atrac;

				if(num_atrac)
				{
					vAtraction.clear();

					for(i=0;i<num_atrac;i++)
					{
						fin >> auxLine;
						fin >> auxColumn;
						fin >> auxI;
						fin >> auxJ;
						fin >> a;
						fin >> b;
						fin >> c;
						fin >> d;

						atraction.Line(auxLine);
						atraction.Column(auxColumn);
						atraction.IndexI(auxI);
						atraction.IndexJ(auxJ);
						atraction.CoefA(a);
						atraction.CoefB(b);
						atraction.CoefC(c);
						atraction.CoefD(d);
						atraction.IndexSubRegion(j);

						vAtraction.push_back(atraction);

					}//fecha for(i=0;i<num_atrac;i++)
				}//fecha if(num_atrac)
			}//fecha if if(aux != "'NATR'")


		}

		fin >> iprint;
		if(iprint == 'T') save_go_file =true;
		else save_go_file = false;

		fin >> istore;

		fin >> aksi;

		fin >> num_obstac;

		fin.close();

	} //fecha else

	return codeErr;

}//end of ReadIME

//-----------------------------------------------------------------------------------------------------
//	ReadPLT()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Leitura do arquivo que contém as coordenadas dos pontos gerados pelo gerador de malhas
//				estruturadas
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::ReadPLT(std::string filename)
{
	unsigned int i, num_total = 0;
	char aux[250];
	double x, y;
	int codeErr = 0;
	CLevPoint o_point;

	vMeshPoints.clear();

	std::ifstream fin(filename.c_str());

	if(fin.fail())
		codeErr = 100;
	else {
		fin >> aux;
		fin >> aux;
		fin >> aux;
		fin >> num_J;

		fin >> aux;
		fin >> aux;
		fin >> num_I;

		fin >> aux;
		fin >> aux;

		num_total = num_I * num_J;

		for(i=0;i<num_total;i++) {
			fin >> x;
			fin >> y;

			o_point.X(x);
			o_point.Y(y);

			vMeshPoints.push_back(o_point);
		}

		if(vMeshPoints.size() != num_total)
			codeErr = 10;

		fin.close();
	}

	if(!codeErr)
		MeshOk(true);

	return codeErr;
}


//-----------------------------------------------------------------------------------------------------
//	ReadPLT()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Leitura do arquivo que contém as coordenadas dos pontos gerados pelo gerador de malhas
//				estruturadas e conectividades dos pontos dos baricentros
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::ReadBAR(std::string filename)
{
	int				i;
	unsigned int num_total = 0;
	char            aux[250];
	double			x, y;

	int				num_rectangular_elem;

	int				codeErr = 0;

	CLevPoint			o_point;
	CRectangularElement	el;
	Aresta_Geometria	aresta_geo;

	std::vector<int> vertices;
	std::vector<int> viz;


	vBaricenterPoints.clear();
	vRectangularElements.clear();
	vArestasGeometria.clear();

	std::ifstream fin(filename.c_str());

	if(fin.fail())
		codeErr = 100;
	else {
		fin >> aux;
		fin >> aux;
		fin >> aux;
		fin >> num_J;

		fin >> aux;
		fin >> aux;
		fin >> num_I;

		fin >> aux;
		fin >> aux;

		num_total = num_I * num_J;

		for(i=0;i<(signed)num_total;i++) {
			fin >> x;
			fin >> y;
			o_point.X(x);
			o_point.Y(y);
			vBaricenterPoints.push_back(o_point);
		}

		if(vBaricenterPoints.size() != num_total)
			codeErr = 10;

		if(!codeErr)
		{
			int v1, v2, v3, v4, viz1, viz2, viz3, viz4;

			fin >> num_rectangular_elem;

			for(i=0;i<num_rectangular_elem;i++)
			{
				vertices.clear();
				viz.clear();

				fin >> v1;
				fin >> v2;
				fin >> v3;
				fin >> v4;
				vertices.push_back(v1-1);
				vertices.push_back(v2-1);
				vertices.push_back(v3-1);
				vertices.push_back(v4-1);

				fin >> viz1;
				fin >> viz2;
				fin >> viz3;
				fin >> viz4;
				if(viz1 > 0) viz1--;
				viz.push_back(viz1);

				if(viz2 > 0) viz2--;
				viz.push_back(viz2);

				if(viz3 > 0) viz3--;
				viz.push_back(viz3);

				if(viz4 > 0) viz4--;
				viz.push_back(viz4);

				el.SetVertices(vertices);
				el.SetNeighbors(viz);

				vRectangularElements.push_back(el);

				// verifica se vizinho viz1 é -1: se for a aresta v1v2 é de geometria
				// no máximo duas arestas podem ser de geometria
				if(viz1 == -1)
				{
					aresta_geo.g1 = v1-1;
					aresta_geo.g2 = v2-1;
					aresta_geo.prim = 0;
					aresta_geo.primType = LINELEV;
					vArestasGeometria.push_back(aresta_geo);
				}
				if(viz2 == -1)
				{
					aresta_geo.g1 = v2-1;
					aresta_geo.g2 = v3-1;
					aresta_geo.prim = 0;
					aresta_geo.primType = LINELEV;
					vArestasGeometria.push_back(aresta_geo);
				}
				if(viz3 == -1)
				{
					aresta_geo.g1 = v3-1;
					aresta_geo.g2 = v4-1;
					aresta_geo.prim = 0;
					aresta_geo.primType = LINELEV;
					vArestasGeometria.push_back(aresta_geo);
				}
				if(viz4 == -1)
				{
					aresta_geo.g1 = v4-1;
					aresta_geo.g2 = v1-1;
					aresta_geo.prim = 0;
					aresta_geo.primType = LINELEV;
					vArestasGeometria.push_back(aresta_geo);
				}

			}

		}

		fin.close();
	}

//	if(!codeErr) MeshOk(true);

	return codeErr;

}//end of ReadBAR


//-----------------------------------------------------------------------------------------------------
//	SavePLT()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Gravação do arquivo que contém as coordenadas dos pontos gerados pelo gerador de malhas
//				estruturadas. Implementada para Save As.
//
//	parâmetros:	std::string fileName: nome do arquivo com path completo.
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
#if 0
int CStructuredMeshPreProcessor::SavePLT(std::string filename)
{
	int				i;
	//int				num_I, num_J=0, num_total=0;
	int				num_total = 0, size =0;
	double			x, y;

	int				codeErr = 0;

	CLevPoint		o_point;


	std::ofstream fout(filename.c_str());

	if(fout.fail())
	{
		codeErr = 100;
	}
	else
	{
		fout << "VARIABLES=X,Y " << "\n";
		fout << "ZONE, J= ";

		fout << num_J;


		fout << ", I= ";

		fout << num_I;


		fout << ", F=POINT" << "\n";

		num_total = num_I * num_J;

		size = vMeshPoints.size();

		for(i=0;i<size;i++)
		{
			x = vMeshPoints[i].X();
			y = vMeshPoints[i].Y();

			fout << x;
			fout << "  ";
			fout << y;

			fout << "\n";
		}


		fout.close();
	}

	return codeErr;

}//end of SavePLT
#endif
int CStructuredMeshPreProcessor::SavePLT(std::string filename)
{
	int				i;
	int				num_total = 0, size =0;
	double			x, y;
	int				codeErr = 0;
	FILE			*parq = NULL;
	char			msg_open[1000] = " \n\nCould not open file for storing:    ";
	char			msg_writing[1000] = " \n\n Writing file :    ";
	int				codeRet= 0;

	CLevPoint		o_point;


	parq = fopen(filename.c_str(),"w");
	if (!parq)
	{
		throw COutput_Exception(msg_open);
		if(parq) fclose(parq);
		codeErr = 100;
		return 100;
	}
	else
	{
		codeRet = fprintf(parq, "VARIABLES=X,Y\n");
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 100;}
		codeRet = fprintf(parq, "ZONE, J= %d, I= %d, F=POINT\n",num_J, num_I);
		if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 100;}

		num_total = num_I * num_J;

		size = vMeshPoints.size();

		for(i=0;i<size;i++)
		{
			x = vMeshPoints[i].X();
			y = vMeshPoints[i].Y();

			codeRet = fprintf(parq, "    %10.5E  %10.5E\n",x, y);
			if(codeRet < 0){ throw COutput_Exception(msg_writing) ; return 100;}
		}


		fclose(parq);
	}

	return codeErr;

}//end of SavePLT

//---------------------------------------------------------------------------------------
// Descrição: Recebe o nome do projeto, com path e monta o nome dos arquivos
//---------------------------------------------------------------------------------------
void CStructuredMeshPreProcessor::BuildFileNames(std::string nome)
{
	GMEfilename = nome;
	IMEfilename = nome;
	PLTfilename = nome;
	SRDfilename = nome;

	GMEfilename.append(".gme");
	IMEfilename.append(".ime");
	PLTfilename.append(".plt");
	SRDfilename.append(".srd");

	// Para montar os nomes fixos
	unsigned int j;
	short pos_aux;
	int   i, size;
	std::string	nome_GME_FIX = "Ggv1.gme";
	std::string	nome_IME_FIX = "iv1.ime";
	std::string	nome_PLT_FIX = "Ggv1.plt";
	std::string	nome_BAR_FIX = "Gtv1.plt";

	FIX_GMEfilename = " ";
	FIX_IMEfilename = " ";
	FIX_PLTfilename = " ";
	FIX_BARfilename = " ";

	pos_aux = nome.rfind('\\');

	// Nome GME fixo
	size = pos_aux+ 2 + nome_GME_FIX.size();
	FIX_GMEfilename.resize(size);

	for(i=0;i<=pos_aux;i++)
	 	FIX_GMEfilename[i] = nome[i];

	for(j=0;j<=nome_GME_FIX.size();j++)
	 	FIX_GMEfilename[pos_aux+j+1] = nome_GME_FIX[j];

	// Nome IME fixo
	size = pos_aux+ 2 + nome_IME_FIX.size();
	FIX_IMEfilename.resize(size);

	for(i=0;i<=pos_aux;i++)
	 	FIX_IMEfilename[i] = nome[i];

	for(j=0;j<=nome_IME_FIX.size();j++)
	 	FIX_IMEfilename[pos_aux+j+1] = nome_IME_FIX[j];

	// Nome PLT fixo
	size = pos_aux+ 2 + nome_PLT_FIX.size();
	FIX_PLTfilename.resize(size);

	for(i=0;i<=pos_aux;i++)
	 	FIX_PLTfilename[i] = nome[i];

	for(j=0;j<=nome_PLT_FIX.size();j++)
	 	FIX_PLTfilename[pos_aux+j+1] = nome_PLT_FIX[j];

	// nome BAR fixo
	size = pos_aux+ 2 + nome_BAR_FIX.size();
	FIX_BARfilename.resize(size);

	for(i=0;i<=pos_aux;i++)
	 	FIX_BARfilename[i] = nome[i];

	for(j=0;j<=nome_BAR_FIX.size();j++)
	 	FIX_BARfilename[pos_aux+j+1] = nome_BAR_FIX[j];
} // End of SetProjectName


//-----------------------------------------------------------------------------------------------------
//	ReadAll()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Le todos os arquivos relativos à geração de malha estruturada
//
//
//	retorno:
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::ReadAll(std::string projectpathname, CLevGeoData * pModel)
{
	int codeErr = 0;

	BuildFileNames(projectpathname);

	codeErr = ReadSRD(SRDfilename, pModel);


	if(!codeErr)
		codeErr = ReadIME(IMEfilename);


	if(!codeErr)
		codeErr = ReadPLT(PLTfilename);

	return 	codeErr;

}//end of ReadAll


//-----------------------------------------------------------------------------------------------------
//	SaveAll()
//-----------------------------------------------------------------------------------------------------
//
//	Descrição: 	Grava todos os arquivos relativos à geração de malha estruturada
//
//
//	retorno: 0 : sucesso
//
//	Observações:
//
//......................................................................................................
//	Versão:			1.0
//	Autor:			Nancy Mieko Abe
//	Data:			Março/2005
//	Modificado por:
//	Modificado em:
//	Motivo:
//-----------------------------------------------------------------------------------------------------
int CStructuredMeshPreProcessor::SaveAll(std::string projectpathname)
{
	unsigned int i;
	int codeErr = 0, predis_ok = 0;

	BuildFileNames(projectpathname);

	codeErr = SaveSRD(SRDfilename);

	if(!codeErr)
		codeErr = SaveIME(IMEfilename);

	if(!codeErr) {
		for(i=0;i<vSubRegions.size() && predis_ok == 0;i++)
			predis_ok = CheckSubRegionFrontiersPredisConsistency(i);

		if(!predis_ok)
			codeErr = SaveGME(GMEfilename);
		else
			codeErr = 10;
	}

	if(!codeErr) {
		if(vMeshPoints.size())
			codeErr = SavePLT(PLTfilename);
	}

	// grava arquivos com nomes fixos
	if(!codeErr)
		codeErr = SaveIME(FIX_IMEfilename);

	if(!codeErr)
		codeErr = SaveGME(FIX_GMEfilename);

	if(!codeErr)
		codeErr = SavePLT(FIX_PLTfilename);

	return 	codeErr;
} // End of SaveAll


int CStructuredMeshPreProcessor::GetNumPointsSouthFrontier(void)
{
	int num;

	num = vSubRegions[0].GetNumOfPredisSegmentsSouthFrontier();

	return num;
}


int CStructuredMeshPreProcessor::GetNumPointsAllEastFrontier(void)
{
	unsigned int i;
	int num_aux=0;

	// somatório do número de segmentos das fronteiras leste
	for(i=0;i<vSubRegions.size();i++)
		num_aux += vSubRegions[i].GetNumOfPredisSegmentsEastFrontier();

	return num_aux;

}

void CStructuredMeshPreProcessor::Scale(double factor)
{
	unsigned int i;

	for(i=0;i<vMeshPoints.size();i++)
		vMeshPoints[i].Scale(factor);

	for(i=0;i<vMeshPoints.size();i++)
		vMeshPoints[i].FixEntity(false);
}
