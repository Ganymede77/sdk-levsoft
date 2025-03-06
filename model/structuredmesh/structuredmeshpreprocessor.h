#ifndef __STRUCTUREDMESHPREPROCESSOR_H_
#define __STRUCTUREDMESHPREPROCESSOR_H_

//===================================================================================================
//	Arquivo:			structuredmeshpreprocessor.h
//...................................................................................................
//	Descri��o:			Interface da classe CStructuredMeshPreProcessor
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//
//---------------------------------------------------------------------------------------------------
//	Classe CStructuredMeshPreProcessor
//---------------------------------------------------------------------------------------------------
//	Dados membros:
//
//	Associa��es:
//			classe: CLevGeoData (modelo geom�trico)
//
//	Descri��o:
//		- Design Pattern: observer
//		Classe utilizada para agregar os dados e funcionalidades necess�rios para gera��o
//			de uma malha estruturada. A classe CStructuredMeshPreProcessor armazena as infoma��es
//          de quais primitivas comp�em uma fronteira mas n�o altera a estrutura
//			de dados do modelo geom�trico. Por outro lado, quando a geometria � alterada,
//			ela deve ser atualizada, para evitar inconsistencias com o modelo modificado.
//
//
//
//	Observa��es:
//
//...................................................................................................
//	Data:				21/Fevereiro/2005
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================
#include <deque>

#include "levgeodata.h"
#include "observerbase.h"

#include "levfrontier.h"
#include "levregion.h"
#include "atraction.h"
#include "obstacles.h"
#include "subregion.h"
#include "rectangularelement.h"

#include "lev_memory_exception.hpp"
#include "lev_input_exception.hpp"
#include "lev_output_exception.hpp"

class CStructuredMeshPreProcessor : public CObserverBase
{
protected:

//	CLevGeoData * pModel;//*NMA*: verificar se é necessario manter esta associacao

	bool	rotation;
	bool	save_go_file;
	int		max_iterations;
	double	precision;
	double  relaxation_factor;
	double	scale_factor;
	bool	isAxisimetric;
	bool	meshOk;

	int		num_I, num_J;

	std::string project_name;
	std::string GMEfilename;
	std::string IMEfilename;
	std::string PLTfilename;
	std::string SRDfilename;

	std::string FIX_GMEfilename;//nome fixo para arquivo .gme
	std::string FIX_IMEfilename;//nome fixo para arquivo .ime
	std::string FIX_PLTfilename;//nome fixo para arquivo .plt
	std::string FIX_BARfilename;//nome fixo para arquivo .bar


public:
	vpCLevFrontier	vpFrontiers;
	vCSubRegion		vSubRegions;


	std::vector<CStructuredMeshAtraction>	vAtraction;
	std::vector<CStructuredMeshObstacle>	vObstacles;

	std::deque<CLevPoint>					vMeshPoints;
	std::deque<CLevPoint>					vBaricenterPoints;
	std::deque<Aresta_Geometria>			vArestasGeometria;
	std::deque<CRectangularElement>			vRectangularElements;

//operations

			CStructuredMeshPreProcessor();
			~CStructuredMeshPreProcessor();

			void		Associate (CLevGeoData * pm);//*NMA*: verificar se � necess�rio manter esta associa��o
			void		Update(GEOMETRICOPERATION,CLevPrimitive * pPrimitive = NULL);

	CLevFrontier*		CreateFrontier(void);
			int			IncludeFrontier(CLevFrontier &);
			int			IncludeSubRegion(CSubRegion &);

			int			IsAFrontier(CLevPoint & , CLevGroup & , int &,double, double);

			int			IdentifySubregions(CLevGeoData *);

			int			CheckSubRegionFrontiersPredisConsistency(int);
			void		ResetSubRegionsData(void);
			void		RemoveFrontier(int);
			void		RemoveFrontier(CLevFrontier *);
	CLevFrontier*		GetFrontier(int);
	CSubRegion*			GetSubRegion(int);
			int			GetNumFrontiers(void);
			int			GetNumSubRegions(void);

			void		SetRotation(bool);
			void		SetSave_go_File(bool);
			void		SetMaxIterations(int);
			void		SetPrecision(double);
			void		SetRelaxationFactor(double);
			void		SetScaleFactor(double);
			void		SetIsAxisimetric(bool);

			bool		GetRotation(void);
			bool		GetSave_go_File(void);
			int			GetMaxIterations(void);
			double		GetPrecision(void);
			double		GetRelaxationFactor(void);
			double		GetScaleFactor(void);
			bool		GetIsAxisimetric(void);

			void		BuildFileNames(std::string);

			std::string GetGMEFileName(void){return GMEfilename;}
			std::string GetIMEFileName(void){return IMEfilename;}
			std::string GetPLTFileName(void){return PLTfilename;}
			std::string GetFIXGMEFileName(void){return FIX_GMEfilename;}
			std::string GetFIXIMEFileName(void){return FIX_IMEfilename;}
			std::string GetFIXPLTFileName(void){return FIX_PLTfilename;}
			std::string GetFIXBARFileName(void){return FIX_BARfilename;}


			// dados para gera��o da malha estruturada
			int			SaveAll(std::string);
			int			SaveGME(std::string); //grava dados de entrada para o gerador de malha estruturada
			int			SaveIME(std::string); //grava especifica��es para o gerador de malha estruturada
			int			SaveSRD(std::string); //grava dados de sub-regi�es e fronteiras
			int			SavePLT(std::string); //grava arquivo de pontos gerado pelo gerador de malhas

			int			ReadAll(std::string, CLevGeoData *); // le todos os arquivos relativos a malha estruturada
			int			ReadSRD(std::string, CLevGeoData *pModel); //l� dados de sub-regi�es e fronteiras
			int			ReadIME(std::string); //le especifica��es para o gerador de malha estruturada
			int			ReadPLT(std::string); // le arquivo de pontos gerado pelo gerador de malhas
			int			ReadBAR(std::string); // le arquivo de pontos gerado pelo gerador de malhas

			void		SetIdToValidFrontiers(void);

			void		MeshOk(bool);
			bool		MeshOk(void);

			int			GetNumI(void){return num_I;} //informa��es lidas do arquivo gravado pelo gerador de malhas
			int			GetNumJ(void){return num_J;} //informa��es lidas do arquivo gravado pelo gerador de malhas

			int			GetNumPointsSouthFrontier(void);
			int			GetNumPointsAllEastFrontier(void);

			void		Scale(double factor);


};



#endif
