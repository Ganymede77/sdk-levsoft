#ifndef __LEVPHYSDATA__H_
#define __LEVPHYSDATA__H_


//=============================================================================
//	Arquivo:			levphysdata.h
//.............................................................................
//	Descrição:			Interface da classe CLevPhysData
//.............................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//-----------------------------------------------------------------------------
//	Classe CLevPhysData
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associações:
//
//	Observações:
//
//	Esta classe faz o tratamento de dados físicos. É uma classe do tipo
//  observer e visitor da classe CLevGeoData. Inicialmente recebeu as funções
//  da classe CLevGeoPhysData, a qual foi desativada.
//
//.............................................................................
//	Data:				14/Abril/2005
//	Modificado em:
//	Modificado por:
//	Motivo:
//=============================================================================


#include "boundcond.h"
#include "levgeodata.h"
#include "material_lev.h"
#include "observerbase.h"


class CLevPhysData : public CObserverBase
{

public:
	CLevPhysData(); //Constructor
	~CLevPhysData(); //Destructor

	CLevGroup				oGroupBoundaryConditions;

	// vetor que guarda objetos CBoundCond em uso (nas primtivas é armazenado um índice para este vetor)
	vCLevBoundCond			vBoundaryConditions;

	// vetor que guarda objetos CSource em uso (nas primtivas é armazenado um índice para este vetor)
	vCLevSource				vSourcesInUse;

	// vetor que guarda objetos CMaterial lidos do banco de dados
	vCLevMaterial			vMaterialDB; // Materiais do "banco de dados"
	// vetor que guarda objetos CMaterial em uso (nas primtivas é armazenado um índice para este vetor)
	vCLevMaterial			vMaterialInUse; // Materiais em uso

//Attributes
protected:

	std::vector<std::string> propfismei_lines; //*NMA* 2016-03-23: armazena as linhas do arquivo propfis.mei como string, assim nao eh necessario saber quantas colunas tem no arquivo

	double	**materiais;
	int		*codigos;
	int						NM, NELT, NCCOR, NMAG; // auxiliares que não usadas para gravar *.mei

	lev_enum::LEV_BOOL				isBoundaryConditionModified;
	lev_enum::LEV_BOOL				isMaterialModified;
	lev_enum::LEV_BOOL				isSourceModified;

	bool					readpropfisOk;

	CLevGeoData				*pModel;


//Operations
public:
			void			AssociateGeoModel(CLevGeoData *);

			void			Update(GEOMETRICOPERATION,CLevPrimitive * pPrimitive = NULL);


	virtual void			FreeMemory(void);
	virtual void			Reinitialize(void);

	//........................ Condições de Contorno .....................................
			void			GetBoundaryConditions(vCLevBoundCond &);
			int				GetNumValidBoundaryConditions(void);
			int				GetNumBoundaryConditions(void);

			long			IncludeBoundaryCondition(CLevGroup &, CBoundCond & ); //associate
			int				RemoveBoundaryCondition(CLevGroup &);

			long			IncludeDirichletNode(double);
			long			IncludeInBoundaryConditionList(CBoundCond & );


			//........................ Materials ..................................................

			long			IncludeInMaterialInUseList(long);
			long			IncludeInMaterialList(CMaterial &);
			void			GetMaterialsInUse(vCLevMaterial &);
			void			GetMaterialsDB(vCLevMaterial &);

			int				Get_Num_Materials();
//			std::string		Get_Material_Name_Color(int, long &); //*NMA* 06/01/2015: substituido QColor por levcolorsRGB
			std::string		Get_Material_Name_Color(int, CLEV_ColorRGB &); //*NMA* 06/01/2015: substituido QColor por levcolorsRGB

			int				GetNumValidMaterialsInUse(void);
			int				GetNumMaterialsInUse(void);
			bool			IsAPermanentMagnet(int);

//			int				AssociateMaterialToRegion(int,long , long  , CLevPoint &, double); //*NMA* 06/01/2015: QColor substituido por CLEV_ColorRGB
			int				AssociateMaterialToRegion(int,long , CLEV_ColorRGB, CLevPoint &, double); //*NMA* 06/01/2015: QColor substituido por CLEV_ColorRGB
			int				AssociateMaterialToRegion(int,long , CLevPoint &, double);

//........................ Source ..........................................................
			int				AssociateSourceToRegion(int,CSource &);
			void			GetSourcesInUse(vCLevSource &);
			int				GetNumValidSourcesInUse(void);
			int				GetNumSourcesInUse(void);

			int				RemoveSourceInUse(int );
			int				RemoveMaterialInUse(int );

			long			IncludeInSourceInUseList(CSource & );

			//................................................................................
			//......................... operadores............................................
			//................................................................................
	CLevPhysData & operator =  ( const CLevPhysData &d);

	//................................................................................
	//......................... Gravação e Leitura............................................
	//................................................................................

	virtual int				Read(void);
	//..........................Condições de contorno..................................
			int				SaveBoundaryConditionsInUse(std::string );
	virtual int				ReadBoundaryConditionsInUse(std::string );

			int				SaveBoundaryConditionsPrimitives(std::string );
	virtual int				ReadBoundaryConditionsPrimitives(std::string );

	//.......................... Fontes ..................................
			int				SaveSourcesInUse(std::string );
	virtual int				ReadSourcesInUse(std::string );

	//.......................... Materiais ..................................
			int				SaveMaterialsInUse(std::string );
	virtual int				ReadMaterialsInUse(std::string );

	int				SaveMaterialsInUse01(std::string );
virtual int				ReadMaterialsInUse01(std::string );

	bool			ReadpropfisOk(void) {return readpropfisOk;}
	void			ReadpropfisOk(bool status) {readpropfisOk=status;}
	int				ReadPropFis(std::string ); //*NMA* 2015-03-26: OBSOLETO : substituido por ReadPropFis01(std::string );
	int				SavePropFis(std::string); //*NMA* 2015-03-26: OBSOLETO : substituido por sAVEPropFis01(std::string );

	int				ReadPropFis01(std::string ); //eliminado o uso da matriz dimensionada estaticamente e le as propriedades considerando a soma de de NELT, NCCOR e NMAG
	int				SavePropFis01(std::string);

	int				ReadPropFis02(std::string ); //le as linhas do arquivo como strings e armazena as linhas no vetor de strings propfismei_lines
	int				SavePropFis02(std::string); // grava o arquivo profis.mei a partir do vetor de strings propfismei_lines

	//................................................................................
			lev_enum::LEV_BOOL		IsBoundaryConditionModified(void);
			void			IsBoundaryConditionModified(lev_enum::LEV_BOOL);
			lev_enum::LEV_BOOL		IsMaterialModified(void);
			void			IsMaterialModified(lev_enum::LEV_BOOL);
			lev_enum::LEV_BOOL		IsSourceModified(void);
			void			IsSourceModified(lev_enum::LEV_BOOL);
};

#endif
