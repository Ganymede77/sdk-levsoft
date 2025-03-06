#ifndef __LEV_GEODATA_H_
#define __LEV_GEODATA_H_

//=============================================================================
//	Arquivo:			LevGeoData.h
//.............................................................................
//	Descricao:			Interface da classe CLevGeoData
//.............................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//-----------------------------------------------------------------------------
//	Classe CLevGeoData:
//-----------------------------------------------------------------------------
//	Dados membros:
//
//	Associacoes:
//
//	Observacoes:
//
//.............................................................................
//	Data:				Junho/2000
//	Modificado em:
//	Modificado por:
//	Motivo:
//=============================================================================

#include <iostream>
#include "levpoint.h"
#include "levline.h"
#include "levellipse.h"
#include "levgroup.h"
#include "levfrontier.h"
#include "levregion.h"
#include "worldwindow.h"
#include "project_model.h"
#include "levenum.h"
#include "levtypedefmalha.h"
#include "levtypedef.h"
#include "arrowdevice.h"
#include "modulatordevice.h"
#include "pcfdevice.h"
#include "elemento_2d.hpp"
#include "lev_memory_exception.hpp"
#include "lev_input_exception.hpp"
#include "observerbase.h"
#include "ponto.hpp"

class CLevGeoDataHistoryKeeper; // Declaracao para CLevPreModel conhecer a classe

class CLevGeoData
{

protected:

	vpCObserverBase	vpObservers;

public:

	static CLevGeoDataHistoryKeeper * historykeeper;

	// *NMA*: s� para DEBUG
	CLevPrimitive * pPrimDebug;
	CLevPoint * pPointDebug;

	lev::CProject_Model	* project;

	// Primitivas de modelagem geom�trica
	vCArrowDevice vArrowDevice;
	vCLevLine vEdges;
	vCLevGroup vGroups;
	vCModulatorDevice vModulatorDevice;
	vCPcfDevice	vPcfDevice;

	vpCLevPrimitive	vpPrimitives;
	vpCLevPoint	vpPoints;
	vpCLevPoint	vpInvisiblePoints;
	vpCLevRegion vpModelRegions;

	// Dados para identifica��o de regioes fechadas do modelo geom�trico
	vElemento2D	vElementos_fronteira;
	vCLevPtMalha vPontos_fronteira;
	vArestaGeometria vArestasGeometria_fronteira;

	int numValidPrimitives;
	int numValidPoints;
	int numValidEdges;

	CLevGeoData();
	~CLevGeoData();

	//=========================================================================
	// RYT: OK
	CLevGeoData(const CLevGeoData &other);  // Construtor de copia
	CLevGeoData & operator=(const CLevGeoData &other); // Operador de atribuicao
	//=========================================================================

	void BuildObjectsHistory(void);
	void RecoverObjectsHistory(void);

	virtual void Reinitialize(void);
	virtual void FreeMemory(void);

	int	Include(CLevPoint &, CLevPoint &, bool verifyCrosses = true); // Inclus�o de linha
	int	Include(CLevEllipse &elipse, bool verifyCrosses); // Inclus�o de elipse
	int	Include(double xc, double yc, double xSemiA, double ySemiA, double angleIni, double angleFim); // Inclus�o de elipse

	virtual int	Include(CLevGroup &);
	virtual int	IncludeArrowDevice(CArrowDevice &arrow);
	virtual int	IncludeModulatorDevice(CModulatorDevice &);
	virtual int	IncludePcfDevice(CPcfDevice &);

	virtual int	IsAPrimitive(CLevPoint &, CLevLine &, int &, double, double ); //linha
	virtual int IsAPrimitive(CLevPoint &, CLevEllipse &, int &, double , double );//elipse
	int IsAGroup(CLevPoint & , CLevGroup & , int &,double, double);

	void DeleteGroup(int);
	void Ungroup(int);

	void LocateLineCrosses(int);
	void LocateLineCrosses(void); // para atualizar ap�s move
	void LocateEllipsesCrosses(int);
	void LocateEllipsesCrosses(void);
	void LocateCrosses(void);
	void LocateGivenLineAllEllipsesCrosses(int);
	void LocateGivenEllipseAllLinesCrosses(int);

	CLevGroup FindPrimitivesInsideRect(CLevPoint&, CLevPoint&);

	CLevGroup *	GetGroup(int);
	int	GetNumValidPoints(void); // n�mero de pontos v�lidos
	int	GetNumValidPrimitives(void); // n�mero de primitivas v�lidos
	int	GetNumValidEllipses(void); //numero de elipses v�lidas
	int	GetNumGroups(void);

	void MovePrimitive(int pos, double deltax, double deltay, double deltaz=0, bool locateCrosses=true);
	void MovePoint(int pos, double deltax, double deltay, double deltaz=0, bool locateCrosses=true);
	void MoveGroup(int pos, double deltax, double deltay, double deltaz=0, bool locateCrosses=true);

	void ReflectionLine(int pos);

	void RotateGroup(int,double, CLevPoint &, bool = true);

	int	CreateByRotation(CLevEllipse , CLevPoint &, double );
	int	CreateByRotation(CLevLine , CLevPoint &, double );

	int	CreateByDisplacement(CLevLine, double , double );
	int	CreateByDisplacement(CLevEllipse, double , double );
	int	CreateByDisplacement(CLevGroup, double , double );

	int	CreateByReflection(CLevLine, OPERATIONCONTROL,CLevPoint &,CLevPoint & );

	int	CreateByReflection(CLevEllipse, OPERATIONCONTROL,CLevPoint &,CLevPoint & );

	lev_enum::LEV_BOOL IsGeometryModified(void) { return isGeometryModified; }
	void IsGeometryModified(lev_enum::LEV_BOOL);

	int	SaveGeometricModel(std::string);
	int	SaveModelMeshFrontier(std::string); // grava dados da malha de fronteira para identifica��o de regi�es fechadas do modelo geom�trico
	int	SaveModelRegions(std::string );

	int	ReadELL(std::string);

	void SetIdToValidPoints(void);
	void SetIdToValidPrimitives(void);
	void SetIdToValidModelRegions(void);

	void PreDiscretizePrimitive(int , vCLevPoint &);
	void GetPrimitivePreDiscretization(int,vCLevPoint &);
	void SetPrimitivePreDiscretizationData(int position,int nPoints, double factor, CLevPoint clickedPoint);

	void MeshFronteiraOk(bool status){meshfronteiraOk = status;}
	bool MeshFronteiraOk(void){return meshfronteiraOk;}

	void ModelRegionsOk(bool status){modelregionsOk = status;}
	bool ModelRegionsOk(void){return modelregionsOk;}

	void ModelPrimitivesOk(bool status){modelprimitivesOk = status;}
	bool ModelPrimitivesOk(void){return modelprimitivesOk;}

	CLevLine * IncludeEdge(CLevPoint &, CLevPoint &,int & );
	void BuildEdges(void);
	void BuildEdgesFronteira(void);
	void BuildVetorArestasPrimitivas(vArestaGeometria &);

	//*NMA* Deve ser eliminado quando as fun��es forem para Doc
	void Associate(lev::CProject_Model *pProj) { project = pProj; }

	void AttachObserver(CObserverBase *);
	void DetachObserver(CObserverBase *);
	void Notify(GEOMETRICOPERATION, CLevPrimitive *pPrim=NULL);

	virtual int	DeletePoint(int);
	virtual void DeletePrimitive(int);
	void Associate(CLevGeoDataHistoryKeeper * hk){ historykeeper = hk;}

	CLevPoint * Include(CLevPoint &ro_point,GEOENTITYTYPE type = GEOM);// inclus�o de pontos
	int Include(CLevLine &); // Inclus�o de linhas
	int	Include(CLevEllipse &); // Inclus�o de elipses
	// Regi�es fechadas do modelo geom�trico
	int	IncludeRegion(CLevRegion &);
	CLevRegion * GetRegion(int);
	int	GetNumRegions(void);
	int	GetNumValidRegions(void);
	void CleanRegionsVector(void);

	// Regi�es fechadas do modelo geom�trico
	virtual int	GetPosition(CLevPoint *);
	virtual int	GetPosition(CLevPrimitive*);
	virtual	int	GetNumPrimitives(void);
	virtual	int	GetNumLines(void);
	int	GetNumPoints(void);
	CLevPrimitive *	GetPrimitive(int);
	CLevPoint *	GetPoint(int);
	virtual int	IsAPoint(CLevPoint &, int &, double , double );
	virtual	int	IsARegion(CLevPoint &);
	virtual bool IsInside(double, double);
	virtual	CWorld_Window GetDomainSize(void);
	virtual double GetXMin(void);
	virtual double GetXMax(void);
	virtual double GetYMin(void);
	virtual double GetYMax(void);
	virtual void SetXMin(double);
	virtual void SetXMax(double);
	virtual void SetYMin(double);
	virtual void SetYMax(double);
	virtual void TakeLimits(void);
	virtual void Scale(double);
	vCLevPoint GetElementVerticesCoordFronteira(int );
	int	GetNumMeshElementsFronteira(void);
	virtual int	Read(void);
	virtual	int	ReadGeometricModel(std::string);
	virtual	int	ReadModelRegions(std::string);
	virtual	int	ReadModelMeshFrontier(std::string); // le dados da malha de fronteira para identifica��o de regi�es fechadas do modelo geom�trico
	virtual	int	ReadStruturedMeshData(std::string filename);

	virtual int ReadAuxiliarPrimitivesIndexes(std::string );
			int SaveAuxiliarPrimitivesIndexes(std::string );

protected:

	lev_enum::LEV_BOOL isGeometryModified;
	CWorld_Window original;// informa��o sobre as dimens�es do dominio derivado das coordenadas dos pontos do modelo geom�trico
	bool meshfronteiraOk;
	bool modelregionsOk;
	bool modelprimitivesOk;

	CLevPoint	* CreatePoint(void);
	CLevLine	* CreateLine(void);
	CLevEllipse * CreateEllipse(void);
	CLevRegion	* CreateRegion(void);

	int	InterssectedLine(int, CLevPoint &, bool = true);
	int	InterssectedLine(int, vCLevPoint &, bool = true);
	int	InterssectedEllipse(int , CLevPoint &, bool = true);
	int	InterssectedEllipse(int , vCLevPoint &, bool = true);
	void MergeLines(int, int);
	void Resolution(CLevPoint o_point, double &deltax, double &deltay);
};


//===================================================================================================
//	Arquivo:			LevGeoData.h
//...................................................................................................
//	Descri��o:			Interface da classe CLevGeoDataHistoryKeeper
//...................................................................................................
//	Autores:			Nancy Mieko Abe
//						Angelo Passaro
//...................................................................................................
//
//	Observa��es:		Esta classe � utilizada para armazenar os ponteiros para os objetos
//						- CLevPrimitive e CLevPoint
//						alocados durante a edi��o do modelo. Este armazenamento � necess�rio devido
//						ao mecanismo de UNDO adotado, no qual c�pias de objetos (ponteiros para objetos)
//						CLevGeoPhysData s�o guardadas.
//						Somente o destrutor desta classe dever� desalocar a mem�ria alocada anteriormente
//						para os objetos citados acima. As outras classes desta hierarquia simplesmente
//						devem liberar os vetores que armazenam os ponteiros.
//
//...................................................................................................
//	Data:				Novembro/2004
//	Modificado em:
//	Modificado por:
//	Motivo:
//===================================================================================================
class CLevGeoDataHistoryKeeper: public CLevGeoData
{
public:

	CLevGeoDataHistoryKeeper() { }
	~CLevGeoDataHistoryKeeper()
	{
		//		int i;
		//		int num_prim = vpPrimitives.size();
		//		CLevPrimitive	* po_primitive;
		//		CLevRegion		* po_region;
		//
		//		for(i=0;i<vpPrimitives.size();i++)
		//		{
		//			po_primitive = vpPrimitives[i];
		//			if(po_primitive)
		//			{
		//				delete [] po_primitive;
		//				po_primitive = NULL;
		//			}
		//		}
		//
		//		for(i=0;i<vpPoints.size();i++)
		//		{
		//			if(vpPoints[i])
		//			{
		//				delete [] vpPoints[i];
		//				vpPoints[i] = NULL;
		//			}
		//		}
		//
		//		for(i=0;i<vpInvisiblePoints.size();i++)
		//		{
		//			if(vpInvisiblePoints[i])
		//			{
		//				delete [] vpInvisiblePoints[i];
		//				vpInvisiblePoints[i] = NULL;
		//			}
		//		}
		//
		//		int num_regions = vpModelRegions.size();
		//		for(i=0;i<vpModelRegions.size();i++)
		//		{
		//			po_region = vpModelRegions[i];
		//			if(vpModelRegions[i])
		//			{
		//				delete [] vpModelRegions[i];
		//				vpModelRegions[i] = NULL;
		//			}
		//		}
	}
};


#endif
